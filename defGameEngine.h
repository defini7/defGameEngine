#pragma once

#pragma region license
/***
*	BSD 3-Clause License
	Copyright (c) 2021, 2022, 2023 Alex
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

#pragma region sample
/**
* Example:
	#define DGE_APPLICATION
	#include "defGameEngine.h"

	class Sample : public def::GameEngine
	{
	public:
		Sample()
		{
			SetTitle("Sample");
		}

	protected:
		bool OnUserCreate() override
		{
			return true;
		}

		bool OnUserUpdate(float fDeltaTime) override
		{
			for (int i = 0; i < ScreenWidth(); i++)
				for (int j = 0; j < ScreenHeight(); j++)
					Draw(i, j, def::RandomPixel());

			return true;
		}
	};

	int main()
	{
		Sample demo;

		if (demo.Construct(256, 240, 4, 4))
			demo.Run();

		return 0;
	}
**/
#pragma endregion

#pragma region includes

#include <iostream>
#include <cstdio>
#include <string>
#include <chrono>
#include <vector>
#include <cmath>
#include <list>
#include <memory>s

#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#pragma warning(disable:4996)
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#ifdef _WIN32

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")

#else
/*
* link libraries
* to your project
*/
#endif

#pragma endregion

#define DGE_ASSERT(expr, msg) \
	if (!(expr)) \
	{ \
		std::cout << msg << std::endl; \
		exit(1); \
	}

namespace def
{
	// Keyboard keys constants for convenient usage
	namespace Key
	{
		unsigned int SPACE = 32;
		unsigned int APOSTROPHE = 39;
		unsigned int COMMA = 44;
		unsigned int MINUS = 45;
		unsigned int PERIOD = 46;
		unsigned int SLASH = 47;
		unsigned int K0 = 48;
		unsigned int K1 = 49;
		unsigned int K2 = 50;
		unsigned int K3 = 51;
		unsigned int K4 = 52;
		unsigned int K5 = 53;
		unsigned int K6 = 54;
		unsigned int K7 = 55;
		unsigned int K8 = 56;
		unsigned int K9 = 57;
		unsigned int SEMICOLON = 59;
		unsigned int EQUAL = 61;
		unsigned int A = 65;
		unsigned int B = 66;
		unsigned int C = 67;
		unsigned int D = 68;
		unsigned int E = 69;
		unsigned int F = 70;
		unsigned int G = 71;
		unsigned int H = 72;
		unsigned int I = 73;
		unsigned int J = 74;
		unsigned int K = 75;
		unsigned int L = 76;
		unsigned int M = 77;
		unsigned int N = 78;
		unsigned int O = 79;
		unsigned int P = 80;
		unsigned int Q = 81;
		unsigned int R = 82;
		unsigned int S = 83;
		unsigned int T = 84;
		unsigned int U = 85;
		unsigned int V = 86;
		unsigned int W = 87;
		unsigned int X = 88;
		unsigned int Y = 89;
		unsigned int Z = 90;
		unsigned int LEFT_BRACKET = 91; /* [ */
		unsigned int BACKSLASH = 92; /* \ */
		unsigned int RIGHT_BRACKET = 93; /* ] */
		unsigned int GRAVE_ACCENT = 96; /* ` */
		unsigned int WORLD_1 = 161; /* non-US #1 */
		unsigned int WORLD_2 = 162; /* non-US #2 */
		unsigned int ESCAPE = 256;
		unsigned int ENTER = 257;
		unsigned int TAB = 258;
		unsigned int BACKSPACE = 259;
		unsigned int INSERT = 260;
		unsigned int DEL = 261;
		unsigned int RIGHT = 262;
		unsigned int LEFT = 263;
		unsigned int DOWN = 264;
		unsigned int UP = 265;
		unsigned int PAGE_UP = 266;
		unsigned int PAGE_DOWN = 267;
		unsigned int HOME = 268;
		unsigned int END = 269;
		unsigned int CAPS_LOCK = 280;
		unsigned int SCROLL_LOCK = 281;
		unsigned int NUM_LOCK = 282;
		unsigned int PRINT_SCREEN = 283;
		unsigned int PAUSE = 284;
		unsigned int F1 = 290;
		unsigned int F2 = 291;
		unsigned int F3 = 292;
		unsigned int F4 = 293;
		unsigned int F5 = 294;
		unsigned int F6 = 295;
		unsigned int F7 = 296;
		unsigned int F8 = 297;
		unsigned int F9 = 298;
		unsigned int F10 = 299;
		unsigned int F11 = 300;
		unsigned int F12 = 301;
		unsigned int F13 = 302;
		unsigned int F14 = 303;
		unsigned int F15 = 304;
		unsigned int F16 = 305;
		unsigned int F17 = 306;
		unsigned int F18 = 307;
		unsigned int F19 = 308;
		unsigned int F20 = 309;
		unsigned int F21 = 310;
		unsigned int F22 = 311;
		unsigned int F23 = 312;
		unsigned int F24 = 313;
		unsigned int F25 = 314;
		unsigned int NP_0 = 320;
		unsigned int NP_1 = 321;
		unsigned int NP_2 = 322;
		unsigned int NP_3 = 323;
		unsigned int NP_4 = 324;
		unsigned int NP_5 = 325;
		unsigned int NP_6 = 326;
		unsigned int NP_7 = 327;
		unsigned int NP_8 = 328;
		unsigned int NP_9 = 329;
		unsigned int NP_DECIMAL = 330;
		unsigned int NP_DIVIDE = 331;
		unsigned int NP_MULTIPLY = 332;
		unsigned int NP_SUBTRACT = 333;
		unsigned int NP_ADD = 334;
		unsigned int NP_ENTER = 335;
		unsigned int NP_EQUAL = 336;
		unsigned int LEFT_SHIFT = 340;
		unsigned int LEFT_CONTROL = 341;
		unsigned int LEFT_ALT = 342;
		unsigned int LEFT_SUPER = 343;
		unsigned int RIGHT_SHIFT = 344;
		unsigned int RIGHT_CONTROL = 345;
		unsigned int RIGHT_ALT = 346;
		unsigned int RIGHT_SUPER = 347;
		unsigned int MENU = 348;
	}

	template <typename T>
	struct v2d
	{
		v2d() = default;
		v2d(T x, T y)
		{
			this->x = x;
			this->y = y;
		}

		T x = (T)0;
		T y = (T)0;

		v2d<T> operator=(const v2d<T>& v)
		{
			this->x = v.x;
			this->y = v.y;
			return *this;
		}

		friend v2d<T> operator+(const v2d<T>& v1, const v2d<T>& v2) { return v2d<T>(v1.x + v2.x, v1.y + v2.y); }
		friend v2d<T> operator-(const v2d<T>& v1, const v2d<T>& v2) { return v2d<T>(v1.x - v2.x, v1.y - v2.y); }
		friend v2d<T> operator*(const v2d<T>& v1, const v2d<T>& v2) { return v2d<T>(v1.x * v2.x, v1.y * v2.y); }
		friend v2d<T> operator/(const v2d<T>& v1, const v2d<T>& v2) { return v2d<T>(v1.x / v2.x, v1.y / v2.y); }
		friend v2d<T> operator+(const v2d<T>& v1, const T& v2) { return v2d<T>(v1.x + v2, v1.y + v2); }
		friend v2d<T> operator-(const v2d<T>& v1, const T& v2) { return v2d<T>(v1.x - v2, v1.y - v2); }
		friend v2d<T> operator*(const v2d<T>& v1, const T& v2) { return v2d<T>(v1.x * v2, v1.y * v2); }
		friend v2d<T> operator/(const v2d<T>& v1, const T& v2) { return v2d<T>(v1.x / v2, v1.y / v2); }

		v2d<T>& operator+=(const v2d<T>& v)
		{
			this->x += v.x;
			this->y += v.y;
			return *this;
		}

		v2d<T>& operator-=(const v2d<T>& v)
		{
			this->x -= v.x;
			this->y -= v.y;
			return *this;
		}

		v2d<T>& operator*=(const v2d<T>& v)
		{
			this->x *= v.x;
			this->y *= v.y;
			return *this;
		}

		v2d<T>& operator/=(const v2d<T>& v)
		{
			this->x /= v.x;
			this->y /= v.y;
			return *this;
		}

		v2d<T>& operator+=(const T& v)
		{
			this->x += v;
			this->y += v;
			return *this;
		}

		v2d<T>& operator-=(const T& v)
		{
			this->x -= v;
			this->y -= v;
			return *this;
		}

		v2d<T>& operator*=(const T& v)
		{
			this->x *= v;
			this->y *= v;
			return *this;
		}

		v2d<T>& operator/=(const T& v)
		{
			this->x /= v;
			this->y /= v;
			return *this;
		}

		friend bool operator==(const v2d<T>& v1, const v2d<T>& v2) { return v1.x == v2.x && v1.y == v2.y; }
		friend bool operator!=(const v2d<T>& v1, const v2d<T>& v2) { return v1.x != v2.x || v1.y != v2.y; }
		friend bool operator<(const v2d<T>& v1, const v2d<T>& v2) { return v1.x < v2.x&& v1.y < v2.y; }
		friend bool operator>(const v2d<T>& v1, const v2d<T>& v2) { return v1.x > v2.x && v1.y > v2.y; }
		friend bool operator<=(const v2d<T>& v1, const v2d<T>& v2) { return v1.x <= v2.x && v1.y <= v2.y; }
		friend bool operator>=(const v2d<T>& v1, const v2d<T>& v2) { return v1.x >= v2.x && v1.y >= v2.y; }

