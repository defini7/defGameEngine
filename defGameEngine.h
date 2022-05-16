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
					Draw(i, j, def::Pixel(rand() % 255, rand() % 255, rand() % 255, 255));

			return true;
		}

	};

	int main(int argc, char* argv[]) // argc and argv are necessary
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

#if defined(PLATFORM_OPENGL) && defined(PLATFORM_SDL2)
#undef PLATFORM_SDL2
#endif

#if !defined(PLATFORM_OPENGL) && !defined(PLATFORM_SDL2)
#define PLATFORM_OPENGL
#pragma message You have not specified platform, game engine uses OpenGL by default
#endif

#include <iostream>
#include <string>
#include <thread>
#include <vector>

#if defined(PLATFORM_OPENGL)
	#include <Windows.h>
	#include <gl/GL.h>

	#if defined(_WIN32) && !defined(__MINGW32__)
		#pragma comment(lib, "opengl32.lib")
	#endif

	#if defined(STB_IMAGE_IMPLEMENTATION)
		#include "stb_image.h"
	#else
		#include <gdiplus.h>

		#if defined(__MINGW32__)
			#include <gdiplus/gdiplusinit.h>
		#else
			#include <gdiplusinit.h>
		#endif

		#if !defined(__MINGW32__)
			#pragma comment(lib, "gdiplus.lib")
		#endif
	#endif
#else
	#define PLATFORM_SDL2
	#include <SDL.h>
	#include <SDL_image.h>
#endif

namespace def
{
	enum MODE
	{
		ALPHA
	};

	template <class T>
	class vec2d_basic
	{
	public:
		vec2d_basic()
		{
			this->x = 0;
			this->y = 0;
		}

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

		friend vec2d_basic<T> operator/(const vec2d_basic<T> v1, const T v)
		{
			return { v1.x / v, v1.y / v };
		}

		float mag()
		{
			return sqrtf(this->x * this->x + this->y * this->y);
		}

		vec2d_basic<T> norm()
		{
			return { this->x / mag(), this->y / mag() };
		}

		// linear interpolation
		vec2d_basic<T> lerp(float x, float y, float t)
		{
			return { this->x + t * (x - this->x), this->y + t * (y - this->y) };
		}

		float dot(vec2d_basic<T> v, float angle)
		{
			return (this->x * v.x + this->y * v.y);
		}

		vec2d_basic<T> reflect()
		{
			return vec2d_basic<T>(-this->x, -this->y);
		}

		float distance()
		{
			return sqrtf(this->x * this->x + this->y * this->y);
		}
	};

	using vf2d = vec2d_basic<float>;
	using vi2d = vec2d_basic<int>;
	using vd2d = vec2d_basic<double>;

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
		Pixel()
		{
			r = 0;
			g = 0;
			b = 0;
			a = 255;
		}

		Pixel(uint8_t cr, uint8_t cg, uint8_t cb, uint8_t ca) : r(cr), g(cg), b(cb), a(ca)
		{
			
		}

		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;

		friend Pixel operator+(Pixel& lhs, float& rhs)
		{
			return Pixel(lhs.r + rhs, lhs.g + rhs, lhs.b + rhs, lhs.a);
		}

		friend Pixel operator-(Pixel& lhs, float& rhs)
		{
			return Pixel(lhs.r - rhs, lhs.g - rhs, lhs.b - rhs, lhs.a);
		}

		friend Pixel operator*(Pixel& lhs, float& rhs)
		{
			return Pixel(lhs.r * rhs, lhs.g * rhs, lhs.b * rhs, lhs.a);
		}

