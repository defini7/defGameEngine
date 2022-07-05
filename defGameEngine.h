#pragma once

#pragma region license
/***
*	BSD 3-Clause License
	Copyright (c) 2021, Alex
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
					Draw(i, j, def::Pixel(rand() % 255, rand() % 255, rand() % 255));

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

#if defined(__linux__)
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#else
#include <SDL.h>
#include <SDL_image.h>
#endif

#if defined(__MINGW32__)
#include <Windows.h>
#endif

#pragma endregion

#if defined(SDL_MAIN_NEEDED) || !defined(SDL_MAIN_AVAILABLE)
#if defined(__linux__)
#define main() main(int argc, char** argv)
#else
#define main() SDL_main(int argc, char** argv)
#endif
#elif defined(__MINGW32__)
#define SDL_MAIN_HANDLED
#undef main
#define main() __clrcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
#else
#undef main
#define main() SDL_main(int argc, char** argv)
#endif

namespace def
{
	// KEYBOARD SCANCODES

	typedef SDL_Scancode KeyCode;
	
	namespace Key
	{
		constexpr KeyCode SPACE = SDL_SCANCODE_SPACE;
		constexpr KeyCode BACK = SDL_SCANCODE_BACKSPACE;
		constexpr KeyCode DELETE = SDL_SCANCODE_DELETE;
		constexpr KeyCode HOME = SDL_SCANCODE_HOME;
		constexpr KeyCode END = SDL_SCANCODE_END;
		constexpr KeyCode RETURN = SDL_SCANCODE_RETURN;
		constexpr KeyCode TAB = SDL_SCANCODE_TAB;
		constexpr KeyCode INSERT = SDL_SCANCODE_INSERT;
		constexpr KeyCode LSHIFT = SDL_SCANCODE_LSHIFT;
		constexpr KeyCode RSHIFT = SDL_SCANCODE_RSHIFT;
		constexpr KeyCode LCONTROL = SDL_SCANCODE_LCTRL;
		constexpr KeyCode RCONTROL = SDL_SCANCODE_RCTRL;
		constexpr KeyCode PRIOR = SDL_SCANCODE_PAGEUP;
		constexpr KeyCode NEXT = SDL_SCANCODE_PAGEDOWN;
		constexpr KeyCode ESCAPE = SDL_SCANCODE_ESCAPE;
		constexpr KeyCode UP = SDL_SCANCODE_UP;
		constexpr KeyCode DOWN = SDL_SCANCODE_DOWN;
		constexpr KeyCode LEFT = SDL_SCANCODE_LEFT;
		constexpr KeyCode RIGHT = SDL_SCANCODE_RIGHT;
		constexpr KeyCode F1 = SDL_SCANCODE_F1;
		constexpr KeyCode F2 = SDL_SCANCODE_F2;
		constexpr KeyCode F3 = SDL_SCANCODE_F3;
		constexpr KeyCode F4 = SDL_SCANCODE_F4;
		constexpr KeyCode F5 = SDL_SCANCODE_F5;
		constexpr KeyCode F6 = SDL_SCANCODE_F6;
		constexpr KeyCode F7 = SDL_SCANCODE_F7;
		constexpr KeyCode F8 = SDL_SCANCODE_F8;
		constexpr KeyCode F9 = SDL_SCANCODE_F9;
		constexpr KeyCode F10 = SDL_SCANCODE_F10;
		constexpr KeyCode F11 = SDL_SCANCODE_F11;
		constexpr KeyCode F12 = SDL_SCANCODE_F12;
		constexpr KeyCode MENU = SDL_SCANCODE_MENU;
		constexpr KeyCode A = SDL_SCANCODE_A;
		constexpr KeyCode B = SDL_SCANCODE_B;
		constexpr KeyCode C = SDL_SCANCODE_C;
		constexpr KeyCode D = SDL_SCANCODE_D;
		constexpr KeyCode E = SDL_SCANCODE_E;
		constexpr KeyCode F = SDL_SCANCODE_F;
		constexpr KeyCode G = SDL_SCANCODE_G;
		constexpr KeyCode H = SDL_SCANCODE_H;
		constexpr KeyCode I = SDL_SCANCODE_I;
		constexpr KeyCode J = SDL_SCANCODE_J;
		constexpr KeyCode K = SDL_SCANCODE_K;
		constexpr KeyCode L = SDL_SCANCODE_L;
		constexpr KeyCode M = SDL_SCANCODE_M;
		constexpr KeyCode N = SDL_SCANCODE_N;
		constexpr KeyCode O = SDL_SCANCODE_O;
		constexpr KeyCode P = SDL_SCANCODE_P;
		constexpr KeyCode Q = SDL_SCANCODE_Q;
		constexpr KeyCode R = SDL_SCANCODE_R;
		constexpr KeyCode S = SDL_SCANCODE_S;
		constexpr KeyCode T = SDL_SCANCODE_T;
		constexpr KeyCode U = SDL_SCANCODE_U;
		constexpr KeyCode V = SDL_SCANCODE_V;
		constexpr KeyCode W = SDL_SCANCODE_W;
		constexpr KeyCode X = SDL_SCANCODE_X;
		constexpr KeyCode Y = SDL_SCANCODE_Y;
		constexpr KeyCode Z = SDL_SCANCODE_Z;
		constexpr KeyCode K0 = SDL_SCANCODE_0;
		constexpr KeyCode K1 = SDL_SCANCODE_1;
		constexpr KeyCode K2 = SDL_SCANCODE_2;
		constexpr KeyCode K3 = SDL_SCANCODE_3;
		constexpr KeyCode K4 = SDL_SCANCODE_4;
		constexpr KeyCode K5 = SDL_SCANCODE_5;
		constexpr KeyCode K6 = SDL_SCANCODE_6;
		constexpr KeyCode K7 = SDL_SCANCODE_7;
		constexpr KeyCode K8 = SDL_SCANCODE_8;
		constexpr KeyCode K9 = SDL_SCANCODE_9;
		constexpr KeyCode BACKSLASH = SDL_SCANCODE_BACKSLASH;
		constexpr KeyCode COMMA = SDL_SCANCODE_COMMA;
		constexpr KeyCode EQUALS = SDL_SCANCODE_EQUALS;
		constexpr KeyCode LEFT_BRACKET = SDL_SCANCODE_LEFTBRACKET;
		constexpr KeyCode RIGHT_BRACKET = SDL_SCANCODE_RIGHTBRACKET;
		constexpr KeyCode MINUS = SDL_SCANCODE_MINUS;
		constexpr KeyCode PERIOD = SDL_SCANCODE_PERIOD;
		constexpr KeyCode SEMICOLON = SDL_SCANCODE_SEMICOLON;
		constexpr KeyCode SLASH = SDL_SCANCODE_SLASH;
	}

	// END KEYBOARD SCANCODES

	enum FlipMode : uint8_t
	{
		FM_NONE,
		FM_HORIZONTAL,
		FM_VERTICAL
	};

	enum WindowState : uint16_t
	{
		WS_MINIMIZED = 64,
		WS_FOCUS = 512,
		WS_CURSOR_FOCUS = 1024
	};

	template <class T>
	struct vec2d_basic
	{
		vec2d_basic() = default;
		vec2d_basic(T x, T y)
		{
			this->x = x;
			this->y = y;
		}

		T x;
		T y;

		friend vec2d_basic<T> operator+(const vec2d_basic<T>& v1, const vec2d_basic<T>& v2)
		{
			return { v1.x + v2.x, v1.y + v2.y };
		}

		friend vec2d_basic<T> operator-(const vec2d_basic<T> v1, const vec2d_basic<T>& v2)
		{
			return { v1.x - v2.x, v1.y - v2.y };
		}

		friend vec2d_basic<T> operator*(const vec2d_basic<T> v1, const vec2d_basic<T>& v2)
		{
			return { v1.x * v2.x, v1.y * v2.y };
		}

		friend vec2d_basic<T> operator/(const vec2d_basic<T> v1, const vec2d_basic<T>& v2)
		{
			return { v1.x / v2.x, v1.y / v2.y };
		}

		friend vec2d_basic<T> operator+(const vec2d_basic<T> v1, const T v)
		{
			return { v1.x + v, v1.y + v };
		}

		friend vec2d_basic<T> operator-(const vec2d_basic<T> v1, const T v)
		{
			return { v1.x - v, v1.y - v };
		}

		friend vec2d_basic<T> operator*(const vec2d_basic<T> v1, const T v)
		{
			return { v1.x * v, v1.y * v };
		}

		friend vec2d_basic<T> operator/(vec2d_basic<T> v1, const T v)
		{
			return { v1.x / v, v1.y / v };
		}

		friend vec2d_basic<T> operator+=(vec2d_basic<T>& v1, const vec2d_basic<T>& v2)
		{
			v1.x += v2.x;
			v1.y += v2.y;
			return v1;
		}

		friend vec2d_basic<T> operator-=(vec2d_basic<T> v1, const vec2d_basic<T>& v2)
		{
			v1.x -= v2.x;
			v1.y -= v2.y;
			return v1;
		}

		friend vec2d_basic<T> operator*=(vec2d_basic<T> v1, const vec2d_basic<T>& v2)
		{
			v1.x *= v2.x;
			v1.y *= v2.y;
			return v1;
		}

		friend vec2d_basic<T> operator/=(vec2d_basic<T> v1, const vec2d_basic<T>& v2)
		{
			v1.x /= v2.x;
			v1.y /= v2.y;
			return v1;
		}

		friend vec2d_basic<T> operator+=(vec2d_basic<T> v1, const T v)
		{
			v1.x += v;
			v1.y += v;
			return v1;
		}

		friend vec2d_basic<T> operator-=(vec2d_basic<T> v1, const T v)
		{
			v1.x -= v;
			v1.y -= v;
			return v1;
		}

		friend vec2d_basic<T> operator*=(vec2d_basic<T> v1, const T v)
		{
			v1.x *= v;
			v1.y *= v;
			return v1;
		}

		friend vec2d_basic<T> operator/=(vec2d_basic<T> v1, const T v)
		{
			v1.x /= v;
			v1.y /= v;
			return v1;
		}

		friend bool operator<(const vec2d_basic<T> v1, const vec2d_basic<T> v)
		{
			return v1.x < v.x && v1.y < v.y;
		}

		friend bool operator>(const vec2d_basic<T> v1, const vec2d_basic<T> v)
		{
			return v1.x > v.x && v1.y > v.y;
		}

		T mag()
		{
			return sqrtf(this->x * this->x + this->y * this->y);
		}

		vec2d_basic<T> norm()
		{
			return { this->x / mag(), this->y / mag() };
		}

		vec2d_basic<T> lerp(float x, float y, float t)
		{
			return { this->x + t * (x - this->x), this->y + t * (y - this->y) };
		}

		T dot(vec2d_basic<T> v)
		{
			return (this->x * v.x + this->y * v.y);
		}

		vec2d_basic<T> reflect()
		{
			return vec2d_basic<T>(-this->x, -this->y);
		}

		T mag() const 
		{ 
			return T(sqrt(this->x * this->x + this->y * this->y));
		}

		T mag2() const 
		{ 
			return this->x * this->x + this->y * this->y;
		}

		vec2d_basic<T> abs()
		{
			return vec2d_basic<T>(std::abs(this->x), std::abs(this->y));
		}
	};

	typedef vec2d_basic<int> vi2d;
	typedef vec2d_basic<float> vf2d;
	typedef vec2d_basic<double> vd2d;

	struct rcode
	{
		bool ok;
		std::string info;
	};

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

		friend Pixel operator+(Pixel& lhs, float& rhs)
		{
			return Pixel(lhs.r + (uint8_t)rhs, lhs.g + (uint8_t)rhs, lhs.b + (uint8_t)rhs, lhs.a);
		}

		friend Pixel operator-(Pixel& lhs, float& rhs)
		{
			return Pixel(lhs.r - (uint8_t)rhs, lhs.g - (uint8_t)rhs, lhs.b - (uint8_t)rhs, lhs.a);
		}

		friend Pixel operator*(Pixel& lhs, float& rhs)
		{
			return Pixel(lhs.r * (uint8_t)rhs, lhs.g * (uint8_t)rhs, lhs.b * (uint8_t)rhs, lhs.a);
		}

		friend Pixel operator/(Pixel& lhs, float& rhs)
		{
			return Pixel(lhs.r / (uint8_t)rhs, lhs.g / (uint8_t)rhs, lhs.b / (uint8_t)rhs, lhs.a);
		}

		friend Pixel operator+=(Pixel& lhs, float& rhs)
		{
			lhs.r += (uint8_t)rhs;
			lhs.g += (uint8_t)rhs;
			lhs.b += (uint8_t)rhs;

			return lhs;
		}

		friend Pixel operator-=(Pixel& lhs, float& rhs)
		{
			lhs.r -= (uint8_t)rhs;
			lhs.g -= (uint8_t)rhs;
			lhs.b -= (uint8_t)rhs;

			return lhs;
		}

		friend Pixel operator*=(Pixel& lhs, float& rhs)
		{
			lhs.r *= (uint8_t)rhs;
			lhs.g *= (uint8_t)rhs;
			lhs.b *= (uint8_t)rhs;

			return lhs;
		}

		friend Pixel operator/=(Pixel& lhs, float& rhs)
		{
			lhs.r /= (uint8_t)rhs;
			lhs.g /= (uint8_t)rhs;
			lhs.b /= (uint8_t)rhs;

			return lhs;
		}

		friend bool operator==(Pixel& lhs, Pixel& rhs)
		{
			return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b;
		}
	};

	Pixel BLACK(0, 0, 0, 0),
		DARK_BLUE(0, 55, 218, 255),
		DARK_GREEN(19, 161, 14, 255),
		DARK_CYAN(59, 120, 255, 255),
		DARK_RED(197, 15, 31, 255),
		DARK_MAGENTA(136, 32, 152, 255),
		DARK_GREY(118, 118, 118, 255),
		ORANGE(255, 165, 0, 255),
		GREY(204, 204, 204, 255),
		BLUE(0, 0, 255, 255),
		GREEN(0, 255, 0, 255),
		CYAN(58, 150, 221, 255),
		RED(255, 0, 0, 255),
		MAGENTA(180, 0, 158, 255),
		YELLOW(255, 255, 0, 255),
		WHITE(255, 255, 255, 255),
		NONE(0, 0, 0, 0);

	Pixel RandomPixel(bool bRandomAlpha = false)
	{
		return Pixel(rand() % 256, rand() % 256, rand() % 256, bRandomAlpha ? rand() % 256 : 255);
	}


	/********************************
	* @ SPRITE CLASS IMPLEMENTATION *
	*********************************/

	class Sprite
	{
	public:
		Sprite() = default;
		Sprite(int32_t w, int32_t h)
		{
			if (w > 0 && h > 0)
				Create(w, h);
		}

		Sprite(std::string filename)
		{
			rcode rc;

			if (filename.ends_with(".spr"))
				rc = LoadSprSprite(std::wstring(filename.begin(), filename.end()));
			else
				rc = LoadSprite(filename);

			if (!rc.ok)
				std::cerr << rc.info << "\n";

			m_sFilename = filename;
		}

		~Sprite()
		{
			SDL_FreeSurface(m_sdlSurface);
		}

		SDL_Surface* m_sdlSurface;

		SDL_Rect m_sdlFileRect;
		SDL_Rect m_sdlCoordRect;
		uint32_t m_nTexId;

	private:
		uint32_t m_nWidth;
		uint32_t m_nHeight;

		std::string m_sFilename;

	public:
		void Create(int32_t w, int32_t h)
		{
			m_sdlSurface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);;

			m_nWidth = w;
			m_nHeight = h;

			unsigned char* pixels = (unsigned char*)m_sdlSurface->pixels;

			for (int i = 0; i < w * h * 4; i++)
				pixels[i] = 0;
		}

		rcode LoadSprite(std::string filename)
		{
			rcode rc;
			rc.ok = false;
			rc.info = "Ok";
			
			m_sdlSurface = IMG_Load(filename.c_str());

			if (!m_sdlSurface)
			{
				rc.info = "SDL: ";
				rc.info += SDL_GetError();
				return rc;
			}

			m_nWidth = m_sdlSurface->w;
			m_nHeight = m_sdlSurface->h;

			rc.ok = true;
			return rc;
		}

		rcode LoadSprSprite(std::wstring filename)
		{
			rcode rc;
			rc.ok = false;
			rc.info = "Ok";

			FILE* f = nullptr;
			_wfopen_s(&f, filename.c_str(), L"rb");
			if (f == nullptr)
			{
				rc.info = "Can't load spr file!";
				return rc;
			}

			std::fread(&m_nWidth, sizeof(int), 1, f);
			std::fread(&m_nHeight, sizeof(int), 1, f);

			if (m_nWidth < 1 || m_nHeight < 1)
			{
				rc.info = "Width or height of sprite less than 1 pixel";
				return rc;
			}

			short* colours = new short[m_nWidth * m_nHeight];
			std::fread(colours, sizeof(short), m_nWidth * m_nHeight, f);

			short* glyphs = new short[m_nWidth * m_nHeight];
			std::fread(glyphs, sizeof(short), m_nWidth * m_nHeight, f);

			std::fclose(f);

			Create(m_nWidth, m_nHeight);

			for (uint32_t i = 0; i < m_nWidth; i++)
				for (uint32_t j = 0; j < m_nHeight; j++)
				{
					def::Pixel p;

					if (glyphs[j * m_nWidth + i] == L' ')
					{
						p = def::NONE;
						continue;
					}

					switch ((int)colours[j * m_nWidth + i])
					{
					case 0:  p = def::BLACK; break;
					case 1:  p = def::DARK_BLUE; break;
					case 2:  p = def::DARK_GREEN; break;
					case 3:  p = def::DARK_CYAN; break;
					case 4:  p = def::DARK_RED; break;
					case 5:  p = def::DARK_MAGENTA; break;
					case 6:  p = def::YELLOW; break; // It should be DARK_YELLOW, but for now it is how it is
					case 7:	 p = def::GREY; break;
					case 8:	 p = def::DARK_GREY; break;
					case 9:	 p = def::BLUE; break;
					case 10: p = def::GREEN; break;
					case 11: p = def::CYAN; break;
					case 12: p = def::RED; break;
					case 13: p = def::MAGENTA; break;
					case 14: p = def::YELLOW; break;
					case 15: p = def::WHITE; break;
					}

					SetPixel(i, j, p);
				}

			delete[] colours;
			delete[] glyphs;

			rc.ok = true;
			return rc;
		}

		void SetTexId(uint32_t id)
		{
			m_nTexId = id;
		}

		uint32_t GetTexId() const
		{
			return m_nTexId;
		}

		uint32_t GetWidth() const
		{
			return m_nWidth;
		}

		uint32_t GetHeight() const
		{
			return m_nHeight;
		}

		std::string GetFilename() const
		{
			return m_sFilename;
		}

		void SetPixel(int32_t x, int32_t y, Pixel p)
		{
			unsigned char* pixels = (unsigned char*)m_sdlSurface->pixels;

			pixels[4 * (y * m_nWidth + x) + 0] = p.b;
			pixels[4 * (y * m_nWidth + x) + 1] = p.g;
			pixels[4 * (y * m_nWidth + x) + 2] = p.r;
			pixels[4 * (y * m_nWidth + x) + 3] = p.a;
		}

		Pixel GetPixel(int32_t x, int32_t y)
		{
			unsigned char* pixels = (unsigned char*)m_sdlSurface->pixels;

			Pixel p;

			p.b = pixels[4 * (y * m_nWidth + x) + 0];
			p.g = pixels[4 * (y * m_nWidth + x) + 1];
			p.r = pixels[4 * (y * m_nWidth + x) + 2];
			p.a = pixels[4 * (y * m_nWidth + x) + 3];

			return p;
		}
	};

	/************************************
	* @ END SPRITE CLASS IMPLEMENTATION *
	*************************************/


	/******************************
	* @ MAIN CLASS IMPLEMENTATION *
	******************************/

	class GameEngine
	{
	public:
		GameEngine()
		{
			m_sAppName = "Undefined";

			m_nKeyNewState = new uint8_t[256];
		}

	private:
		std::string m_sAppName;
		std::string m_sIconFileName;

		uint32_t m_nScreenWidth;
		uint32_t m_nScreenHeight;

		SDL_Window* m_sdlWindow = nullptr;
		SDL_Renderer* m_sdlRenderer = nullptr;

		bool m_bAppThreadActive;

		KeyState m_sKeys[512];
		KeyState m_sMouse[5];

		uint8_t m_nKeyOldState[512];
		uint8_t* m_nKeyNewState;

		float m_fWheelDelta = 0.0f;

		uint8_t m_nMouseOldState[5];
		uint8_t m_nMouseNewState[5];

		int32_t m_nMouseX = -1;
		int32_t m_nMouseY = -1;

		float m_fDeltaTime;

		std::vector<SDL_Texture*> m_vecTextures;
		SDL_Rect* m_sdlRect;

		Sprite* m_sprFont;

		bool m_bEnableSound = false;

	public:
		virtual bool OnUserCreate() = 0;
		virtual bool OnUserUpdate(float fDeltaTime) = 0;
		virtual void OnUserDestroy() { return; }

		rcode Construct(int nWidth, int nHeight, bool bFullScreen = false, bool bVSync = false)
		{
			rcode rc;
			rc.ok = false;

			auto get_sdl_err = [&]()
			{
				rc.info = "SDL: ";
				rc.info += SDL_GetError();
				return rc;
			};

			auto set_err = [&](std::string text)
			{
				rc.info = text;
				return rc;
			};

			if (nWidth < 0 || nHeight < 0)
				return set_err("Width or height less than zero");

			m_nScreenWidth = nWidth;
			m_nScreenHeight = nHeight;

			uint32_t f = SDL_INIT_VIDEO | SDL_INIT_EVENTS;

			if (m_bEnableSound)
				f |= SDL_INIT_AUDIO;

			if (SDL_Init(f) < 0)
				return get_sdl_err();

			m_sdlWindow = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_nScreenWidth, m_nScreenHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

			if (!m_sdlWindow)
				return get_sdl_err();

			SDL_SetWindowFullscreen(m_sdlWindow, bFullScreen);

			f = SDL_RENDERER_ACCELERATED;

			if (bVSync)
				f |= SDL_RENDERER_PRESENTVSYNC;

			m_sdlRenderer = SDL_CreateRenderer(m_sdlWindow, -1, f);

			SDL_Surface* icon = IMG_Load(m_sIconFileName.c_str());

			SDL_SetWindowIcon(m_sdlWindow, icon);
			SDL_FreeSurface(icon);

			ConstructFontSprite();

			rc.ok = true;
			rc.info = "Ok";

			return rc;
		}

		void Run()
		{
			m_bAppThreadActive = true;
			AppThread();
		}

	private:
		void AppThread()
		{
			if (m_bEnableSound)
			{
				if (!EnableAudio())
				{
					m_bAppThreadActive = false;
					m_bEnableSound = false;
				}
			}

			if (!OnUserCreate())
				m_bAppThreadActive = false;

			if (m_bAppThreadActive)
			{
				auto tp1 = std::chrono::system_clock::now();
				auto tp2 = tp1;

				for (int i = 0; i < 512; i++)
					m_sKeys[i] = { false, false, false };

				for (int i = 0; i < 512; i++)
					m_nKeyOldState[i] = 0;

				for (int i = 0; i < 5; i++)
					m_nMouseOldState[i] = 0;

				for (int i = 0; i < 512; i++)
					m_nKeyNewState[i] = 0;

				std::string title = "github.com/defini7 - defGameEngine - " + m_sAppName + " - FPS: ";

				while (m_bAppThreadActive)
				{
					tp2 = std::chrono::system_clock::now();
					m_fDeltaTime = std::chrono::duration<float>(tp2 - tp1).count();
					tp1 = tp2;

					SDL_SetWindowTitle(m_sdlWindow, (title + std::to_string(int(1.0f / m_fDeltaTime))).c_str());

					SDL_Event evt;
					while (SDL_PollEvent(&evt))
					{
						switch (evt.type)
						{
						case SDL_QUIT:
							m_bAppThreadActive = false;
							break;

						case SDL_MOUSEBUTTONDOWN:
						{
							switch (evt.button.button)
							{
							case SDL_BUTTON_LEFT:
								m_nMouseNewState[0] = 1;
								break;
							case SDL_BUTTON_RIGHT:
								m_nMouseNewState[1] = 1;
								break;
							case SDL_BUTTON_MIDDLE:
								m_nMouseNewState[2] = 1;
								break;
							case SDL_BUTTON_X1:
								m_nMouseNewState[3] = 1;
								break;
							case SDL_BUTTON_X2:
								m_nMouseNewState[4] = 1;
								break;
							}
						}
						break;

						case SDL_MOUSEBUTTONUP:
						{
							switch (evt.button.button)
							{
							case SDL_BUTTON_LEFT:
								m_nMouseNewState[0] = 0;
								break;
							case SDL_BUTTON_RIGHT:
								m_nMouseNewState[1] = 0;
								break;
							case SDL_BUTTON_MIDDLE:
								m_nMouseNewState[2] = 0;
								break;
							case SDL_BUTTON_X1:
								m_nMouseNewState[3] = 0;
								break;
							case SDL_BUTTON_X2:
								m_nMouseNewState[4] = 0;
								break;
							}
						}
						break;

						case SDL_MOUSEMOTION:
						{
							m_nMouseX = evt.motion.x;
							m_nMouseY = evt.motion.y;
						}
						break;

						case SDL_MOUSEWHEEL:
						{
							m_fWheelDelta += evt.wheel.y;
						}
						break;

						case SDL_KEYDOWN: case SDL_KEYUP:
							m_nKeyNewState = (uint8_t*)SDL_GetKeyboardState(NULL);
							break;

						}
					}

					for (int i = 0; i < 512; i++)
					{
						m_sKeys[i].bPressed = false;
						m_sKeys[i].bReleased = false;

						if (m_nKeyNewState[i] != m_nKeyOldState[i])
						{
							if (m_nKeyNewState[i])
							{
								m_sKeys[i].bPressed = !m_sKeys[i].bHeld;
								m_sKeys[i].bHeld = true;
							}
							else
							{
								m_sKeys[i].bReleased = true;
								m_sKeys[i].bHeld = false;
							}
						}

						m_nKeyOldState[i] = m_nKeyNewState[i];
					}

					for (int m = 0; m < 5; m++)
					{
						m_sMouse[m].bPressed = false;
						m_sMouse[m].bReleased = false;

						if (m_nMouseNewState[m] != m_nMouseOldState[m])
						{
							if (m_nMouseNewState[m])
							{
								m_sMouse[m].bPressed = true;
								m_sMouse[m].bHeld = true;
							}
							else
							{
								m_sMouse[m].bReleased = true;
								m_sMouse[m].bHeld = false;
							}
						}

						m_nMouseOldState[m] = m_nMouseNewState[m];
					}

					if (!OnUserUpdate(m_fDeltaTime))
						m_bAppThreadActive = false;

					SDL_RenderPresent(m_sdlRenderer);
				}
			}

			OnUserDestroy();

			QuitApp();
		}

		void QuitApp()
		{
			delete m_sprFont;

			for (auto& t : m_vecTextures)
				SDL_DestroyTexture(t);

			SDL_CloseAudioDevice(m_sdlAudioDeviceID);
			SDL_DestroyRenderer(m_sdlRenderer);
			SDL_DestroyWindow(m_sdlWindow);
			SDL_Quit();
		}

		void ConstructFontSprite()
		{
			std::string data;
			data += "?Q`0001oOch0o01o@F40o0<AGD4090LAGD<090@A7ch0?00O7Q`0600>00000000";
			data += "O000000nOT0063Qo4d8>?7a14Gno94AA4gno94AaOT0>o3`oO400o7QN00000400";
			data += "Of80001oOg<7O7moBGT7O7lABET024@aBEd714AiOdl717a_=TH013Q>00000000";
			data += "720D000V?V5oB3Q_HdUoE7a9@DdDE4A9@DmoE4A;Hg]oM4Aj8S4D84@`00000000";
			data += "OaPT1000Oa`^13P1@AI[?g`1@A=[OdAoHgljA4Ao?WlBA7l1710007l100000000";
			data += "ObM6000oOfMV?3QoBDD`O7a0BDDH@5A0BDD<@5A0BGeVO5ao@CQR?5Po00000000";
			data += "Oc``000?Ogij70PO2D]??0Ph2DUM@7i`2DTg@7lh2GUj?0TO0C1870T?00000000";
			data += "70<4001o?P<7?1QoHg43O;`h@GT0@:@LB@d0>:@hN@L0@?aoN@<0O7ao0000?000";
			data += "OcH0001SOglLA7mg24TnK7ln24US>0PL24U140PnOgl0>7QgOcH0K71S0000A000";
			data += "00H00000@Dm1S007@DUSg00?OdTnH7YhOfTL<7Yh@Cl0700?@Ah0300700000000";
			data += "<008001QL00ZA41a@6HnI<1i@FHLM81M@@0LG81?O`0nC?Y7?`0ZA7Y300080000";
			data += "O`082000Oh0827mo6>Hn?Wmo?6HnMb11MP08@C11H`08@FP0@@0004@000000000";
			data += "00P00001Oab00003OcKP0006@6=PMgl<@440MglH@000000`@000001P00000000";
			data += "Ob@8@@00Ob@8@Ga13R@8Mga172@8?PAo3R@827QoOb@820@0O`0007`0000007P0";
			data += "O`000P08Od400g`<3V=P0G`673IP0`@3>1`00P@6O`P00g`<O`000GP800000000";
			data += "?P9PL020O`<`N3R0@E4HC7b0@ET<ATB0@@l6C4B0O`H3N7b0?P01L3R000000020";

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
		}

	public:
		bool SetTitle(const char* title);
		void SetIcon(const char* filename);
		void ShowCursor(bool bShow);
		bool IsCursorHidden();
		WindowState GetScreenState();

		virtual void Draw(int32_t x, int32_t y, Pixel p = WHITE);
		virtual void Clear(Pixel p = WHITE);
		virtual void FillRectangle(int32_t x, int32_t y, int32_t x1, int32_t y1, Pixel p = WHITE);
		virtual void DrawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, Pixel p = WHITE);
		virtual void DrawRectangle(int32_t x, int32_t y, int32_t x1, int32_t y1, Pixel p = WHITE);
		virtual void DrawTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, Pixel p = WHITE);
		virtual void FillTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, Pixel p = WHITE);
		virtual void DrawCircle(int32_t x, int32_t y, uint32_t r, Pixel p = WHITE);
		virtual void FillCircle(int32_t x, int32_t y, uint32_t r, Pixel p = WHITE);
		virtual void DrawWireFrameModel(std::vector<std::pair<float, float>>& vecModelCoordinates, int32_t x, int32_t y, float r, float s, Pixel p = WHITE);
		virtual void DrawString(int32_t x, int32_t y, std::string s, Pixel p = WHITE, float scale = 1.0f);

		virtual void DrawSprite(int32_t x, int32_t y, Sprite* spr, float angle = 0.0f, float scale = 1.0f, FlipMode fm = FM_NONE);
		virtual void DrawPartialSprite(int32_t x, int32_t y, int32_t fx1, int32_t fy1, int32_t fx2, int32_t fy2, Sprite* spr);
		Sprite* CreateSprite(std::string filename);

		KeyState GetKey(KeyCode keyCode) const;
		KeyState GetMouse(short btnCode) const;
		uint32_t GetMouseX() const;
		uint32_t GetMouseY() const;

		template<typename T>
		vec2d_basic<T> GetMouse() const;

		template<typename T>
		vec2d_basic<T> GetScreenSize() const;

		uint32_t GetScreenWidth() const;
		uint32_t GetScreenHeight() const;
		uint32_t GetFPS() const;
		float GetDeltaTime() const;
		float GetWheelDelta() const;

	protected:
		// AUDIO ENGINE CLASS

		void EnableSound()
		{
			m_bEnableSound = true;
		}

		struct sAudioSample
		{
			sAudioSample() = default;
			sAudioSample(const char* sWavFile, GameEngine& ge)
			{
				uint8_t* wavData;
				SDL_AudioSpec fileSpec;
				uint32_t streamLen = 0;

				if (!SDL_LoadWAV(sWavFile, &fileSpec, (uint8_t**)&wavData, &streamLen))
				{
					std::cerr << "SDL: couldn't load audio file: " << SDL_GetError() << '\n';
					bSampleValid = false;
					return;
				}

				SDL_AudioCVT cvt;
				if (SDL_BuildAudioCVT(&cvt, fileSpec.format, fileSpec.channels, fileSpec.freq, ge.m_sdlSampleSpec.format, ge.m_sdlSampleSpec.channels, ge.m_sdlSampleSpec.freq) < 0)
				{
					std::cerr << "SDL: failed to build cvt: " << SDL_GetError() << '\n';
					bSampleValid = false;
					SDL_FreeWAV(wavData);
					return;
				}

				cvt.buf = (uint8_t*)malloc(streamLen * cvt.len_mult);
				cvt.len = streamLen;

				memcpy(cvt.buf, wavData, streamLen);

				SDL_FreeWAV(wavData);

				if (SDL_ConvertAudio(&cvt) < 0)
				{
					std::cerr << "SDL: failed to convert audio!: " << SDL_GetError() << '\n';
					bSampleValid = false;
					SDL_free(cvt.buf);
					return;
				}

				fSample = (float*)cvt.buf;
				nSamples = cvt.len_cvt / sizeof(float) / ge.m_sdlSpec.channels;
				bSampleValid = true;

				SDL_free(cvt.buf);
			}

			~sAudioSample()
			{
				SDL_FreeWAV((uint8_t*)fSample);
			}

			float* fSample = nullptr;
			uint32_t nSamples = 0;
			bool bSampleValid = false;
		};

	private:

		std::vector<sAudioSample> vecAudioSamples;

		struct sCurrentlyPlayingSample
		{
			int nAudioSampleID = 0;
			long nSamplePosition = 0;
			bool bFinished = false;
			bool bLoop = false;
		};

		std::list<sCurrentlyPlayingSample> listActiveSamples;

	public:

		// 16-bit WAVE file @ 44100Hz
		int32_t LoadAudioSample(const char* sWavFile)
		{
			if (!m_bEnableSound)
				return -1;

			sAudioSample a(sWavFile, *this);
			if (a.bSampleValid)
			{
				vecAudioSamples.push_back(a);
				return vecAudioSamples.size();
			}
			else
				return -1;
		}

		void PlaySample(int id, bool bLoop = false)
		{
			sCurrentlyPlayingSample a;

			a.nAudioSampleID = id;
			a.nSamplePosition = 0;
			a.bFinished = false;
			a.bLoop = bLoop;

			listActiveSamples.push_back(a);
		}

		void StopSample(int id)
		{
			listActiveSamples.remove_if([id](const sCurrentlyPlayingSample& a) { return a.nAudioSampleID == id; });
		}

		bool EnableAudio(uint32_t nSampleRate = 44100, uint32_t nChannels = 1, uint32_t nBlocks = 8, uint32_t nBlockSamples = 512)
		{
			SDL_AudioSpec wanted;

			SDL_zero(wanted);

			wanted.channels = nChannels;
			wanted.format = AUDIO_S16;
			wanted.freq = nSampleRate;
			wanted.samples = nBlockSamples;
			wanted.userdata = this;
			wanted.callback = forwardCallback;

			SDL_zero(m_sdlSampleSpec);

			m_sdlSampleSpec.channels = nChannels;
			m_sdlSampleSpec.format = AUDIO_F32;
			m_sdlSampleSpec.freq = nSampleRate;
			m_sdlSampleSpec.userdata = this;

			m_sdlAudioDeviceID = SDL_OpenAudioDevice(0, 0, &wanted, &m_sdlSpec, 0);

			if (!m_sdlAudioDeviceID)
			{
				std::cout << "Failed to open audio device!\n" << SDL_GetError() << '\n';
				return false;
			}

			SDL_PauseAudioDevice(m_sdlAudioDeviceID, 0);

			return true;
		}

		void DisableAudio()
		{
			SDL_CloseAudioDevice(m_sdlAudioDeviceID);
		}

	private:

		static void forwardCallback(void* userdata, uint8_t* byteStream, int len)
		{
			static_cast<GameEngine*>(userdata)->AudioThread(userdata, byteStream, len);
		}

		void AudioThread(void* userdata, uint8_t* byteStream, int len)
		{
			m_fGlobalTime = 0.0f;

			float fTimeStep = 1.0f / (float)m_sdlSpec.freq;
			float fMaxSample = float(std::pow(2, (sizeof(short) * 8) - 1) - 1);
			short nPreviousSample = 0;

			memset(byteStream, 0, len);

			int16_t* buf = (int16_t*)byteStream;

			auto clip = [](float fSample, float fMax)
			{
				if (fSample >= 0.0f)
					return std::fmin(fSample, fMax);
				else
					return std::fmax(fSample, -fMax);
			};

			uint32_t i = 0;
			for (unsigned int n = 0; n < len / sizeof(int16_t); n += m_sdlSpec.channels)
			{
				for (unsigned int c = 0; c < m_sdlSpec.channels; c++)
				{
					int16_t sample = (int16_t)(clip(GetMixerOutput(c, fTimeStep * i, fTimeStep), 1.0f) * fMaxSample);
					buf[i] = sample;
					i++;
				}

				m_fGlobalTime += fTimeStep;
			}
		}

	public:

		virtual float OnUserSoundSample(int nChannel, float fGlobalTime, float fTimeStep)
		{
			return 0.0f;
		}

		virtual float OnUserSoundFilter(int nChannel, float fGlobalTime, float fSample)
		{
			return fSample;
		}

		float GetMixerOutput(int nChannel, float fGlobalTime, float fTimeStep)
		{
			float fMixerSample = 0.0f;

			for (auto& s : listActiveSamples)
			{
				if (nChannel == 0)
					s.nSamplePosition += (long)((float)m_sdlSpec.freq * fTimeStep);

				if (s.nSamplePosition < (long)vecAudioSamples[s.nAudioSampleID - 1].nSamples)
					fMixerSample += vecAudioSamples[s.nAudioSampleID - 1].fSample[(s.nSamplePosition * m_sdlSpec.channels) + nChannel];
				else
					s.bFinished = true;
			}

			listActiveSamples.remove_if([](const sCurrentlyPlayingSample& s) { return s.bFinished; });

			fMixerSample += OnUserSoundSample(nChannel, fGlobalTime, fTimeStep);

			return OnUserSoundFilter(nChannel, fGlobalTime, fMixerSample);
		}

	public:
		SDL_AudioDeviceID m_sdlAudioDeviceID;
		SDL_AudioSpec m_sdlSpec, m_sdlSampleSpec;

		float m_fGlobalTime = 0.0f;

};

	bool GameEngine::SetTitle(const char* title)
	{
		if (sizeof(title) > 128)
		{
			std::cerr << "Max length of title is 128, but got " << sizeof(title) << " characters.";
			return false;
		}

		m_sAppName = title;

		return true;
	}

	void GameEngine::SetIcon(const char* filename)
	{
		m_sIconFileName = filename;
	}

	void GameEngine::ShowCursor(bool bShow)
	{
		SDL_ShowCursor(bShow);
	}

	bool GameEngine::IsCursorHidden()
	{
		return SDL_ShowCursor(SDL_QUERY);
	}

	WindowState GameEngine::GetScreenState() // Example: GetScreenState() & def::WM_FOCUS
	{
		return (WindowState)SDL_GetWindowFlags(m_sdlWindow);
	}

	void GameEngine::Draw(int32_t x, int32_t y, Pixel p)
	{
		if (x >= 0 && y >= 0 && x < m_nScreenWidth && y < m_nScreenHeight)
		{
			SDL_SetRenderDrawColor(m_sdlRenderer, p.r, p.g, p.b, p.a);
			SDL_RenderDrawPoint(m_sdlRenderer, x, y);
		}
	}

	void GameEngine::Clear(Pixel p)
	{
		SDL_SetRenderDrawColor(m_sdlRenderer, p.r, p.g, p.b, p.a);
		SDL_RenderClear(m_sdlRenderer);
	}

	void GameEngine::FillRectangle(int32_t x, int32_t y, int32_t x1, int32_t y1, Pixel p)
	{
		for (int i = x; i <= x1; i++)
			for (int j = y; j <= y1; j++)
				Draw(i, j, p);
	}

	void GameEngine::DrawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, Pixel p)
	{
		SDL_SetRenderDrawColor(m_sdlRenderer, p.r, p.g, p.b, p.a);
		SDL_RenderDrawLine(m_sdlRenderer, x1, y1, x2, y2);
	}

	void GameEngine::DrawRectangle(int32_t x, int32_t y, int32_t x1, int32_t y1, Pixel p)
	{
		DrawLine(x, y, x1, y, p);
		DrawLine(x1, y, x1, y1, p);
		DrawLine(x1, y1, x, y1, p);
		DrawLine(x, y1, x, y, p);
	}

	void GameEngine::DrawTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, Pixel p)
	{
		DrawLine(x1, y1, x2, y2, p);
		DrawLine(x2, y2, x3, y3, p);
		DrawLine(x3, y3, x1, y1, p);
	}

	// https://www.avrfreaks.net/sites/default/files/triangles.c
	void GameEngine::FillTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, Pixel p)
	{
		auto drawline = [&](int32_t sx, int32_t ex, int32_t ny) { for (int32_t i = sx; i <= ex; i++) Draw(i, ny, p); };

		int32_t t1x, t2x, y, minx, maxx, t1xp, t2xp;
		bool changed1 = false;
		bool changed2 = false;
		int32_t signx1, signx2, dx1, dy1, dx2, dy2;
		int32_t e1, e2;

		// Sort vertices
		if (y1 > y2) { std::swap(y1, y2); std::swap(x1, x2); }
		if (y1 > y3) { std::swap(y1, y3); std::swap(x1, x3); }
		if (y2 > y3) { std::swap(y2, y3); std::swap(x2, x3); }

		t1x = t2x = x1;
		y = y1;   // Starting points
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

		// Flat top, just process the second half
		if (y1 == y2)
			goto next;

		e1 = (int32_t)(dx1 >> 1);

		for (int32_t i = 0; i < dx1;)
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
				if (changed1) break;
				else t1x += signx1;
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
						t2xp = signx2;//t2x += signx2;
					else
						goto next2;
				}

				if (changed2)
					break;
				else
					t2x += signx2;
			}
		next2:
			if (minx > t1x) minx = t1x; if (minx > t2x) minx = t2x;
			if (maxx < t1x) maxx = t1x; if (maxx < t2x) maxx = t2x;
			drawline(minx, maxx, y);    // Draw line from min to max points found on the y
											// Now increase y
			if (!changed1) t1x += signx1;
			t1x += t1xp;
			if (!changed2) t2x += signx2;
			t2x += t2xp;
			y += 1;
			if (y == y2) break;

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

		for (int32_t i = 0; i <= dx1; i++)
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

	void GameEngine::DrawCircle(int32_t x, int32_t y, uint32_t r, Pixel p)
	{
		if (!r) return;

		int32_t x1 = 0;
		int32_t y1 = r;
		int32_t p1 = 3 - 2 * r;

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
				p1 += 4 * x1++ + 6;
			else
				p1 += 4 * (x1++ - y1--) + 10;
		}
	}

	void GameEngine::FillCircle(int32_t x, int32_t y, uint32_t r, Pixel p)
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

	void GameEngine::DrawWireFrameModel(std::vector<std::pair<float, float>>& vecModelCoordinates, int32_t x, int32_t y, float r, float s, Pixel p)
	{
		std::vector<std::pair<float, float>> vecTransformedCoordinates;
		int verts = vecModelCoordinates.size();
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

		for (int i = 0; i < verts + 1; i++)
		{
			int j = i + 1;
			DrawLine((int)vecTransformedCoordinates[i % verts].first, (int)vecTransformedCoordinates[i % verts].second,
				(int)vecTransformedCoordinates[j % verts].first, (int)vecTransformedCoordinates[j % verts].second, p);
		}
	}

	void GameEngine::DrawString(int32_t x, int32_t y, std::string s, Pixel p, float scale)
	{
		int32_t sx = 0;
		int32_t sy = 0;

		for (auto c : s)
		{
			if (c == '\n')
			{
				sx = 0; sy += int32_t(8.0f * scale);
			}
			else if (c == '\t')
			{
				sx += int32_t(32.0f * scale);
			}
			else
			{
				int32_t ox = (c - 32) % 16;
				int32_t oy = (c - 32) / 16;

				if (scale > 1)
				{
					for (uint32_t i = 0; i < 8; i++)
						for (uint32_t j = 0; j < 8; j++)
							if (m_sprFont->GetPixel(i + ox * 8, j + oy * 8).r > 0)
								for (uint32_t is = 0; is < scale; is++)
									for (uint32_t js = 0; js < scale; js++)
										Draw(x + sx + int32_t(i * scale) + is, y + sy + int32_t(j * scale) + js, p);
				}
				else
				{
					for (uint32_t i = 0; i < 8; i++)
						for (uint32_t j = 0; j < 8; j++)
							if (m_sprFont->GetPixel(i + ox * 8, j + oy * 8).r > 0)
								Draw(x + sx + i, y + sy + j, p);
				}
				sx += int32_t(8.0f * scale);
			}
		}
	}

	Sprite* GameEngine::CreateSprite(std::string filename)
	{
		Sprite* spr = new Sprite(filename);

		spr->SetTexId(m_vecTextures.size());

		m_vecTextures.push_back(SDL_CreateTextureFromSurface(m_sdlRenderer, spr->m_sdlSurface));

		spr->m_sdlCoordRect.w = spr->GetWidth();
		spr->m_sdlCoordRect.h = spr->GetHeight();

		spr->m_sdlFileRect = spr->m_sdlCoordRect;

		return spr;
	}

	void GameEngine::DrawSprite(int32_t x, int32_t y, Sprite* spr, float angle, float scale, FlipMode fm)
	{
		spr->m_sdlCoordRect.x = int32_t((float)x * scale);
		spr->m_sdlCoordRect.y = int32_t((float)y * scale);

		SDL_RenderSetScale(m_sdlRenderer, scale, scale);

		SDL_RenderCopyEx(m_sdlRenderer, m_vecTextures[spr->GetTexId()], &spr->m_sdlFileRect, &spr->m_sdlCoordRect, angle, nullptr, (SDL_RendererFlip)fm);

		SDL_RenderSetScale(m_sdlRenderer, 1.0f, 1.0f);
	}

	void GameEngine::DrawPartialSprite(int32_t x, int32_t y, int32_t fx1, int32_t fy1, int32_t fx2, int32_t fy2, Sprite* spr)
	{
		for (int i = fx1; i < fx2; i++)
			for (int j = fy1; j < fy2; j++)
			{
				def::Pixel p = spr->GetPixel(i, j);

				if (p != def::NONE)
					Draw(x + (i - fx1), y + (j - fy1), p);
			}
	}

	KeyState GameEngine::GetKey(KeyCode keyCode) const
	{
		return m_sKeys[keyCode];
	}

	KeyState GameEngine::GetMouse(short btnCode) const
	{
		return m_sMouse[btnCode];
	}

	uint32_t GameEngine::GetMouseX() const
	{
		return m_nMouseX;
	}

	uint32_t GameEngine::GetMouseY() const
	{
		return m_nMouseY;
	}

	template <typename T>
	vec2d_basic<T> GameEngine::GetMouse() const
	{
		return vec2d_basic<T>((T)m_nMouseX, (T)m_nMouseY);
	}

	uint32_t GameEngine::GetScreenWidth() const
	{
		return m_nScreenWidth;
	}

	uint32_t GameEngine::GetScreenHeight() const
	{
		return m_nScreenHeight;
	}

	template <typename T>
	vec2d_basic<T> GameEngine::GetScreenSize() const
	{
		return vec2d_basic<T>((T)m_nScreenWidth, (T)m_nScreenHeight);
	}

	uint32_t GameEngine::GetFPS() const
	{
		return 1.0f / m_fDeltaTime;
	}

	float GameEngine::GetDeltaTime() const
	{
		return m_fDeltaTime;
	}

	float GameEngine::GetWheelDelta() const
	{
		return m_fWheelDelta;
	}

	/**********************************
	* @ END MAIN CLASS IMPLEMENTATION *
	**********************************/
}

/***************************************
* @ END DEF-GAME-ENGINE IMPLEMENTATION *
***************************************/