		friend v2d<T> operator*(const float& lhs, const v2d<T>& rhs)
		{
			return v2d<T>((T)(lhs * (float)rhs.x), (T)(lhs * (float)rhs.y));
		}

		friend v2d<T> operator * (const double& lhs, const v2d<T>& rhs)
		{
			return v2d<T>((T)(lhs * (double)rhs.x), (T)(lhs * (double)rhs.y));
		}

		friend v2d<T> operator * (const int& lhs, const v2d<T>& rhs)
		{
			return v2d<T>((T)(lhs * (int)rhs.x), (T)(lhs * (int)rhs.y));
		}

		friend v2d<T> operator / (const float& lhs, const v2d<T>& rhs)
		{
			return v2d<T>((T)(lhs / (float)rhs.x), (T)(lhs / (float)rhs.y));
		}

		friend v2d<T> operator / (const double& lhs, const v2d<T>& rhs)
		{
			return v2d<T>((T)(lhs / (double)rhs.x), (T)(lhs / (double)rhs.y));
		}

		friend v2d<T> operator / (const int& lhs, const v2d<T>& rhs)
		{
			return v2d<T>((T)(lhs / (int)rhs.x), (T)(lhs / (int)rhs.y));
		}

		operator v2d<int>()	   const { return { static_cast<int32_t>(this->x),  static_cast<int32_t>(this->y) }; }
		operator v2d<float>()  const { return { static_cast<float>(this->x),	static_cast<float>(this->y) }; }
		operator v2d<double>() const { return { static_cast<double>(this->x),	static_cast<double>(this->y) }; }

		v2d<T> clamp(const v2d<T>& start, const v2d<T>& end) const
		{
			v2d<T> clamped;
			clamped.x = this->x;
			clamped.y = this->y;
			if (clamped.x < start.x) clamped.x = start.x;
			if (clamped.y < start.y) clamped.y = start.y;
			if (clamped.x > end.x) clamped.x = end.x;
			if (clamped.y > end.y) clamped.y = end.y;
			return clamped;
		}

		float dot(const v2d<T>& v) { return this->x * v.x + this->y * v.y; }
		float length() { return sqrtf(dot(*this)); }

		T mag() { return static_cast<T>(sqrtf(this->x * this->x + this->y * this->y)); }
		T mag2() { return static_cast<T>(this->x * this->x + this->y * this->y); }

		float man(const v2d<T>& v) { return std::abs(this->x - v.x) + std::abs(this->y - v.y); }

		v2d<T> max(const v2d<T>& v) { return v2d<T>(std::max(this->x, v.x), std::max(this->y, v.y)); }
		v2d<T> min(const v2d<T>& v) { return v2d<T>(std::min(this->x, v.x), std::min(this->y, v.y)); }

		v2d<T> norm() { float n = 1.0f / mag(); return v2d<T>(this->x * n, this->y * n); }
		v2d<T> abs() { return v2d<T>(std::abs(this->x), std::abs(this->y)); }
		v2d<T> perp() { return v2d<T>(-this->y, this->x); }
		v2d<T> floor() { return v2d<T>(std::floor(this->x), std::floor(this->y)); }
		v2d<T> ceil() { return v2d<T>(std::ceil(this->x), std::ceil(this->y)); }
		v2d<T> round() { return v2d<T>(std::round(this->x), std::round(this->y)); }
		v2d<T> cart() { return v2d<T>(cos(this->y) * this->x, sin(this->y) * this->x); }
		v2d<T> polar() { return v2d<T>(mag(), atan2(this->y, this->x)); }
		v2d<T>& ref() { return *this; }

		operator std::string() const { return "(" + std::to_string(this->x) + ", " + std::to_string(this->y) + ")"; }
	};

	typedef v2d<int> vi2d;
	typedef v2d<float> vf2d;
	typedef v2d<double> vd2d;

	struct KeyState
	{
		bool bHeld;
		bool bReleased;
		bool bPressed;
	};

	struct Pixel
	{
		Pixel(uint8_t _r = 0, uint8_t _g = 0, uint8_t _b = 0, uint8_t _a = 255U) : r(_r), g(_g), b(_b), a(_a) {}

		enum Mode
		{
			DEFAULT,
			ALPHA,
			MASK,
			CUSTOM
		};

		uint8_t r, g, b, a;

		template <typename T>
		friend Pixel operator+(const Pixel& lhs, T rhs) { return Pixel(uint8_t((T)lhs.r + rhs), uint8_t((T)lhs.g + rhs), uint8_t((T)lhs.b + rhs), lhs.a); }

		template <typename T>
		friend Pixel operator-(const Pixel& lhs, T rhs) { return Pixel(uint8_t((T)lhs.r - rhs), uint8_t((T)lhs.g - rhs), uint8_t((T)lhs.b - rhs), lhs.a); }

		template <typename T>
		friend Pixel operator*(const Pixel& lhs, T rhs) { return Pixel(uint8_t((T)lhs.r * rhs), uint8_t((T)lhs.g * rhs), uint8_t((T)lhs.b * rhs), lhs.a); }

		template <typename T>
		friend Pixel operator/(const Pixel& lhs, T rhs) { return Pixel(uint8_t((T)lhs.r / rhs), uint8_t((T)lhs.g / rhs), uint8_t((T)rhs.b / rhs), lhs.a); }

		template <typename T>
		friend Pixel& operator+=(const Pixel& lhs, T rhs)
		{
			lhs.r += rhs;
			lhs.g += rhs;
			lhs.b += rhs;
			return lhs;
		}

		template <typename T>
		friend Pixel& operator-=(const Pixel& lhs, T rhs)
		{
			lhs.r -= rhs;
			lhs.g -= rhs;
			lhs.b -= rhs;

			return lhs;
		}

		template <typename T>
		friend Pixel& operator*=(const Pixel& lhs, T rhs)
		{
			lhs.r *= rhs;
			lhs.g *= rhs;
			lhs.b *= rhs;

			return lhs;
		}

		template <typename T>
		friend Pixel& operator/=(const Pixel& lhs, T rhs)
		{
			lhs.r /= rhs;
			lhs.g /= rhs;
			lhs.b /= rhs;

			return lhs;
		}

		friend Pixel operator+(const Pixel& lhs, const Pixel& rhs) { return Pixel(lhs.r + rhs.r, lhs.g + rhs.g, lhs.b + rhs.b, lhs.a); }
		friend Pixel operator-(const Pixel& lhs, const Pixel& rhs) { return Pixel(lhs.r - rhs.r, lhs.g - rhs.g, lhs.b - rhs.b, lhs.a); }
		friend Pixel operator*(const Pixel& lhs, const Pixel& rhs) { return Pixel(lhs.r * rhs.r, lhs.g * rhs.g, lhs.b * rhs.b, lhs.a); }
		friend Pixel operator/(const Pixel& lhs, const Pixel& rhs) { return Pixel(lhs.r / rhs.r, lhs.g / rhs.g, lhs.b / rhs.b, lhs.a); }

		friend Pixel operator+=(const Pixel& lhs, const Pixel& rhs) { return lhs + rhs; }
		friend Pixel operator-=(const Pixel& lhs, const Pixel& rhs) { return lhs - rhs; }
		friend Pixel operator*=(const Pixel& lhs, const Pixel& rhs) { return lhs * rhs; }
		friend Pixel operator/=(const Pixel& lhs, const Pixel& rhs) { return lhs / rhs; }

		friend bool operator==(const Pixel& lhs, const Pixel& rhs) { return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b; }
		friend bool operator!=(const Pixel& lhs, const Pixel& rhs) { return lhs.r != rhs.r || lhs.g != rhs.g || lhs.b != rhs.b; }
	};

	Pixel PixelF(float r, float g, float b)
	{
		return Pixel(uint8_t(r * 255.0f), uint8_t(g * 255.0f), uint8_t(b * 255.0f));
	}

	static Pixel BLACK(0, 0, 0, 0),
		DARK_BLUE(0, 55, 218, 255),
		DARK_GREEN(19, 161, 14, 255),
		DARK_CYAN(59, 120, 255, 255),
		DARK_RED(197, 15, 31, 255),
		DARK_MAGENTA(136, 32, 152, 255),
		DARK_GREY(118, 118, 118, 255),
		DARK_ORANGE(255, 140, 0, 255),
		DARK_BROWN(76, 63, 47, 255),
		DARK_PURPLE(112, 31, 126, 255),
		ORANGE(255, 165, 0, 255),
		GREY(200, 200, 200, 255),
		BLUE(0, 0, 255, 255),
		GREEN(0, 255, 0, 255),
		CYAN(0, 255, 255, 255),
		RED(255, 0, 0, 255),
		MAGENTA(180, 0, 158, 255),
		YELLOW(255, 255, 0, 255),
		WHITE(255, 255, 255, 255),
		GOLD(255, 203, 0, 255),
		PINK(255, 109, 194, 255),
		MAROON(190, 33, 55, 255),
		LIME(0, 158, 47, 255),
		BROWN(127, 106, 79, 255),
		BEIGE(211, 176, 131, 255),
		VIOLET(135, 60, 190, 255),
		PURPLE(200, 122, 255, 255),
		NONE(0, 0, 0, 0);

	Pixel RandomPixel(bool bRandomAlpha = false)
	{
		return Pixel(rand() % 256, rand() % 256, rand() % 256, bRandomAlpha ? rand() % 256 : 255);
	}

	float   Byte2Float(uint8_t b) { return (float)b / 255.0f; }
	uint8_t Float2Byte(float f) { return uint8_t(f * 255.0f); }