		friend Pixel operator/(Pixel& lhs, float& rhs)
		{
			return Pixel(lhs.r / rhs, lhs.g / rhs, lhs.b / rhs, lhs.a);
		}
	};

	Pixel DARK_BLUE = Pixel(0, 55, 218, 255);
	Pixel DARK_GREEN = Pixel(19, 161, 14, 255);
	Pixel DARK_CYAN = Pixel(59, 120, 255, 255);
	Pixel DARK_RED = Pixel(197, 15, 31, 255);
	Pixel DARK_MAGENTA = Pixel(136, 32, 152, 255);
	Pixel DARK_GREY = Pixel(118, 118, 118, 255);
	Pixel ORANGE = Pixel(255, 165, 0, 255);
	Pixel GREY = Pixel(204, 204, 204, 255);
	Pixel BLUE = Pixel(0, 0, 255, 255);
	Pixel GREEN = Pixel(0, 255, 0, 255);
	Pixel CYAN = Pixel(58, 150, 221, 255);
	Pixel RED = Pixel(255, 0, 0, 255);
	Pixel MAGENTA = Pixel(180, 0, 158, 255);
	Pixel YELLOW = Pixel(255, 255, 0, 255);
	Pixel WHITE = Pixel(255, 255, 255, 255);
	Pixel BLACK = Pixel(0, 0, 0, 0);

#define RANDOM_PIXEL def::Pixel(rand() % 255, rand() % 255, rand() % 255, 255)
#define RANDOM_PIXEL_ALPHA def::Pixel(rand() % 255, rand() % 255, rand() % 255, rand() % 255)

	class Sprite
	{
	public:
		Sprite()
		{
			Create(8, 8);
		}

		Sprite(int32_t w, int32_t h)
		{
			if (w > 0 && h > 0)
				Create(w, h);
		}

		Sprite(std::string filename)
		{
			rcode rc = LoadSprite(filename);
			
			if (!rc.ok)
				std::cerr << rc.info << "\n";
			
			m_sFilename = filename;
		}

		~Sprite()
		{
#if defined(PLATFORM_SDL2)
			delete m_sdlSurface;
#endif
		}

#if defined(PLATFORM_SDL2)
		SDL_Surface* m_sdlSurface;

		SDL_Rect m_sdlFileRect;
		SDL_Rect m_sdlCoordRect;
		uint32_t m_nTexId;
#endif

	private:
		uint32_t m_nWidth;
		uint32_t m_nHeight;

		std::string m_sFilename;

	public:
		void Create(int32_t w, int32_t h)
		{
#if defined(PLATFORM_SDL2)
			m_sdlSurface = new SDL_Surface;

			m_nWidth = w;
			m_nHeight = h;

			m_sdlSurface->pixels = new unsigned char[w * h * 4];
			memset(m_sdlSurface->pixels, 0, sizeof(m_sdlSurface) * sizeof(unsigned char));
#elif defined(PLATFORM_OPENGL)
			m_vecPixels.resize(w * h);

			for (int x = 0; x < w; x++)
				for (int y = 0; y < h; y++)
					m_vecPixels[y * w + x] = Pixel(255, 255, 255, 255);
#endif
		}

		rcode LoadSprite(std::string filename)
		{
			rcode rc;
			rc.ok = false;
			rc.info = "Ok";

#if defined(PLATFORM_SDL2)
			m_sdlSurface = IMG_Load(filename.c_str());

			if (!m_sdlSurface)
			{
				rc.info += "SDL: ";
				rc.info += SDL_GetError();
				return rc;
			}

			m_nWidth = m_sdlSurface->w;
			m_nHeight = m_sdlSurface->h;

#elif defined(PLATFORM_OPENGL)
			
			if (!LoadImageResource(filename))
			{
				rc.info = "Can't load image!";
				return rc;
			}
#endif

			rc.ok = true;
			return rc;
		}

#if defined(PLATFORM_OPENGL)
		std::wstring ConvertS2W(std::string s)
		{
#ifdef __MINGW32__
			wchar_t* buffer = new wchar_t[s.length() + 1];
			mbstowcs(buffer, s.c_str(), s.length());
			buffer[s.length()] = L'\0';
#else
			int count = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, NULL, 0);
			wchar_t* buffer = new wchar_t[count];
			MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, buffer, count);
