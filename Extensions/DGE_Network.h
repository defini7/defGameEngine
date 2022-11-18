#pragma once

#pragma region license
/***
*	BSD 3-Clause License
	Copyright (c) 2021, 2022 Alex
	All rights reserved.
	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:
	1. Redistributions of source code must retain the above copyright notice, this
	   list of conditions and the following disclaimer.
	2. Redistributions in binary form must reproduce the above copyright notice,
	   this list of conditions and the following disclaimer in the documentation
	   and/or other materials provided with the distribution.
	3. Neither the name of the copyright holder nor the names of its
	   contributors may be used to endorse or promote products derived from
	   this software without specific prior written permission.
	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
	AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
	IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
	FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
	DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
	SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
	CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
	OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***/
#pragma endregion

#ifdef DGE_NETWORK

#pragma region includes

#include <iostream>
#include <memory>
#include <vector>
#include <deque>
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <optional>
#include <mutex>
#include <thread>

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

#pragma endregion

namespace def
{
	namespace net
	{
		template <typename T>
		struct message_header
		{
			T id{};
			uint32_t size = 0;
		};

		template <typename T>
		struct message
		{
			message_header<T> header{};
			std::vector<uint8_t> body;

			size_t size() const
			{
				return body.size();
			}

			template <typename DataType>
			void push(const DataType& data)
			{
				// Check if data is trivially copyable
				static_assert(std::is_standard_layout<DataType>::value, "Data is too complex");

				// Save vector size, so that would be index of new element
				size_t end = body.size();

				// Resize it to fit new element
				body.resize(end + sizeof(DataType));

				// Add new element
				memcpy(body.data() + end, &data, sizeof(DataType));

				// Update size
				header.size = size();
			}

			template <typename DataType>
			void get(DataType& data)
			{
				// Check if data is trivially copyable
				static_assert(std::is_standard_layout<DataType>::value, "Data is too complex");

				// Calculate new size of the vector
				size_t end = body.size() - sizeof(DataType);

				// Copy element in user variable
				memcpy(&data, body.data() + end, sizeof(DataType));

				// Shrink vector
				body.resize(end);

				// Update size
				header.size = size();
			}

			// More user friendly way to perform
			// pushing to and getting out of the vector
			template <typename DataType>
			message<T>& operator<<(const DataType& data)
			{
				push(data);
				return *this;
			}

			template <typename DataType>
			message<T>& operator>>(DataType& data)
			{
				get(data);
				return *this;
			}
		};

		template <typename T>
		class connection;

		template <typename T>
		struct owned_message
		{
			std::shared_ptr<connection<T>> remote = nullptr;
			message<T> msg;
		};

		template <class T>
		class ts_deque
		{
		public:
			ts_deque() = default;
			ts_deque(const ts_deque<T>&) = delete;
			~ts_deque() { clear(); }

		public:
			const T& front()
			{
				std::scoped_lock lock(m_muxQueue);
				return m_deqQueue.front();
			}

			const T& back()
			{
				std::scoped_lock lock(m_muxQueue);
				return m_deqQueue.back();
			}

			void push_back(const T& value)
			{
				std::scoped_lock lock(m_muxQueue);
				m_deqQueue.emplace_back(std::move(value));

				// signal std::condition_variable to wake up thread
				std::unique_lock<std::mutex> ul(m_muxWaiting);
				m_cvWaiting.notify_one();
			}

			void push_front(const T& value)
			{
				std::scoped_lock lock(m_muxQueue);
				m_deqQueue.emplace_front(std::move(value));

				// signal std::condition_variable to wake up thread
				std::unique_lock<std::mutex> ul(m_muxWaiting);
				m_cvWaiting.notify_one();
			}

			bool empty()
			{
				std::scoped_lock lock(m_muxQueue);
				return m_deqQueue.empty();
			}

			size_t size()
			{
				std::scoped_lock lock(m_muxQueue);
				return m_deqQueue.size();
			}