	class Sprite
	{
	public:
		Sprite() = default;
		Sprite(int32_t width, int32_t height, int32_t channels = 4)
		{
			if (width > 0 && height > 0 && channels > 0)
				Create(width, height, channels);
		}

		Sprite(const std::string& sFilename)
		{
			Load(sFilename);
		}

		~Sprite()
		{
			if (pPixelData != nullptr)
				delete[] pPixelData;
		}

	public:
		int32_t nWidth;
		int32_t nHeight;
		int32_t nChannels;

		uint8_t* pPixelData = nullptr;

	public:
		void Create(const int32_t width, const int32_t height, const int32_t channels = 4)
		{
			if (pPixelData != nullptr) delete[] pPixelData;

			nWidth = width;
			nHeight = height;
			nChannels = channels;

			pPixelData = new uint8_t[nWidth * nHeight * nChannels];
			memset(pPixelData, 0, nWidth * nHeight * nChannels * sizeof(uint8_t));
		}

		void Load(const std::string& sFilename)
		{
			nChannels = 4;
			pPixelData = stbi_load(sFilename.c_str(), &nWidth, &nHeight, nullptr, nChannels);
			DGE_ASSERT(pPixelData, "[stb_image Error] Text: " << stbi_failure_reason())
		}

		enum class FileType { BMP, PNG, JPG, TGA, TGA_RLE };

		void Save(const std::string& sFilename, const FileType type)
		{
			int err;

			switch (type)
			{
			case FileType::BMP: err = stbi_write_bmp(sFilename.c_str(), nWidth, nHeight, nChannels, pPixelData); break;
			case FileType::PNG: err = stbi_write_png(sFilename.c_str(), nWidth, nHeight, nChannels, pPixelData, nWidth * nChannels); break;
			case FileType::JPG: err = stbi_write_jpg(sFilename.c_str(), nWidth, nHeight, nChannels, pPixelData, 100); break;
			case FileType::TGA: err = stbi_write_tga(sFilename.c_str(), nWidth, nHeight, nChannels, pPixelData); break;
			case FileType::TGA_RLE:
			{
				stbi_write_tga_with_rle = 1;
				err = stbi_write_tga(sFilename.c_str(), nWidth, nHeight, nChannels, pPixelData);
				stbi_write_tga_with_rle = 0;
			}
			break;

			}

			DGE_ASSERT(err == 1, "[stb_image_write Error] Code: " << err)
		}

		bool SetPixel(const int32_t x, const int32_t y, const Pixel& p)
		{
			if (x >= 0 && y >= 0 && x < nWidth && y < nHeight)
			{
				size_t i = nChannels * (y * nWidth + x);

				pPixelData[i] = p.r;
				pPixelData[i + 1] = p.g;
				pPixelData[i + 2] = p.b;
				pPixelData[i + 3] = p.a;

				return true;
			}

			return false;
		}

		Pixel GetPixel(const int32_t x, const int32_t y)
		{
			if (x >= 0 && y >= 0 && x < nWidth && y < nHeight)
			{
				size_t i = nChannels * (y * nWidth + x);

				return Pixel(
					pPixelData[i],
					pPixelData[i + 1],
					pPixelData[i + 2],
					pPixelData[i + 3]
				);
			}
			else
			{
				return BLACK;
			}
		}

		void SetPixelData(const uint8_t* data)
		{
			memcpy(pPixelData, data, nWidth * nHeight * nChannels * sizeof(uint8_t));
		}

		void SetPixelData(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a = 255)
		{
			for (int i = 0; i < nWidth; i++)
				for (int j = 0; j < nHeight; j++)
				{
					size_t idx = nChannels * (j * nWidth + i);
					pPixelData[idx] = r;
					pPixelData[idx + 1] = g;
					pPixelData[idx + 2] = b;
					pPixelData[idx + 3] = a;
				}
		}

		Pixel Sample(const float x, const float y)
		{
			return GetPixel(
				std::min(int32_t(x * (float)nWidth), nWidth - 1),
				std::min(int32_t(y * (float)nHeight), nHeight - 1)
			);
		}
	};

	class Texture
	{
	public:
		Texture(Sprite* pSprite)
		{
			Construct(pSprite, false);
		}

		Texture(const std::string& sFileName)
		{
			Construct(new Sprite(sFileName), true);
		}

		enum TextureStructure : int32_t
		{
			DEFAULT,
			FAN,
			STRIP
		};

	private:
		void Construct(Sprite* pSprite, bool bDeleteSprite)
		{
			Load(pSprite);

			fUVScaleX = 1.0f / (float)pSprite->nWidth;
			fUVScaleY = 1.0f / (float)pSprite->nHeight;

			nWidth = pSprite->nWidth;
			nHeight = pSprite->nHeight;

			if (bDeleteSprite) delete pSprite;
		}

	public:
		GLuint nTexId;

		float fUVScaleX;
		float fUVScaleY;

		int32_t nWidth;
		int32_t nHeight;

	public:
		void Load(Sprite* pSprite)
		{
			GLenum nFormat = 0;

			switch (pSprite->nChannels)
			{
			case 3: nFormat = GL_RGB; break;
			case 4: nFormat = GL_RGBA; break;
			}

			DGE_ASSERT(nFormat != 0, "[Texture.Load Error] Text: Invalid number of channels")

			glGenTextures(1, &nTexId);
			glBindTexture(GL_TEXTURE_2D, nTexId);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				nFormat,
				pSprite->nWidth,
				pSprite->nHeight,
				0,
				nFormat,
				GL_UNSIGNED_BYTE,
				pSprite->pPixelData
			);

			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void Update(Sprite* pSprite)
		{
			glBindTexture(GL_TEXTURE_2D, nTexId);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pSprite->nWidth, pSprite->nHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pSprite->pPixelData);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	};

	struct Graphic
	{
		Graphic() = default;
		Graphic(const std::string& sFileName) { Load(sFileName); }
		Graphic(const int32_t nWidth, const int32_t nHeight) { Load(nWidth, nHeight); }

		~Graphic()
		{
			if (pTexture != nullptr) delete pTexture;
			if (pSprite != nullptr)  delete pSprite;
		}

		Texture* pTexture = nullptr;
		Sprite* pSprite = nullptr;

		void Load(const std::string& sFileName)
		{
			pSprite = new Sprite(sFileName);
			pTexture = new Texture(pSprite);
		}

		void Load(const int32_t nWidth, const int32_t nHeight)
		{
			pSprite = new Sprite(nWidth, nHeight);
			pTexture = new Texture(pSprite);
		}

		void Save(const std::string& sFilename, const Sprite::FileType type)
		{
			return pSprite->Save(sFilename, type);
		}

		void UpdateTexture() { pTexture->Update(pSprite); }
	};

	enum class WindowState
	{
		NONE,
		MAXIMIZED,
		FOCUSED
	};

	struct TextureInstance
	{
		Texture* tex = nullptr;

		int32_t structure = Texture::FAN;
		int32_t points = 0;

		std::vector<Pixel> tint;
		std::vector<vf2d> vert;
		std::vector<vf2d> uv;
	};

	class GameEngine
	{
	public:
		GameEngine();
		virtual ~GameEngine();

	private:
		std::string m_sAppName;

		vi2d m_vWindowSize;
		vi2d m_vScreenSize;
		vf2d m_vInvScreenSize;
		vi2d m_vPixelSize;

		GLFWwindow* m_Window;
		GLFWmonitor* m_Monitor;

		bool m_bAppRunning;
		bool m_bFullScreen;
		bool m_bVSync;
		bool m_bDirtyPixel;

		KeyState m_Keys[512];
		KeyState m_Mouse[5];

		bool m_nKeyOldState[512];
		bool m_nKeyNewState[512];

		bool m_nMouseOldState[5];
		bool m_nMouseNewState[5];

		vi2d m_vMouse;

		Sprite* m_sprFont;

		Graphic* m_pScreen;
		Graphic* m_pDrawTarget;
		std::vector<TextureInstance> m_vecTextures;
		Pixel m_pixTint;

		int32_t m_nPixelMode;
		int32_t m_nTextureStructure;

		float m_fTickTimer;

		Pixel(*m_funcShader)(const vi2d&, const Pixel&, const Pixel&) = nullptr;

	public:
		virtual bool OnUserCreate() = 0;
		virtual bool OnUserUpdate(float fDeltaTime) = 0;

		void Construct(int32_t nScreenWidth, int32_t nScreenHeight, int32_t nPixelWidth, int32_t nPixelHeight, bool bFullScreen = false, bool bVSync = false, bool bDirtyPixel = false);
		void Run();

	private:
		void Destroy();
		bool AppThread();

		static void DrawQuad(const Pixel& pixTint);
		static void ErrorCallback(int nErvoid, const char* sDesc);

	public:
		bool Draw(const vi2d& pos, Pixel p = WHITE);
		virtual bool Draw(int32_t x, int32_t y, Pixel p = WHITE);

		void DrawLine(const vi2d& pos1, const vi2d& pos2, const Pixel& p = WHITE);
		virtual void DrawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const Pixel& p = WHITE);