#endif
			std::wstring w(buffer);
			delete[] buffer;
			return w;
		}

		bool LoadImageResource(std::string filename)
		{
			m_vecPixels.clear();

#if defined(STB_IMAGE_IMPLEMENTATION)
			int nChannels;
			std::string fn(filename.begin(), filename.end());

			unsigned char* bmp = stbi_load(fn.c_str(), &nWidth, &nHeight, &nChannels, 0);

			if (!bmp)
				return false;

			m_vecPixels.resize(nWidth * nHeight);

			for (int x = 0; x < nWidth; x++)
				for (int y = 0; y < nHeight; y++)
				{
					unsigned char* pPixelOffset = bmp + (y * nWidth + x) * nChannels;
					SetPixel(x, y, Pixel(pPixelOffset[0], pPixelOffset[1], pPixelOffset[2], nChannels >= 4 ? pPixelOffset[3] : 0xff));
				}

			stbi_image_free(bmp);
#else
			Gdiplus::Bitmap* bmp = Gdiplus::Bitmap::FromFile(ConvertS2W(filename).c_str());

			if (!bmp)
				return false;

			if (bmp->GetLastStatus() != Gdiplus::Ok)
				return false;

			m_nWidth = bmp->GetWidth();
			m_nHeight = bmp->GetHeight();

			m_vecPixels.resize(m_nWidth * m_nHeight);

			for (int x = 0; x < m_nWidth; x++)
				for (int y = 0; y < m_nHeight; y++)
				{
					Gdiplus::Color c;
					bmp->GetPixel(x, y, &c);
					SetPixel(x, y, Pixel(c.GetRed(), c.GetGreen(), c.GetBlue(), c.GetAlpha()));
				}
			delete bmp;
#endif
			return true;
		}

		std::vector<Pixel> m_vecPixels;

		Pixel* GetData()
		{
			return m_vecPixels.data();
		}
#endif
		
#if defined(PLATFORM_SDL2)
		void SetTexId(uint32_t id)
		{
			m_nTexId = id;
		}

		uint32_t GetTexId() const
		{
			return m_nTexId;
		}
#endif

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
#if defined(PLATFORM_SDL2)
			unsigned char* pixels = (unsigned char*)m_sdlSurface->pixels;
			
			pixels[4 * (y * m_nWidth + x) + 0] = p.r;
			pixels[4 * (y * m_nWidth + x) + 1] = p.g;
			pixels[4 * (y * m_nWidth + x) + 2] = p.b;
			pixels[4 * (y * m_nWidth + x) + 3] = p.a;
#elif defined(PLATFORM_OPENGL)
			m_vecPixels[y * m_nWidth + x] = p;
#endif
		}

		Pixel GetPixel(int32_t x, int32_t y)
		{
#if defined(PLATFORM_SDL2)
			unsigned char* pixels = (unsigned char*)m_sdlSurface->pixels;

			Pixel p;

			p.r = pixels[4 * (y * m_nWidth + x) + 0];
			p.g = pixels[4 * (y * m_nWidth + x) + 1];
			p.b = pixels[4 * (y * m_nWidth + x) + 2];
			p.a = pixels[4 * (y * m_nWidth + x) + 3];

			return p;
#elif defined(PLATFORM_OPENGL)
			return m_vecPixels[y * m_nWidth + x];
#endif
		}
	};

	class GameEngine
	{
	public:
		GameEngine()
		{
			m_sAppName = "Undefined";
			
#if defined(PLATFORM_SDL2)
			m_nKeyNewState = new uint8_t[256];
#elif defined(PLATFORM_OPENGL)
			m_nKeyNewState = new short[256];
#endif
			
#if defined(PLATFORM_OPENGL)
	#if !defined(STB_IMAGE_IMPLEMENTATION)
			Gdiplus::GdiplusStartupInput gdiplusStartupInput;
			GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, nullptr);
	#endif
#endif

		}

		virtual ~GameEngine()
		{
#if defined(PLATFORM_SDL2)
			SDL_DestroyRenderer(m_sdlRenderer);
			SDL_DestroyWindow(m_sdlWindow);
			SDL_Quit();
#endif
#if defined(PLATFORM_OPENGL)
			Gdiplus::GdiplusShutdown(m_gdiplusToken);
			DisableOpenGL(m_hWnd, m_hDC, m_hRC);
#endif
		}

	private:
		std::string m_sAppName;
		
		int32_t m_nScreenWidth;
		int32_t m_nScreenHeight;

		int32_t m_nPixelWidth;
		int32_t m_nPixelHeight;
		