			void clear()
			{
				std::scoped_lock lock(m_muxQueue);
				m_deqQueue.clear();
			}

			T pop_front()
			{
				std::scoped_lock lock(m_muxQueue);
				auto t = std::move(m_deqQueue.front());
				m_deqQueue.pop_front();
				return t;
			}

			T pop_back()
			{
				std::scoped_lock lock(m_muxQueue);
				auto t = std::move(m_deqQueue.back());
				m_deqQueue.pop_back();
				return t;
			}

			std::deque<T>::iterator begin()
			{
				std::scoped_lock lock(m_muxQueue);
				return m_deqQueue.begin();
			}

			std::deque<T>::iterator end()
			{
				std::scoped_lock lock(m_muxQueue);
				return m_deqQueue.end();
			}

			std::deque<T>::const_iterator cbegin()
			{
				std::scoped_lock lock(m_muxQueue);
				return m_deqQueue.cbegin();
			}

			std::deque<T>::const_iterator cend()
			{
				std::scoped_lock lock(m_muxQueue);
				return m_deqQueue.cend();
			}

			void wait()
			{
				// while there are no signals
				// we "freeze" the thread

				// there could be some errors,
				// but it doesn't really matter

				while (empty())
				{
					std::unique_lock<std::mutex> ul(m_muxWaiting);
					m_cvWaiting.wait(ul);
				}
			}

		private:
			std::mutex m_muxQueue;
			std::deque<T> m_deqQueue;

			std::condition_variable m_cvWaiting;
			std::mutex m_muxWaiting;
		};

		template <typename T>
		class client
		{
		public:
			client() {}

			virtual ~client()
			{
				disconnect();
			}

		public:
			bool connect(const std::string& host, const uint16_t port)
			{
				// instead of using asio::error_code, we can use try catch,
				// because asio::error_code is the std::error_code
				try
				{
					asio::ip::tcp::resolver resolver(m_context);
					m_endpoints = resolver.resolve(host, std::to_string(port));

					m_connection = std::make_unique<connection<T>>(
						connection<T>::side::client,
						m_context,
						asio::ip::tcp::socket(m_context),
						m_tsqMessagesIn
						);

					m_connection->connect_server(m_endpoints);

					m_tContext = std::thread([&]() { m_context.run(); });
				}
				catch (std::exception& e)
				{
					std::cerr << "[CLIENT] Unable to connect to " <<
						host << ": " << e.what() << std::endl;

					return false;
				}

				std::cout << "[CLIENT] Connected!" << std::endl;
				return false;
			}

			void disconnect()
			{
				// If connection is valid then we disconnect
				if (connected())
					m_connection->disconnect();

				m_context.stop();

				if (m_tContext.joinable())
					m_tContext.join();
			}

			bool connected()
			{
				if (m_connection)
					return m_connection->connected();

				return false;
			}

			void send(const message<T>& msg)
			{
				if (connected())
					m_connection->send(msg);
			}

			ts_deque<owned_message<T>>& messages()
			{
				return m_tsqMessagesIn;
			}

		private:
			// We need unique context for each client
			asio::io_context m_context;

			// We need thread to perform tasks
			std::thread m_tContext;

			// Also we must have connection to manage data
			std::unique_ptr<connection<T>> m_connection;

			// Just incoming messages
			ts_deque<owned_message<T>> m_tsqMessagesIn;

			// Save endpoints
			asio::ip::tcp::resolver::results_type m_endpoints;
		};

		template <typename T>
		class server;

		template <typename T>
		class connection : public std::enable_shared_from_this<connection<T>>
		{
		public:
			enum class side
			{
				server,
				client
			};