		void DrawTriangle(const vi2d& pos1, const vi2d& pos2, const vi2d& pos3, const Pixel& p = WHITE);
		virtual void DrawTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, const Pixel& p = WHITE);

		void FillTriangle(const vi2d& pos1, const vi2d& pos2, const vi2d& pos3, const Pixel& p = WHITE);
		virtual void FillTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, const Pixel& p = WHITE);

		void DrawRectangle(const vi2d& pos, const vi2d& size, const Pixel& p = WHITE);
		virtual void DrawRectangle(int32_t x, int32_t y, int32_t sx, int32_t sy, const Pixel& p = WHITE);

		void FillRectangle(const vi2d& pos, const vi2d& size, const Pixel& p = WHITE);
		virtual void FillRectangle(int32_t x, int32_t y, int32_t sx, int32_t sy, const Pixel& p = WHITE);

		void DrawCircle(const vi2d& pos, int32_t radius, const Pixel& p = WHITE);
		virtual void DrawCircle(int32_t x, int32_t y, int32_t radius, const Pixel& p = WHITE);

		void FillCircle(const vi2d& pos, int32_t radius, const Pixel& p = WHITE);
		virtual void FillCircle(int32_t x, int32_t y, int32_t radius, const Pixel& p = WHITE);
		
		void DrawEllipse(const vi2d& pos, const vi2d& size, const Pixel& p = WHITE);
		virtual void DrawEllipse(int x, int y, int sx, int sy, const Pixel& p = def::WHITE);

		void FillEllipse(const vi2d& pos, const vi2d& size, const Pixel& p = WHITE);
		virtual void FillEllipse(int x, int y, int sx, int sy, const Pixel& p = def::WHITE);

		void DrawSprite(const vi2d& pos, Sprite* sprite);
		virtual void DrawSprite(int32_t x, int32_t y, Sprite* sprite);

		void DrawPartialSprite(const vi2d& pos, const vi2d& fpos, const vi2d& fsize, Sprite* sprite);
		virtual void DrawPartialSprite(int32_t x, int32_t y, int32_t fx, int32_t fy, int32_t fsizeX, int32_t fsizeY, Sprite* sprite);

		void DrawTexture(const vf2d& pos, Texture* tex, const vf2d& scale = { 1.0f, 1.0f }, const Pixel& tint = WHITE);
		virtual void DrawTexture(float x, float y, Texture* tex, float scaleX = 1.0f, float scaleY = 1.0f, const Pixel& tint = WHITE);

		void DrawPartialTexture(const vf2d& pos, const vi2d& filePos, const vi2d& fileSize, Texture* tex, const vf2d& scale = { 1.0f, 1.0f }, const Pixel& tint = WHITE);
		virtual void DrawPartialTexture(float x, float y, float filePosX, float filePosY, float fileSizeX, float fileSizeY, Texture* tex, float scaleX = 1.0f, float scaleY = 1.0f, const Pixel& tint = WHITE);

		virtual void DrawWarpedTexture(const std::vector<vf2d>& points, Texture* tex, const Pixel& tint = WHITE);

		void DrawRotatedTexture(const vf2d& pos, float r, Texture* tex, const vf2d& center = { 0.0f, 0.0f }, const vf2d& scale = { 1.0f, 1.0f }, const Pixel& tint = WHITE);
		virtual void DrawRotatedTexture(float x, float y, float r, Texture* tex, float centerX = 0.0f, float centerY = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f, const Pixel& tint = WHITE);

		void DrawWireFrameModel(const std::vector<vf2d>& vecModelCoordinates, const vf2d& pos, float r = 0.0f, float s = 1.0f, const Pixel& p = WHITE);
		virtual void DrawWireFrameModel(const std::vector<vf2d>& vecModelCoordinates, float x, float y, float r = 0.0f, float s = 1.0f, const Pixel& p = WHITE);

		void FillWireFrameModel(const std::vector<vf2d>& vecModelCoordinates, const vf2d& pos, float r = 0.0f, float s = 1.0f, const Pixel& p = WHITE);
		virtual void FillWireFrameModel(const std::vector<vf2d>& vecModelCoordinates, float x, float y, float r = 0.0f, float s = 1.0f, const Pixel& p = WHITE);

		void DrawString(const vi2d& pos, const std::string& text, const Pixel& p = WHITE);
		virtual void DrawString(int32_t x, int32_t y, const std::string& text, const Pixel& p = WHITE);

		virtual void Clear(const Pixel& p);

		KeyState GetKey(uint32_t k);
		KeyState GetMouse(uint32_t k);

		vi2d GetMouse();

		int32_t MouseX();
		int32_t MouseY();

		void SetTitle(const std::string& sTitle);

		vi2d ScreenSize();

		int32_t ScreenWidth();
		int32_t ScreenHeight();

		bool IsFullScreen();
		bool IsVSync();

		void SetIcon(const std::string& sFileName);

		void SetDrawTarget(Graphic* pTarget);
		Graphic* GetDrawTarget();

		WindowState GetWindowState();
		GLFWwindow* GetWindow();

		void DrawTexture(const TextureInstance& tex);

		void SetPixelMode(int32_t nPixelMode);
		int32_t GetPixelMode();

		void SetTextureStructure(int32_t nTextureStructure);
		int32_t GetTextureStructure(int32_t nTextureStructure);

		void ClearBuffer(const Pixel& p);

		void SetTint(const Pixel& p);

		void SetShader(Pixel(*func)(const vi2d& vPos, const Pixel& pixPrev, const Pixel& pixCur));

	};