#if defined(PLATFORM_SDL2)
		SDL_Window* m_sdlWindow = nullptr;
		SDL_Renderer* m_sdlRenderer = nullptr;
#endif

#if defined(PLATFORM_OPENGL)
		HWND m_hWnd;
		HDC m_hDC;
		HGLRC m_hRC;

		std::vector<uint32_t> m_vecTextures;

		ULONG_PTR m_gdiplusToken;
#endif
		
		bool m_bAppThreadActive;

		KeyState m_sKeys[512];
		KeyState m_sMouse[5];
		
#if defined(PLATFORM_SDL2)
		uint8_t m_nKeyOldState[512];
		uint8_t* m_nKeyNewState;
#endif
#if defined(PLATFORM_OPENGL)
		short m_nKeyOldState[512];
		short* m_nKeyNewState;
#endif

		uint8_t m_nMouseOldState[5];
		uint8_t m_nMouseNewState[5];
		
		int32_t m_nMouseX = -1;
		int32_t m_nMouseY = -1;

#if defined(PLATFORM_SDL2)
		std::vector<SDL_Texture*> m_vecTextures;
		SDL_Rect* m_sdlRect;
#endif

		Sprite* sprFont;

	public:
		virtual bool OnUserCreate() = 0;
		virtual bool OnUserUpdate(float fDeltaTime) = 0;
		virtual void OnUserDestroy() { return; }

#if defined(PLATFORM_OPENGL)
		void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
		{
			PIXELFORMATDESCRIPTOR pfd;

			*hDC = GetDC(hwnd);

			ZeroMemory(&pfd, sizeof(pfd));

			pfd.nSize = sizeof(pfd);
			pfd.nVersion = 1;
			pfd.dwFlags = PFD_DRAW_TO_WINDOW |
				PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
			pfd.iPixelType = PFD_TYPE_RGBA;
			pfd.cColorBits = 24;
			pfd.cDepthBits = 16;
			pfd.iLayerType = PFD_MAIN_PLANE;

			SetPixelFormat(*hDC, ChoosePixelFormat(*hDC, &pfd), &pfd);

			*hRC = wglCreateContext(*hDC);

			wglMakeCurrent(*hDC, *hRC);
		}

		void DisableOpenGL(HWND hwnd, HDC hDC, HGLRC hRC)
		{
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(hRC);
			ReleaseDC(hwnd, hDC);
		}

		static LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			POINT cursor;
			switch (msg)
			{
			case WM_DESTROY:
				PostQuitMessage(0);
				DestroyWindow(hWnd);
				return 0;
				
			}

			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