		public:
			connection(side parent, asio::io_context& context, asio::ip::tcp::socket socket, ts_deque<owned_message<T>>& messages)
				: m_context(context), m_socket(std::move(socket)), m_tsqMessagesIn(messages)
			{
				m_nOwner = parent;

				if (m_nOwner == side::server)
				{
					// Make random data (take a time so it will always be "random")
					// and send it to client to transorm and give it back
					m_nKnockOut = uint64_t(std::chrono::system_clock::now().time_since_epoch().count());

					// Pre-generate that so we can check it later
					m_nKnockCheck = encrypt(m_nKnockOut);
				}
				else
				{
					m_nKnockIn = 0;
					m_nKnockOut = 0;
				}
			}

			virtual ~connection() = default;

		public:
			uint32_t id() const
			{
				return m_nID;
			}

			void connect_client(server<T>* serv, uint32_t id = 0)
			{
				if (m_nOwner == side::server)
				{
					if (connected())
					{
						m_nID = id;

						WriteValidation();
						ReadValidation(serv);
					}
				}
			}

			void connect_server(const asio::ip::tcp::resolver::results_type& endpoints)
			{
				if (m_nOwner == side::client)
				{
					asio::async_connect(m_socket, endpoints,
						[this](asio::error_code ec, asio::ip::tcp::endpoint ep)
						{
							if (!ec)
							{
								// So let's start writing message
								//ReadHeader();

								ReadValidation();
							}
						}
					);
				}
			}

			void disconnect()
			{
				if (connected())
				{
					// If socket is open then just close it
					asio::post(m_context, [this]() { m_socket.close(); });
				}
			}

			bool connected() const
			{
				return m_socket.is_open();
			}

			void send(const message<T>& msg)
			{
				asio::post(m_context,
					[this, msg]()
					{
						bool bWasEmpty = m_tsqMessagesOut.empty();
						m_tsqMessagesOut.push_back(msg);

						if (bWasEmpty)
							WriteHeader();
					}
				);
			}

			uint64_t encrypt(uint64_t n)
			{
				uint64_t out;

				out = n ^ 0xF00BA4BA7;
				out = (out & 0xF0F0F0F0F0F0F0) >> 4 | (out & 0x0F0F0F0F0F0F0F) << 4;
				out = out ^ 0xF00BA71324;

				return out;
			}

		private:
			void WriteHeader()
			{
				// Writing message header
				asio::async_write(m_socket, asio::buffer(&m_tsqMessagesOut.front().header, sizeof(message_header<T>)),
					[this](asio::error_code ec, size_t length)
					{
						if (!ec)
						{
							// Check if message has a body
							if (m_tsqMessagesOut.front().body.size() > 0)
							{
								// so let's write body
								WriteBody();
							}
							else
							{
								// it doesn't have a message body
								// so just remove it
								m_tsqMessagesOut.pop_front();

								// If the deque isn't empty then there are more messages
								if (!m_tsqMessagesOut.empty())
									WriteHeader();
							}
						}
						else
						{
							// if it fails then just write fail reason
							// and close socket
							std::cerr << '[' << id() << "] " << ec.message() << std::endl;
							m_socket.close();
						}
					}
				);
			}

			void WriteBody()
			{
				asio::async_write(m_socket, asio::buffer(m_tsqMessagesOut.front().body.data(), m_tsqMessagesOut.front().body.size()),
					[this](asio::error_code ec, size_t length)
					{
						if (!ec)
						{
							// Remove message from queue
							// because sending was over
							m_tsqMessagesOut.pop_front();

							// If there are still any messages
							// then do message stuff again
							if (!m_tsqMessagesOut.empty())
								WriteHeader();
						}
						else
						{
							// if it fails then just write fail reason
							// and close socket
							std::cerr << '[' << id() << "] " << ec.message() << std::endl;
							m_socket.close();
						}
					}
				);
			}

			void ReadHeader()
			{
				asio::async_read(m_socket, asio::buffer(&m_msgCache.header, sizeof(message_header<T>)),
					[this](asio::error_code ec, size_t length)
					{
						if (!ec)
						{
							// Check if message has a body
							if (m_msgCache.header.size > 0)
							{
								// Then resize body vector and read it
								m_msgCache.body.resize(m_msgCache.header.size);
								ReadBody();
							}
							else
							{
								// else push it to incoming messages
								PushToIncomingQueue();
							}
						}
						else
						{
							// if it fails then just write fail reason
							// and close socket
							std::cerr << '[' << id() << "] " << ec.message() << std::endl;
							m_socket.close();
						}
					}
				);
			}