#ifdef DGE_APPLICATION
#undef DGE_APPLICATION

	GameEngine::GameEngine()
	{
		m_sAppName = "Undefined";
		m_vMouse = { -1, -1 };

		m_Window = nullptr;
		m_Monitor = nullptr;

		m_sprFont = nullptr;

		m_pDrawTarget = nullptr;
		m_pixTint = { 255, 255, 255, 255 };

		m_nPixelMode = Pixel::DEFAULT;
		m_nTextureStructure = Texture::FAN;

		m_fTickTimer = 0.0f;
	}

	GameEngine::~GameEngine() { Destroy(); }

	void GameEngine::Destroy()
	{
		SetDrawTarget(m_pScreen);

		if (m_pDrawTarget != nullptr) delete m_pDrawTarget;
		if (m_sprFont != nullptr) delete m_sprFont;

		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	bool GameEngine::AppThread()
	{
		if (!OnUserCreate())
		{
			m_bAppRunning = false;
			return false;
		}

		auto tp1 = std::chrono::system_clock::now();
		auto tp2 = tp1;

		for (int i = 0; i < 512; i++)
		{
			m_Keys[i] = { false, false, false };
			m_nKeyOldState[i] = 0;
			m_nKeyNewState[i] = 0;
		}

		for (int i = 0; i < 5; i++)
		{
			m_Mouse[i] = { false, false, false };
			m_nMouseOldState[i] = 0;
			m_nMouseNewState[i] = 0;
		}

		std::string sTitle = "github.com/defini7 - defGameEngine - " + m_sAppName + " - FPS: 0";
		glfwSetWindowTitle(m_Window, sTitle.c_str());

		while (m_bAppRunning)
		{
			tp2 = std::chrono::system_clock::now();
			float fDeltaTime = std::chrono::duration<float>(tp2 - tp1).count();
			tp1 = tp2;

			m_bAppRunning = !glfwWindowShouldClose(m_Window);

			for (int i = 0; i < 512; i++)
			{
				m_nKeyNewState[i] = glfwGetKey(m_Window, i);

				m_Keys[i].bPressed = false;
				m_Keys[i].bReleased = false;

				if (m_nKeyNewState[i] != m_nKeyOldState[i])
				{
					if (m_nKeyNewState[i])
					{
						m_Keys[i].bPressed = !m_Keys[i].bHeld;
						m_Keys[i].bHeld = true;
					}
					else
					{
						m_Keys[i].bReleased = true;
						m_Keys[i].bHeld = false;
					}
				}

				m_nKeyOldState[i] = m_nKeyNewState[i];
			}

			for (int i = 0; i < 5; i++)
			{
				m_nMouseNewState[i] = glfwGetMouseButton(m_Window, i);

				m_Mouse[i].bPressed = false;
				m_Mouse[i].bReleased = false;

				if (m_nMouseNewState[i] != m_nMouseOldState[i])
				{
					if (m_nMouseNewState[i])
					{
						m_Mouse[i].bPressed = true;
						m_Mouse[i].bHeld = true;
					}
					else
					{
						m_Mouse[i].bReleased = true;
						m_Mouse[i].bHeld = false;
					}
				}

				m_nMouseOldState[i] = m_nMouseNewState[i];
			}

			double dMouseX, dMouseY;
			glfwGetCursorPos(m_Window, &dMouseX, &dMouseY);

			m_vMouse = vi2d(dMouseX, dMouseY) / m_vPixelSize;

			if (!OnUserUpdate(fDeltaTime))
				m_bAppRunning = false;

			ClearBuffer(BLACK);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			m_pDrawTarget->UpdateTexture();
			glBindTexture(GL_TEXTURE_2D, m_pDrawTarget->pTexture->nTexId);

			DrawQuad(m_pixTint);

			for (const auto& t : m_vecTextures) DrawTexture(t);
			m_vecTextures.clear();

			if (m_bVSync) glfwSwapBuffers(m_Window);
			else		  glFlush();

			glfwPollEvents();

			m_fTickTimer += fDeltaTime;
			if (m_fTickTimer >= 1.0f)
			{
				sTitle = "github.com/defini7 - defGameEngine - " + m_sAppName + " - FPS: " + std::to_string(int(1.0f / fDeltaTime));
				glfwSetWindowTitle(m_Window, sTitle.c_str());
				m_fTickTimer = 0.0f;
			}
		}

		return true;
	}

	void GameEngine::DrawTexture(const TextureInstance& ti)
	{
		if (ti.tex == nullptr) glBindTexture(GL_TEXTURE_2D, 0);
		else				   glBindTexture(GL_TEXTURE_2D, ti.tex->nTexId);

		switch (ti.structure)
		{
		case Texture::DEFAULT: glBegin(GL_TRIANGLES);		break;
		case Texture::FAN:		glBegin(GL_TRIANGLE_FAN);	break;
		case Texture::STRIP:	glBegin(GL_TRIANGLE_STRIP);	break;
		}

		for (int32_t i = 0; i < ti.points; i++)
		{
			glColor4ub(ti.tint[i].r, ti.tint[i].g, ti.tint[i].b, ti.tint[i].a);
			glTexCoord2f(ti.uv[i].x, ti.uv[i].y);
			glVertex2f(ti.vert[i].x, ti.vert[i].y);
		}

		glEnd();
	}

	void GameEngine::DrawQuad(const Pixel& pixTint)
	{
		glBegin(GL_QUADS);
		glColor4ub(pixTint.r, pixTint.g, pixTint.b, pixTint.a);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, 1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, 1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, -1.0f);
		glEnd();
	}

	void GameEngine::Run()
	{
		m_bAppRunning = true;
		AppThread();
	}

	void GameEngine::ErrorCallback(int nErrCode, const char* sDesc)
	{
		if (nErrCode != GLFW_INVALID_ENUM)
		{
			std::cout << "[GLFW Error] Code: "
				<< "0x000" << std::hex << nErrCode
				<< ", text: " << sDesc << std::endl;

			exit(1);
		}
	}

	void GameEngine::Construct(int32_t nScreenWidth, int32_t nScreenHeight, int32_t nPixelWidth, int32_t nPixelHeight, bool bFullScreen, bool bVSync, bool bDirtyPixel)
	{
		glfwSetErrorCallback(ErrorCallback);

		if (glfwInit() == 0) return;

		m_vWindowSize = { nScreenWidth * nPixelWidth, nScreenHeight * nPixelHeight };
		m_vScreenSize = { nScreenWidth, nScreenHeight };
		m_vInvScreenSize = 1.0f / vf2d(m_vScreenSize);
		m_vPixelSize = { nPixelWidth, nPixelHeight };

		m_bFullScreen = bFullScreen;
		m_bVSync = bVSync;

		m_bDirtyPixel = bDirtyPixel;

		m_Monitor = glfwGetPrimaryMonitor();
		if (!m_Monitor) return;

		const GLFWvidmode* videoMode = glfwGetVideoMode(m_Monitor);
		if (!videoMode) return;

		// On some laptops with integrated graphics VSync does not work at all!
		if (m_bVSync)
		{
			glfwSwapInterval(1);
			glfwWindowHint(GLFW_REFRESH_RATE, videoMode->refreshRate);
		}
		else
		{
			glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE);
		}

		if (m_bFullScreen)
		{
			m_vWindowSize = vi2d(videoMode->width, videoMode->height);
			m_vScreenSize = m_vWindowSize / m_vPixelSize;

			m_Window = glfwCreateWindow(m_vWindowSize.x, m_vWindowSize.y, "", m_Monitor, NULL);
			if (!m_Window) return;

			glfwSetWindowMonitor(m_Window, m_Monitor,
				0, 0, m_vWindowSize.x, m_vWindowSize.y, videoMode->refreshRate);
		}
		else
		{
			m_Window = glfwCreateWindow(m_vWindowSize.x, m_vWindowSize.y, "", NULL, NULL);
			if (!m_Window) return;
		}

		glfwMakeContextCurrent(m_Window);
		glViewport(0, 0, m_vWindowSize.x, m_vWindowSize.y);

		glEnable(GL_TEXTURE_2D);
		if (!m_bDirtyPixel) glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		m_pScreen = new Graphic(m_vScreenSize.x, m_vScreenSize.y);
		m_pDrawTarget = m_pScreen;
		Clear(BLACK);

		std::string data =
			"?Q`0001oOch0o01o@F40o0<AGD4090LAGD<090@A7ch0?00O7Q`0600>00000000"
			"O000000nOT0063Qo4d8>?7a14Gno94AA4gno94AaOT0>o3`oO400o7QN00000400"
			"Of80001oOg<7O7moBGT7O7lABET024@aBEd714AiOdl717a_=TH013Q>00000000"
			"720D000V?V5oB3Q_HdUoE7a9@DdDE4A9@DmoE4A;Hg]oM4Aj8S4D84@`00000000"
			"OaPT1000Oa`^13P1@AI[?g`1@A=[OdAoHgljA4Ao?WlBA7l1710007l100000000"
			"ObM6000oOfMV?3QoBDD`O7a0BDDH@5A0BDD<@5A0BGeVO5ao@CQR?5Po00000000"
			"Oc``000?Ogij70PO2D]??0Ph2DUM@7i`2DTg@7lh2GUj?0TO0C1870T?00000000"
			"70<4001o?P<7?1QoHg43O;`h@GT0@:@LB@d0>:@hN@L0@?aoN@<0O7ao0000?000"
			"OcH0001SOglLA7mg24TnK7ln24US>0PL24U140PnOgl0>7QgOcH0K71S0000A000"
			"00H00000@Dm1S007@DUSg00?OdTnH7YhOfTL<7Yh@Cl0700?@Ah0300700000000"
			"<008001QL00ZA41a@6HnI<1i@FHLM81M@@0LG81?O`0nC?Y7?`0ZA7Y300080000"
			"O`082000Oh0827mo6>Hn?Wmo?6HnMb11MP08@C11H`08@FP0@@0004@000000000"
			"00P00001Oab00003OcKP0006@6=PMgl<@440MglH@000000`@000001P00000000"
			"Ob@8@@00Ob@8@Ga13R@8Mga172@8?PAo3R@827QoOb@820@0O`0007`0000007P0"
			"O`000P08Od400g`<3V=P0G`673IP0`@3>1`00P@6O`P00g`<O`000GP800000000"
			"?P9PL020O`<`N3R0@E4HC7b0@ET<ATB0@@l6C4B0O`H3N7b0?P01L3R000000020";

		m_sprFont = new Sprite(128, 48);
		int px = 0, py = 0;

		for (size_t b = 0; b < 1024; b += 4)
		{
			uint32_t sym1 = (uint32_t)data[b + 0] - 48;
			uint32_t sym2 = (uint32_t)data[b + 1] - 48;
			uint32_t sym3 = (uint32_t)data[b + 2] - 48;
			uint32_t sym4 = (uint32_t)data[b + 3] - 48;
			uint32_t r = sym1 << 18 | sym2 << 12 | sym3 << 6 | sym4;

			for (int i = 0; i < 24; i++)
			{
				uint8_t k = (r & (1 << i)) ? 255 : 0;
				m_sprFont->SetPixel(px, py, Pixel(k, k, k, k));
				if (++py == 48) { px++; py = 0; }
			}
		}
	}

	bool GameEngine::Draw(int32_t x, int32_t y, Pixel p)
	{
		if (!m_pDrawTarget) return false;
		Sprite* target = m_pDrawTarget->pSprite;

		if (m_nPixelMode == Pixel::CUSTOM)
		{
			return target->SetPixel(x, y, m_funcShader({ x, y }, target->GetPixel(x, y), p));
		}

		else if (m_nPixelMode == Pixel::DEFAULT)
		{
			return target->SetPixel(x, y, p);
		}

		else if (m_nPixelMode == Pixel::MASK)
		{
			if (p.a == 255) return target->SetPixel(x, y, p);
		}

		else if (m_nPixelMode == Pixel::ALPHA)
		{
			Pixel d = target->GetPixel(x, y);
			float a = (float)(p.a / 255.0f);
			float c = 1.0f - a;
			float r = a * (float)p.r + c * (float)d.r;
			float g = a * (float)p.g + c * (float)d.g;
			float b = a * (float)p.b + c * (float)d.b;
			return target->SetPixel(x, y, Pixel((uint8_t)r, (uint8_t)g, (uint8_t)b));
		}

		return false;
	}

	void GameEngine::DrawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const Pixel& p)
	{
		int32_t x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;

		dx = x2 - x1;
		dy = y2 - y1;

		dx1 = abs(dx);
		dy1 = abs(dy);

		px = 2 * dy1 - dx1;
		py = 2 * dx1 - dy1;

		if (dy1 <= dx1)
		{
			if (dx >= 0)
			{
				x = x1;
				y = y1;
				xe = x2;
			}
			else
			{
				x = x2;
				y = y2;
				xe = x1;
			}

			Draw(x, y, p);

			for (i = 0; x < xe; i++)
			{
				x++;

				if (px < 0)
					px = px + 2 * dy1;
				else
				{
					if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
						y = y + 1;
					else
						y = y - 1;

					px = px + 2 * (dy1 - dx1);
				}

				Draw(x, y, p);
			}
		}
		else
		{
			if (dy >= 0)
			{
				x = x1;
				y = y1;
				ye = y2;
			}
			else
			{
				x = x2;
				y = y2;
				ye = y1;
			}

			Draw(x, y, p);

			for (i = 0; y < ye; i++)
			{
				y = y + 1;

				if (py <= 0)
					py = py + 2 * dx1;
				else
				{
					if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
						x = x + 1;
					else
						x = x - 1;

					py = py + 2 * (dx1 - dy1);
				}

				Draw(x, y, p);
			}
		}
	}

	void GameEngine::DrawTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, const Pixel& p)
	{
		DrawLine(x1, y1, x2, y2, p);
		DrawLine(x2, y2, x3, y3, p);
		DrawLine(x3, y3, x1, y1, p);
	}

	void GameEngine::FillTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, const Pixel& p)
	{
		auto drawline = [&](int32_t sx, int32_t ex, int32_t ny) { for (int i = sx; i <= ex; i++) Draw(i, ny, p); };

		int32_t t1x, t2x, y, minx, maxx, t1xp, t2xp;

		bool changed1 = false;
		bool changed2 = false;

		int32_t signx1, signx2, dx1, dy1, dx2, dy2;
		int32_t e1, e2;

		if (y1 > y2) { std::swap(y1, y2); std::swap(x1, x2); }
		if (y1 > y3) { std::swap(y1, y3); std::swap(x1, x3); }
		if (y2 > y3) { std::swap(y2, y3); std::swap(x2, x3); }

		t1x = t2x = x1; y = y1;
		dx1 = (int32_t)(x2 - x1);

		if (dx1 < 0)
		{
			dx1 = -dx1;
			signx1 = -1;
		}
		else
		{
			signx1 = 1;
		}

		dy1 = (int32_t)(y2 - y1);
		dx2 = (int32_t)(x3 - x1);

		if (dx2 < 0)
		{
			dx2 = -dx2;
			signx2 = -1;
		}
		else
		{
			signx2 = 1;
		}

		dy2 = (int32_t)(y3 - y1);

		if (dy1 > dx1)
		{
			std::swap(dx1, dy1);
			changed1 = true;
		}

		if (dy2 > dx2)
		{
			std::swap(dy2, dx2);
			changed2 = true;
		}

		e2 = (int32_t)(dx2 >> 1);
		if (y1 == y2)  goto next;
		e1 = (int32_t)(dx1 >> 1);

		for (int i = 0; i < dx1;)
		{
			t1xp = 0; t2xp = 0;
			if (t1x < t2x)
			{
				minx = t1x;
				maxx = t2x;
			}
			else
			{
				minx = t2x;
				maxx = t1x;
			}

			while (i < dx1)
			{
				i++;
				e1 += dy1;

				while (e1 >= dx1)
				{
					e1 -= dx1;

					if (changed1)
						t1xp = signx1;
					else
						goto next1;
				}

				if (changed1)
					break;
				else
					t1x += signx1;
			}

		next1:
			while (1)
			{
				e2 += dy2;
				while (e2 >= dx2)
				{
					e2 -= dx2;
					if (changed2)
						t2xp = signx2;
					else
						goto next2;
				}

				if (changed2)
					break;
				else
					t2x += signx2;
			}

		next2:
			if (minx > t1x) minx = t1x;
			if (minx > t2x) minx = t2x;
			if (maxx < t1x) maxx = t1x;
			if (maxx < t2x) maxx = t2x;

			drawline(minx, maxx, y);
			if (!changed1) t1x += signx1;

			t1x += t1xp;
			if (!changed2) t2x += signx2;

			t2x += t2xp;
			y += 1;

			if (y == y2) break;
		}

	next:
		dx1 = (int32_t)(x3 - x2);

		if (dx1 < 0)
		{
			dx1 = -dx1;
			signx1 = -1;
		}
		else
		{
			signx1 = 1;
		}

		dy1 = (int32_t)(y3 - y2);
		t1x = x2;

		if (dy1 > dx1)
		{
			std::swap(dy1, dx1);
			changed1 = true;
		}
		else
		{
			changed1 = false;
		}

		e1 = (int32_t)(dx1 >> 1);

		for (int i = 0; i <= dx1; i++)
		{
			t1xp = 0;
			t2xp = 0;

			if (t1x < t2x)
			{
				minx = t1x;
				maxx = t2x;
			}
			else
			{
				minx = t2x;
				maxx = t1x;
			}

			while (i < dx1)
			{
				e1 += dy1;

				while (e1 >= dx1)
				{
					e1 -= dx1;
					if (changed1)
					{
						t1xp = signx1;
						break;
					}
					else
					{
						goto next3;
					}
				}

				if (changed1)
					break;
				else
					t1x += signx1;

				if (i < dx1) i++;
			}

		next3:
			while (t2x != x3)
			{
				e2 += dy2;

				while (e2 >= dx2)
				{
					e2 -= dx2;

					if (changed2)
						t2xp = signx2;
					else
						goto next4;
				}

				if (changed2)
					break;
				else
					t2x += signx2;
			}

		next4:
			if (minx > t1x) minx = t1x;
			if (minx > t2x) minx = t2x;
			if (maxx < t1x) maxx = t1x;
			if (maxx < t2x) maxx = t2x;

			drawline(minx, maxx, y);
			if (!changed1) t1x += signx1;

			t1x += t1xp;
			if (!changed2) t2x += signx2;

			t2x += t2xp;
			y += 1;

			if (y > y3) return;
		}
	}

	void GameEngine::DrawRectangle(int32_t x, int32_t y, int32_t sx, int32_t sy, const Pixel& p)
	{
		for (int i = 0; i < sx; i++)
		{
			Draw(x + i, y, p);
			Draw(x + i, y + sy, p);
		}

		for (int j = 0; j < sy; j++)
		{
			Draw(x, y + j, p);
			Draw(x + sx, y + j, p);
		}

		Draw(x + sx, y + sy, p);
	}

	void GameEngine::FillRectangle(int32_t x, int32_t y, int32_t sx, int32_t sy, const Pixel& p)
	{
		for (int i = x; i < x + sx; i++)
			for (int j = y; j < y + sy; j++)
				Draw(i, j, p);
	}

	void GameEngine::DrawCircle(int32_t x, int32_t y, int32_t r, const Pixel& p)
	{
		int32_t x1 = 0;
		int32_t y1 = r;
		int32_t p1 = 3 - 2 * r;

		while (y1 >= x1)
		{
			Draw(x - x1, y - y1, p);
			Draw(x - y1, y - x1, p);
			Draw(x + y1, y - x1, p);
			Draw(x + x1, y - y1, p);
			Draw(x - x1, y + y1, p);
			Draw(x - y1, y + x1, p);
			Draw(x + y1, y + x1, p);
			Draw(x + x1, y + y1, p);

			if (p1 < 0) p1 += 4 * x1++ + 6;
			else p1 += 4 * (x1++ - y1--) + 10;
		}
	}

	void GameEngine::FillCircle(int32_t x, int32_t y, int32_t r, const Pixel& p)
	{
		int32_t x1 = 0;
		int32_t y1 = r;
		int32_t p1 = 3 - 2 * r;

		auto drawline = [&](int32_t sx, int32_t ex, int32_t ny)
		{
			for (int i = sx; i <= ex; i++) Draw(i, ny, p);
		};

		while (y1 >= x1)
		{
			drawline(x - x1, x + x1, y - y1);
			drawline(x - y1, x + y1, y - x1);
			drawline(x - x1, x + x1, y + y1);
			drawline(x - y1, x + y1, y + x1);

			if (p1 < 0) p1 += 4 * x1++ + 6;
			else p1 += 4 * (x1++ - y1--) + 10;
		}
	}

	void GameEngine::DrawEllipse(int32_t x, int32_t y, int32_t sx, int32_t sy, const Pixel& p)
	{
		int x1 = x + sx;
		int y1 = y + sy;

		int a = abs(x1 - x);
		int b = abs(y1 - y);
		int b1 = b & 1;

		int dx = 4 * (1 - a) * b * b;
		int dy = 4 * (b1 + 1) * a * a;

		int err = dx + dy + b1 * a * a;

		if (x > x1)
		{
			x = x1;
			x1 += a;
		}

		if (y > y1) y = y1;

		y += (b + 1) / 2;
		y1 = y - b1;

		a *= 8 * a;
		b1 = 8 * b * b;

		do
		{
			Draw(x1, y, p);
			Draw(x, y, p);
			Draw(x, y1, p);
			Draw(x1, y1, p);
			int e2 = 2 * err;
			if (e2 <= dy)
			{
				y++;
				y1--;
				err += dy += a;
			}
			if (e2 >= dx || 2 * err > dy)
			{
				x++;
				x1--;
				err += dx += b1;
			}
		} while (x <= x1);

		while (y - y1 < b)
		{
			Draw(x - 1, y, p);
			Draw(x1 + 1, y++, p);
			Draw(x - 1, y1, p);
			Draw(x1 + 1, y1--, p);
		}
	}

	void GameEngine::FillEllipse(int32_t x, int32_t y, int32_t sx, int32_t sy, const Pixel& p)
	{
		int x1 = x + sx;
		int y1 = y + sy;

		auto drawline = [&](int32_t sx, int32_t ex, int32_t ny)
		{
			for (int i = sx; i <= ex; i++) Draw(i, ny, p);
		};

		int a = abs(x1 - x);
		int b = abs(y1 - y);
		int b1 = b & 1;

		int dx = 4 * (1 - a) * b * b;
		int dy = 4 * (b1 + 1) * a * a;

		int err = dx + dy + b1 * a * a;

		if (x > x1)
		{
			x = x1;
			x1 += a;
		}

		if (y > y1) y = y1;

		y += (b + 1) / 2;
		y1 = y - b1;

		a *= 8 * a;
		b1 = 8 * b * b;

		int z = 0;

		do
		{
			drawline(x, x1, y);
			drawline(x, x1, y1);

			int e2 = 2 * err;
			if (e2 <= dy)
			{
				y++;
				y1--;
				err += dy += a;
			}
			if (e2 >= dx || 2 * err > dy)
			{
				x++;
				x1--;
				err += dx += b1;
			}
		} while (x <= x1);

		while (y - y1 < b)
		{
			drawline(x - 1, x1 + 1, y++);
			drawline(x - 1, x1 + 1, y1--);
		}
	}

	void GameEngine::DrawSprite(int32_t x, int32_t y, Sprite* sprite)
	{
		for (int i = 0; i < sprite->nWidth; i++)
			for (int j = 0; j < sprite->nHeight; j++)
				Draw(x + i, y + j, sprite->GetPixel(i, j));
	}

	void GameEngine::DrawPartialSprite(int32_t x, int32_t y, int32_t fx, int32_t fy, int32_t fsx, int32_t fsy, Sprite* sprite)
	{
		for (int i = fx, x1 = 0; i < fx + fsx; i++, x1++)
			for (int j = fy, y1 = 0; j < fy + fsy; j++, y1++)
				Draw(x + x1, y + y1, sprite->GetPixel(i, j));
	}

	void GameEngine::DrawTexture(float x, float y, Texture* tex, float scaleX, float scaleY, const Pixel& tint)
	{
		vf2d vScreenPos =
		{
			(x * m_vInvScreenSize.x) * 2.0f - 1.0f,
			((y * m_vInvScreenSize.y) * 2.0f - 1.0f) * -1.0f
		};

		vf2d vScreenSize =
		{
			vScreenPos.x + (2.0f * (float(tex->nWidth) * (1.0f / (float)ScreenWidth()))) * scaleX,
			vScreenPos.y - (2.0f * (float(tex->nHeight) * (1.0f / (float)ScreenHeight()))) * scaleY
		};

		TextureInstance ti;
		ti.tex = tex;
		ti.points = 4;
		ti.structure = m_nTextureStructure;
		ti.tint = { tint, tint, tint, tint };
		ti.vert = { vScreenPos, { vScreenPos.x, vScreenSize.y }, vScreenSize, { vScreenSize.x, vScreenPos.y } };
		ti.uv = { { 0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f} };

		m_vecTextures.push_back(ti);
	}

	void GameEngine::DrawPartialTexture(float x, float y, float filePosX, float filePosY, float fileSizeX, float fileSizeY, Texture* tex, float scaleX, float scaleY, const Pixel& tint)
	{
		vf2d vScreenSpacePos =
		{
			  (x * m_vInvScreenSize.x) * 2.0f - 1.0f,
			-((y * m_vInvScreenSize.y) * 2.0f - 1.0f)
		};

		vf2d vScreenSpaceSize =
		{
			  ((x + fileSizeX * scaleX) * m_vInvScreenSize.x) * 2.0f - 1.0f,
			-(((y + fileSizeY * scaleY) * m_vInvScreenSize.y) * 2.0f - 1.0f)
		};

		vf2d vQuantisedPos = ((vScreenSpacePos * vf2d(m_vWindowSize)) + vf2d(0.5f, 0.5f)).floor() / vf2d(m_vWindowSize);
		vf2d vQuantisedSize = ((vScreenSpaceSize * vf2d(m_vWindowSize)) + vf2d(0.5f, -0.5f)).ceil() / vf2d(m_vWindowSize);

		vf2d tl = (vf2d(filePosX, filePosY) + vf2d(0.0001f, 0.0001f)) * vf2d(tex->fUVScaleX, tex->fUVScaleY);
		vf2d br = (vf2d(filePosX, filePosY) + vf2d(fileSizeX, fileSizeY) - vf2d(0.0001f, 0.0001f)) * vf2d(tex->fUVScaleX, tex->fUVScaleY);

		TextureInstance ti;
		ti.tex = tex;
		ti.points = 4;
		ti.structure = m_nTextureStructure;
		ti.tint = { tint, tint, tint, tint };
		ti.vert = { { vQuantisedPos.x, vQuantisedPos.y }, { vQuantisedPos.x, vQuantisedSize.y }, { vQuantisedSize.x, vQuantisedSize.y }, { vQuantisedSize.x, vQuantisedPos.y } };
		ti.uv = { tl, { tl.x, br.y }, br, { br.x, tl.y } };
		m_vecTextures.push_back(ti);
	}

	void GameEngine::DrawRotatedTexture(float x, float y, float r, Texture* tex, float centerX, float centerY, float scaleX, float scaleY, const Pixel& tint)
	{
		TextureInstance ti;
		ti.tex = tex;
		ti.points = 4;
		ti.structure = m_nTextureStructure;
		ti.tint = { tint, tint, tint, tint };

		vf2d vCenter = vf2d(centerX, centerY);
		vf2d vScale = vf2d(scaleX, scaleY);

		ti.vert = {
			(vf2d(0.0f, 0.0f) - vCenter) * vScale,
			(vf2d(0.0f, float(tex->nHeight)) - vCenter) * vScale,
			(vf2d(float(tex->nWidth), float(tex->nHeight)) - vCenter) * vScale,
			(vf2d(float(tex->nWidth), 0.0f) - vCenter) * vScale
		};

		float c = cos(r), s = sin(r);
		for (int i = 0; i < ti.points; i++)
		{
			vf2d o = { ti.vert[i].x * c - ti.vert[i].y * s, ti.vert[i].x * s + ti.vert[i].y * c };
			ti.vert[i] = vf2d(x, y) + o;
			ti.vert[i] = ti.vert[i] * m_vInvScreenSize * 2.0f - vf2d(1.0f, 1.0f);
			ti.vert[i].y *= -1.0f;
		}

		ti.uv = { { 0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f} };
		m_vecTextures.push_back(ti);
	}

	void GameEngine::DrawWarpedTexture(const std::vector<vf2d>& points, Texture* tex, const Pixel& tint)
	{
		TextureInstance di;
		di.tex = tex;
		di.structure = m_nTextureStructure;
		di.points = 4;
		di.tint = { tint, tint, tint, tint };
		di.vert.resize(di.points);
		di.uv = { { 0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f} };

		float rd = ((points[2].x - points[0].x) * (points[3].y - points[1].y) - (points[3].x - points[1].x) * (points[2].y - points[0].y));

		if (rd != 0.0f)
		{
			rd = 1.0f / rd;

			float rn = ((points[3].x - points[1].x) * (points[0].y - points[1].y) - (points[3].y - points[1].y) * (points[0].x - points[1].x)) * rd;
			float sn = ((points[2].x - points[0].x) * (points[0].y - points[1].y) - (points[2].y - points[0].y) * (points[0].x - points[1].x)) * rd;

			vf2d center;
			if (!(rn < 0.0f || rn > 1.0f || sn < 0.0f || sn > 1.0f)) center = points[0] + rn * (points[2] - points[0]);

			float d[4];
			for (int i = 0; i < 4; i++)	d[i] = (points[i] - center).mag();

			for (int i = 0; i < 4; i++)
			{
				float q = d[i] == 0.0f ? 1.0f : (d[i] + d[(i + 2) & 3]) / d[(i + 2) & 3];
				di.uv[i] *= q;
				di.vert[i] = { (points[i].x * m_vInvScreenSize.x) * 2.0f - 1.0f, ((points[i].y * m_vInvScreenSize.y) * 2.0f - 1.0f) * -1.0f };
			}

			m_vecTextures.push_back(di);
		}
	}

	void GameEngine::DrawWireFrameModel(const std::vector<vf2d>& modelCoordinates, float x, float y, float r, float s, const Pixel& p)
	{
		int32_t verts = modelCoordinates.size();

		std::vector<vf2d> vecCoordinates;
		vecCoordinates.resize(verts);

		for (int i = 0; i < verts; i++)
		{
			vecCoordinates[i].x = modelCoordinates[i].x * cosf(r) - modelCoordinates[i].y * sinf(r);
			vecCoordinates[i].y = modelCoordinates[i].x * sinf(r) + modelCoordinates[i].y * cosf(r);
		}

		for (int i = 0; i < verts; i++)
		{
			vecCoordinates[i].x = vecCoordinates[i].x * s;
			vecCoordinates[i].y = vecCoordinates[i].y * s;
		}

		for (int i = 0; i < verts; i++)
		{
			vecCoordinates[i].x = vecCoordinates[i].x + x;
			vecCoordinates[i].y = vecCoordinates[i].y + y;
		}

		for (int i = 0; i < verts + 1; i++)
		{
			int32_t j = (i + 1);
			DrawLine((int32_t)vecCoordinates[i % verts].x, (int32_t)vecCoordinates[i % verts].y,
				(int32_t)vecCoordinates[j % verts].x, (int32_t)vecCoordinates[j % verts].y, p);
		}
	}

	void GameEngine::FillWireFrameModel(const std::vector<vf2d>& modelCoordinates, float x, float y, float r, float s, const Pixel& p)
	{
		int32_t verts = modelCoordinates.size();

		std::vector<vf2d> vecCoordinates;
		vecCoordinates.resize(verts);

		for (int i = 0; i < verts; i++)
		{
			vecCoordinates[i].x = modelCoordinates[i].x * cosf(r) - modelCoordinates[i].y * sinf(r);
			vecCoordinates[i].y = modelCoordinates[i].x * sinf(r) + modelCoordinates[i].y * cosf(r);
		}

		for (int i = 0; i < verts; i++)
		{
			vecCoordinates[i].x = vecCoordinates[i].x * s;
			vecCoordinates[i].y = vecCoordinates[i].y * s;
		}

		for (int i = 0; i < verts; i++)
		{
			vecCoordinates[i].x = vecCoordinates[i].x + x;
			vecCoordinates[i].y = vecCoordinates[i].y + y;
		}

		auto GetAngle = [](const def::vf2d& p1, const def::vf2d& p2)
		{
			float fTheta = atan2(p2.y, p2.x) - atan2(p1.y, p1.x);
			while (fTheta > 3.14159f) fTheta -= 3.14159f * 2.0f;
			while (fTheta < -3.14159f) fTheta += 3.14159f * 2.0f;
			return fTheta;
		};

		auto PointInPolygon = [&](const def::vf2d& p)
		{
			float fAngle = 0.0f;

			def::vf2d p1, p2;
			for (int i = 0; i < verts; i++)
			{
				p1 = vecCoordinates[i] - p;
				p2 = vecCoordinates[(i + 1) % verts] - p;
				fAngle += GetAngle(p1, p2);
			}

			return abs(fAngle) < 3.14159f;
		};

		def::vf2d vMin = vecCoordinates.front();
		def::vf2d vMax = vecCoordinates.front();

		for (int i = 1; i < verts; i++)
		{
			if (vMin.x > vecCoordinates[i].x) vMin.x = vecCoordinates[i].x;
			if (vMin.y > vecCoordinates[i].y) vMin.y = vecCoordinates[i].y;

			if (vMax.x < vecCoordinates[i].x) vMax.x = vecCoordinates[i].x;
			if (vMax.y < vecCoordinates[i].y) vMax.y = vecCoordinates[i].y;
		}

		def::vf2d vPoint;
		for (vPoint.x = vMin.x; vPoint.x < vMax.x; vPoint.x++)
			for (vPoint.y = vMin.y; vPoint.y < vMax.y; vPoint.y++)
				if (PointInPolygon(vPoint))
					Draw(vPoint, p);
	}

	void GameEngine::DrawString(int32_t x, int32_t y, const std::string& s, const Pixel& p)
	{
		int32_t sx = 0;
		int32_t sy = 0;

		for (auto c : s)
		{
			if (c == '\n')
			{
				sx = 0;
				sy += 8;
			}
			else if (c == '\t')
				sx += 32;
			else
			{
				int32_t ox = (c - 32) % 16;
				int32_t oy = (c - 32) / 16;

				for (uint32_t i = 0; i < 8; i++)
					for (uint32_t j = 0; j < 8; j++)
						if (m_sprFont->GetPixel(i + ox * 8, j + oy * 8).r > 0)
							Draw(x + sx + i, y + sy + j, p);

				sx += 8;
			}
		}
	}

	void GameEngine::Clear(const Pixel& p)
	{
		for (int32_t x = 0; x < m_pDrawTarget->pSprite->nWidth; x++)
			for (int32_t y = 0; y < m_pDrawTarget->pSprite->nHeight; y++)
				m_pDrawTarget->pSprite->SetPixel(x, y, p);
	}

	KeyState GameEngine::GetKey(uint32_t k) { return m_Keys[k]; }
	KeyState GameEngine::GetMouse(uint32_t k) { return m_Mouse[k]; }

	int32_t GameEngine::MouseX() { return m_vMouse.x; }
	int32_t GameEngine::MouseY() { return m_vMouse.y; }

	int32_t GameEngine::ScreenWidth() { return m_vScreenSize.x; }
	int32_t GameEngine::ScreenHeight() { return m_vScreenSize.y; }

	bool GameEngine::IsFullScreen() { return m_bFullScreen; }
	bool GameEngine::IsVSync() { return m_bVSync; }

	void GameEngine::SetIcon(const std::string& sFilename)
	{
		Sprite sprIcon(sFilename);

		GLFWimage img;
		img.width = sprIcon.nWidth;
		img.height = sprIcon.nHeight;
		img.pixels = sprIcon.pPixelData;
		glfwSetWindowIcon(m_Window, 1, &img);
	}

	void GameEngine::SetDrawTarget(Graphic* pTarget)
	{
		m_pDrawTarget = pTarget ? pTarget : m_pScreen;
	}

	Graphic* GameEngine::GetDrawTarget() { return m_pDrawTarget; }

	void GameEngine::SetTitle(const std::string& sTitle) { m_sAppName = sTitle; }

	WindowState GameEngine::GetWindowState()
	{
		int32_t f = static_cast<int32_t>(WindowState::NONE);
		if (glfwGetWindowAttrib(m_Window, GLFW_FOCUSED)) f |= static_cast<int32_t>(WindowState::FOCUSED);
		if (glfwGetWindowAttrib(m_Window, GLFW_MAXIMIZED)) f |= static_cast<int32_t>(WindowState::MAXIMIZED);
		return static_cast<WindowState>(f);
	}

	GLFWwindow* GameEngine::GetWindow() { return m_Window; }

	void GameEngine::SetPixelMode(int32_t nPixelMode) { m_nPixelMode = nPixelMode; }
	int32_t GameEngine::GetPixelMode() { return m_nPixelMode; }

	void GameEngine::SetTextureStructure(int32_t nTextureStructure) { m_nTextureStructure = nTextureStructure; }
	int32_t GameEngine::GetTextureStructure(int32_t nTextureStructure) { return m_nTextureStructure; }

	bool GameEngine::Draw(const vi2d& pos, Pixel p)
	{
		return Draw(pos.x, pos.y, p);
	}

	void GameEngine::DrawLine(const vi2d& pos1, const vi2d& pos2, const Pixel& p)
	{
		DrawLine(pos1.x, pos1.y, pos2.x, pos2.y, p);
	}

	void GameEngine::DrawTriangle(const vi2d& pos1, const vi2d& pos2, const vi2d& pos3, const Pixel& p)
	{
		DrawTriangle(pos1.x, pos1.y, pos2.x, pos2.y, pos3.x, pos3.y, p);
	}

	void GameEngine::FillTriangle(const vi2d& pos1, const vi2d& pos2, const vi2d& pos3, const Pixel& p)
	{
		FillTriangle(pos1.x, pos1.y, pos2.x, pos2.y, pos3.x, pos3.y, p);
	}

	void GameEngine::DrawRectangle(const vi2d& pos, const vi2d& size, const Pixel& p)
	{
		DrawRectangle(pos.x, pos.y, size.x, size.y, p);
	}

	void GameEngine::FillRectangle(const vi2d& pos, const vi2d& size, const Pixel& p)
	{
		FillRectangle(pos.x, pos.y, size.x, size.y, p);
	}

	void GameEngine::DrawCircle(const vi2d& pos, int32_t r, const Pixel& p)
	{
		DrawCircle(pos.x, pos.y, r, p);
	}

	void GameEngine::FillCircle(const vi2d& pos, int32_t r, const Pixel& p)
	{
		FillCircle(pos.x, pos.y, r, p);
	}

	void GameEngine::DrawEllipse(const vi2d& pos, const vi2d& size, const Pixel& p)
	{
		DrawEllipse(pos.x, pos.y, size.x, size.y, p);
	}

	void GameEngine::FillEllipse(const vi2d& pos, const vi2d& size, const Pixel& p)
	{
		FillEllipse(pos.x, pos.y, size.x, size.y, p);
	}

	void GameEngine::DrawSprite(const vi2d& pos, Sprite* spr)
	{
		DrawSprite(pos.x, pos.y, spr);
	}

	void GameEngine::DrawPartialSprite(const vi2d& pos, const vi2d& fpos, const vi2d& fsize, Sprite* spr)
	{
		DrawPartialSprite(pos.x, pos.y, fpos.x, fpos.y, fsize.x, fsize.y, spr);
	}

	void GameEngine::DrawTexture(const vf2d& pos, Texture* tex, const vf2d& scale, const Pixel& tint)
	{
		DrawTexture(pos.x, pos.y, tex, scale.x, scale.y, tint);
	}

	void GameEngine::DrawPartialTexture(const vf2d& pos, const vi2d& filePos, const vi2d& fileSize, Texture* tex, const vf2d& scale, const Pixel& tint)
	{
		DrawPartialTexture(pos.x, pos.y, filePos.x, filePos.y, fileSize.x, fileSize.y, tex, scale.x, scale.y, tint);
	}

	void GameEngine::DrawRotatedTexture(const vf2d& pos, float r, Texture* tex, const vf2d& center, const vf2d& scale, const Pixel& tint)
	{
		DrawRotatedTexture(pos.x, pos.y, r, tex, center.x, center.y, scale.x, scale.y, tint);
	}

	void GameEngine::DrawWireFrameModel(const std::vector<vf2d>& vecModelCoordinates, const vf2d& pos, float r, float s, const Pixel& p)
	{
		DrawWireFrameModel(vecModelCoordinates, pos.x, pos.y, r, s, p);
	}

	void GameEngine::FillWireFrameModel(const std::vector<vf2d>& vecModelCoordinates, const vf2d& pos, float r, float s, const Pixel& p)
	{
		FillWireFrameModel(vecModelCoordinates, pos.x, pos.y, r, s, p);
	}

	void GameEngine::DrawString(const vi2d& pos, const std::string& text, const Pixel& p)
	{
		DrawString(pos.x, pos.y, text, p);
	}

	vi2d GameEngine::ScreenSize() { return m_vScreenSize; }
	vi2d GameEngine::GetMouse() { return m_vMouse; }

	void GameEngine::ClearBuffer(const Pixel& p)
	{
		glClearColor(p.r / 255.0f, p.g / 255.0f, p.b / 255.0f, p.a / 255.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void GameEngine::SetTint(const Pixel& p)
	{
		m_pixTint = p;
	}

	void GameEngine::SetShader(Pixel(*func)(const vi2d& vPos, const Pixel& pixPrev, const Pixel& pixCur))
	{
		m_funcShader = func;
		m_nPixelMode = m_funcShader ? Pixel::CUSTOM : Pixel::DEFAULT;
	}

#endif

}