#endif

		rcode Construct(int nWidth, int nHeight, int nPixelWidth, int nPixelHeight, bool bFullScreen = false)
		{
			rcode rc;
			rc.ok = false;

#if defined(PLATFORM_SDL2)
			auto get_sdl_err = [&]()
			{
				rc.info += "SDL: ";
				rc.info += SDL_GetError();
				return rc;
			};
#endif
			auto set_err = [&](std::string text)
			{
				rc.info = text;
				return rc;
			};

			if (nWidth < 0 && nHeight < 0)
				return set_err("Width or height less than zero");

			m_nScreenWidth = nWidth;
			m_nScreenHeight = nHeight;

			m_nPixelWidth = nPixelWidth;
			m_nPixelHeight = nPixelHeight;

#if defined(PLATFORM_SDL2)
			if (SDL_Init(SDL_INIT_VIDEO) > 0)
				return get_sdl_err();

			m_sdlWindow = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_nScreenWidth * m_nPixelWidth, m_nScreenHeight * m_nPixelHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

			if (!m_sdlWindow)
				return get_sdl_err();

			SDL_SetWindowFullscreen(m_sdlWindow, bFullScreen);

			m_sdlRenderer = SDL_CreateRenderer(m_sdlWindow, -1, SDL_RENDERER_ACCELERATED);

#elif defined(PLATFORM_OPENGL)

			WNDCLASS wc;
			wc.style = CS_HREDRAW | CS_VREDRAW;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;

			wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
			wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);

			wc.hIcon = LoadIcon(GetModuleHandle(nullptr), nullptr);
			
			wc.lpszClassName = (LPCWSTR)m_sAppName.c_str();
			wc.lpszMenuName = nullptr;
			
			wc.hInstance = GetModuleHandle(nullptr);

			wc.lpfnWndProc = WinProc;

			RegisterClass(&wc);

			DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
			DWORD dwStyle = WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_THICKFRAME;

			int nTopLeftX = CW_USEDEFAULT;
			int nTopLeftY = CW_USEDEFAULT;

			if (bFullScreen)
			{
				dwExStyle = 0;
				dwStyle = WS_VISIBLE | WS_POPUP;

				HMONITOR hmon = MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTONEAREST);
				MONITORINFO mi = { sizeof(mi) };

				if (!GetMonitorInfo(hmon, &mi))
					return set_err("Can't get info about monitor, try to disable full screen mode or fix it somehow");

				m_nScreenWidth = mi.rcMonitor.right;
				m_nScreenHeight = mi.rcMonitor.bottom;

				nTopLeftX = 0;
				nTopLeftY = 0;
			}

			m_hWnd = CreateWindowEx(dwExStyle, (LPCWSTR)m_sAppName.c_str(), (LPCWSTR)m_sAppName.c_str(), dwStyle,
				nTopLeftX, nTopLeftY, m_nScreenWidth * m_nPixelWidth, m_nScreenHeight * m_nPixelHeight, nullptr, nullptr, GetModuleHandle(nullptr), nullptr);

			if (!m_hWnd)
				return set_err("Can't create window");
#endif

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
#if defined(PLATFORM_OPENGL)
			ShowWindow(m_hWnd, SW_SHOW);
			ShowWindow(GetConsoleWindow(), SW_SHOW);
			
			EnableOpenGL(m_hWnd, &m_hDC, &m_hRC);
			
			glOrtho(0, m_nScreenWidth * m_nPixelWidth, m_nScreenHeight * m_nPixelHeight, 0, -1.0f, 1.0f);
			glViewport(0, 0, m_nScreenWidth * m_nPixelWidth, m_nScreenHeight * m_nPixelHeight);