			void ReadBody()
			{
				asio::async_read(m_socket, asio::buffer(m_msgCache.body.data(), m_msgCache.body.size()),
					[this](asio::error_code ec, size_t length)
					{
						if (!ec)
						{
							// when bytes arrive
							// we just push message to incoming messages
							PushToIncomingQueue();
						}
						else
						{
							// if it fails then just write fail reason
							// and close socket
							std::cerr << '[' << id() << "] " << ec.message() << std::endl;
							m_socket.close();
						}
					}
				);
			}

			void PushToIncomingQueue()
			{
				// Convert to owned_message and save it in queue
				if (m_nOwner == side::server)
				{
					m_tsqMessagesIn.push_back({
						this->shared_from_this(),
						m_msgCache
						});
				}
				else
				{
					m_tsqMessagesIn.push_back({
						nullptr,
						m_msgCache
						});
				}

				// Prime context to recieve new message
				ReadHeader();
			}

			void ReadValidation(server<T>* serv = nullptr)
			{
				asio::async_read(m_socket, asio::buffer(&m_nKnockIn, sizeof(uint64_t)),
					[this, serv](std::error_code ec, std::size_t length)
					{
						if (!ec)
						{
							if (m_nOwner == side::server)
							{
								// Compare sent data to actual solution
								if (m_nKnockIn == m_nKnockCheck)
								{
									std::cout << "Client Validated" << std::endl;
									serv->OnClientValidated(this->shared_from_this());

									ReadHeader();
								}
								else
								{
									std::cout << "Client Disconnected (Validation failed)" << std::endl;
									m_socket.close();
								}
							}
							else
							{
								m_nKnockOut = encrypt(m_nKnockIn);

								WriteValidation();
							}
						}
						else
						{
							// Some biggerfailure occured
							std::cout << "Client Disconnected (ReadValidation)" << std::endl;
							m_socket.close();
						}
					});
			}

			void WriteValidation()
			{
				asio::async_write(m_socket, asio::buffer(&m_nKnockOut, sizeof(uint64_t)),
					[this](std::error_code ec, std::size_t length)
					{
						if (!ec)
						{
							// Validation data sent, clients should sit and wait
							// for a response (or a closure)
							if (m_nOwner == side::client)
								ReadHeader();
						}
						else
						{
							m_socket.close();
						}
					});
			}

		private:
			side m_nOwner = side::server;

			asio::ip::tcp::socket m_socket;
			asio::io_context& m_context;

			message<T> m_msgCache;

			ts_deque<message<T>> m_tsqMessagesOut;
			ts_deque<owned_message<T>>& m_tsqMessagesIn;

			uint32_t m_nID = 0;

			uint64_t m_nKnockIn = 0;
			uint64_t m_nKnockOut = 0;
			uint64_t m_nKnockCheck = 0;

		};

