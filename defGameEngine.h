#pragma once

#pragma region license
/***
*	BSD 3-Clause License
	Copyright (c) 2022, 2023 Alex
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

		bool OnUserUpdate(float deltaTime) override
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

		demo.Construct(256, 240, 4, 4);
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
#include <memory>
#include <algorithm>

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

#ifndef DGE_ASSERT
#define DGE_ASSERT(expr, msg) \
	if (!(expr)) \
	{ \
		std::cout << msg << std::endl; \
		exit(1); \
	}
#endif

#ifndef DGE_MIX
#define DGE_MIX(x, y, f, t) t(float(x) * (1.0f - f) + float(y) * f)
#endif

#ifndef DGE_MAX
#define DGE_MAX(x, y) (((x) > (y)) ? (x) : (y)) 
#endif

#ifndef DGE_MIN
#define DGE_MIN(x, y) (((x) < (y)) ? (x) : (y)) 
#endif

namespace def
{
	// Keyboard keys constants for convenient usage
	namespace Key
	{
		enum Keys : uint32_t
		{
			SPACE = 32, APOSTROPHE = 39, COMMA = 44, MINUS, PERIOD, SLASH,

			K0, K1, K2, K3, K4, K5, K6, K7, K8, K9,

			SEMICOLON = 59, EQUAL = 61,

			A = 65, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S,
			T, U, V, W, X, Y, Z,

			LEFT_BRACKET, BACKSLASH, RIGHT_BRACKET,
			GRAVE_ACCENT = 96, WORLD_1 = 161, WORLD_2 = 162,

			ESCAPE = 256, ENTER, TAB, BACKSPACE, INSERT, DEL, RIGHT, LEFT,
			DOWN, UP, PAGE_UP, PAGE_DOWN, HOME, END,
			CAPS_LOCK = 280, SCROLL_LOCK, NUM_LOCK, PRINT_SCREEN, PAUSE,

			F1 = 290, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13,
			F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24, F25,

			NP_0 = 320, NP_1, NP_2, NP_3, NP_4, NP_5, NP_6, NP_7, NP_8,
			NP_9, NP_DECIMAL, NP_DIVIDE, NP_MULTIPLY, NP_SUBTRACT, NP_ADD,
			NP_ENTER, NP_EQUAL,

			LEFT_SHIFT = 340, LEFT_CONTROL, LEFT_ALT, LEFT_SUPER, RIGHT_SHIFT,
			RIGHT_CONTROL, RIGHT_ALT, RIGHT_SUPER, MENU,
		};
	}

	template <typename T>
	struct v2d
	{
		v2d() = default;
		v2d(const T& x, const T& y);

		T x = (T)0;
		T y = (T)0;

		friend bool operator==(const v2d<T>& v1, const v2d<T>& v2) { return v1.x == v2.x && v1.y == v2.y; }
		friend bool operator!=(const v2d<T>& v1, const v2d<T>& v2) { return v1.x != v2.x || v1.y != v2.y; }
		friend bool operator<(const v2d<T>& v1, const v2d<T>& v2) { return v1.x < v2.x&& v1.y < v2.y; }
		friend bool operator>(const v2d<T>& v1, const v2d<T>& v2) { return v1.x > v2.x && v1.y > v2.y; }
		friend bool operator<=(const v2d<T>& v1, const v2d<T>& v2) { return v1.x <= v2.x && v1.y <= v2.y; }
		friend bool operator>=(const v2d<T>& v1, const v2d<T>& v2) { return v1.x >= v2.x && v1.y >= v2.y; }

		friend v2d<T> operator+(const v2d<T>& v1, const v2d<T>& v2) { return v2d<T>(v1.x + v2.x, v1.y + v2.y); }
		friend v2d<T> operator-(const v2d<T>& v1, const v2d<T>& v2) { return v2d<T>(v1.x - v2.x, v1.y - v2.y); }
		friend v2d<T> operator*(const v2d<T>& v1, const v2d<T>& v2) { return v2d<T>(v1.x * v2.x, v1.y * v2.y); }
		friend v2d<T> operator/(const v2d<T>& v1, const v2d<T>& v2) { return v2d<T>(v1.x / v2.x, v1.y / v2.y); }
		friend v2d<T> operator%(const v2d<T>& v1, const v2d<T>& v2) { return v2d<T>(v1.x % v2.x, v1.y % v2.y); }

		friend v2d<T> operator+(const v2d<T>& v1, const T& v2) { return v2d<T>(v1.x + v2, v1.y + v2); }
		friend v2d<T> operator-(const v2d<T>& v1, const T& v2) { return v2d<T>(v1.x - v2, v1.y - v2); }
		friend v2d<T> operator*(const v2d<T>& v1, const T& v2) { return v2d<T>(v1.x * v2, v1.y * v2); }
		friend v2d<T> operator/(const v2d<T>& v1, const T& v2) { return v2d<T>(v1.x / v2, v1.y / v2); }
		friend v2d<T> operator%(const v2d<T>& v1, const T& v2) { return v2d<T>(v1.x % v2, v1.y % v2); }

		friend v2d<T> operator*(const float& lhs, const v2d<T>& rhs) { return v2d<T>((T)(lhs * (float)rhs.x), (T)(lhs * (float)rhs.y)); }
		friend v2d<T> operator*(const double& lhs, const v2d<T>& rhs) { return v2d<T>((T)(lhs * (double)rhs.x), (T)(lhs * (double)rhs.y)); }
		friend v2d<T> operator*(const int& lhs, const v2d<T>& rhs) { return v2d<T>((T)(lhs * (int)rhs.x), (T)(lhs * (int)rhs.y)); }
		friend v2d<T> operator/(const float& lhs, const v2d<T>& rhs) { return v2d<T>((T)(lhs / (float)rhs.x), (T)(lhs / (float)rhs.y)); }
		friend v2d<T> operator/(const double& lhs, const v2d<T>& rhs) { return v2d<T>((T)(lhs / (double)rhs.x), (T)(lhs / (double)rhs.y)); }
		friend v2d<T> operator/(const int& lhs, const v2d<T>& rhs) { return v2d<T>((T)(lhs / (int)rhs.x), (T)(lhs / (int)rhs.y)); }

		operator v2d<int>() const { return { static_cast<int32_t>(this->x), static_cast<int32_t>(this->y) }; }
		operator v2d<float>() const { return { static_cast<float>(this->x),static_cast<float>(this->y) }; }
		operator v2d<double>() const { return { static_cast<double>(this->x), static_cast<double>(this->y) }; }
		operator v2d<uint32_t>() const { return { static_cast<uint32_t>(this->x), static_cast<uint32_t>(this->y) }; }

		v2d<T>& operator+=(const v2d<T>& v);
		v2d<T>& operator-=(const v2d<T>& v);
		v2d<T>& operator*=(const v2d<T>& v);
		v2d<T>& operator/=(const v2d<T>& v);

		v2d<T>& operator+=(const T& v);
		v2d<T>& operator-=(const T& v);
		v2d<T>& operator*=(const T& v);
		v2d<T>& operator/=(const T& v);
		v2d<T>& operator%=(const T& v);

		v2d<T> clamp(const v2d<T>& start, const v2d<T>& end) const;

		float dot(const v2d<T>& v) const;
		float length() const;

		T mag() const;
		T mag2() const;

		float man(const v2d<T>& v) const;

		v2d<T> max(const v2d<T>& v) const;
		v2d<T> min(const v2d<T>& v) const;

		void swap(v2d<T>& v);

		v2d<T> norm() const;
		v2d<T> abs() const;
		v2d<T> perp() const;
		v2d<T> floor() const;
		v2d<T> ceil() const;
		v2d<T> round() const;
		v2d<T> cart() const;
		v2d<T> polar() const;
		v2d<T>& ref() const;
		operator std::string() const;
		std::string str() const;
	};

	typedef v2d<int> vi2d;
	typedef v2d<float> vf2d;
	typedef v2d<uint32_t> vu2d;

	struct KeyState
	{
		bool held;
		bool released;
		bool pressed;
	};

	struct Pixel
	{
		Pixel(uint8_t r = 0u, uint8_t g = 0u, uint8_t b = 0u, uint8_t a = 255u);

		enum Mode
		{
			DEFAULT,
			ALPHA,
			MASK,
			CUSTOM
		};

		uint8_t r, g, b, a;

		Pixel mix(const def::Pixel& rhs, const float factor) const;
		Pixel& clamp();
		Pixel& ref();
		std::string str() const;

		Pixel operator+(const float rhs) const;
		Pixel operator-(const float rhs) const;
		Pixel operator*(const float rhs) const;
		Pixel operator/(const float rhs) const;

		Pixel& operator+=(const float rhs);
		Pixel& operator-=(const float rhs);
		Pixel& operator*=(const float rhs);
		Pixel& operator/=(const float rhs);

		Pixel operator+(const def::Pixel& rhs) const;
		Pixel operator-(const def::Pixel& rhs) const;
		Pixel operator*(const def::Pixel& rhs) const;
		Pixel operator/(const def::Pixel& rhs) const;

		Pixel& operator+=(const def::Pixel& rhs);
		Pixel& operator-=(const def::Pixel& rhs);
		Pixel& operator*=(const def::Pixel& rhs);
		Pixel& operator/=(const def::Pixel& rhs);

		bool operator==(const def::Pixel& rhs) const;
		bool operator!=(const def::Pixel& rhs) const;
		bool operator>(const def::Pixel& rhs) const;
		bool operator<(const def::Pixel& rhs) const;
		bool operator>=(const def::Pixel& rhs) const;
		bool operator<=(const def::Pixel& rhs) const;

		bool operator==(const float rhs) const;
		bool operator!=(const float rhs) const;
		bool operator>(const float rhs) const;
		bool operator<(const float rhs) const;
		bool operator>=(const float rhs) const;
		bool operator<=(const float rhs) const;
	};

	static Pixel BLACK(0, 0, 0, 255),
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

	Pixel PixelF(float r, float g, float b, float a = 1.0f)
	{
		return Pixel(uint8_t(r * 255.0f), uint8_t(g * 255.0f), uint8_t(b * 255.0f), uint8_t(a * 255.0f));
	}

	Pixel RandomPixel(bool isRandomAlpha = false)
	{
		return Pixel(rand() % 256, rand() % 256, rand() % 256, isRandomAlpha ? rand() % 256 : 255);
	}

#define DGE_B2F(v) ((float)v / 255.0f)
#define DGE_F2B(v) uint8_t(v * 255.0f)

	class Sprite
	{
	public:
		enum class FileType { BMP, PNG, JPG, TGA, TGA_RLE };

		enum class SampleMethod { LINEAR, BILINEAR, TRILINEAR };
		enum class WrapMethod { NONE, REPEAT, MIRROR, CLAMP };

		Sprite() = default;
		Sprite(const int32_t width, const int32_t height, const int32_t channels = 4);
		Sprite(const std::string& filename);
		~Sprite();

	public:
		int32_t width;
		int32_t height;
		int32_t channels;

		uint8_t* pixelData = nullptr;

	public:
		void Create(const int32_t width, const int32_t height, const int32_t channels = 4);
		void Load(const std::string& filename);

		void Save(const std::string& filename, const FileType type) const;

		bool SetPixel(const def::vi2d& pos, const Pixel& p);
		Pixel GetPixel(const def::vi2d& pos, const WrapMethod wrap = WrapMethod::NONE) const;

		void SetPixelData(const uint8_t* data);
		void SetPixelData(const def::Pixel& col);

		Pixel Sample(const def::vf2d& uv, const SampleMethod sampleMethod, const WrapMethod wrapMethod) const;
	};

	struct Texture
	{
		enum TextureStructure : int32_t
		{
			DEFAULT,
			FAN,
			STRIP
		};

		Texture(Sprite* sprite);
		Texture(const std::string& fileName);

		GLuint id;

		float UVScaleX;
		float UVScaleY;

		int width;
		int height;

		void Load(Sprite* sprite);
		void Update(Sprite* sprite);

	private:
		void Construct(Sprite* sprite, bool deleteSprite);
	};

	struct Graphic
	{
		Graphic() = default;
		Graphic(const std::string& fileName);
		Graphic(const int32_t width, const int32_t height);

		~Graphic();

		Texture* texture = nullptr;
		Sprite* sprite = nullptr;

		void Load(const std::string& fileName);
		void Load(const int32_t width, const int32_t height);
		void Save(const std::string& filename, const Sprite::FileType type) const;
		void UpdateTexture();
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
		std::string m_AppName;

		vi2d m_WindowSize;
		vi2d m_ScreenSize;
		vf2d m_InvScreenSize;
		vi2d m_PixelSize;
		vi2d m_MaxWindowSize;

		GLFWwindow* m_Window;
		GLFWmonitor* m_Monitor;

		bool m_IsAppRunning;
		bool m_IsFullScreen;
		bool m_IsVSync;
		bool m_IsDirtyPixel;

		KeyState m_Keys[512];
		KeyState m_Mouse[5];

		bool m_KeyOldState[512];
		bool m_KeyNewState[512];

		bool m_MouseOldState[5];
		bool m_MouseNewState[5];

		vi2d m_MousePos;

		Sprite* m_Font;

		Graphic* m_Screen;
		Graphic* m_DrawTarget;
		std::vector<TextureInstance> m_Textures;
		Pixel m_Tint;

		int32_t m_PixelMode;
		int32_t m_TextureStructure;

		float m_TickTimer;

		Pixel(*m_Shader)(const vi2d&, const Pixel&, const Pixel&) = nullptr;

	public:
		inline static std::vector<std::string> s_DropCache;

		virtual bool OnUserCreate() = 0;
		virtual bool OnUserUpdate(float deltaTime) = 0;

		void Construct(int32_t screenWidth, int32_t screenHeight, int32_t pixelWidth, int32_t pixelHeight, bool isFullScreen = false, bool isVSync = false, bool isDirtyPixel = false);
		void Run();

	private:
		void Destroy();
		void AppThread();

		static void DrawQuad(const Pixel& tint);
		static void ErrorCallback(int errCode, const char* desc);
		static void DropCallback(GLFWwindow* window, int pathCount, const char* paths[]);

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

		void DrawPartialRotatedTexture(const vf2d& pos, const vf2d& fpos, const vf2d& fsize, float r, Texture* tex, const vf2d& center = { 0.0f, 0.0f }, const vf2d& scale = { 1.0f, 1.0f }, const Pixel& tint = WHITE);
		virtual void DrawPartialRotatedTexture(float x, float y, float fx, float fy, float fw, float fh, float r, Texture* tex, float centerX = 0.0f, float centerY = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f, const Pixel& tint = WHITE);

		void DrawWireFrameModel(const std::vector<vf2d>& modelCoordinates, const vf2d& pos, float r = 0.0f, float s = 1.0f, const Pixel& p = WHITE);
		virtual void DrawWireFrameModel(const std::vector<vf2d>& modelCoordinates, float x, float y, float r = 0.0f, float s = 1.0f, const Pixel& p = WHITE);

		void FillWireFrameModel(const std::vector<vf2d>& modelCoordinates, const vf2d& pos, float r = 0.0f, float s = 1.0f, const Pixel& p = WHITE);
		virtual void FillWireFrameModel(const std::vector<vf2d>& modelCoordinates, float x, float y, float r = 0.0f, float s = 1.0f, const Pixel& p = WHITE);

		void DrawString(const vi2d& pos, const std::string& text, const Pixel& p = WHITE);
		virtual void DrawString(int32_t x, int32_t y, const std::string& text, const Pixel& p = WHITE);

		virtual void Clear(const Pixel& p);

		KeyState GetKey(uint32_t k) const;
		KeyState GetMouse(uint32_t k) const;

		vi2d GetMouse() const;

		int32_t MouseX() const;
		int32_t MouseY() const;

		void SetTitle(const std::string& title);

		vi2d ScreenSize() const;
		vi2d MaxScreenSize() const;

		int32_t ScreenWidth() const;
		int32_t ScreenHeight() const;

		bool IsFullScreen() const;
		bool IsVSync() const;
		bool IsFocused() const;

		void SetIcon(const std::string& fileName);

		void SetDrawTarget(Graphic* target);
		Graphic* GetDrawTarget() const;

		WindowState GetWindowState() const;
		GLFWwindow* GetWindow() const;

		static std::vector<std::string>& GetDropped();

		void DrawTexture(const TextureInstance& tex);

		void SetPixelMode(int32_t pixelMode);
		int32_t GetPixelMode() const;

		void SetTextureStructure(int32_t textureStructure);
		int32_t GetTextureStructure() const;

		void ClearBuffer(const Pixel& p);
		void SetTint(const Pixel& p);

		void SetShader(Pixel(*func)(const vi2d& pos, const Pixel& prev, const Pixel& cur));

	};

#ifdef DGE_APPLICATION
#undef DGE_APPLICATION

	template <class T>
	v2d<T>::v2d(const T& x, const T& y)
	{
		this->x = x;
		this->y = y;
	}

	template <class T>
	v2d<T>& v2d<T>::operator+=(const v2d<T>& v)
	{
		this->x += v.x;
		this->y += v.y;
		return *this;
	}

	template <class T>
	v2d<T>& v2d<T>::operator-=(const v2d<T>& v)
	{
		this->x -= v.x;
		this->y -= v.y;
		return *this;
	}

	template <class T>
	v2d<T>& v2d<T>::operator*=(const v2d<T>& v)
	{
		this->x *= v.x;
		this->y *= v.y;
		return *this;
	}

	template <class T>
	v2d<T>& v2d<T>::operator/=(const v2d<T>& v)
	{
		this->x /= v.x;
		this->y /= v.y;
		return *this;
	}

	template <class T>
	v2d<T>& v2d<T>::operator+=(const T& v)
	{
		this->x += v;
		this->y += v;
		return *this;
	}

	template <class T>
	v2d<T>& v2d<T>::operator-=(const T& v)
	{
		this->x -= v;
		this->y -= v;
		return *this;
	}

	template <class T>
	v2d<T>& v2d<T>::operator*=(const T& v)
	{
		this->x *= v;
		this->y *= v;
		return *this;
	}

	template <class T>
	v2d<T>& v2d<T>::operator/=(const T& v)
	{
		this->x /= v;
		this->y /= v;
		return *this;
	}

	template <class T>
	v2d<T>& v2d<T>::operator%=(const T& v)
	{
		this->x %= v;
		this->y %= v;
		return *this;
	}

	template <class T>
	v2d<T> v2d<T>::clamp(const v2d<T>& start, const v2d<T>& end) const
	{
		v2d<T> clamped = { this->x, this->y };
		clamped.x = std::clamp(clamped.x, start.x, end.x);
		clamped.y = std::clamp(clamped.y, start.y, end.y);
		return clamped;
	}

	template <class T> float v2d<T>::dot(const v2d<T>& v) const { return this->x * v.x + this->y * v.y; }
	template <class T> float v2d<T>::length() const { return sqrtf(dot(*this)); }

	template <class T> T v2d<T>::mag() const { return static_cast<T>(sqrtf(this->x * this->x + this->y * this->y)); }
	template <class T> T v2d<T>::mag2() const { return static_cast<T>(this->x * this->x + this->y * this->y); }
	template <class T> float v2d<T>::man(const v2d<T>& v) const { return std::abs(this->x - v.x) + std::abs(this->y - v.y); }

	template <class T> v2d<T> v2d<T>::max(const v2d<T>& v) const { return v2d<T>(DGE_MAX(this->x, v.x), DGE_MAX(this->y, v.y)); }
	template <class T> v2d<T> v2d<T>::min(const v2d<T>& v) const { return v2d<T>(DGE_MIN(this->x, v.x), DGE_MIN(this->y, v.y)); }

	template <class T>
	void v2d<T>::swap(v2d<T>& v)
	{
		std::swap(x, v.x);
		std::swap(y, v.y);
	}

	template <class T> v2d<T> v2d<T>::norm() const { float n = 1.0f / mag(); return v2d<T>(this->x * n, this->y * n); }
	template <class T> v2d<T> v2d<T>::abs() const { return v2d<T>(std::abs(this->x), std::abs(this->y)); }
	template <class T> v2d<T> v2d<T>::perp() const { return v2d<T>(-this->y, this->x); }
	template <class T> v2d<T> v2d<T>::floor() const { return v2d<T>(std::floor(this->x), std::floor(this->y)); }
	template <class T> v2d<T> v2d<T>::ceil() const { return v2d<T>(std::ceil(this->x), std::ceil(this->y)); }
	template <class T> v2d<T> v2d<T>::round() const { return v2d<T>(std::round(this->x), std::round(this->y)); }
	template <class T> v2d<T> v2d<T>::cart() const { return v2d<T>(cos(this->y) * this->x, sin(this->y) * this->x); }
	template <class T> v2d<T> v2d<T>::polar() const { return v2d<T>(mag(), atan2(this->y, this->x)); }
	template <class T> v2d<T>& v2d<T>::ref() const { return *this; }

	template <class T> v2d<T>::operator std::string() const { return "(" + std::to_string(this->x) + ", " + std::to_string(this->y) + ")"; }
	template <class T> std::string v2d<T>::str() const { return operator std::string(); }

	Pixel::Pixel(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) {}

	Pixel Pixel::mix(const def::Pixel& rhs, const float factor) const
	{
		return Pixel(
			DGE_MIX(r, rhs.r, factor, uint8_t),
			DGE_MIX(g, rhs.g, factor, uint8_t),
			DGE_MIX(b, rhs.b, factor, uint8_t),
			DGE_MIX(a, rhs.a, factor, uint8_t)
		);
	}

	Pixel& Pixel::clamp()
	{
		r = DGE_MIN(255, DGE_MAX(0, r));
		g = DGE_MIN(255, DGE_MAX(0, g));
		b = DGE_MIN(255, DGE_MAX(0, b));
		return ref();
	}
	
	Pixel& Pixel::ref() { return *this; }

	std::string Pixel::str() const
	{
		return '(' +
			std::to_string(r) + ',' +
			std::to_string(g) + ',' +
			std::to_string(b) + ',' +
			std::to_string(a) + ')';
	}

	Pixel Pixel::operator+(const float rhs) const { return Pixel(r + rhs, g + rhs, b + rhs, a).clamp(); }
	Pixel Pixel::operator-(const float rhs) const { return Pixel(r - rhs, g - rhs, b - rhs, a).clamp(); }
	Pixel Pixel::operator*(const float rhs) const { return Pixel(r * rhs, g * rhs, b * rhs, a).clamp(); }
	Pixel Pixel::operator/(const float rhs) const { return Pixel(r / rhs, g / rhs, b / rhs, a).clamp(); }

	Pixel& Pixel::operator+=(const float rhs)
	{
		r += rhs;
		g += rhs;
		b += rhs;
		return ref().clamp();
	}

	Pixel& Pixel::operator-=(const float rhs)
	{
		r -= rhs;
		g -= rhs;
		b -= rhs;
		return ref().clamp();
	}

	Pixel& Pixel::operator*=(const float rhs)
	{
		r *= rhs;
		g *= rhs;
		b *= rhs;
		return ref().clamp();
	}

	Pixel& Pixel::operator/=(const float rhs)
	{
		r /= rhs;
		g /= rhs;
		b /= rhs;
		return ref().clamp();
	}

	Pixel Pixel::operator+(const Pixel& rhs) const { return Pixel(r + rhs.r, g + rhs.g, b + rhs.b, a).clamp(); }
	Pixel Pixel::operator-(const Pixel& rhs) const { return Pixel(r - rhs.r, g - rhs.g, b - rhs.b, a).clamp(); }
	Pixel Pixel::operator*(const Pixel& rhs) const { return Pixel(r * rhs.r, g * rhs.g, b * rhs.b, a).clamp(); }
	Pixel Pixel::operator/(const Pixel& rhs) const { return Pixel(r / rhs.r, g / rhs.g, b / rhs.b, a).clamp(); }

	Pixel& Pixel::operator+=(const Pixel& rhs)
	{
		r += rhs.r;
		g += rhs.g;
		b += rhs.b;
		return ref().clamp();
	}

	Pixel& Pixel::operator-=(const Pixel& rhs)
	{
		r -= rhs.r;
		g -= rhs.g;
		b -= rhs.b;
		return ref().clamp();
	}

	Pixel& Pixel::operator*=(const Pixel& rhs)
	{
		r *= rhs.r;
		g *= rhs.g;
		b *= rhs.b;
		return ref().clamp();
	}

	Pixel& Pixel::operator/=(const Pixel& rhs)
	{
		r /= rhs.r;
		g /= rhs.g;
		b /= rhs.b;
		return ref().clamp();
	}

	bool Pixel::operator==(const Pixel& rhs) const { return r == rhs.r && g == rhs.g && b == rhs.b; }
	bool Pixel::operator!=(const Pixel& rhs) const { return r != rhs.r || g != rhs.g || b != rhs.b; }
	bool Pixel::operator>(const Pixel& rhs) const { return r > rhs.r && g > rhs.g && b > rhs.b; }
	bool Pixel::operator<(const Pixel& rhs) const { return r < rhs.r&& g < rhs.g&& b < rhs.b; }
	bool Pixel::operator>=(const Pixel& rhs) const { return r >= rhs.r && g >= rhs.g && b >= rhs.b; }
	bool Pixel::operator<=(const Pixel& rhs) const { return r <= rhs.r && g <= rhs.g && b <= rhs.b; }

	bool Pixel::operator==(const float rhs) const { return r == rhs && g == rhs && b == rhs; }
	bool Pixel::operator!=(const float rhs) const { return r != rhs && g != rhs && b != rhs; }
	bool Pixel::operator>(const float rhs) const { return r > rhs && g > rhs && b > rhs; }
	bool Pixel::operator<(const float rhs) const { return r < rhs&& g < rhs&& b < rhs; }
	bool Pixel::operator>=(const float rhs) const { return r >= rhs && g >= rhs && b >= rhs; }
	bool Pixel::operator<=(const float rhs) const { return r <= rhs && g <= rhs && b <= rhs; }

	Sprite::Sprite(int32_t width, int32_t height, int32_t channels)
	{
		Create(width, height, channels);
	}

	Sprite::Sprite(const std::string& fileName)
	{
		Load(fileName);
	}

	Sprite::~Sprite()
	{
		if (pixelData != nullptr)
			delete[] pixelData;
	}

	void Sprite::Create(const int32_t width, const int32_t height, const int32_t channels)
	{
		DGE_ASSERT(width > 0 && height > 0 && channels > 0, "[Sprite.Create Error] Width, height and channels should be > 0")
			if (pixelData != nullptr) delete[] pixelData;

		this->width = width;
		this->height = height;
		this->channels = channels;

		pixelData = new uint8_t[width * height * channels];
		memset(pixelData, 0, width * height * channels * sizeof(uint8_t));
	}

	void Sprite::Load(const std::string& fileName)
	{
		channels = 4;
		pixelData = stbi_load(fileName.c_str(), &width, &height, nullptr, channels);
		DGE_ASSERT(pixelData, "[stb_image Error] " << stbi_failure_reason())
	}

	void Sprite::Save(const std::string& fileName, const FileType type) const
	{
		int err;

		switch (type)
		{
		case FileType::BMP: err = stbi_write_bmp(fileName.c_str(), width, height, channels, pixelData); break;
		case FileType::PNG: err = stbi_write_png(fileName.c_str(), width, height, channels, pixelData, width * channels); break;
		case FileType::JPG: err = stbi_write_jpg(fileName.c_str(), width, height, channels, pixelData, 100); break;
		case FileType::TGA: err = stbi_write_tga(fileName.c_str(), width, height, channels, pixelData); break;
		case FileType::TGA_RLE:
		{
			stbi_write_tga_with_rle = 1;
			err = stbi_write_tga(fileName.c_str(), width, height, channels, pixelData);
			stbi_write_tga_with_rle = 0;
		}
		break;

		}

		DGE_ASSERT(err == 1, "[stb_image_write Error] Code: " << err)
	}

	bool Sprite::SetPixel(const def::vi2d& pos, const Pixel& p)
	{
		if (pos >= def::vi2d(0, 0) && pos < def::vi2d(width, height))
		{
			size_t i = channels * (pos.y * width + pos.x);

			pixelData[i] = p.r;
			pixelData[i + 1] = p.g;
			pixelData[i + 2] = p.b;
			pixelData[i + 3] = p.a;

			return true;
		}

		return false;
	}

	Pixel Sprite::GetPixel(const def::vi2d& pos, const WrapMethod wrap) const
	{
		auto get_pixel = [&](const def::vi2d& xy)
		{
			size_t i = channels * (xy.y * width + xy.x);
			return Pixel(
				pixelData[i],
				pixelData[i + 1],
				pixelData[i + 2],
				pixelData[i + 3]
			);
		};

		def::vi2d size = { width, height };

		switch (wrap)
		{
		case WrapMethod::NONE:
		{
			if (pos >= def::vi2d(0, 0) && pos < size)
				return get_pixel(pos);
		}
		break;

		case WrapMethod::REPEAT:
			return get_pixel((pos % size).abs());

		case WrapMethod::MIRROR:
		{
			def::vi2d q = pos / size;
			def::vi2d m = (pos % size).abs();

			if (q.x % 2 == 0) m.x = width - m.x - 1;
			if (q.y % 2 == 0) m.y = height - m.y - 1;

			return get_pixel(m);
		}

		case WrapMethod::CLAMP:
			return get_pixel(pos.clamp({ 0, 0 }, size));

		}

		return def::BLACK;
	}

	void Sprite::SetPixelData(const uint8_t* data)
	{
		memcpy(pixelData, data, width * height * channels * sizeof(uint8_t));
	}

	void Sprite::SetPixelData(const def::Pixel& col)
	{
		for (int i = 0; i < width; i++)
			for (int j = 0; j < height; j++)
			{
				size_t idx = channels * (j * width + i);
				pixelData[idx] = col.r;
				pixelData[idx + 1] = col.g;
				pixelData[idx + 2] = col.b;
				pixelData[idx + 3] = col.a;
			}
	}

	Pixel Sprite::Sample(const def::vf2d& uv, const SampleMethod sample, const WrapMethod wrap) const
	{
		def::vf2d denorm = uv * def::vf2d(width, height);

		switch (sample)
		{
		case SampleMethod::LINEAR:
			return GetPixel(denorm.floor(), wrap);

		case SampleMethod::BILINEAR:
		{
			def::vf2d offset = denorm - denorm.floor();

			def::Pixel tl = GetPixel(offset + def::vf2d(0, 0), wrap);
			def::Pixel tr = GetPixel(offset + def::vf2d(1, 0), wrap);
			def::Pixel bl = GetPixel(offset + def::vf2d(0, 1), wrap);
			def::Pixel br = GetPixel(offset + def::vf2d(1, 1), wrap);

			def::Pixel topCol = tr * offset.x + tl * (1.0f - offset.x);
			def::Pixel bottomCol = br * offset.x + bl * (1.0f - offset.x);

			return bottomCol * offset.y + topCol * (1.0f - offset.y);
		}

		case SampleMethod::TRILINEAR:
		{
			def::vi2d center = (denorm - def::vf2d(0.5f, 0.5f)).floor();
			def::vf2d offset = (denorm - def::vf2d(0.5f, 0.5f)) - def::vf2d(center);

			struct Pixelf
			{
				float r = 0.0f;
				float g = 0.0f;
				float b = 0.0f;
				float a = 0.0f;
			};

			Pixelf splineX[4][4];

			def::vi2d s;
			for (s.y = 0; s.y < 4; s.y++)
				for (s.x = 0; s.x < 4; s.x++)
				{
					def::Pixel p = GetPixel(center + s - def::vi2d(1, 1), wrap);
					splineX[s.y][s.x] = { (float)p.r, (float)p.g, (float)p.b, (float)p.a };
				}

			def::vf2d t = offset;
			def::vf2d tt = t * t;
			def::vf2d ttt = tt * tt;

			def::vf2d q[4];
			q[0] = 0.5f * (-1.0f * ttt + 2.0f * tt - t);
			q[1] = 0.5f * (3.0f * ttt - 5.0f * tt + def::vf2d(2.0f, 2.0f));
			q[2] = 0.5f * (-3.0f * ttt + 4.0f * tt + t);
			q[3] = 0.5f * (ttt - tt);

			Pixelf splineY[4];
			
			for (int s = 0; s < 4; s++)
			{
				for (int i = 0; i < 4; i++)
				{
					splineY[s].r += splineX[s][i].r * q[i].x;
					splineY[s].g += splineX[s][i].g * q[i].x;
					splineY[s].b += splineX[s][i].b * q[i].x;
					splineY[s].a += splineX[s][i].a * q[i].x;
				}
			}

			Pixelf pix;
			for (int i = 0; i < 4; i++)
			{
				pix.r += splineY[i].r * q[i].y;
				pix.g += splineY[i].g * q[i].y;
				pix.b += splineY[i].b * q[i].y;
				pix.a += splineY[i].a * q[i].y;
			}

			return def::Pixel(
				(uint8_t)std::clamp(pix.r, 0.0f, 255.0f),
				(uint8_t)std::clamp(pix.g, 0.0f, 255.0f),
				(uint8_t)std::clamp(pix.b, 0.0f, 255.0f),
				(uint8_t)std::clamp(pix.a, 0.0f, 255.0f)
			);
		}
		
		}
	}

	Texture::Texture(Sprite* sprite) { Construct(sprite, false); }
	Texture::Texture(const std::string& fileName) { Construct(new Sprite(fileName), true); }

	void Texture::Construct(Sprite* sprite, bool deleteSprite)
	{
		Load(sprite);

		UVScaleX = 1.0f / (float)sprite->width;
		UVScaleY = 1.0f / (float)sprite->height;

		width = sprite->width;
		height = sprite->height;

		if (deleteSprite) delete sprite;
	}

	void Texture::Load(Sprite* sprite)
	{
		GLenum format = 0;

		switch (sprite->channels)
		{
		case 3: format = GL_RGB; break;
		case 4: format = GL_RGBA; break;
		}

		DGE_ASSERT(format != 0, "[Texture.Load Error] Text: Invalid number of channels: " << sprite->channels)

		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(
			GL_TEXTURE_2D,
			0, format,
			sprite->width,
			sprite->height,
			0, format,
			GL_UNSIGNED_BYTE,
			sprite->pixelData
		);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::Update(Sprite* sprite)
	{
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sprite->width, sprite->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, sprite->pixelData);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	Graphic::Graphic(const std::string& fileName) { Load(fileName); }
	Graphic::Graphic(const int32_t width, const int32_t height) { Load(width, height); }

	Graphic::~Graphic()
	{
		if (texture != nullptr) delete texture;
		if (sprite != nullptr) delete sprite;
	}

	void Graphic::Load(const std::string& fileName)
	{
		sprite = new Sprite(fileName);
		texture = new Texture(sprite);
	}

	void Graphic::Load(const int32_t width, const int32_t height)
	{
		sprite = new Sprite(width, height);
		texture = new Texture(sprite);
	}

	void Graphic::Save(const std::string& fileName, const Sprite::FileType type) const { return sprite->Save(fileName, type); }
	void Graphic::UpdateTexture() { texture->Update(sprite); }

	GameEngine::GameEngine()
	{
		m_AppName = "Undefined";
		m_MousePos = { -1, -1 };

		m_Window = nullptr;
		m_Monitor = nullptr;

		m_Font = nullptr;
		m_DrawTarget = nullptr;
		m_Tint = { 255, 255, 255, 255 };

		m_PixelMode = Pixel::DEFAULT;
		m_TextureStructure = Texture::FAN;

		m_TickTimer = 0.0f;
	}

	GameEngine::~GameEngine() { Destroy(); }

	void GameEngine::Destroy()
	{
		SetDrawTarget(m_Screen);

		if (m_DrawTarget != nullptr) delete m_DrawTarget;
		if (m_Font != nullptr) delete m_Font;

		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	void GameEngine::AppThread()
	{
		if (!OnUserCreate())
			m_IsAppRunning = false;

		auto tp1 = std::chrono::system_clock::now();
		auto tp2 = tp1;

		for (int i = 0; i < 512; i++)
		{
			m_Keys[i] = { false, false, false };
			m_KeyOldState[i] = false;
			m_KeyNewState[i] = false;
		}

		for (int i = 0; i < 5; i++)
		{
			m_Mouse[i] = { false, false, false };
			m_MouseOldState[i] = false;
			m_MouseNewState[i] = false;
		}

		std::string title = "github.com/defini7 - defGameEngine - " + m_AppName + " - FPS: 0";
		glfwSetWindowTitle(m_Window, title.c_str());

		while (m_IsAppRunning)
		{
			tp2 = std::chrono::system_clock::now();
			float deltaTime = std::chrono::duration<float>(tp2 - tp1).count();
			tp1 = tp2;

			m_IsAppRunning = !glfwWindowShouldClose(m_Window);

			for (int i = 0; i < 512; i++)
			{
				m_KeyNewState[i] = glfwGetKey(m_Window, i);

				m_Keys[i].pressed = false;
				m_Keys[i].released = false;

				if (m_KeyNewState[i] != m_KeyOldState[i])
				{
					if (m_KeyNewState[i])
					{
						m_Keys[i].pressed = !m_Keys[i].held;
						m_Keys[i].held = true;
					}
					else
					{
						m_Keys[i].released = true;
						m_Keys[i].held = false;
					}
				}

				m_KeyOldState[i] = m_KeyNewState[i];
			}

			for (int i = 0; i < 5; i++)
			{
				m_MouseNewState[i] = glfwGetMouseButton(m_Window, i);

				m_Mouse[i].pressed = false;
				m_Mouse[i].released = false;

				if (m_MouseNewState[i] != m_MouseOldState[i])
				{
					if (m_MouseNewState[i])
					{
						m_Mouse[i].pressed = true;
						m_Mouse[i].held = true;
					}
					else
					{
						m_Mouse[i].released = true;
						m_Mouse[i].held = false;
					}
				}

				m_MouseOldState[i] = m_MouseNewState[i];
			}

			double mouseX, mouseY;
			glfwGetCursorPos(m_Window, &mouseX, &mouseY);

			m_MousePos = vi2d(mouseX, mouseY) / m_PixelSize;

			if (!OnUserUpdate(deltaTime))
				m_IsAppRunning = false;

			ClearBuffer(BLACK);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			m_DrawTarget->UpdateTexture();
			glBindTexture(GL_TEXTURE_2D, m_DrawTarget->texture->id);

			DrawQuad(m_Tint);

			for (const auto& ti : m_Textures) DrawTexture(ti);
			m_Textures.clear();

			if (m_IsVSync)
				glfwSwapBuffers(m_Window);
			else
				glFlush();

			glfwPollEvents();

			m_TickTimer += deltaTime;
			if (m_TickTimer >= 1.0f)
			{
				title = "github.com/defini7 - defGameEngine - " + m_AppName + " - FPS: " + std::to_string(int(1.0f / deltaTime));
				glfwSetWindowTitle(m_Window, title.c_str());
				m_TickTimer = 0.0f;
			}
		}
	}

	void GameEngine::DrawTexture(const TextureInstance& ti)
	{
		if (ti.tex == nullptr) glBindTexture(GL_TEXTURE_2D, 0);
		else				   glBindTexture(GL_TEXTURE_2D, ti.tex->id);

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

	void GameEngine::DrawQuad(const Pixel& tint)
	{
		glBegin(GL_QUADS);
		glColor4ub(tint.r, tint.g, tint.b, tint.a);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, 1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, 1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, -1.0f);
		glEnd();
	}

	void GameEngine::Run()
	{
		m_IsAppRunning = true;
		AppThread();
	}

	void GameEngine::ErrorCallback(int errCode, const char* desc)
	{
		if (errCode != GLFW_INVALID_ENUM)
		{
			std::cout << "[GLFW Error] Code: "
				<< "0x000" << std::hex << errCode
				<< ", text: " << desc << std::endl;

			exit(1);
		}
	}

	void GameEngine::DropCallback(GLFWwindow* window, int pathCount, const char* paths[])
	{
		s_DropCache.clear();
		s_DropCache.reserve(pathCount);

		for (int i = 0; i < pathCount; i++)
			s_DropCache[i] = paths[i];
	}

	void GameEngine::Construct(int32_t screenWidth, int32_t screenHeight, int32_t pixelWidth, int32_t pixelHeight, bool isFullScreen, bool isVSync, bool isDirtyPixel)
	{
		glfwSetErrorCallback(ErrorCallback);

		glfwInit();

		m_ScreenSize = { screenWidth, screenHeight };
		m_InvScreenSize = 1.0f / vf2d(m_ScreenSize);
		m_PixelSize = { pixelWidth, pixelHeight };
		m_WindowSize = m_ScreenSize * m_PixelSize;

		m_IsFullScreen = isFullScreen;
		m_IsVSync = isVSync;

		m_IsDirtyPixel = isDirtyPixel;

		m_Monitor = glfwGetPrimaryMonitor();
		if (!m_Monitor) return;

		const GLFWvidmode* videoMode = glfwGetVideoMode(m_Monitor);
		if (!videoMode) return;

		m_MaxWindowSize = { videoMode->width, videoMode->height };

		if (!m_IsVSync)
			glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE);

		if (m_IsFullScreen)
		{
			m_WindowSize = vi2d(videoMode->width, videoMode->height);
			m_ScreenSize = m_WindowSize / m_PixelSize;

			m_Window = glfwCreateWindow(m_WindowSize.x, m_WindowSize.y, "", m_Monitor, NULL);
			if (!m_Window) return;

			glfwSetWindowMonitor(m_Window, m_Monitor,
				0, 0, m_WindowSize.x, m_WindowSize.y, videoMode->refreshRate);
		}
		else
		{
			m_Window = glfwCreateWindow(m_WindowSize.x, m_WindowSize.y, "", NULL, NULL);
			if (!m_Window) return;
		}

		glfwMakeContextCurrent(m_Window);
		glViewport(0, 0, m_WindowSize.x, m_WindowSize.y);

		glEnable(GL_TEXTURE_2D);
		if (!m_IsDirtyPixel) glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		if (m_IsVSync)
		{
			glfwSwapInterval(1);
			glfwWindowHint(GLFW_REFRESH_RATE, videoMode->refreshRate);
		}

		m_Screen = new Graphic(m_ScreenSize.x, m_ScreenSize.y);
		m_DrawTarget = m_Screen;
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

		m_Font = new Sprite(128, 48);
		def::vi2d p;

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
				m_Font->SetPixel({ p.x, p.y }, { k, k, k, k });
				if (++p.y == 48) { p.x++; p.y = 0; }
			}
		}

		glfwSetDropCallback(m_Window, DropCallback);
	}

	bool GameEngine::Draw(int32_t x, int32_t y, Pixel p)
	{
		if (!m_DrawTarget) return false;
		Sprite* target = m_DrawTarget->sprite;

		switch (m_PixelMode)
		{
		case Pixel::CUSTOM: return target->SetPixel({ x, y }, m_Shader({ x, y }, target->GetPixel({ x, y }), p));
		case Pixel::DEFAULT: return target->SetPixel({ x, y }, p);
		case Pixel::MASK:
		{
			if (p.a == 255)
				return target->SetPixel({ x, y }, p);
		}
		break;

		case Pixel::ALPHA:
		{
			Pixel d = target->GetPixel({ x, y });
			float a = (float)(p.a / 255.0f);
			float c = 1.0f - a;
			float r = a * (float)p.r + c * (float)d.r;
			float g = a * (float)p.g + c * (float)d.g;
			float b = a * (float)p.b + c * (float)d.b;
			return target->SetPixel({ x, y }, { (uint8_t)r, (uint8_t)g, (uint8_t)b });
		}

		}

		return false;
	}

	void GameEngine::DrawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const Pixel& p)
	{
		int32_t dx = x2 - x1;
		int32_t dy = y2 - y1;

		int32_t dx1 = abs(dx);
		int32_t dy1 = abs(dy);

		int32_t px = 2 * dy1 - dx1;
		int32_t py = 2 * dx1 - dy1;

		int32_t x, y, xe, ye;

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

			for (int _i = 0; x < xe; _i++)
			{
				x++;

				if (px < 0)
					px = px + 2 * dy1;
				else
				{
					y += ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) ? 1 : -1;
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

			for (int _i = 0; y < ye; _i++)
			{
				y++;

				if (py <= 0)
					py = py + 2 * dx1;
				else
				{
					x += ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) ? 1 : -1;
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
		auto drawLine = [&](int32_t sx, int32_t ex, int32_t ny)
		{
			for (int i = sx; i <= ex; i++)
				Draw(i, ny, p);
		};

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

		if (dy1 > dx1) { std::swap(dx1, dy1); changed1 = true; }
		if (dy2 > dx2) { std::swap(dy2, dx2); changed2 = true; }

		e2 = (int32_t)(dx2 >> 1);
		if (y1 == y2) goto next;
		e1 = (int32_t)(dx1 >> 1);

		for (int i = 0; i < dx1;)
		{
			t1xp = t2xp = 0;

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

			while (i++ < dx1)
			{
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

			drawLine(minx, maxx, y);
			if (!changed1) t1x += signx1;

			t1x += t1xp;
			if (!changed2) t2x += signx2;

			t2x += t2xp;
			y++;

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
			signx1 = 1;

		dy1 = (int32_t)(y3 - y2);
		t1x = x2;

		if (dy1 > dx1)
		{
			std::swap(dy1, dx1);
			changed1 = true;
		}
		else
			changed1 = false;

		e1 = (int32_t)(dx1 >> 1);

		for (int i = 0; i <= dx1; i++)
		{
			t1xp = t2xp = 0;

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
						goto next3;
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

			drawLine(minx, maxx, y);
			if (!changed1) t1x += signx1;

			t1x += t1xp;
			if (!changed2) t2x += signx2;

			t2x += t2xp;
			y++;

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

		for (int i = 0; i < sy; i++)
		{
			Draw(x, y + i, p);
			Draw(x + sx, y + i, p);
		}

		Draw(x + sx, y + sy, p);
	}

	void GameEngine::FillRectangle(int32_t x, int32_t y, int32_t sx, int32_t sy, const Pixel& p)
	{
		for (int i = 0; i < sx; i++)
			for (int j = 0; j < sy; j++)
				Draw(x + i, y + j, p);
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

		auto drawLine = [&](int32_t sx, int32_t ex, int32_t ny)
		{
			for (int i = sx; i <= ex; i++)
				Draw(i, ny, p);
		};

		while (y1 >= x1)
		{
			drawLine(x - x1, x + x1, y - y1);
			drawLine(x - y1, x + y1, y - x1);
			drawLine(x - x1, x + x1, y + y1);
			drawLine(x - y1, x + y1, y + x1);

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

		auto drawLine = [&](int32_t sx, int32_t ex, int32_t ny)
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
			drawLine(x, x1, y);
			drawLine(x, x1, y1);

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
			drawLine(x - 1, x1 + 1, y++);
			drawLine(x - 1, x1 + 1, y1--);
		}
	}

	void GameEngine::DrawSprite(int32_t x, int32_t y, Sprite* sprite)
	{
		for (int i = 0; i < sprite->width; i++)
			for (int j = 0; j < sprite->height; j++)
				Draw(x + i, y + j, sprite->GetPixel({ i, j }));
	}

	void GameEngine::DrawPartialSprite(int32_t x, int32_t y, int32_t fx, int32_t fy, int32_t fsx, int32_t fsy, Sprite* sprite)
	{
		for (int i = fx, x1 = 0; i < fx + fsx; i++, x1++)
			for (int j = fy, y1 = 0; j < fy + fsy; j++, y1++)
				Draw(x + x1, y + y1, sprite->GetPixel({ i, j }));
	}

	void GameEngine::DrawTexture(float x, float y, Texture* tex, float scaleX, float scaleY, const Pixel& tint)
	{
		vf2d screenPos =
		{
			(x * m_InvScreenSize.x) * 2.0f - 1.0f,
			((y * m_InvScreenSize.y) * 2.0f - 1.0f) * -1.0f
		};

		vf2d screenSize =
		{
			screenPos.x + (2.0f * (float(tex->width) * m_InvScreenSize.x)) * scaleX,
			screenPos.y - (2.0f * (float(tex->height) * m_InvScreenSize.y)) * scaleY
		};

		TextureInstance ti;
		ti.tex = tex;
		ti.points = 4;
		ti.structure = m_TextureStructure;
		ti.tint = { tint, tint, tint, tint };
		ti.vert = { screenPos, { screenPos.x, screenSize.y }, screenSize, { screenSize.x, screenPos.y } };
		ti.uv = { { 0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f} };

		m_Textures.push_back(ti);
	}

	void GameEngine::DrawPartialTexture(float x, float y, float filePosX, float filePosY, float fileSizeX, float fileSizeY, Texture* tex, float scaleX, float scaleY, const Pixel& tint)
	{
		vf2d screenSpacePos =
		{
			  (x * m_InvScreenSize.x) * 2.0f - 1.0f,
			-((y * m_InvScreenSize.y) * 2.0f - 1.0f)
		};

		vf2d screenSpaceSize =
		{
			  ((x + fileSizeX * scaleX) * m_InvScreenSize.x) * 2.0f - 1.0f,
			-(((y + fileSizeY * scaleY) * m_InvScreenSize.y) * 2.0f - 1.0f)
		};

		vf2d quantisedPos = ((screenSpacePos * vf2d(m_WindowSize)) + vf2d(0.5f, 0.5f)).floor() / vf2d(m_WindowSize);
		vf2d quantisedSize = ((screenSpaceSize * vf2d(m_WindowSize)) + vf2d(0.5f, -0.5f)).ceil() / vf2d(m_WindowSize);

		vf2d tl = (vf2d(filePosX, filePosY) + vf2d(0.0001f, 0.0001f)) * vf2d(tex->UVScaleX, tex->UVScaleY);
		vf2d br = (vf2d(filePosX, filePosY) + vf2d(fileSizeX, fileSizeY) - vf2d(0.0001f, 0.0001f)) * vf2d(tex->UVScaleX, tex->UVScaleY);

		TextureInstance ti;
		ti.tex = tex;
		ti.points = 4;
		ti.structure = m_TextureStructure;
		ti.tint = { tint, tint, tint, tint };
		ti.vert = { quantisedPos, { quantisedPos.x, quantisedSize.y }, quantisedSize, { quantisedSize.x, quantisedPos.y } };
		ti.uv = { tl, { tl.x, br.y }, br, { br.x, tl.y } };
		m_Textures.push_back(ti);
	}

	void GameEngine::DrawRotatedTexture(float x, float y, float r, Texture* tex, float centerX, float centerY, float scaleX, float scaleY, const Pixel& tint)
	{
		TextureInstance ti;
		ti.tex = tex;
		ti.points = 4;
		ti.structure = m_TextureStructure;
		ti.tint = { tint, tint, tint, tint };

		vf2d center = vf2d(centerX, centerY);
		vf2d scale = vf2d(scaleX, scaleY);

		ti.vert = {
			(vf2d(0.0f, 0.0f) - center) * scale,
			(vf2d(0.0f, float(tex->height)) - center) * scale,
			(vf2d(float(tex->width), float(tex->height)) - center) * scale,
			(vf2d(float(tex->width), 0.0f) - center) * scale
		};

		float c = cos(r), s = sin(r);
		for (int i = 0; i < ti.points; i++)
		{
			vf2d o = { ti.vert[i].x * c - ti.vert[i].y * s, ti.vert[i].x * s + ti.vert[i].y * c };
			ti.vert[i] = vf2d(x, y) + o;
			ti.vert[i] = ti.vert[i] * m_InvScreenSize * 2.0f - vf2d(1.0f, 1.0f);
			ti.vert[i].y *= -1.0f;
		}

		ti.uv = { { 0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f} };
		m_Textures.push_back(ti);
	}

	void GameEngine::DrawPartialRotatedTexture(float x, float y, float fx, float fy, float fw, float fh, float r, Texture* tex, float centerX, float centerY, float scaleX, float scaleY, const Pixel& tint)
	{
		TextureInstance ti;
		ti.tex = tex;
		ti.points = 4;
		ti.structure = m_TextureStructure;
		ti.tint = { tint, tint, tint, tint };

		vf2d center = vf2d(centerX, centerY);
		vf2d scale = vf2d(scaleX, scaleY);

		vf2d screenSpacePos =
		{
			  (x * m_InvScreenSize.x) * 2.0f - 1.0f,
			-((y * m_InvScreenSize.y) * 2.0f - 1.0f)
		};

		vf2d screenSpaceSize =
		{
			  ((x + fw * scaleX) * m_InvScreenSize.x) * 2.0f - 1.0f,
			-(((y + fh * scaleY) * m_InvScreenSize.y) * 2.0f - 1.0f)
		};

		vf2d quantisedPos = ((screenSpacePos * vf2d(m_WindowSize)) + vf2d(0.5f, 0.5f)).floor() / vf2d(m_WindowSize);
		vf2d quantisedSize = ((screenSpaceSize * vf2d(m_WindowSize)) + vf2d(0.5f, -0.5f)).ceil() / vf2d(m_WindowSize);

		vf2d tl = (vf2d(fx, fy) + vf2d(0.0001f, 0.0001f)) * vf2d(tex->UVScaleX, tex->UVScaleY);
		vf2d br = (vf2d(fx, fy) + vf2d(fx, fy) - vf2d(0.0001f, 0.0001f)) * vf2d(tex->UVScaleX, tex->UVScaleY);

		ti.vert = { quantisedPos, { quantisedPos.x, quantisedSize.y }, quantisedSize, { quantisedSize.x, quantisedPos.y } };

		float c = cos(r), s = sin(r);
		for (int i = 0; i < ti.points; i++)
		{
			vf2d o = { ti.vert[i].x * c - ti.vert[i].y * s, ti.vert[i].x * s + ti.vert[i].y * c };
			ti.vert[i] = vf2d(x, y) + o;
			ti.vert[i] = ti.vert[i] * m_InvScreenSize * 2.0f - vf2d(1.0f, 1.0f);
			ti.vert[i].y *= -1.0f;
		}

		ti.uv = { tl, { tl.x, br.y }, br, { br.x, tl.y } };
		m_Textures.push_back(ti);
	}

	void GameEngine::DrawWarpedTexture(const std::vector<vf2d>& points, Texture* tex, const Pixel& tint)
	{
		TextureInstance di;
		di.tex = tex;
		di.structure = m_TextureStructure;
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
				di.vert[i] = { (points[i].x * m_InvScreenSize.x) * 2.0f - 1.0f, ((points[i].y * m_InvScreenSize.y) * 2.0f - 1.0f) * -1.0f };
			}

			m_Textures.push_back(di);
		}
	}

	void GameEngine::DrawWireFrameModel(const std::vector<vf2d>& modelCoordinates, float x, float y, float r, float s, const Pixel& p)
	{
		int32_t verts = modelCoordinates.size();

		std::vector<vf2d> coordinates;
		coordinates.resize(verts);

		for (int i = 0; i < verts; i++)
		{
			coordinates[i].x = modelCoordinates[i].x * cosf(r) - modelCoordinates[i].y * sinf(r);
			coordinates[i].y = modelCoordinates[i].x * sinf(r) + modelCoordinates[i].y * cosf(r);
		}

		for (int i = 0; i < verts; i++)
		{
			coordinates[i].x = coordinates[i].x * s;
			coordinates[i].y = coordinates[i].y * s;
		}

		for (int i = 0; i < verts; i++)
		{
			coordinates[i].x = coordinates[i].x + x;
			coordinates[i].y = coordinates[i].y + y;
		}

		for (int i = 0; i < verts + 1; i++)
		{
			int32_t j = i + 1;
			DrawLine((int32_t)coordinates[i % verts].x, (int32_t)coordinates[i % verts].y,
				(int32_t)coordinates[j % verts].x, (int32_t)coordinates[j % verts].y, p);
		}
	}

	void GameEngine::FillWireFrameModel(const std::vector<vf2d>& modelCoordinates, float x, float y, float r, float s, const Pixel& p)
	{
		int32_t verts = modelCoordinates.size();

		std::vector<vf2d> coordinates;
		coordinates.resize(verts);

		for (int i = 0; i < verts; i++)
		{
			coordinates[i].x = modelCoordinates[i].x * cosf(r) - modelCoordinates[i].y * sinf(r);
			coordinates[i].y = modelCoordinates[i].x * sinf(r) + modelCoordinates[i].y * cosf(r);
		}

		for (int i = 0; i < verts; i++)
		{
			coordinates[i].x = coordinates[i].x * s;
			coordinates[i].y = coordinates[i].y * s;
		}

		for (int i = 0; i < verts; i++)
		{
			coordinates[i].x = coordinates[i].x + x;
			coordinates[i].y = coordinates[i].y + y;
		}

		auto GetAngle = [](const def::vf2d& p1, const def::vf2d& p2)
		{
			float theta = atan2(p2.y, p2.x) - atan2(p1.y, p1.x);
			while (theta > 3.14159f) theta -= 3.14159f * 2.0f;
			while (theta < -3.14159f) theta += 3.14159f * 2.0f;
			return theta;
		};

		auto PointInPolygon = [&](const def::vf2d& p)
		{
			float angle = 0.0f;

			def::vf2d p1, p2;
			for (int i = 0; i < verts; i++)
			{
				p1 = coordinates[i] - p;
				p2 = coordinates[(i + 1) % verts] - p;
				angle += GetAngle(p1, p2);
			}

			return std::abs(angle) < 3.14159f;
		};

		def::vf2d vMin = coordinates.front();
		def::vf2d vMax = coordinates.front();

		for (int i = 1; i < verts; i++)
		{
			if (vMin.x > coordinates[i].x) vMin.x = coordinates[i].x;
			if (vMin.y > coordinates[i].y) vMin.y = coordinates[i].y;

			if (vMax.x < coordinates[i].x) vMax.x = coordinates[i].x;
			if (vMax.y < coordinates[i].y) vMax.y = coordinates[i].y;
		}

		def::vf2d point;
		for (point.x = vMin.x; point.x < vMax.x; point.x++)
			for (point.y = vMin.y; point.y < vMax.y; point.y++)
			{
				if (PointInPolygon(point))
					Draw(point, p);
			}
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

				for (int32_t i = 0; i < 8; i++)
					for (int32_t j = 0; j < 8; j++)
						if (m_Font->GetPixel({ i + ox * 8, j + oy * 8 }).r > 0)
							Draw(x + sx + i, y + sy + j, p);

				sx += 8;
			}
		}
	}

	void GameEngine::Clear(const Pixel& p)
	{
		for (int32_t x = 0; x < m_DrawTarget->sprite->width; x++)
			for (int32_t y = 0; y < m_DrawTarget->sprite->height; y++)
				m_DrawTarget->sprite->SetPixel({ x, y }, p);
	}

	KeyState GameEngine::GetKey(uint32_t k) const { return m_Keys[k]; }
	KeyState GameEngine::GetMouse(uint32_t k) const { return m_Mouse[k]; }

	int32_t GameEngine::MouseX() const { return m_MousePos.x; }
	int32_t GameEngine::MouseY() const { return m_MousePos.y; }

	int32_t GameEngine::ScreenWidth() const { return m_ScreenSize.x; }
	int32_t GameEngine::ScreenHeight() const { return m_ScreenSize.y; }

	bool GameEngine::IsFullScreen() const { return m_IsFullScreen; }
	bool GameEngine::IsVSync() const { return m_IsVSync; }
	bool GameEngine::IsFocused() const { return (bool)glfwGetWindowAttrib(this->m_Window, GLFW_FOCUSED); }

	void GameEngine::SetIcon(const std::string& fileName)
	{
		Sprite sprIcon(fileName);

		GLFWimage img;
		img.width = sprIcon.width;
		img.height = sprIcon.height;
		img.pixels = sprIcon.pixelData;
		glfwSetWindowIcon(m_Window, 1, &img);
	}

	void GameEngine::SetDrawTarget(Graphic* target)
	{
		if (target == nullptr)
			m_DrawTarget = m_Screen;
		else
		{
			m_DrawTarget = target;
			m_DrawTarget->UpdateTexture();
		}
	}

	Graphic* GameEngine::GetDrawTarget() const { return m_DrawTarget; }
	void GameEngine::SetTitle(const std::string& title) { m_AppName = title; }

	WindowState GameEngine::GetWindowState() const
	{
		int32_t f = static_cast<int32_t>(WindowState::NONE);
		if (glfwGetWindowAttrib(m_Window, GLFW_FOCUSED)) f |= static_cast<int32_t>(WindowState::FOCUSED);
		if (glfwGetWindowAttrib(m_Window, GLFW_MAXIMIZED)) f |= static_cast<int32_t>(WindowState::MAXIMIZED);
		return static_cast<WindowState>(f);
	}

	GLFWwindow* GameEngine::GetWindow() const { return m_Window; }
	std::vector<std::string>& GameEngine::GetDropped() { return s_DropCache; }

	void GameEngine::SetPixelMode(int32_t pixelMode) { m_PixelMode = pixelMode; }
	int32_t GameEngine::GetPixelMode() const { return m_PixelMode; }

	void GameEngine::SetTextureStructure(int32_t textureStructure) { m_TextureStructure = textureStructure; }
	int32_t GameEngine::GetTextureStructure() const { return m_TextureStructure; }

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

	void GameEngine::DrawPartialRotatedTexture(const vf2d& pos, const vf2d& fpos, const vf2d& fsize, float r, Texture* tex, const vf2d& center, const vf2d& scale, const Pixel& tint)
	{
		DrawPartialRotatedTexture(pos.x, pos.y, fpos.x, fpos.y, fsize.x, fsize.y, r, tex, center.x, center.y, scale.x, scale.y, tint);
	}

	void GameEngine::DrawWireFrameModel(const std::vector<vf2d>& modelCoordinates, const vf2d& pos, float r, float s, const Pixel& p)
	{
		DrawWireFrameModel(modelCoordinates, pos.x, pos.y, r, s, p);
	}

	void GameEngine::FillWireFrameModel(const std::vector<vf2d>& modelCoordinates, const vf2d& pos, float r, float s, const Pixel& p)
	{
		FillWireFrameModel(modelCoordinates, pos.x, pos.y, r, s, p);
	}

	void GameEngine::DrawString(const vi2d& pos, const std::string& text, const Pixel& p)
	{
		DrawString(pos.x, pos.y, text, p);
	}

	vi2d GameEngine::ScreenSize() const { return m_ScreenSize; }
	vi2d GameEngine::MaxScreenSize() const { return m_MaxWindowSize; };
	vi2d GameEngine::GetMouse() const { return m_MousePos; }

	void GameEngine::ClearBuffer(const Pixel& p)
	{
		glClearColor(p.r / 255.0f, p.g / 255.0f, p.b / 255.0f, p.a / 255.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void GameEngine::SetTint(const Pixel& p)
	{
		m_Tint = p;
	}

	void GameEngine::SetShader(Pixel(*func)(const vi2d& pos, const Pixel& prev, const Pixel& cur))
	{
		m_Shader = func;
		m_PixelMode = m_Shader ? Pixel::CUSTOM : Pixel::DEFAULT;
	}

#endif

}