#endif

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

				while (m_bAppThreadActive)
				{
					tp2 = std::chrono::system_clock::now();
					std::chrono::duration<float> elapsedTime = tp2 - tp1;
					tp1 = tp2;
					
					float fDeltaTime = elapsedTime.count();
					
					char s[256];
					sprintf_s(s, 256, "github.com/defini7 - %s - FPS: %3.2f", m_sAppName.c_str(), 1.0f / fDeltaTime);

#if defined(PLATFORM_SDL2)
					SDL_SetWindowTitle(m_sdlWindow, s);
#elif defined(PLATFORM_OPENGL)
					SetWindowTextA(m_hWnd, s);
#endif

#if defined(PLATFORM_SDL2)
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
							uint32_t mouse_mask = SDL_GetMouseState(&m_nMouseX, &m_nMouseY);

							m_nMouseX /= m_nPixelWidth;
							m_nMouseY /= m_nPixelHeight;
						}
						break;

						case SDL_KEYDOWN: case SDL_KEYUP:
						{
							m_nKeyNewState = (uint8_t*)SDL_GetKeyboardState(NULL);

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
						}
						break;

						}
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

					SDL_RenderSetScale(m_sdlRenderer, m_nPixelWidth, m_nPixelHeight);

					if (!OnUserUpdate(fDeltaTime))
						m_bAppThreadActive = false;
					
					SDL_RenderPresent(m_sdlRenderer);

#elif defined(PLATFORM_OPENGL)

					MSG msg = { 0 };

					if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
					{
						auto update_pos = [&]()
						{
							RECT rctWin;
							RECT rctCli;

							GetWindowRect(m_hWnd, &rctWin);
							GetClientRect(m_hWnd, &rctCli);

							int oy = (rctWin.bottom - rctCli.bottom) - (rctWin.top - rctCli.top);
							
							m_nMouseX = LOWORD(msg.lParam) / m_nPixelWidth;
							m_nMouseY = (HIWORD(msg.lParam) + oy) / m_nPixelHeight;
						};

						switch (msg.message)
						{
						case WM_QUIT:			m_bAppThreadActive = false;												break;
						case WM_MOUSEMOVE:		update_pos();															break;
						case WM_LBUTTONDOWN:	update_pos(); m_nMouseNewState[0] = 1;									break;
						case WM_LBUTTONUP:		update_pos(); m_nMouseNewState[0] = 0;									break;
						case WM_RBUTTONDOWN:	update_pos(); m_nMouseNewState[1] = 1;									break;
						case WM_RBUTTONUP:		update_pos(); m_nMouseNewState[1] = 0;									break;
						case WM_MBUTTONDOWN:	update_pos(); m_nMouseNewState[2] = 1;									break;
						case WM_MBUTTONUP:		update_pos(); m_nMouseNewState[2] = 0;									break;
						case WM_XBUTTONDOWN:	update_pos(); m_nMouseNewState[(HIWORD(msg.wParam) == 2) ? 4 : 3] = 1;  break;
						case WM_XBUTTONUP:		update_pos(); m_nMouseNewState[(HIWORD(msg.wParam) == 2) ? 4 : 3] = 0;  break;

						default:
						{
							TranslateMessage(&msg);
							DispatchMessage(&msg);
						}

						}
					}
					else
					{
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

						glPushMatrix();

						if (!OnUserUpdate(fDeltaTime))
							m_bAppThreadActive = false;

						glPopMatrix();

						SwapBuffers(m_hDC);
					}

					for (int i = 0; i < 256; i++)
					{
						m_nKeyNewState[i] = GetAsyncKeyState(i);

						m_sKeys[i].bPressed = false;
						m_sKeys[i].bReleased = false;

						if (m_nKeyNewState[i] != m_nKeyOldState[i])
						{
							if (m_nKeyNewState[i] & 0x8000)
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
#endif
				}
			}
		}

#if defined(PLATFORM_SDL2)
		SDL_Rect* GetPixelRect(int32_t x, int32_t y)
		{
			SDL_Rect* rct = new SDL_Rect;
			rct->x = x * m_nPixelWidth;
			rct->y = y * m_nPixelHeight;
			rct->w = m_nPixelWidth;
			rct->h = m_nPixelHeight;

			return rct;
		}