		template <typename T>
		class server
		{
		public:
			server(uint16_t port) : m_acceptor(m_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
			{

			}

			virtual ~server()
			{
				stop();
			}

		public:
			bool start()
			{
				// instead of using asio::error_code, we can use try catch,
				// because asio::error_code is the std::error_code
				try
				{
					wait_connection();

					m_tContext = std::thread([this]() { m_context.run(); });
				}
				catch (std::exception& e)
				{
					std::cout << "[SERVER] Unable to connect: " << e.what() << std::endl;
					return false;
				}

				std::cout << "[SERVER] Started" << std::endl;
				return true;
			}

			void stop()
			{
				m_context.stop();

				if (m_tContext.joinable())
					m_tContext.join();

				std::cout << "[SERVER] Stopped" << std::endl;
			}

			void wait_connection()
			{
				m_acceptor.async_accept(
					[&](asio::error_code ec, asio::ip::tcp::socket socket)
					{
						if (!ec)
						{
							std::cout << "[SERVER] New connection: " << socket.remote_endpoint() << std::endl;

							std::shared_ptr<connection<T>> conn =
								std::make_shared<connection<T>>(
									connection<T>::side::server,
									m_context, std::move(socket), m_tsqMessagesIn
									);

							if (OnClientConnect(conn))
							{
								m_deqConnections.push_back(std::move(conn));

								m_deqConnections.back()->connect_client(this, m_nIDCounter++);

								std::cout << "[SERVER] Connection " << m_deqConnections.back()->id()
									<< " was approved" << std::endl;
							}
							else
							{
								// Deny connection if user wants that
								std::cout << "[SERVER] Connection " << conn << " was denied" << std::endl;
							}
						}
						else
						{
							// Can't connect to the server
							std::cout << "[SERVER] Connection error: " << ec.message() << std::endl;
						}

						wait_connection();
					}
				);
			}

			void send(std::shared_ptr<connection<T>> client, const message<T>& msg)
			{
				if (client && client->connected())
				{
					// If client is valid and still connected
					// then we send message
					client->send(msg);
				}
				else
				{
					// client isn't valid or isn't connected
					// so we just delete client

					OnClientDisconnect(client);

					client.reset();

					m_deqConnections.erase(
						std::remove(
							m_deqConnections.begin(),
							m_deqConnections.end(),
							client
						),
						m_deqConnections.end()
					);
				}
			}

			void send_all(const message<T>& msg, std::shared_ptr<connection<T>> ignored = nullptr)
			{
				bool bAnyInvalid = false;

				for (auto& client : m_deqConnections)
				{
					if (client && client->connected())
					{
						// Client is valid and connected
						// so we just send message if it's not an ignored client
						if (client != ignored)
							client->send(msg);
					}
					else
					{
						// client isn't valid or isn't connected
						// so we just delete client

						OnClientConnect(client);

						client.reset();

						// Set flag to optimize program, because
						// erase in deque is very expensive operation
						bAnyInvalid = true;
					}
				}

				if (bAnyInvalid)
				{
					// Just remove invalid clients
					m_deqConnections.erase(
						std::remove(
							m_deqConnections.begin(),
							m_deqConnections.end(),
							nullptr
						),
						m_deqConnections.end()
					);
				}
			}

		public:
			// User must call that to update state
			void update(size_t max = -1, bool wait = false)
			{
				// Store amount of messages
				size_t messages_count = 0;

				// While there are any messages and the message count is still good
				while (messages_count < max && !m_tsqMessagesIn.empty())
				{
					// Get message
					owned_message<T> msg = m_tsqMessagesIn.pop_front();

					OnMessage(msg.remote, msg.msg);

					messages_count++;
				}
			}

			// Called when a client connects
			virtual bool OnClientConnect(std::shared_ptr<connection<T>> client)
			{
				return false;
			}

			// Called when a client disconnects
			virtual void OnClientDisconnect(std::shared_ptr<connection<T>> client)
			{

			}

			virtual void OnClientValidated(std::shared_ptr<connection<T>> client)
			{

			}

			// Called when a message arrives
			virtual void OnMessage(std::shared_ptr<connection<T>> client, message<T>& msg)
			{

			}

		private:
			// Incoming messages
			ts_deque<owned_message<T>> m_tsqMessagesIn;

			// Will store all connections
			std::deque<std::shared_ptr<connection<T>>> m_deqConnections;

			// Unique ASIO context for each server
			asio::io_context m_context;

			// We must have thread to perform tasks
			std::thread m_tContext;

			// ASIO acceptor
			asio::ip::tcp::acceptor m_acceptor;

			// All IDs will start from this number
			uint32_t m_nIDCounter = 10000;
		};

	}
}

#endif
