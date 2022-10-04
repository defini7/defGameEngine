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

#ifdef DGE_RANDGENERATOR

#pragma region includes
#include <ctime>
#include <cstdint>
#include <list>
#include <vector>
#include <deque>
#include <stack>
#pragma endregion

namespace def
{
	class RandGenerator
	{
	public:
		RandGenerator()
		{
			m_nSeed = time(0);
		}

	private:
		uint32_t m_nSeed = 0;

	private:
		uint32_t Lehmer(uint32_t seed)
		{
			uint32_t nLehmer;
			uint64_t tmp;

			nLehmer = seed + 0xE120FC15;
			tmp = (uint64_t)nLehmer * 0x4A39B70D;

			uint32_t m1 = (tmp >> 32) ^ tmp;
			tmp = (uint64_t)m1 * 0x12FAD5C9;

			uint32_t m2 = (tmp >> 32) ^ tmp;
			return m2;
		}

		void Clamp(uint32_t& n, uint32_t min, uint32_t max)
		{
			n = (n % (max - min)) + min;
		}

	public:
		void Seed(uint32_t seed)
		{
			m_nSeed = seed;
		}

		uint32_t RandInt()
		{
			uint32_t n = Lehmer(m_nSeed);
			return n;
		}

		uint32_t RandInt(uint32_t min, uint32_t max)
		{
			uint32_t n = Lehmer(m_nSeed);
			Clamp(n, min, max);
			return n;
		}
		
		std::list<uint32_t> RandList(uint32_t n)
		{
			std::list<uint32_t> l;

			for (int i = 0; i < n; i++)
				l.push_back(RandInt());

			return l;
		}

		std::list<uint32_t> RandList(uint32_t n, uint32_t min, uint32_t max)
		{
			std::list<uint32_t> l;

			for (int i = 0; i < n; i++)
				l.push_back(RandInt(min, max));

			return l;
		}

		std::vector<uint32_t> RandVector(uint32_t n)
		{
			std::vector<uint32_t> l;

			for (int i = 0; i < n; i++)
				l.push_back(RandInt());

			return l;
		}

		std::vector<uint32_t> RandVector(uint32_t n, uint32_t min, uint32_t max)
		{
			std::vector<uint32_t> l;

			for (int i = 0; i < n; i++)
				l.push_back(RandInt(min, max));

			return l;
		}

		std::deque<uint32_t> RandDeque(uint32_t n)
		{
			std::deque<uint32_t> l;

			for (int i = 0; i < n; i++)
				l.push_back(RandInt());

			return l;
		}

		std::deque<uint32_t> RandDeque(uint32_t n, uint32_t min, uint32_t max)
		{
			std::deque<uint32_t> l;

			for (int i = 0; i < n; i++)
				l.push_back(RandInt(min, max));

			return l;
		}

		std::stack<uint32_t> RandStack(uint32_t n)
		{
			std::stack<uint32_t> l;

			for (int i = 0; i < n; i++)
				l.push(RandInt());

			return l;
		}

		std::stack<uint32_t> RandStack(uint32_t n, uint32_t min, uint32_t max)
		{
			std::stack<uint32_t> l;

			for (int i = 0; i < n; i++)
				l.push(RandInt(min, max));

			return l;
		}

	};
}

#endif
