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

#pragma region sample
/**
* Example:
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
			for (int i = 0; i < GetScreenWidth(); i++)
				for (int j = 0; j < GetScreenHeight(); j++)
					Draw(i, j, def::RandomPixel());

			return true;
		}
	};

	int main()
	{
		Sample demo;

		def::rcode err = demo.Construct(256, 240, 4, 4);

		if (err.ok)
			demo.Run();
		else
			std::cerr << err.info << "\n";

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

#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#ifdef _WIN32

#if defined(_MSVC_LANG) && !defined(_CRT_SECURE_NO_WARNINGS)
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

namespace def
{
	// 3 possible states of window
	enum WindowState
	{
		WS_NONE,
		WS_MAXIMIZED,
		WS_FOCUSED
	};

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

	// Pi constant
	const float PI = 2.0f * acosf(0.0f);

	// Basic 2d vector implementation for convenient usage
	template <typename T>
	struct vec2d_basic
	{
		vec2d_basic() = default;
		vec2d_basic(T x, T y)
		{
			this->x = x;
			this->y = y;
		}

		T x = (T)0;
		T y = (T)0;

		vec2d_basic<T>& operator=(const vec2d_basic<T>& v)
		{
			this->x = v.x;
			this->y = v.y;
			return *this;
		}

		vec2d_basic<T> operator+(const vec2d_basic<T>& v) { return vec2d_basic<T>(this->x + v.x, this->y + v.y); }
		vec2d_basic<T> operator-(const vec2d_basic<T>& v) { return vec2d_basic<T>(this->x - v.x, this->y - v.y); }
		vec2d_basic<T> operator*(const vec2d_basic<T>& v) { return vec2d_basic<T>(this->x * v.x, this->y * v.y); }
		vec2d_basic<T> operator/(const vec2d_basic<T>& v) { return vec2d_basic<T>(this->x / v.x, this->y / v.y); }
		vec2d_basic<T> operator+(const T& v) { return vec2d_basic<T>(this->x + v, this->y + v); }
		vec2d_basic<T> operator-(const T& v) { return vec2d_basic<T>(this->x - v, this->y - v); }
		vec2d_basic<T> operator*(const T& v) { return vec2d_basic<T>(this->x * v, this->y * v); }
		vec2d_basic<T> operator/(const T& v) { return vec2d_basic<T>(this->x / v, this->y / v); }

		vec2d_basic<T>& operator+=(const vec2d_basic<T>& v)
		{
			this->x += v.x;
			this->y += v.y;
			return *this;
		}

		vec2d_basic<T>& operator-=(const vec2d_basic<T>& v)
		{
			this->x -= v.x;
			this->y -= v.y;
			return *this;
		}

		vec2d_basic<T>& operator*=(const vec2d_basic<T>& v)
		{
			this->x *= v.x;
			this->y *= v.y;
			return *this;
		}

		vec2d_basic<T>& operator/=(const vec2d_basic<T>& v)
		{
			this->x /= v.x;
			this->y /= v.y;
			return *this;
		}

		vec2d_basic<T>& operator+=(const T& v)
		{
			this->x += v;
			this->y += v;
			return *this;
		}

		vec2d_basic<T>& operator-=(const T& v)
		{
			this->x -= v;
			this->y -= v;
			return *this;
		}

		vec2d_basic<T>& operator*=(const T& v)
		{
			this->x *= v;
			this->y *= v;
			return *this;
		}

		vec2d_basic<T>& operator/=(const T& v)
		{
			this->x /= v;
			this->y /= v;
			return *this;
		}

		bool operator==(const vec2d_basic<T>& v) { return this->x == v.x && this->y == v.y; }
		bool operator!=(const vec2d_basic<T>& v) { return this->x != v.x || this->y != v.y; }
		bool operator<(const vec2d_basic<T>& v) { return this->x < v.x&& this->y < v.y; }
		bool operator>(const vec2d_basic<T>& v) { return this->x > v.x && this->y > v.y; }
		bool operator<=(const vec2d_basic<T>& v) { return this->x <= v.x && this->y <= v.y; }
		bool operator>=(const vec2d_basic<T>& v) { return this->x >= v.x && this->y >= v.y; }

		friend vec2d_basic<T> operator*(const float& lhs, const vec2d_basic<T>& rhs)
		{
			return vec2d_basic<T>((T)(lhs * (float)rhs.x), (T)(lhs * (float)rhs.y));
		}

		friend vec2d_basic<T> operator * (const double& lhs, const vec2d_basic<T>& rhs)
		{
			return vec2d_basic<T>((T)(lhs * (double)rhs.x), (T)(lhs * (double)rhs.y));
		}

		friend vec2d_basic<T> operator * (const int& lhs, const vec2d_basic<T>& rhs)
		{
			return vec2d_basic<T>((T)(lhs * (int)rhs.x), (T)(lhs * (int)rhs.y));
		}

		friend vec2d_basic<T> operator / (const float& lhs, const vec2d_basic<T>& rhs)
		{
			return vec2d_basic<T>((T)(lhs / (float)rhs.x), (T)(lhs / (float)rhs.y));
		}

		friend vec2d_basic<T> operator / (const double& lhs, const vec2d_basic<T>& rhs)
		{
			return vec2d_basic<T>((T)(lhs / (double)rhs.x), (T)(lhs / (double)rhs.y));
		}

		friend vec2d_basic<T> operator / (const int& lhs, const vec2d_basic<T>& rhs)
		{
			return vec2d_basic<T>((T)(lhs / (int)rhs.x), (T)(lhs / (int)rhs.y));
		}

		operator vec2d_basic<int>()	   const { return { static_cast<int32_t>(this->x),  static_cast<int32_t>(this->y) }; }
		operator vec2d_basic<float>()  const { return { static_cast<float>(this->x),	static_cast<float>(this->y) }; }
		operator vec2d_basic<double>() const { return { static_cast<double>(this->x),	static_cast<double>(this->y) }; }

		vec2d_basic<T> clamp(const vec2d_basic<T>& start, const vec2d_basic<T>& end) const
		{
			vec2d_basic<T> temp;

			temp.x = this->x;
			temp.y = this->y;

			if (temp.x < start.x) temp.x = start.x;
			if (temp.y < start.y) temp.y = start.y;

			if (temp.x > end.x) temp.x = end.x;
			if (temp.y > end.y) temp.y = end.y;

			return temp;
		}

		float dot(vec2d_basic<T> v) { return this->x * v.x + this->y * v.y; }
		float length() { return sqrtf(dot(*this)); }

		T mag() { return static_cast<T>(sqrtf(this->x * this->x + this->y * this->y)); }
		T mag2() { return static_cast<T>(this->x * this->x + this->y * this->y); }

		vec2d_basic<T> norm() { float n = 1.0f / mag(); return vec2d_basic<T>(this->x * n, this->y * n); }
		vec2d_basic<T> abs() { return vec2d_basic<T>(std::abs(this->x), std::abs(this->y)); }
		vec2d_basic<T> perp() { return vec2d_basic<T>(-this->y, this->x); }
		vec2d_basic<T> floor() { return vec2d_basic<T>(std::floor(this->x), std::floor(this->y)); }
		vec2d_basic<T> ceil() { return vec2d_basic<T>(std::ceil(this->x), std::ceil(this->y)); }
		vec2d_basic<T> cart() { return vec2d_basic<T>(cos(this->y) * this->x, sin(this->y) * this->x); }
		vec2d_basic<T> polar() { return vec2d_basic<T>(mag(), atan2(this->y, this->x)); }
		vec2d_basic<T>& ref() { return *this; }

		operator std::string() const { return "(" + std::to_string(this->x) + ", " + std::to_string(this->y) + ")"; }
	};

	typedef vec2d_basic<int> vi2d;
	typedef vec2d_basic<float> vf2d;
	typedef vec2d_basic<double> vd2d;

	// Holds information about error
	struct rcode
	{
		rcode() = default;

		rcode(bool bOk)
		{
			ok = bOk;
			info = "ok";
		}

		rcode(bool bOk, const std::string& sInfo)
		{
			ok = bOk;
			info = sInfo;
		}

		operator bool()
		{
			return ok;
		}

		bool ok;
		std::string info;
	};

	// Holds 3 possible states of each key
	struct KeyState
	{
		bool bHeld;
		bool bReleased;
		bool bPressed;
	};

	struct Pixel
	{
		Pixel() = default;
		Pixel(uint8_t cr, uint8_t cg, uint8_t cb, uint8_t ca = 255U)
		{
			r = cr;
			g = cg;
			b = cb;
			a = ca;
		}

		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;

		template <typename T>
		friend Pixel operator+(const Pixel& lhs, uint8_t rhs) { return Pixel(uint8_t(lhs.r + rhs), uint8_t(lhs.g + rhs), uint8_t(lhs.b + rhs), lhs.a); }

		template <typename T>
		friend Pixel operator-(const Pixel& lhs, uint8_t rhs) { return Pixel(uint8_t(lhs.r - rhs), uint8_t(lhs.g - rhs), uint8_t(lhs.b - rhs), lhs.a); }

		template <typename T>
		friend Pixel operator*(const Pixel& lhs, uint8_t rhs) { return Pixel(uint8_t(lhs.r * rhs), uint8_t(lhs.g * rhs), uint8_t(lhs.b * rhs), lhs.a); }

		template <typename T>
		friend Pixel operator/(const Pixel& lhs, uint8_t rhs) { return Pixel(uint8_t(lhs.r / rhs), uint8_t(lhs.g / rhs), uint8_t((T)rhs.b / rhs), lhs.a); }

		template <typename T>
		friend Pixel operator+=(const Pixel& lhs, uint8_t rhs)
		{
			lhs.r += rhs;
			lhs.g += rhs;
			lhs.b += rhs;

			return lhs;
		}

		template <typename T>
		friend Pixel operator-=(const Pixel& lhs, uint8_t rhs)
		{
			lhs.r -= rhs;
			lhs.g -= rhs;
			lhs.b -= rhs;

			return lhs;
		}

		template <typename T>
		friend Pixel operator*=(const Pixel& lhs, uint8_t rhs)
		{
			lhs.r *= rhs;
			lhs.g *= rhs;
			lhs.b *= rhs;

			return lhs;
		}

		template <typename T>
		friend Pixel operator/=(const Pixel& lhs, uint8_t rhs)
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

		friend Pixel operator+=(const Pixel& lhs, const Pixel& rhs)
		{
			return lhs + rhs;
		}

		friend Pixel operator-=(const Pixel& lhs, const Pixel& rhs)
		{
			return lhs - rhs;
		}

		friend Pixel operator*=(const Pixel& lhs, const Pixel& rhs)
		{
			return lhs * rhs;
		}

		friend Pixel operator/=(const Pixel& lhs, const Pixel& rhs)
		{
			return lhs / rhs;
		}

		friend bool operator==(Pixel& lhs, Pixel& rhs) { return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b; }
		friend bool operator!=(Pixel& lhs, Pixel& rhs) { return lhs.r != rhs.r || lhs.g != rhs.g || lhs.b != rhs.b; }
	};

	def::Pixel PixelF(float r, float g, float b)
	{
		return def::Pixel(uint8_t(r * 255.0f), uint8_t(g * 255.0f), uint8_t(b * 255.0f));
	}

	// Standart colors for convenient usage
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
		CYAN(58, 150, 221, 255),
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

	// Converts byte (0 - 255) to float (0.0f - 1.0f)
	float Byte2Float(uint8_t b)
	{
		return (float)b / 255.0f;
	}

	// Converts float (0.0f - 1.0f) to byte (0 - 255)
	uint8_t Float2Byte(float f)
	{
		return uint8_t(f * 255.0f);
	}


	/********************************
	* @ SPRITE CLASS IMPLEMENTATION *
	********************************/

	class Sprite
	{
	public:
		Sprite() = default;
		Sprite(int32_t w, int32_t h)
		{
			if (w > 0 && h > 0)
				Create(w, h);
		}

		Sprite(const std::string& filename)
		{
			m_sFilename = filename;

			rcode rc = LoadSprite();

			if (!rc.ok)
				std::cerr << rc.info << "\n";

			m_bOk = rc.ok;
		}

		~Sprite()
		{
			stbi_image_free(m_pData);
		}

	private:
		int32_t m_nWidth;
		int32_t m_nHeight;
		int32_t m_nChannels;

		std::string m_sFilename;

		uint8_t* m_pData;

		bool m_bOk = true;

	public:
		void Create(int32_t w, int32_t h)
		{
			m_nWidth = w;
			m_nHeight = h;

			m_nChannels = 4;

			m_pData = new uint8_t[w * h * m_nChannels];

			for (int i = 0; i < w * h * m_nChannels; i++)
				m_pData[i] = 0;
		}

		rcode LoadSprite()
		{
			rcode rc(false);

			m_pData = stbi_load(m_sFilename.c_str(), &m_nWidth, &m_nHeight, &m_nChannels, 0);

			if (!m_pData)
			{
				rc.info = "stb_image: ";
				rc.info += stbi_failure_reason();
				return rc;
			}

			rc.ok = true;
			return rc;
		}

		uint32_t GetWidth() const
		{
			return m_nWidth;
		}

		uint32_t GetHeight() const
		{
			return m_nHeight;
		}

		vi2d GetSize() const
		{
			return vi2d(m_nWidth, m_nHeight);
		}

		std::string GetFilename() const
		{
			return m_sFilename;
		}

		bool IsOk()
		{
			return m_bOk;
		}

		void SetPixel(int32_t x, int32_t y, const Pixel& p)
		{
			m_pData[m_nChannels * (y * m_nWidth + x) + 0] = p.r;
			m_pData[m_nChannels * (y * m_nWidth + x) + 1] = p.g;
			m_pData[m_nChannels * (y * m_nWidth + x) + 2] = p.b;
			m_pData[m_nChannels * (y * m_nWidth + x) + 3] = p.a;
		}

		Pixel GetPixel(int32_t x, int32_t y)
		{
			if (x < 0 || y < 0 || x >= m_nWidth || y >= m_nHeight)
				return BLACK;

			Pixel p = Pixel(
				m_pData[m_nChannels * (y * m_nWidth + x) + 0],
				m_pData[m_nChannels * (y * m_nWidth + x) + 1],
				m_pData[m_nChannels * (y * m_nWidth + x) + 2],
				m_pData[m_nChannels * (y * m_nWidth + x) + 3]
			);

			return p;
		}

		uint8_t* GetPixelData()
		{
			return m_pData;
		}

		void SetPixelData(uint8_t* data)
		{
			for (int i = 0; i < m_nWidth * m_nHeight * m_nChannels; i++)
				m_pData[i] = data[i];
		}

		void SetPixelData(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
		{
			for (int i = 0; i < m_nWidth; i++)
				for (int j = 0; j < m_nHeight; j++)
				{
					m_pData[m_nChannels * (j * m_nWidth + i) + 0] = r;
					m_pData[m_nChannels * (j * m_nWidth + i) + 1] = g;
					m_pData[m_nChannels * (j * m_nWidth + i) + 2] = b;
					m_pData[m_nChannels * (j * m_nWidth + i) + 3] = a;
				}
		}

		int GetChannels()
		{
			return m_nChannels;
		}

		Pixel Sample(float x, float y)
		{
			int32_t sx = std::min(int32_t(x * (float)m_nWidth), m_nWidth - 1);
			int32_t sy = std::min(int32_t(y * (float)m_nHeight), m_nHeight - 1);
			return GetPixel(sx, sy);
		}
	};

	/************************************
	* @ END SPRITE CLASS IMPLEMENTATION *
	************************************/


	/*********************************
	* @ TEXTURE CLASS IMPLEMENTATION *
	*********************************/

	class Texture
	{
	public:
		Texture(Sprite* spr)
		{
			m_sprInstance = std::unique_ptr<Sprite>(spr);
			Construct();
		}

		Texture(const std::string& filename)
		{
			m_sprInstance = std::make_unique<def::Sprite>(filename);
			Construct();
		}

	private:
		void Construct()
		{
			LoadTexture(m_sprInstance->GetPixelData());

			m_fUVScaleX = 1.0f / (float)m_sprInstance->GetWidth();
			m_fUVScaleY = 1.0f / (float)m_sprInstance->GetHeight();
		}

	private:
		std::unique_ptr<Sprite> m_sprInstance;

		GLuint m_nTexId;

		float m_fUVScaleX;
		float m_fUVScaleY;

	public:
		void LoadTexture(uint8_t* pixel_data)
		{
			GLenum nFormat = 0;

			switch (m_sprInstance->GetChannels())
			{
			case 3: nFormat = GL_RGB; break;
			case 4: nFormat = GL_RGBA; break;
			}

			if (nFormat == 0)
			{
				std::cerr << "Invalid number of channels for " << m_sprInstance->GetFilename() << std::endl;
				abort();
			}

			glGenTextures(1, &m_nTexId);
			glBindTexture(GL_TEXTURE_2D, m_nTexId);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				nFormat,
				m_sprInstance->GetWidth(),
				m_sprInstance->GetHeight(),
				0,
				nFormat,
				GL_UNSIGNED_BYTE,
				pixel_data
			);

			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void SetTexId(uint32_t id)
		{
			m_nTexId = id;
		}

		uint32_t GetTexId()
		{
			return m_nTexId;
		}

		Sprite* Spr()
		{
			return m_sprInstance.get();
		}

		float GetUVScaleX()
		{
			return m_fUVScaleX;
		}

		float GetUVScaleY()
		{
			return m_fUVScaleY;
		}
	};

	/*************************************
	* @ END TEXTURE CLASS IMPLEMENTATION *
	*************************************/


	/**********************************
	* @ GRAPHIC STRUCT IMPLEMENTATION *
	**********************************/

	struct Graphic
	{
		Graphic() = default;

		Graphic(const std::string& filename)
		{
			Load(filename);
		}

		~Graphic()
		{
			delete tex;
			delete spr;
		}

		Texture* tex;
		Sprite* spr;

		bool Load(const std::string& filename)
		{
			spr = new Sprite(filename);

			if (spr->IsOk())
			{
				tex = new Texture(spr);
				return true;
			}

			return false;
		}
	};

	/**************************************
	* @ END GRAPHIC STRUCT IMPLEMENTATION *
	**************************************/


	/******************************************
	* @ MAIN GAME ENGINE CLASS IMPLEMENTATION *
	******************************************/

	class GameEngine
	{
	public:
		GameEngine()
		{
			m_sAppName = "Undefined";

			m_bCustomIcon = false;
		}

		virtual ~GameEngine()
		{
			Destroy();
		}

	private:
		std::string m_sAppName;
		std::string m_sIconFileName;

		uint32_t m_nScreenWidth;
		uint32_t m_nScreenHeight;

		uint32_t m_nPixelWidth;
		uint32_t m_nPixelHeight;

		uint32_t m_nWindowWidth;
		uint32_t m_nWindowHeight;

		GLFWwindow* m_glWindow = nullptr;
		GLFWmonitor* m_glMonitor = nullptr;

		bool m_bAppRunning;

		bool m_bFullScreen;
		bool m_bVSync;

		bool m_bCustomIcon;

		bool m_bShowFPS;

		KeyState m_keys[512];
		KeyState m_mouse[5];

		bool m_nKeyOldState[512];
		bool m_nKeyNewState[512];

		bool m_nMouseOldState[5];
		bool m_nMouseNewState[5];

		int32_t m_nMouseX = -1;
		int32_t m_nMouseY = -1;

		float m_fDeltaTime;

		Sprite* m_sprIcon = nullptr;
		Sprite* m_sprFont = nullptr;

		bool m_bDirtyPixel;

		def::Pixel* m_pScreen = nullptr;

		def::vi2d m_vTrans = { 0.0f, 0.0f };
		def::vf2d m_vScale = { 1.0f, 1.0f };

	public:
		virtual bool OnUserCreate() = 0;
		virtual bool OnUserUpdate(float fDeltaTime) = 0;

		virtual bool OnBeforeUserCreate() { return true; }
		virtual bool OnAfterUserCreate() { return true; }

		virtual bool OnBeforeUserUpdate(float fDeltaTime) { return true; }
		virtual bool OnAfterUserUpdate(float fDeltaTime) { return true; }

		rcode Construct(uint32_t nScreenWidth, uint32_t nScreenHeight, uint32_t nPixelWidth, uint32_t nPixelHeight, bool bFullScreen = false, bool bVSync = false, bool bDirtyPixel = false)
		{
			if (nScreenWidth < 0 || nScreenHeight < 0)
				return rcode(false, "Width or height less than zero");

			if (!glfwInit())
				return rcode(false, "Could not init GLFW!");

			m_nScreenWidth = nScreenWidth;
			m_nScreenHeight = nScreenHeight;

			m_nPixelWidth = nPixelWidth;
			m_nPixelHeight = nPixelHeight;

			m_nWindowWidth = m_nScreenWidth * m_nPixelWidth;
			m_nWindowHeight = m_nScreenHeight * m_nPixelHeight;

			m_bFullScreen = bFullScreen;
			m_bVSync = bVSync;

			m_bDirtyPixel = bDirtyPixel;

			m_glMonitor = glfwGetPrimaryMonitor();

			if (!m_glMonitor)
			{
				Destroy();
				return rcode(false, "No monitors were found!");
			}

			const GLFWvidmode* vm = glfwGetVideoMode(m_glMonitor);

			// On some laptops with integrated graphics VSync does not work at all!
			if (m_bVSync)
			{
				glfwSwapInterval(1);
				glfwWindowHint(GLFW_REFRESH_RATE, vm->refreshRate);
			}
			else
				glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE);

			if (m_bFullScreen)
			{
				m_nScreenWidth = vm->width / m_nPixelWidth;
				m_nScreenHeight = vm->height / m_nPixelHeight;

				m_nWindowWidth = vm->width;
				m_nWindowHeight = vm->height;

				m_glWindow = glfwCreateWindow(m_nWindowWidth, m_nWindowHeight, "", m_glMonitor, NULL);

				glfwSetWindowMonitor(m_glWindow, m_glMonitor,
					0, 0, m_nWindowWidth, m_nWindowHeight, vm->refreshRate);
			}
			else
				m_glWindow = glfwCreateWindow(m_nWindowWidth, m_nWindowHeight, "", NULL, NULL);

			if (!m_glWindow)
			{
				const char* err;
				glfwGetError(&err);
				Destroy();
				return rcode(false, err);
			}

			m_pScreen = new Pixel[m_nScreenWidth * m_nScreenHeight];
			Clear(def::BLACK);

			glfwMakeContextCurrent(m_glWindow);

			glViewport(0, 0, m_nWindowWidth, m_nWindowHeight);
			glOrtho(0.0, (double)m_nWindowWidth, (double)m_nWindowHeight, 0.0, -1.0, 1.0);

			glEnable(GL_TEXTURE_2D);

			if (m_bDirtyPixel)
				glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

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
					int k = r & (1 << i) ? 255 : 0;
					m_sprFont->SetPixel(px, py, Pixel(k, k, k, k));
					if (++py == 48) { px++; py = 0; }
				}
			}

			if (m_bCustomIcon)
			{
				m_sprIcon = new def::Sprite(m_sIconFileName);

				GLFWimage img;

				img.width = m_sprIcon->GetWidth();
				img.height = m_sprIcon->GetHeight();

				img.pixels = m_sprIcon->GetPixelData();

				glfwSetWindowIcon(m_glWindow, 1, &img);
			}

			return rcode(true);
		}

		void Run()
		{
			m_bAppRunning = true;
			AppThread();
		}

	private:
		void Destroy()
		{
			if (m_pScreen)
				delete[] m_pScreen;

			if (m_sprFont)
				delete m_sprFont;

			if (m_sprIcon)
				delete m_sprIcon;

			glfwDestroyWindow(m_glWindow);
			glfwTerminate();
		}

		bool AppThread()
		{
			if (!OnBeforeUserCreate())
			{
				m_bAppRunning = false;
				return false;
			}

			if (!OnUserCreate())
			{
				m_bAppRunning = false;
				return false;
			}

			if (!OnAfterUserCreate())
			{
				m_bAppRunning = false;
				return false;
			}

			auto tp1 = std::chrono::system_clock::now();
			auto tp2 = tp1;

			for (int i = 0; i < 512; i++)
				m_keys[i] = { false, false, false };

			for (int i = 0; i < 5; i++)
				m_mouse[i] = { false, false, false };

			for (int i = 0; i < 512; i++)
				m_nKeyOldState[i] = 0;

			for (int i = 0; i < 5; i++)
				m_nMouseOldState[i] = 0;

			for (int i = 0; i < 512; i++)
				m_nKeyNewState[i] = 0;

			while (m_bAppRunning)
			{
				tp2 = std::chrono::system_clock::now();
				m_fDeltaTime = std::chrono::duration<float>(tp2 - tp1).count();
				tp1 = tp2;

				m_bAppRunning = !glfwWindowShouldClose(m_glWindow);

				for (int i = 0; i < 512; i++)
				{
					m_nKeyNewState[i] = glfwGetKey(m_glWindow, i);

					m_keys[i].bPressed = false;
					m_keys[i].bReleased = false;

					if (m_nKeyNewState[i] != m_nKeyOldState[i])
					{
						if (m_nKeyNewState[i])
						{
							m_keys[i].bPressed = !m_keys[i].bHeld;
							m_keys[i].bHeld = true;
						}
						else
						{
							m_keys[i].bReleased = true;
							m_keys[i].bHeld = false;
						}
					}

					m_nKeyOldState[i] = m_nKeyNewState[i];
				}

				for (int i = 0; i < 5; i++)
				{
					m_nMouseNewState[i] = glfwGetMouseButton(m_glWindow, i);

					m_mouse[i].bPressed = false;
					m_mouse[i].bReleased = false;

					if (m_nMouseNewState[i] != m_nMouseOldState[i])
					{
						if (m_nMouseNewState[i])
						{
							m_mouse[i].bPressed = true;
							m_mouse[i].bHeld = true;
						}
						else
						{
							m_mouse[i].bReleased = true;
							m_mouse[i].bHeld = false;
						}
					}

					m_nMouseOldState[i] = m_nMouseNewState[i];
				}

				double dMouseX, dMouseY;

				glfwGetCursorPos(m_glWindow, &dMouseX, &dMouseY);

				m_nMouseX = (int)dMouseX / m_nPixelWidth;
				m_nMouseY = (int)dMouseY / m_nPixelHeight;

				if (m_bShowFPS)
				{
					std::string title = "github.com/defini7 - " + m_sAppName + " - FPS: " + std::to_string(int(1.0f / m_fDeltaTime));
					glfwSetWindowTitle(m_glWindow, title.c_str());
				}

				if (!OnBeforeUserUpdate(m_fDeltaTime))
					m_bAppRunning = false;

				glPushMatrix();

				if (!OnUserUpdate(m_fDeltaTime))
					m_bAppRunning = false;

				glTranslatef((float)m_vTrans.x, (float)m_vTrans.y, 0.0f);
				glScalef(m_vScale.x, m_vScale.y, 1.0f);

				if (m_nPixelWidth + m_nPixelHeight == 2) glBegin(GL_POINTS);
				else									 glBegin(GL_QUADS);

				for (uint32_t x = 0; x < m_nScreenWidth; x++)
					for (uint32_t y = 0; y < m_nScreenHeight; y++)
					{
						const def::Pixel& p = m_pScreen[y * m_nScreenWidth + x];
						glColor4ub(p.r, p.g, p.b, p.a);

						// Some optimization
						if (m_nPixelWidth == 1 && m_nPixelHeight == 1)
							glVertex2i(x, y);
						else
						{
							int nTopLeftX = x * m_nPixelWidth;
							int nTopLeftY = y * m_nPixelHeight;

							int nTopRightX = x * m_nPixelWidth + m_nPixelWidth;
							int nTopRightY = y * m_nPixelHeight + m_nPixelHeight;

							glVertex2i(nTopLeftX, nTopLeftY);
							glVertex2i(nTopRightX, nTopLeftY);
							glVertex2i(nTopRightX, nTopRightY);
							glVertex2i(nTopLeftX, nTopRightY);
						}
					}

				glEnd();
				glPopMatrix();

				if (!OnAfterUserUpdate(m_fDeltaTime))
					m_bAppRunning = false;

				if (m_bVSync)
					glfwSwapBuffers(m_glWindow);
				else
					glFlush();

				glfwPollEvents();
			}

			return true;
		}

	public:

		void Draw(vi2d pos, const Pixel& p = WHITE);
		virtual void Draw(int32_t x, int32_t y, const Pixel& p = WHITE);

		void DrawLine(vi2d pos1, vi2d pos2, const Pixel& p = WHITE);
		virtual void DrawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const Pixel& p = WHITE);

		void DrawTriangle(vi2d pos1, vi2d pos2, vi2d pos3, const Pixel& p = WHITE);
		virtual void DrawTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, const Pixel& p = WHITE);

		void FillTriangle(vi2d pos1, vi2d pos2, vi2d pos3, const Pixel& p = WHITE);
		virtual void FillTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, const Pixel& p = WHITE);

		void DrawRectangle(vi2d pos, vi2d size, const Pixel& p = WHITE);
		virtual void DrawRectangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const Pixel& p = WHITE);

		void FillRectangle(vi2d pos, vi2d size, const Pixel& p = WHITE);
		virtual void FillRectangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const Pixel& p = WHITE);

		void DrawCircle(vi2d pos, int32_t radius, const Pixel& p = WHITE);
		virtual void DrawCircle(int32_t x, int32_t y, int32_t radius, const Pixel& p = WHITE);

		void FillCircle(vi2d pos, int32_t radius, const Pixel& p = WHITE);
		virtual void FillCircle(int32_t x, int32_t y, int32_t radius, const Pixel& p = WHITE);

		void DrawSprite(vi2d pos, Sprite* sprite);
		virtual void DrawSprite(int32_t x, int32_t y, Sprite* sprite);

		void DrawPartialSprite(vi2d pos, vi2d fpos, vi2d fsize, Sprite* sprite);
		virtual void DrawPartialSprite(int32_t x, int32_t y, int32_t fx, int32_t fy, int32_t fsizex, int32_t fsizey, Sprite* sprite);

		void DrawTexture(vi2d pos, Texture* tex, vf2d scale = { 1.0f, 1.0f }, def::Pixel tint = def::WHITE);
		virtual void DrawTexture(float x, float y, Texture* tex, float scale_x = 1.0f, float scale_y = 1.0f, def::Pixel tint = def::WHITE);

		void DrawPartialTexture(vi2d pos, vi2d fpos, vi2d fsize, Texture* tex, vf2d scale = { 1.0f, 1.0f }, def::Pixel tint = def::WHITE);
		virtual void DrawPartialTexture(float x, float y, int32_t fx, int32_t fy, int32_t fsx, int32_t fsy, Texture* tex, float scale_x = 1.0f, float scale_y = 1.0f, def::Pixel tint = def::WHITE);

		void DrawWireFrameModel(std::vector<std::pair<float, float>>& vecModelCoordinates, vf2d pos, float r = 0.0f, float s = 1.0f, const Pixel& p = WHITE);
		virtual void DrawWireFrameModel(std::vector<std::pair<float, float>>& vecModelCoordinates, float x, float y, float r = 0.0f, float s = 1.0f, const Pixel& p = WHITE);

		void DrawString(vi2d pos, const std::string& text, const Pixel& p = WHITE);
		virtual void DrawString(int32_t x, int32_t y, const std::string& text, const Pixel& p = WHITE);

		virtual void Clear(const Pixel& p);

		KeyState GetKey(uint32_t k);
		KeyState GetMouse(uint32_t k);

		vi2d GetMouse();

		int32_t GetMouseX();
		int32_t GetMouseY();

		void SetTitle(const std::string& title);

		vi2d GetScreenSize();

		int32_t GetScreenWidth();
		int32_t GetScreenHeight();

		void Translate(int32_t x, int32_t y);
		void Translate(const def::vi2d& trans);

		void Scale(float x, float y);
		void Scale(const def::vf2d& scale);

		bool FullScreenEnabled();
		bool VSyncEnabled();

		void SetIcon(const std::string& filename);

		WindowState GetWindowState();

		GLFWwindow* GetWindow();

		void ShowFPS(bool show = true);

	};

	void def::GameEngine::Draw(int32_t x, int32_t y, const Pixel& p)
	{
		if (x >= 0 && y >= 0 && x < GetScreenWidth() && y < GetScreenHeight())
			m_pScreen[y * GetScreenWidth() + x] = p;
	}

	void def::GameEngine::DrawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const Pixel& p)
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

	void def::GameEngine::DrawTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, const Pixel& p)
	{
		DrawLine(x1, y1, x2, y2, p);
		DrawLine(x2, y2, x3, y3, p);
		DrawLine(x3, y3, x1, y1, p);
	}

	void def::GameEngine::FillTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, const Pixel& p)
	{
		auto drawline = [&](int32_t sx, int32_t ex, int32_t ny) { for (int i = sx; i <= ex; i++) Draw(i, ny, p); };

		int32_t t1x, t2x, y, minx, maxx, t1xp, t2xp;

		bool changed1 = false;
		bool changed2 = false;

		int32_t signx1, signx2, dx1, dy1, dx2, dy2;
		int32_t e1, e2;

		// Sort vertices
		if (y1 > y2) { std::swap(y1, y2); std::swap(x1, x2); }
		if (y1 > y3) { std::swap(y1, y3); std::swap(x1, x3); }
		if (y2 > y3) { std::swap(y2, y3); std::swap(x2, x3); }

		t1x = t2x = x1; y = y1;   // Starting points
		dx1 = (int32_t)(x2 - x1);

		if (dx1 < 0)
		{
			dx1 = -dx1;
			signx1 = -1;
		}
		else
			signx1 = 1;

		dy1 = (int32_t)(y2 - y1);

		dx2 = (int32_t)(x3 - x1);

		if (dx2 < 0)
		{
			dx2 = -dx2;
			signx2 = -1;
		}
		else
			signx2 = 1;

		dy2 = (int32_t)(y3 - y1);

		if (dy1 > dx1)
		{   // swap values
			std::swap(dx1, dy1);
			changed1 = true;
		}
		if (dy2 > dx2)
		{   // swap values
			std::swap(dy2, dx2);
			changed2 = true;
		}

		e2 = (int32_t)(dx2 >> 1);

		// Flat top, just process the second half
		if (y1 == y2)
			goto next;

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

			// process first line until y value is about to change
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
			// Move line
		next1:
			// process second line until y value is about to change
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
			if (minx > t1x)
				minx = t1x;

			if (minx > t2x)
				minx = t2x;

			if (maxx < t1x)
				maxx = t1x;

			if (maxx < t2x)
				maxx = t2x;

			drawline(minx, maxx, y);    // Draw line from min to max points found on the y
			// Now increase y
			if (!changed1)
				t1x += signx1;

			t1x += t1xp;

			if (!changed2)
				t2x += signx2;

			t2x += t2xp;
			y += 1;

			if (y == y2)
				break;

		}
	next:
		// Second half
		dx1 = (int32_t)(x3 - x2);

		if (dx1 < 0)
		{
			dx1 = -dx1;
			signx1 = -1;
		}
		else
			signx1 = 1;

		dy1 = (int32_t)(y3 - y2);
		t1x = x2;

		if (dy1 > dx1)
		{   // swap values
			std::swap(dy1, dx1);
			changed1 = true;
		}
		else
			changed1 = false;

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

			// process first line until y value is about to change
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
						goto next3;
				}

				if (changed1)
					break;
				else
					t1x += signx1;

				if (i < dx1)
					i++;
			}
		next3:
			// process second line until y value is about to change
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

			if (minx > t1x)
				minx = t1x;

			if (minx > t2x)
				minx = t2x;

			if (maxx < t1x)
				maxx = t1x;

			if (maxx < t2x)
				maxx = t2x;

			drawline(minx, maxx, y);

			if (!changed1)
				t1x += signx1;

			t1x += t1xp;

			if (!changed2)
				t2x += signx2;

			t2x += t2xp;
			y += 1;

			if (y > y3)
				return;
		}
	}

	void def::GameEngine::DrawRectangle(int32_t x, int32_t y, int32_t sx, int32_t sy, const Pixel& p)
	{
		for (int i = 0; i <= sx; i++)
		{
			Draw(x + i, y, p);
			Draw(x + i, y + sy, p);
		}

		for (int j = 0; j <= sy; j++)
		{
			Draw(x, y + j, p);
			Draw(x + sx, y + j, p);
		}
	}

	void def::GameEngine::FillRectangle(int32_t x, int32_t y, int32_t sx, int32_t sy, const Pixel& p)
	{
		for (int i = x; i <= x + sx; i++)
			for (int j = y; j <= y + sy; j++)
				Draw(i, j, p);
	}

	void def::GameEngine::DrawCircle(int32_t x, int32_t y, int32_t r, const Pixel& p)
	{
		if (!r) return;

		int32_t x1 = 0;
		int32_t y1 = r;
		float p1 = 3.0f - 2.0f * (float)r;

		while (y1 >= x1)
		{
			Draw(x - x1, y - y1, p);	// upper left left
			Draw(x - y1, y - x1, p);	// upper upper left
			Draw(x + y1, y - x1, p);	// upper upper right
			Draw(x + x1, y - y1, p);	// upper right right
			Draw(x - x1, y + y1, p);	// lower left left
			Draw(x - y1, y + x1, p);	// lower lower left
			Draw(x + y1, y + x1, p);	// lower lower right
			Draw(x + x1, y + y1, p);	// lower right right

			if (p1 < 0)
			{
				p1 += 4 * x1 + 6;

				x1++;
			}
			else
			{
				p1 += 4 * (x1 - y1) + 10;

				x1++;
				y1--;
			}
		}
	}

	void def::GameEngine::FillCircle(int32_t x, int32_t y, int32_t r, const Pixel& p)
	{
		if (!r) return;

		int32_t x1 = 0;
		int32_t y1 = r;
		int32_t p1 = 3 - 2 * r;

		auto drawline = [&](int32_t sx, int32_t ex, int32_t ny)
		{
			for (int i = sx; i <= ex; i++)
				Draw(i, ny, p);
		};

		while (y1 >= x1)
		{
			drawline(x - x1, x + x1, y - y1);
			drawline(x - y1, x + y1, y - x1);
			drawline(x - x1, x + x1, y + y1);
			drawline(x - y1, x + y1, y + x1);

			if (p1 < 0)
				p1 += 4 * x1++ + 6;
			else
				p1 += 4 * (x1++ - y1--) + 10;
		}
	}

	void def::GameEngine::DrawSprite(int32_t x, int32_t y, Sprite* sprite)
	{
		if (sprite == nullptr)
			return;

		if (!sprite->IsOk())
			return;

		for (int i = 0; i < sprite->GetWidth(); i++)
			for (int j = 0; j < sprite->GetHeight(); j++)
			{
				def::Pixel p = sprite->GetPixel(i, j);

				if ((float)p.a / 255.0f > 0.1f)
					Draw(x + i, y + j, p);
			}
	}

	void def::GameEngine::DrawPartialSprite(int32_t x, int32_t y, int32_t fx, int32_t fy, int32_t fsx, int32_t fsy, Sprite* sprite)
	{
		if (sprite == nullptr)
			return;

		if (!sprite->IsOk())
			return;

		for (int i = fx, x1 = 0; i < fx + fsx; i++, x1++)
			for (int j = fy, y1 = 0; j < fy + fsy; j++, y1++)
			{
				def::Pixel p = sprite->GetPixel(i, j);

				if ((float)p.a / 255.0f > 0.1f)
					Draw(x + x1, y + y1, sprite->GetPixel(i, j));
			}
	}

	void GameEngine::DrawTexture(float x, float y, Texture* tex, float scale_x, float scale_y, def::Pixel tint)
	{
		if (tex == nullptr)
			return;

		if (!tex->Spr()->IsOk())
			return;

		glScalef(scale_x * (float)m_nPixelWidth, scale_y * (float)m_nPixelHeight, 1.0f);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glColor4ub(tint.r, tint.g, tint.b, tint.a);

		glBindTexture(GL_TEXTURE_2D, tex->GetTexId());

		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(x / scale_x, y / scale_y + tex->Spr()->GetHeight());
		glTexCoord2f(1.0f, 1.0f); glVertex2f(x / scale_x + tex->Spr()->GetWidth(), y / scale_y + tex->Spr()->GetHeight());
		glTexCoord2f(1.0f, 0.0f); glVertex2f(x / scale_x + tex->Spr()->GetWidth(), y / scale_y);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(x / scale_x, y / scale_y);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);

		glDisable(GL_BLEND);

		glEnd();
	}

	void GameEngine::DrawPartialTexture(float x, float y, int32_t fx, int32_t fy, int32_t fsx, int32_t fsy, Texture* tex, float scale_x, float scale_y, def::Pixel tint)
	{
		if (tex == nullptr)
			return;

		if (!tex->Spr()->IsOk())
			return;

		glScalef(scale_x * (float)m_nPixelWidth, scale_y * (float)m_nPixelHeight, 1.0f);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glColor4ub(tint.r, tint.g, tint.b, tint.a);

		glBindTexture(GL_TEXTURE_2D, tex->GetTexId());

		float us = tex->GetUVScaleX();
		float vs = tex->GetUVScaleY();

		x /= scale_x;
		y /= scale_y;

		glBegin(GL_QUADS);
		glTexCoord2f((float)fx * us, (float)fy * vs);				glVertex2f(x, y);
		glTexCoord2f((float)fx * us, float(fy + fsy) * vs);			glVertex2f(x, y + fsy);
		glTexCoord2f(float(fx + fsx) * us, float(fy + fsy) * vs);	glVertex2f(x + fsx, y + fsy);
		glTexCoord2f(float(fx + fsx) * us, (float)fy * vs);			glVertex2f(x + fsx, y);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);

		glDisable(GL_BLEND);

		glEnd();
	}

	void def::GameEngine::DrawWireFrameModel(std::vector<std::pair<float, float>>& vecModelCoordinates, float x, float y, float r, float s, const Pixel& p)
	{
		// pair.first = x coordinate
		// pair.second = y coordinate

		// Create translated model vector of coordinate pairs
		std::vector<std::pair<float, float>> vecTransformedCoordinates;
		int32_t verts = vecModelCoordinates.size();
		vecTransformedCoordinates.resize(verts);

		// Rotate
		for (int i = 0; i < verts; i++)
		{
			vecTransformedCoordinates[i].first = vecModelCoordinates[i].first * cosf(r) - vecModelCoordinates[i].second * sinf(r);
			vecTransformedCoordinates[i].second = vecModelCoordinates[i].first * sinf(r) + vecModelCoordinates[i].second * cosf(r);
		}

		// Scale
		for (int i = 0; i < verts; i++)
		{
			vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first * s;
			vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second * s;
		}

		// Translate
		for (int i = 0; i < verts; i++)
		{
			vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first + x;
			vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second + y;
		}

		// Draw Closed Polygon
		for (int i = 0; i < verts + 1; i++)
		{
			int32_t j = (i + 1);
			DrawLine((int32_t)vecTransformedCoordinates[i % verts].first, (int32_t)vecTransformedCoordinates[i % verts].second,
				(int32_t)vecTransformedCoordinates[j % verts].first, (int32_t)vecTransformedCoordinates[j % verts].second, p);
		}
	}

	void def::GameEngine::DrawString(int32_t x, int32_t y, const std::string& s, const Pixel& p)
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

	void def::GameEngine::Clear(const Pixel& p)
	{
		for (int32_t i = 0; i < m_nScreenWidth * m_nScreenHeight; i++)
			m_pScreen[i] = p;
	}

	KeyState GameEngine::GetKey(uint32_t k)
	{
		return m_keys[k];
	}

	KeyState GameEngine::GetMouse(uint32_t k)
	{
		return m_mouse[k];
	}

	int32_t GameEngine::GetMouseX()
	{
		return m_nMouseX;
	}

	int32_t GameEngine::GetMouseY()
	{
		return m_nMouseY;
	}

	int32_t GameEngine::GetScreenWidth()
	{
		return m_nScreenWidth;
	}

	int32_t GameEngine::GetScreenHeight()
	{
		return m_nScreenHeight;
	}

	void GameEngine::Translate(int32_t x, int32_t y)
	{
		m_vTrans.x = x;
		m_vTrans.y = y;
	}

	void GameEngine::Translate(const def::vi2d& trans)
	{
		Translate(trans.x, trans.y);
	}

	void GameEngine::Scale(float x, float y)
	{
		m_vScale.x = x;
		m_vScale.y = y;
	}

	void GameEngine::Scale(const def::vf2d& scale)
	{
		Scale(scale.x, scale.y);
	}

	bool GameEngine::FullScreenEnabled()
	{
		return m_bFullScreen;
	}

	bool GameEngine::VSyncEnabled()
	{
		return m_bVSync;
	}

	void GameEngine::SetIcon(const std::string& filename)
	{
		m_sIconFileName = filename;
		m_bCustomIcon = true;
	}

	void GameEngine::SetTitle(const std::string& title)
	{
		m_sAppName = title;
	}

	void GameEngine::ShowFPS(bool show)
	{
		m_bShowFPS = show;
	}

	WindowState GameEngine::GetWindowState()
	{
		bool bFocused = glfwGetWindowAttrib(m_glWindow, GLFW_FOCUSED);
		bool bMaximized = glfwGetWindowAttrib(m_glWindow, GLFW_MAXIMIZED);

		int f = WS_NONE;

		if (bFocused)
			f |= WS_FOCUSED;

		if (bMaximized)
			f |= WS_MAXIMIZED;

		return static_cast<WindowState>(f);
	}

	GLFWwindow* GameEngine::GetWindow()
	{
		return m_glWindow;
	}

	void GameEngine::Draw(vi2d pos, const Pixel& p)
	{
		Draw(pos.x, pos.y, p);
	}

	void GameEngine::DrawLine(vi2d pos1, vi2d pos2, const Pixel& p)
	{
		DrawLine(pos1.x, pos1.y, pos2.x, pos2.y, p);
	}

	void GameEngine::DrawTriangle(vi2d pos1, vi2d pos2, vi2d pos3, const Pixel& p)
	{
		DrawTriangle(pos1.x, pos1.y, pos2.x, pos2.y, pos3.x, pos3.y, p);
	}

	void GameEngine::FillTriangle(vi2d pos1, vi2d pos2, vi2d pos3, const Pixel& p)
	{
		FillTriangle(pos1.x, pos1.y, pos2.x, pos2.y, pos3.x, pos3.y, p);
	}

	void GameEngine::DrawRectangle(vi2d pos, vi2d size, const Pixel& p)
	{
		DrawRectangle(pos.x, pos.y, size.x, size.y, p);
	}

	void GameEngine::FillRectangle(vi2d pos, vi2d size, const Pixel& p)
	{
		FillRectangle(pos.x, pos.y, size.x, size.y, p);
	}

	void GameEngine::DrawCircle(vi2d pos, int32_t r, const Pixel& p)
	{
		DrawCircle(pos.x, pos.y, r, p);
	}

	void GameEngine::FillCircle(vi2d pos, int32_t r, const Pixel& p)
	{
		FillCircle(pos.x, pos.y, r, p);
	}

	void GameEngine::DrawSprite(vi2d pos, Sprite* spr)
	{
		DrawSprite(pos.x, pos.y, spr);
	}

	void GameEngine::DrawPartialSprite(vi2d pos, vi2d fpos, vi2d fsize, Sprite* spr)
	{
		DrawPartialSprite(pos.x, pos.y, fpos.x, fpos.y, fsize.x, fsize.y, spr);
	}

	void GameEngine::DrawTexture(vi2d pos, Texture* Texture, vf2d scale, def::Pixel tint)
	{
		DrawTexture(pos.x, pos.y, Texture, scale.x, scale.y, tint);
	}

	void GameEngine::DrawPartialTexture(vi2d pos, vi2d fpos, vi2d fsize, Texture* Texture, vf2d scale, def::Pixel tint)
	{
		DrawPartialTexture(pos.x, pos.y, fpos.x, fpos.y, fsize.x, fsize.y, Texture, scale.x, scale.y, tint);
	}

	void GameEngine::DrawWireFrameModel(std::vector<std::pair<float, float>>& vecModelCoordinates, vf2d pos, float r, float s, const Pixel& p)
	{
		DrawWireFrameModel(vecModelCoordinates, pos.x, pos.y, r, s, p);
	}

	void GameEngine::DrawString(vi2d pos, const std::string& text, const Pixel& p)
	{
		DrawString(pos.x, pos.y, text, p);
	}

	vi2d GameEngine::GetScreenSize()
	{
		return vi2d(m_nScreenWidth, m_nScreenHeight);
	}

	vi2d GameEngine::GetMouse()
	{
		return vi2d(m_nMouseX, m_nMouseY);
	}

	/******************************************
	* @ MAIN GAME ENGINE CLASS IMPLEMENTATION *
	******************************************/

}