#endif
		
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

			sprFont = new Sprite(128, 48);
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
					sprFont->SetPixel(px, py, Pixel(k, k, k, k));
					if (++py == 48) { px++; py = 0; }
				}
			}
		}

	public:
		void SetTitle(std::string title);
		void Draw(int32_t x, int32_t y, Pixel p = def::WHITE);
		void Clear(Pixel p = def::WHITE);
		void FillRectangle(int32_t x, int32_t y, int32_t x1, int32_t y1, Pixel p = def::WHITE);
		void DrawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, Pixel p = def::WHITE);
		void DrawRectangle(int32_t x, int32_t y, int32_t x1, int32_t y1, Pixel p = def::WHITE);
		void DrawTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, Pixel p = def::WHITE);
		void FillTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, Pixel p = def::WHITE);
		void DrawCircle(int32_t x, int32_t y, uint32_t r, Pixel p = def::WHITE);
		void FillCircle(int32_t x, int32_t y, uint32_t r, Pixel p = def::WHITE);
		void DrawWireFrameModel(std::vector<std::pair<float, float>>& vecModelCoordinates, int32_t x, int32_t y, uint32_t r, float s, Pixel p = def::WHITE);
		void DrawString(int32_t x, int32_t y, std::string s, Pixel p = def::WHITE, float scale = 1.0f);

#if defined(PLATFORM_SDL2)
		void DrawSprite(int32_t x, int32_t y, Sprite* spr, float angle = 0.0f, SDL_RendererFlip flip = SDL_FLIP_NONE);
		
#elif defined(PLATFORM_OPENGL)
		void DrawSprite(int32_t x, int32_t y, Sprite* spr, float angle = 0.0f);
#endif
		Sprite* CreateSprite(std::string filename);

		void SetMode(MODE m);
		void DisableMode(MODE m);
		KeyState GetKey(short keyCode) const;
		KeyState GetMouse(short btnCode) const;
		uint32_t GetMouseX() const;
		uint32_t GetMouseY() const;
		uint32_t GetScreenWidth() const;
		uint32_t GetScreenHeight() const;
		
	};

	void GameEngine::SetTitle(std::string title)
	{
		m_sAppName = title;
	}

	void GameEngine::Draw(int32_t x, int32_t y, Pixel p)
	{
#if defined(PLATFORM_SDL2)
		SDL_SetRenderDrawColor(m_sdlRenderer, p.r, p.g, p.b, p.a);
		SDL_RenderDrawPoint(m_sdlRenderer, x, y);
#elif defined(PLATFORM_OPENGL)
		glColor4ub(p.r, p.g, p.b, p.a);
		glRecti(x * m_nPixelWidth, y * m_nPixelHeight, x * m_nPixelWidth + m_nPixelWidth, y * m_nPixelHeight + m_nPixelHeight);
#endif
	}

	void GameEngine::Clear(Pixel p)
	{
#if defined(PLATFORM_SDL2)
		SDL_SetRenderDrawColor(m_sdlRenderer, p.r, p.g, p.b, p.a);
		SDL_RenderClear(m_sdlRenderer);
#elif defined(PLATFORM_OPENGL)
		glClearColor((float)p.r / 255.0f, (float)p.g / 255.0f, (float)p.b / 255.0f, (float)p.a / 255.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#endif
	}
	
	void GameEngine::FillRectangle(int32_t x, int32_t y, int32_t x1, int32_t y1, Pixel p)
	{
		for (int i = x; i <= x1; i++)
			for (int j = y; j <= y1; j++)
				Draw(i, j, p);
	}

	void GameEngine::DrawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, Pixel p)
	{
#if defined(PLATFORM_SDL2)
		SDL_SetRenderDrawColor(m_sdlRenderer, p.r, p.g, p.b, p.a);
		SDL_RenderDrawLine(m_sdlRenderer, x1, y1, x2, y2);
#elif defined(PLATFORM_OPENGL)
		glScalef(m_nPixelWidth, m_nPixelHeight, 1.0f);

		glColor4ub(p.r, p.g, p.b, p.a);
		glBegin(GL_LINES);
			glVertex2i(x1, y1);
			glVertex2i(x2, y2);
		glEnd();
#endif
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

	void GameEngine::DrawWireFrameModel(std::vector<std::pair<float, float>>& vecModelCoordinates, int32_t x, int32_t y, uint32_t r, float s, Pixel p)
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
				sx = 0; sy += 8 * scale;
			}
			else if (c == '\t')
			{
				sx += 32 * scale;
			}
			else
			{
				int32_t ox = (c - 32) % 16;
				int32_t oy = (c - 32) / 16;

				if (scale > 1)
				{
					for (uint32_t i = 0; i < 8; i++)
						for (uint32_t j = 0; j < 8; j++)
							if (sprFont->GetPixel(i + ox * 8, j + oy * 8).r > 0)
								for (uint32_t is = 0; is < scale; is++)
									for (uint32_t js = 0; js < scale; js++)
										Draw(x + sx + (i * scale) + is, y + sy + (j * scale) + js, p);
				}
				else
				{
					for (uint32_t i = 0; i < 8; i++)
						for (uint32_t j = 0; j < 8; j++)
							if (sprFont->GetPixel(i + ox * 8, j + oy * 8).r > 0)
								Draw(x + sx + i, y + sy + j, p);
				}
				sx += 8 * scale;
			}
		}
	}

	Sprite* GameEngine::CreateSprite(std::string filename)
	{
#if defined(PLATFORM_SDL2)
		Sprite* spr = new Sprite(filename);

		spr->SetTexId(m_vecTextures.size());

		m_vecTextures.push_back(SDL_CreateTextureFromSurface(m_sdlRenderer, spr->m_sdlSurface));

		spr->m_sdlCoordRect.w = m_nPixelWidth * spr->GetWidth();
		spr->m_sdlCoordRect.h = m_nPixelHeight * spr->GetHeight();

		spr->m_sdlFileRect = spr->m_sdlCoordRect;

		return spr;
#endif
		return new def::Sprite(filename);
	}

#if defined(PLATFORM_SDL2)

	void GameEngine::DrawSprite(int32_t x, int32_t y, Sprite* spr, float angle, SDL_RendererFlip flip)
	{
		spr->m_sdlCoordRect.x = x * m_nPixelWidth;
		spr->m_sdlCoordRect.y = y * m_nPixelHeight;

		SDL_RenderCopyEx(m_sdlRenderer, m_vecTextures[spr->GetTexId()], &spr->m_sdlFileRect, &spr->m_sdlCoordRect, angle, nullptr, flip);
	}

#elif defined(PLATFORM_OPENGL)
	
	void GameEngine::DrawSprite(int32_t x, int32_t y, Sprite* spr, float angle)
	{
		glPushMatrix();

		glRotatef(angle, 1.0f, 0.0f, 0.0f);

		glBegin(GL_POINTS);
		for (int32_t i = 0; i < spr->GetWidth(); i++)
			for (int32_t j = 0; j < spr->GetHeight(); j++)
			{
				const Pixel p = spr->GetPixel(i, j);
				glColor4ub(p.r, p.g, p.b, p.a);
				glVertex2f(x + i, y + j);
			}
		glEnd();
		
		glPopMatrix();
	}
#endif

	void GameEngine::SetMode(MODE m)
	{
		switch (m)
		{
		case MODE::ALPHA:
		{
#if defined(PLATFORM_OPENGL)
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#endif
		}
		}
	}

	void GameEngine::DisableMode(MODE m)
	{
		switch (m)
		{
		case MODE::ALPHA:
		{
#if defined(PLATFORM_OPENGL)
			glDisable(GL_BLEND);
#endif
		}
		}
	}

	KeyState GameEngine::GetKey(short keyCode) const
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

	uint32_t GameEngine::GetScreenWidth() const
	{
		return m_nScreenWidth;
	}

	uint32_t GameEngine::GetScreenHeight() const
	{
		return m_nScreenHeight;
	}
}
