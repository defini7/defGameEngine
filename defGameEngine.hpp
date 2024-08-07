#ifndef DEF_GAME_ENGINE_HPP
#define DEF_GAME_ENGINE_HPP

#pragma region License
/***
*	BSD 3-Clause License
	Copyright (c) 2022 - 2024 Alex
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

#pragma region Sample
/**
* Example:
	#define DGE_APPLICATION
	#include "defGameEngine.hpp"

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
					Draw(i, j, def::Pixel::Random());

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

#pragma region Includes

#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <cmath>
#include <algorithm>
#include <functional>

#include "GLFW/glfw3.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

#include "stb_image.h"

#pragma warning(disable : 4996)

// Oh, dear stb_image...
#define SAFE_STBI_FAILURE_REASON() (stbi_failure_reason() ? stbi_failure_reason() : "")

#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#endif

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

namespace def
{
	template <class... T>
	void Assert(bool expr, T&&... args);

	enum class Key
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

		NONE = -1
	};

	enum class Button
	{
		LEFT,
		RIGHT,
		WHEEL,
		MOUSE4,
		MOUSE5
	};

#ifndef DGE_IGNORE_VEC2D

	template <typename T>
	struct vec2d
	{
		static_assert(std::is_arithmetic<T>::value, "def::vec2d<T> must be numeric");

		constexpr vec2d() = default;
		constexpr vec2d(const T& x, const T& y);

		constexpr vec2d(const vec2d&) = default;
		constexpr vec2d& operator=(const vec2d&) = default;

		T x = 0, y = 0;

		constexpr vec2d clamp(const vec2d& start, const vec2d& end) const;
		constexpr vec2d lerp(const vec2d& v, const double t) const;
		constexpr auto dist(const vec2d& v) const;

		constexpr auto dot(const vec2d& v) const;
		constexpr auto cross(const vec2d& v) const;

		constexpr auto angle(const vec2d& v) const;
		constexpr auto length() const;

		constexpr auto mag() const;
		constexpr auto mag2() const;

		constexpr auto man(const vec2d& v) const;

		constexpr vec2d max(const vec2d& v) const;
		constexpr vec2d min(const vec2d& v) const;

		constexpr void swap(vec2d& v);

		constexpr vec2d norm() const;
		constexpr vec2d abs() const;
		constexpr vec2d perp() const;
		constexpr vec2d floor() const;
		constexpr vec2d ceil() const;
		constexpr vec2d round() const;
		constexpr vec2d cart() const;
		constexpr vec2d polar() const;

		constexpr operator std::string() const;
		constexpr std::string str() const;

		template <class F>
		constexpr operator vec2d<F>() const
		{
			return { static_cast<F>(this->x), static_cast<F>(this->y) };
		}
	};

	typedef vec2d<int> vi2d;
	typedef vec2d<float> vf2d;
	typedef vec2d<double> vd2d;

#endif

	struct KeyState
	{
		constexpr KeyState();
		constexpr KeyState(bool held, bool released, bool pressed);

		bool held;
		bool released;
		bool pressed;
	};

	struct Pixel
	{
		constexpr Pixel();
		constexpr Pixel(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);

		enum class Mode
		{
			DEFAULT,
			ALPHA,
			MASK,
			CUSTOM
		};

		union
		{
			struct { uint8_t r, g, b, a; };
			uint8_t rgba[4];
		};

		constexpr Pixel mix(const Pixel& rhs, const float factor) const;
		constexpr std::string str() const;

		constexpr Pixel operator+(const float rhs) const;
		constexpr Pixel operator-(const float rhs) const;
		constexpr Pixel operator*(const float rhs) const;
		constexpr Pixel operator/(const float rhs) const;

		constexpr Pixel& operator+=(const float rhs);
		constexpr Pixel& operator-=(const float rhs);
		constexpr Pixel& operator*=(const float rhs);
		constexpr Pixel& operator/=(const float rhs);

		constexpr Pixel operator+(const Pixel& rhs) const;
		constexpr Pixel operator-(const Pixel& rhs) const;
		constexpr Pixel operator*(const Pixel& rhs) const;
		constexpr Pixel operator/(const Pixel& rhs) const;

		constexpr Pixel& operator+=(const Pixel& rhs);
		constexpr Pixel& operator-=(const Pixel& rhs);
		constexpr Pixel& operator*=(const Pixel& rhs);
		constexpr Pixel& operator/=(const Pixel& rhs);

		constexpr bool operator==(const Pixel& rhs) const;
		constexpr bool operator!=(const Pixel& rhs) const;
		constexpr bool operator>(const Pixel& rhs) const;
		constexpr bool operator<(const Pixel& rhs) const;
		constexpr bool operator>=(const Pixel& rhs) const;
		constexpr bool operator<=(const Pixel& rhs) const;

		constexpr bool operator==(const float rhs) const;
		constexpr bool operator!=(const float rhs) const;
		constexpr bool operator>(const float rhs) const;
		constexpr bool operator<(const float rhs) const;
		constexpr bool operator>=(const float rhs) const;
		constexpr bool operator<=(const float rhs) const;

		constexpr static Pixel Random(bool randomAlpha = false);
		constexpr static Pixel Float(float r, float g, float b, float a = 1.0f);
	};

	static const Pixel
		BLACK(0, 0, 0), DARK_BLUE(0, 55, 218), DARK_GREEN(19, 161, 14), DARK_CYAN(59, 120, 255),
		DARK_RED(197, 15, 31), DARK_MAGENTA(136, 32, 152), DARK_GREY(118, 118, 118),
		DARK_ORANGE(255, 140, 0), DARK_BROWN(76, 63, 47), DARK_PURPLE(112, 31, 126), ORANGE(255, 165, 0),
		GREY(200, 200, 200), BLUE(0, 0, 255), GREEN(0, 255, 0), CYAN(0, 255, 255), RED(255, 0, 0),
		MAGENTA(180, 0, 158), YELLOW(255, 255, 0), WHITE(255, 255, 255), GOLD(255, 203, 0),
		PINK(255, 109, 194), MAROON(190, 33, 55), LIME(0, 158, 47), BROWN(127, 106, 79),
		BEIGE(211, 176, 131), VIOLET(135, 60, 190), PURPLE(200, 122, 255), NONE(0, 0, 0, 0);

	class Sprite
	{
	public:
		enum class FileType { BMP, PNG, JPG, TGA, TGA_RLE };

		enum class SampleMethod { LINEAR, BILINEAR, TRILINEAR };
		enum class WrapMethod { NONE, REPEAT, MIRROR, CLAMP };

		Sprite() = default;
		Sprite(const vi2d& size);
		Sprite(std::string_view fileName);
		~Sprite();

	public:
		vi2d size;
		std::vector<Pixel> pixels;

	public:
		void Create(const vi2d& size);

		void Load(std::string_view fileName);
		void Save(std::string_view fileName, const FileType type) const;

		bool SetPixel(int x, int y, const Pixel& p);
		bool SetPixel(const vi2d& pos, const Pixel& p);
		Pixel GetPixel(int x, int y, const WrapMethod wrap = WrapMethod::NONE) const;
		Pixel GetPixel(const vi2d& pos, const WrapMethod wrap = WrapMethod::NONE) const;

		void SetPixelData(const Pixel& col);

		Pixel Sample(const vf2d& uv, const SampleMethod sampleMethod, const WrapMethod wrapMethod) const;
	};

	struct Texture
	{
		enum class Structure
		{
			DEFAULT,
			FAN,
			STRIP
		};

		Texture(Sprite* sprite);
		Texture(std::string_view fileName);

		uint32_t id;

		vf2d uvScale;
		vi2d size;

		void Load(Sprite* sprite);
		void Update(Sprite* sprite);

	private:
		void Construct(Sprite* sprite, bool deleteSprite);
	};

	struct Graphic
	{
		Graphic() = default;
		Graphic(std::string_view fileName);
		Graphic(const vi2d& size);

		~Graphic();

		Texture* texture = nullptr;
		Sprite* sprite = nullptr;

		void Load(std::string_view fileName);
		void Load(const vi2d& size);
		void Save(std::string_view fileName, const Sprite::FileType type) const;

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
		const Texture* texture = nullptr;

		Texture::Structure structure = Texture::Structure::FAN;
		int points = 0;

		std::vector<Pixel> tint;
		std::vector<vf2d> vertices;
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
		vf2d m_Offset;
		vf2d m_Scale;

		Sprite* m_Font;

		Graphic* m_Screen;
		Graphic* m_DrawTarget;

		std::vector<TextureInstance> m_Textures;

		Pixel m_Tint;

		Pixel::Mode m_PixelMode;
		Texture::Structure m_TextureStructure;

		std::vector<std::string> m_DropCache;

		float m_TickTimer;

		Pixel (*m_Shader)(const vi2d&, const Pixel&, const Pixel&);

	public:
		inline static GameEngine* s_Engine;

		virtual bool OnUserCreate() = 0;
		virtual bool OnUserUpdate(float deltaTime) = 0;
		virtual bool OnAfterDraw();

		bool Construct(int screenWidth, int screenHeight, int pixelWidth, int pixelHeight, bool fullScreen = false, bool vsync = false, bool dirtyPixel = false);
		void Run();

	private:
		void Destroy();
		void AppThread();

		static void DrawQuad(const Pixel& tint);
		static void ErrorCallback(int errorCode, const char* description);
		static void DropCallback(GLFWwindow* window, int pathCount, const char* paths[]);

	public:
		bool Draw(const vi2d& pos, Pixel p = WHITE);
		virtual bool Draw(int x, int y, Pixel p = WHITE);

		void DrawHorizontalLine(int startX, int endX, int y, const Pixel& p = WHITE);

		void DrawLine(const vi2d& pos1, const vi2d& pos2, const Pixel& p = WHITE);
		virtual void DrawLine(int x1, int y1, int x2, int y2, const Pixel& p = WHITE);

		void DrawTriangle(const vi2d& pos1, const vi2d& pos2, const vi2d& pos3, const Pixel& p = WHITE);
		virtual void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const Pixel& p = WHITE);

		void FillTriangle(const vi2d& pos1, const vi2d& pos2, const vi2d& pos3, const Pixel& p = WHITE);
		virtual void FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const Pixel& p = WHITE);

		void DrawRectangle(const vi2d& pos, const vi2d& size, const Pixel& p = WHITE);
		virtual void DrawRectangle(int x, int y, int sizeX, int sizeY, const Pixel& p = WHITE);

		void FillRectangle(const vi2d& pos, const vi2d& size, const Pixel& p = WHITE);
		virtual void FillRectangle(int x, int y, int sizeX, int sizeY, const Pixel& p = WHITE);

		void DrawCircle(const vi2d& pos, int radius, const Pixel& p = WHITE);
		virtual void DrawCircle(int x, int y, int radius, const Pixel& p = WHITE);

		void FillCircle(const vi2d& pos, int radius, const Pixel& p = WHITE);
		virtual void FillCircle(int x, int y, int radius, const Pixel& p = WHITE);

		void DrawEllipse(const vi2d& pos, const vi2d& size, const Pixel& p = WHITE);
		virtual void DrawEllipse(int x, int y, int sizeX, int sizeY, const Pixel& p = WHITE);

		void FillEllipse(const vi2d& pos, const vi2d& size, const Pixel& p = WHITE);
		virtual void FillEllipse(int x, int y, int sizeX, int sizeY, const Pixel& p = WHITE);

		void DrawSprite(const vi2d& pos, const Sprite* sprite);
		virtual void DrawSprite(int x, int y, const Sprite* sprite);

		void DrawPartialSprite(const vi2d& pos, const vi2d& filePos, const vi2d& fileSize, const Sprite* sprite);
		virtual void DrawPartialSprite(int x, int y, int fileX, int fileY, int fileSizeX, int fileSizeY, const Sprite* sprite);

		void DrawTexture(const vf2d& pos, const Texture* tex, const vf2d& scale = { 1.0f, 1.0f }, const Pixel& tint = WHITE);
		virtual void DrawTexture(float x, float y, const Texture* tex, float scaleX = 1.0f, float scaleY = 1.0f, const Pixel& tint = WHITE);

		void DrawPartialTexture(const vf2d& pos, const vf2d& filePos, const vf2d& fileSize, const Texture* tex, const vf2d& scale = { 1.0f, 1.0f }, const Pixel& tint = WHITE);
		virtual void DrawPartialTexture(float x, float y, float filePosX, float filePosY, float fileSizeX, float fileSizeY, const Texture* tex, float scaleX = 1.0f, float scaleY = 1.0f, const Pixel& tint = WHITE);

		virtual void DrawWarpedTexture(const std::vector<vf2d>& points, const Texture* tex, const Pixel& tint = WHITE);

		void DrawRotatedTexture(const vf2d& pos, float r, const Texture* tex, const vf2d& center = { 0.0f, 0.0f }, const vf2d& scale = { 1.0f, 1.0f }, const Pixel& tint = WHITE);
		virtual void DrawRotatedTexture(float x, float y, float r, const Texture* tex, float centerX = 0.0f, float centerY = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f, const Pixel& tint = WHITE);

		void DrawPartialRotatedTexture(const vf2d& pos, const vf2d& filePos, const vf2d& fileSize, float r, const Texture* tex, const vf2d& center = { 0.0f, 0.0f }, const vf2d& scale = { 1.0f, 1.0f }, const Pixel& tint = WHITE);
		virtual void DrawPartialRotatedTexture(float x, float y, float filePosX, float filePosY, float fileWidth, float fileHeight, float r, const Texture* tex, float centerX = 0.0f, float centerY = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f, const Pixel& tint = WHITE);

		void DrawWireFrameModel(const std::vector<vf2d>& modelCoordinates, const vf2d& pos, float r = 0.0f, float s = 1.0f, const Pixel& p = WHITE);
		virtual void DrawWireFrameModel(const std::vector<vf2d>& modelCoordinates, float x, float y, float r = 0.0f, float s = 1.0f, const Pixel& p = WHITE);

		void FillWireFrameModel(const std::vector<vf2d>& modelCoordinates, const vf2d& pos, float r = 0.0f, float s = 1.0f, const Pixel& p = WHITE);
		virtual void FillWireFrameModel(const std::vector<vf2d>& modelCoordinates, float x, float y, float r = 0.0f, float s = 1.0f, const Pixel& p = WHITE);

		void DrawString(const vi2d& pos, std::string_view text, const Pixel& p = WHITE);
		virtual void DrawString(int x, int y, std::string_view text, const Pixel& p = WHITE);

		virtual void Clear(const Pixel& p);

		KeyState GetKey(Key key) const;
		KeyState GetMouse(Button button) const;

		vi2d GetMouse() const;

		int MouseX() const;
		int MouseY() const;

		void SetTitle(std::string_view title);

		vi2d ScreenSize() const;
		vi2d MaxWindowSize() const;

		int ScreenWidth() const;
		int ScreenHeight() const;

		bool IsFullScreen() const;
		bool IsVSync() const;
		bool IsFocused() const;

		void SetIcon(std::string_view fileName);

		void SetDrawTarget(Graphic* target);
		Graphic* GetDrawTarget();

		WindowState GetWindowState() const;
		GLFWwindow* GetWindow() const;

		std::vector<std::string>& GetDropped();

		void DrawTexture(const TextureInstance& texture);

		void SetPixelMode(Pixel::Mode pixelMode);
		Pixel::Mode GetPixelMode() const;

		void SetTextureStructure(Texture::Structure textureStructure);
		Texture::Structure GetTextureStructure() const;

		void ClearBuffer(const Pixel& p);
		void SetTint(const Pixel& tint);

		void SetShader(Pixel(*func)(const vi2d& pos, const Pixel& previous, const Pixel& current));

		Key AnyKey(bool pressed = true, bool held = false, bool released = false);
		std::vector<Key> AnyKeys(bool pressed = true, bool held = false, bool released = false);
	};

#ifdef DGE_APPLICATION
#undef DGE_APPLICATION

	template <class... T>
	void Assert(bool expr, T&&... args)
	{
		if (!expr)
		{
			std::list<const char*> values;
			(values.emplace_back(std::move(args)), ...);

			for (const auto& val : values)
				std::cout << val << std::endl;

			std::cerr << std::endl;

			exit(1);
		}
	}

#ifndef DGE_IGNORE_VEC2D

	template <class T>
	constexpr vec2d<T>::vec2d(const T& x, const T& y)
	{
		this->x = x;
		this->y = y;
	}

	template <class T1, class T2>
	constexpr vec2d<T1>& operator+=(vec2d<T1>& v1, const vec2d<T2>& v2)
	{
		v1.x += v2.x;
		v1.y += v2.y;
		return v1;
	}

	template <class T1, class T2>
	constexpr vec2d<T1>& operator-=(vec2d<T1>& v1, const vec2d<T2>& v2)
	{
		v1.x -= v2.x;
		v1.y -= v2.y;
		return v1;
	}

	template <class T1, class T2>
	constexpr vec2d<T1>& operator*=(vec2d<T1>& v1, const vec2d<T2>& v2)
	{
		v1.x *= v2.x;
		v1.y *= v2.y;
		return v1;
	}

	template <class T1, class T2>
	constexpr vec2d<T1>& operator/=(vec2d<T1>& v1, const vec2d<T2>& v2)
	{
		v1.x /= v2.x;
		v1.y /= v2.y;
		return v1;
	}

	template <class T1, class T2>
	constexpr vec2d<T1>& operator%=(vec2d<T1>& v1, const vec2d<T2>& v2)
	{
		v1.x %= v2.x;
		v1.y %= v2.y;
		return v1;
	}

	template <class T1, class T2>
	constexpr vec2d<T1>& operator+=(vec2d<T1>& v1, const T2& v2)
	{
		v1.x += v2;
		v1.y += v2;
		return v1;
	}

	template <class T1, class T2>
	constexpr vec2d<T1>& operator-=(vec2d<T1>& v1, const T2& v2)
	{
		v1.x -= v2;
		v1.y -= v2;
		return v1;
	}

	template <class T1, class T2>
	constexpr vec2d<T1>& operator*=(vec2d<T1>& v1, const T2& v2)
	{
		v1.x *= v2;
		v1.y *= v2;
		return v1;
	}

	template <class T1, class T2>
	constexpr vec2d<T1>& operator/=(vec2d<T1>& v1, const T2& v2)
	{
		v1.x /= v2;
		v1.y /= v2;
		return v1;
	}

	template <class T1, class T2>
	constexpr vec2d<T1>& operator%=(vec2d<T1>& v1, const T2& v2)
	{
		v1.x %= v2;
		v1.y %= v2;
		return v1;
	}

	template <class T1, class T2>
	constexpr auto operator+(const vec2d<T1>& v1, const vec2d<T2>& v2)
	{
		return vec2d(v1.x + v2.x, v1.y + v2.y);
	}

	template <class T1, class T2>
	constexpr auto operator-(const vec2d<T1>& v1, const vec2d<T2>& v2)
	{
		return vec2d(v1.x - v2.x, v1.y - v2.y);
	}

	template <class T1, class T2>
	constexpr auto operator*(const vec2d<T1>& v1, const vec2d<T2>& v2)
	{
		return vec2d(v1.x * v2.x, v1.y * v2.y);
	}

	template <class T1, class T2>
	constexpr auto operator/(const vec2d<T1>& v1, const vec2d<T2>& v2)
	{
		return vec2d(v1.x / v2.x, v1.y / v2.y);
	}

	template <class T1, class T2>
	constexpr auto operator%(const vec2d<T1>& v1, const vec2d<T2>& v2)
	{
		return vec2d(v1.x % v2.x, v1.y % v2.y);
	}

	template <class T1, class T2>
	constexpr auto operator+(const vec2d<T1>& v1, const T2& v2)
	{
		return vec2d(v1.x + v2, v1.y + v2);
	}

	template <class T1, class T2>
	constexpr auto operator-(const vec2d<T1>& v1, const T2& v2)
	{
		return vec2d(v1.x - v2, v1.y - v2);
	}

	template <class T1, class T2>
	constexpr auto operator*(const vec2d<T1>& v1, const T2& v2)
	{
		return vec2d(v1.x * v2, v1.y * v2);
	}

	template <class T1, class T2>
	constexpr auto operator/(const vec2d<T1>& v1, const T2& v2)
	{
		return vec2d(v1.x / v2, v1.y / v2);
	}

	template <class T1, class T2>
	constexpr auto operator+(const T1& v1, const vec2d<T2>& v2)
	{
		return vec2d(v1 + v2.x, v1 + v2.y);
	}

	template <class T1, class T2>
	constexpr auto operator-(const T1& v1, const vec2d<T2>& v2)
	{
		return vec2d(v1 - v2.x, v1 - v2.y);
	}

	template <class T1, class T2>
	constexpr auto operator*(const T1& v1, const vec2d<T2>& v2)
	{
		return vec2d(v1 * v2.x, v1 * v2.y);
	}

	template <class T1, class T2>
	constexpr auto operator/(const T1& v1, const vec2d<T2>& v2)
	{
		return vec2d(v1 / v2.x, v1 / v2.y);
	}

	template <class T1, class T2>
	constexpr auto operator%(const T1& v1, const vec2d<T2>& v2)
	{
		return vec2d(v1 % v2.x, v1 % v2.y);
	}

	template <class T>
	constexpr auto operator-(const vec2d<T>& v)
	{
		return vec2d(-v.x, -v.y);
	}

	template <class T1, class T2>
	constexpr bool operator==(const vec2d<T1>& v1, const vec2d<T2>& v2)
	{
		return v1.x == v2.x && v1.y == v2.y;
	}

	template <class T1, class T2>
	constexpr bool operator<=(const vec2d<T1>& v1, const vec2d<T2>& v2)
	{
		return v1.x <= v2.x && v1.y <= v2.y;
	}

	template <class T1, class T2>
	constexpr bool operator>=(const vec2d<T1>& v1, const vec2d<T2>& v2)
	{
		return v1.x >= v2.x && v1.y >= v2.y;
	}

	template <class T1, class T2>
	constexpr bool operator<(const vec2d<T1>& v1, const vec2d<T2>& v2)
	{
		return v1.x < v2.x && v1.y < v2.y;
	}

	template <class T1, class T2>
	constexpr bool operator>(const vec2d<T1>& v1, const vec2d<T2>& v2)
	{
		return v1.x > v2.x && v1.y > v2.y;
	}

	template <class T1, class T2>
	constexpr bool operator!=(const vec2d<T1>& v1, const vec2d<T2>& v2)
	{
		return v1.x != v2.x || v1.y != v2.y;
	}

	template <class T>
	constexpr vec2d<T> vec2d<T>::clamp(const vec2d& start, const vec2d& end) const
	{
		return { std::clamp(x, start.x, end.x), std::clamp(y, start.y, end.y) };
	}

	template <class T>
	constexpr vec2d<T> vec2d<T>::lerp(const vec2d& v, const double t) const
	{
		return { (T)std::lerp(x, v.x, t), (T)std::lerp(y, v.y, t) };
	}

	template <class T>
	constexpr auto vec2d<T>::dist(const vec2d<T>& v) const
	{
		return (*this - v).length();
	}

	template <class T>
	constexpr auto vec2d<T>::dot(const vec2d& v) const
	{
		return x * v.x + y * v.y;
	}

	template <class T>
	constexpr auto vec2d<T>::cross(const vec2d& v) const
	{
		return x * v.y - y * v.x;
	}

	template<typename T>
	constexpr auto vec2d<T>::angle(const vec2d& v) const
	{
		return acos(dot(v) / (length() + v.length()));
	}

	template <class T>
	constexpr auto vec2d<T>::length() const
	{
		return mag();
	}

	template <class T>
	constexpr auto vec2d<T>::mag() const
	{
		return static_cast<T>(std::sqrt(x * x + y * y));
	}

	template <class T>
	constexpr auto vec2d<T>::mag2() const
	{
		return static_cast<T>(x * x + y * y);
	}

	template <class T>
	constexpr auto vec2d<T>::man(const vec2d& v) const
	{
		return std::abs(x - v.x) + std::abs(y - v.y);
	}

	template <class T>
	constexpr vec2d<T> vec2d<T>::max(const vec2d& v) const
	{
		return vec2d(std::max(x, v.x), std::max(y, v.y));
	}

	template <class T>
	constexpr vec2d<T> vec2d<T>::min(const vec2d& v) const
	{
		return vec2d(std::min(x, v.x), std::min(y, v.y));
	}

	template <class T>
	constexpr void vec2d<T>::swap(vec2d& v)
	{
		std::swap(x, v.x);
		std::swap(y, v.y);
	}

	template <class T>
	constexpr vec2d<T> vec2d<T>::norm() const
	{
		auto n = (T)1 / mag();
		return vec2d(x * n, y * n);
	}

	template <class T>
	constexpr vec2d<T> vec2d<T>::abs() const
	{
		return vec2d(std::abs(x), std::abs(y));
	}

	template <class T>
	constexpr vec2d<T> vec2d<T>::perp() const
	{
		return vec2d(-y, x);
	}

	template <class T>
	constexpr vec2d<T> vec2d<T>::floor() const
	{
		return vec2d(std::floor(x), std::floor(y));
	}

	template <class T>
	constexpr vec2d<T> vec2d<T>::ceil() const
	{
		return vec2d(std::ceil(x), std::ceil(y));
	}

	template <class T>
	constexpr vec2d<T> vec2d<T>::round() const
	{
		return vec2d(std::round(x), std::round(y));
	}

	template <class T>
	constexpr vec2d<T> vec2d<T>::cart() const
	{
		return vec2d(cos(y) * x, sin(y) * x);
	}

	template <class T>
	constexpr vec2d<T> vec2d<T>::polar() const
	{
		return vec2d(mag(), atan2(y, x));
	}

	template <class T>
	constexpr vec2d<T>::operator std::string() const
	{
		return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
	}

	template <class T>
	constexpr std::string vec2d<T>::str() const
	{
		return operator std::string();
	}

#endif

	constexpr KeyState::KeyState() : held(false), released(false), pressed(false)
	{

	}

	constexpr KeyState::KeyState(bool held, bool released, bool pressed)
		: held(held), released(released), pressed(pressed)
	{

	}

	constexpr Pixel::Pixel() : r(0), g(0), b(0), a(255)
	{

	}

	constexpr Pixel::Pixel(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a)
	{

	}

	constexpr Pixel Pixel::mix(const Pixel& rhs, const float factor) const
	{
		return Pixel(
			(uint8_t)std::lerp(r, rhs.r, factor),
			(uint8_t)std::lerp(g, rhs.g, factor),
			(uint8_t)std::lerp(b, rhs.b, factor),
			(uint8_t)std::lerp(a, rhs.a, factor)
		);
	}

	constexpr std::string Pixel::str() const
	{
		return "(" + std::to_string(r) + ", " + std::to_string(g) + ", " + std::to_string(b) + ", " + std::to_string(a) + ")";
	}

	constexpr Pixel& Pixel::operator+=(const float rhs)
	{
		r = uint8_t(std::clamp((float)r + rhs, 0.0f, 255.0f));
		g = uint8_t(std::clamp((float)g + rhs, 0.0f, 255.0f));
		b = uint8_t(std::clamp((float)b + rhs, 0.0f, 255.0f));
		return *this;
	}

	constexpr Pixel& Pixel::operator-=(const float rhs)
	{
		r = uint8_t(std::clamp((float)r - rhs, 0.0f, 255.0f));
		g = uint8_t(std::clamp((float)g - rhs, 0.0f, 255.0f));
		b = uint8_t(std::clamp((float)b - rhs, 0.0f, 255.0f));
		return *this;
	}

	constexpr Pixel& Pixel::operator*=(const float rhs)
	{
		r = uint8_t(std::clamp((float)r * rhs, 0.0f, 255.0f));
		g = uint8_t(std::clamp((float)g * rhs, 0.0f, 255.0f));
		b = uint8_t(std::clamp((float)b * rhs, 0.0f, 255.0f));
		return *this;
	}

	constexpr Pixel& Pixel::operator/=(const float rhs)
	{
		r = uint8_t(std::clamp((float)r / rhs, 0.0f, 255.0f));
		g = uint8_t(std::clamp((float)g / rhs, 0.0f, 255.0f));
		b = uint8_t(std::clamp((float)b / rhs, 0.0f, 255.0f));
		return *this;
	}

	constexpr Pixel Pixel::operator+(const float rhs) const
	{
		return Pixel(
			uint8_t(std::clamp((float)r + rhs, 0.0f, 255.0f)),
			uint8_t(std::clamp((float)g + rhs, 0.0f, 255.0f)),
			uint8_t(std::clamp((float)b + rhs, 0.0f, 255.0f))
		);
	}

	constexpr Pixel Pixel::operator-(const float rhs) const
	{
		return Pixel(
			uint8_t(std::clamp((float)r - rhs, 0.0f, 255.0f)),
			uint8_t(std::clamp((float)g - rhs, 0.0f, 255.0f)),
			uint8_t(std::clamp((float)b - rhs, 0.0f, 255.0f))
		);
	}

	constexpr Pixel Pixel::operator*(const float rhs) const
	{
		return Pixel(
			uint8_t(std::clamp((float)r * rhs, 0.0f, 255.0f)),
			uint8_t(std::clamp((float)g * rhs, 0.0f, 255.0f)),
			uint8_t(std::clamp((float)b * rhs, 0.0f, 255.0f))
		);
	}

	constexpr Pixel Pixel::operator/(const float rhs) const
	{
		return Pixel(
			uint8_t(std::clamp((float)r / rhs, 0.0f, 255.0f)),
			uint8_t(std::clamp((float)g / rhs, 0.0f, 255.0f)),
			uint8_t(std::clamp((float)b / rhs, 0.0f, 255.0f))
		);
	}

	constexpr Pixel& Pixel::operator+=(const Pixel& rhs)
	{
		r = uint8_t(std::clamp((int)r + (int)rhs.r, 0, 255));
		g = uint8_t(std::clamp((int)g + (int)rhs.g, 0, 255));
		b = uint8_t(std::clamp((int)b + (int)rhs.b, 0, 255));
		return *this;
	}

	constexpr Pixel& Pixel::operator-=(const Pixel& rhs)
	{
		r = uint8_t(std::clamp((int)r - (int)rhs.r, 0, 255));
		g = uint8_t(std::clamp((int)g - (int)rhs.g, 0, 255));
		b = uint8_t(std::clamp((int)b - (int)rhs.b, 0, 255));
		return *this;
	}

	constexpr Pixel& Pixel::operator*=(const Pixel& rhs)
	{
		r = uint8_t(std::clamp((int)r * (int)rhs.r, 0, 255));
		g = uint8_t(std::clamp((int)g * (int)rhs.g, 0, 255));
		b = uint8_t(std::clamp((int)b * (int)rhs.b, 0, 255));
		return *this;
	}

	constexpr Pixel& Pixel::operator/=(const Pixel& rhs)
	{
		r = uint8_t(std::clamp((int)r / (int)rhs.r, 0, 255));
		g = uint8_t(std::clamp((int)g / (int)rhs.g, 0, 255));
		b = uint8_t(std::clamp((int)b / (int)rhs.b, 0, 255));
		return *this;
	}

	constexpr Pixel Pixel::operator+(const Pixel& rhs) const
	{
		return Pixel(
			uint8_t(std::clamp((int)r + (int)rhs.r, 0, 255)),
			uint8_t(std::clamp((int)g + (int)rhs.g, 0, 255)),
			uint8_t(std::clamp((int)b + (int)rhs.b, 0, 255))
		);
	}

	constexpr Pixel Pixel::operator-(const Pixel& rhs) const
	{
		return Pixel(
			uint8_t(std::clamp((int)r - (int)rhs.r, 0, 255)),
			uint8_t(std::clamp((int)g - (int)rhs.g, 0, 255)),
			uint8_t(std::clamp((int)b - (int)rhs.b, 0, 255))
		);
	}

	constexpr Pixel Pixel::operator*(const Pixel& rhs) const
	{
		return Pixel(
			uint8_t(std::clamp((float)r * (float)rhs.r / 255.0f, 0.0f, 255.0f)),
			uint8_t(std::clamp((float)g * (float)rhs.g / 255.0f, 0.0f, 255.0f)),
			uint8_t(std::clamp((float)b * (float)rhs.b / 255.0f, 0.0f, 255.0f))
		);
	}

	constexpr Pixel Pixel::operator/(const Pixel& rhs) const
	{
		return Pixel(
			uint8_t(std::clamp((float)r / (float)rhs.r, 0.0f, 255.0f)),
			uint8_t(std::clamp((float)g / (float)rhs.g, 0.0f, 255.0f)),
			uint8_t(std::clamp((float)b / (float)rhs.b, 0.0f, 255.0f))
		);
	}

	constexpr bool Pixel::operator==(const Pixel& rhs) const { return r == rhs.r && g == rhs.g && b == rhs.b; }
	constexpr bool Pixel::operator!=(const Pixel& rhs) const { return r != rhs.r || g != rhs.g || b != rhs.b; }
	constexpr bool Pixel::operator>(const Pixel& rhs) const { return r > rhs.r && g > rhs.g && b > rhs.b; }
	constexpr bool Pixel::operator<(const Pixel& rhs) const { return r < rhs.r && g < rhs.g && b < rhs.b; }
	constexpr bool Pixel::operator>=(const Pixel& rhs) const { return r >= rhs.r && g >= rhs.g && b >= rhs.b; }
	constexpr bool Pixel::operator<=(const Pixel& rhs) const { return r <= rhs.r && g <= rhs.g && b <= rhs.b; }

	constexpr bool Pixel::operator==(const float rhs) const { return r == rhs && g == rhs && b == rhs; }
	constexpr bool Pixel::operator!=(const float rhs) const { return r != rhs && g != rhs && b != rhs; }
	constexpr bool Pixel::operator>(const float rhs) const { return r > rhs && g > rhs && b > rhs; }
	constexpr bool Pixel::operator<(const float rhs) const { return r < rhs && g < rhs && b < rhs; }
	constexpr bool Pixel::operator>=(const float rhs) const { return r >= rhs && g >= rhs && b >= rhs; }
	constexpr bool Pixel::operator<=(const float rhs) const { return r <= rhs && g <= rhs && b <= rhs; }

	constexpr Pixel Pixel::Random(bool randomAlpha)
	{
		return Pixel(rand() % 256, rand() % 256, rand() % 256, randomAlpha ? rand() % 256 : 255);
	}

	constexpr Pixel Pixel::Float(float r, float g, float b, float a)
	{
		return Pixel(uint8_t(r * 255.0f), uint8_t(g * 255.0f), uint8_t(b * 255.0f), uint8_t(a * 255.0f));
	}

	Sprite::Sprite(const vi2d& size)
	{
		Create(size);
	}

	Sprite::Sprite(std::string_view fileName)
	{
		Load(fileName);
	}

	Sprite::~Sprite()
	{
	}

	void Sprite::Create(const vi2d& size)
	{
		Assert(size.x > 0 && size.y > 0, "[Sprite.Create Error] Width and height should be > 0");

		pixels.clear();
		this->size = size;

		pixels.resize(size.x * size.y);
		std::fill(pixels.begin(), pixels.end(), BLACK);
	}

	void Sprite::Load(std::string_view fileName)
	{
		uint8_t* data = stbi_load(fileName.data(), &size.x, &size.y, NULL, 4);
		Assert(data, "[stb_image Error] ", SAFE_STBI_FAILURE_REASON());

		pixels.clear();
		pixels.resize(size.x * size.y);

		size_t length = size.x * size.y * 4;
		for (size_t i = 0; i < length; i += 4)
		{
			size_t j = i / 4;
			pixels[j].r = data[i + 0];
			pixels[j].g = data[i + 1];
			pixels[j].b = data[i + 2];
			pixels[j].a = data[i + 3];
		}
	}

	void Sprite::Save(std::string_view fileName, const FileType type) const
	{
		int err = 0;

		switch (type)
		{
		case FileType::BMP: err = stbi_write_bmp(fileName.data(), size.x, size.y, 4, pixels.data()); break;
		case FileType::PNG: err = stbi_write_png(fileName.data(), size.x, size.y, 4, pixels.data(), size.x * 4); break;
		case FileType::JPG: err = stbi_write_jpg(fileName.data(), size.x, size.y, 4, pixels.data(), 100); break;
		case FileType::TGA: err = stbi_write_tga(fileName.data(), size.x, size.y, 4, pixels.data()); break;
		case FileType::TGA_RLE:
		{
			stbi_write_tga_with_rle = 1;
			err = stbi_write_tga(fileName.data(), size.x, size.y, 4, pixels.data());
			stbi_write_tga_with_rle = 0;
		}
		break;

		}

		Assert(err == 1, "[stb_image_write Error] Code: ", std::to_string(err).c_str());
	}

	bool Sprite::SetPixel(int x, int y, const Pixel& p)
	{
		if (x >= 0 && y >= 0 && x < size.x && y < size.y)
		{
			pixels[y * size.x + x] = p;
			return true;
		}

		return false;
	}

	bool Sprite::SetPixel(const vi2d& pos, const Pixel& p)
	{
		return SetPixel(pos.x, pos.y, p);
	}

	Pixel Sprite::GetPixel(int x, int y, const WrapMethod wrap) const
	{
		auto GetPixel = [&pixels = pixels, &size = size](int x, int y)
			{
				return pixels[y * size.x + x];
			};

		switch (wrap)
		{
		case WrapMethod::NONE:
		{
			if (x >= 0 && y >= 0 && x < size.x && y < size.y)
				return GetPixel(x, y);
		}
		break;

		case WrapMethod::REPEAT:
			return GetPixel(abs(x) % size.x, abs(y) % size.y);

		case WrapMethod::MIRROR:
		{
			int mx = (x < 0) ? size.x - 1 - abs(x) % size.x : abs(x) % size.x;
			int my = (y < 0) ? size.y - 1 - abs(y) % size.y : abs(y) % size.y;

			return GetPixel(mx, my);
		}

		case WrapMethod::CLAMP:
		{
			int cx = std::clamp(x, 0, size.x - 1);
			int cy = std::clamp(y, 0, size.y - 1);

			return GetPixel(cx, cy);
		}

		}

		return BLACK;
	}

	Pixel Sprite::GetPixel(const vi2d& pos, const WrapMethod wrap) const
	{
		return GetPixel(pos.x, pos.y, wrap);
	}

	void Sprite::SetPixelData(const Pixel& col)
	{
		std::fill(pixels.begin(), pixels.end(), col);
	}

	Pixel Sprite::Sample(const vf2d& uv, const SampleMethod sample, const WrapMethod wrap) const
	{
		vf2d denorm = uv * vf2d(size);

		switch (sample)
		{
		case SampleMethod::LINEAR:
			return GetPixel(denorm, wrap);

		case SampleMethod::BILINEAR:
		{
			vf2d cell = denorm.floor();
			vf2d offset = denorm - cell;

			Pixel tl = GetPixel(cell + vf2d(0, 0), wrap);
			Pixel tr = GetPixel(cell + vf2d(1, 0), wrap);
			Pixel bl = GetPixel(cell + vf2d(0, 1), wrap);
			Pixel br = GetPixel(cell + vf2d(1, 1), wrap);

			Pixel topCol = tr * offset.x + tl * (1.0f - offset.x);
			Pixel bottomCol = br * offset.x + bl * (1.0f - offset.x);

			return bottomCol * offset.y + topCol * (1.0f - offset.y);
		}

		case SampleMethod::TRILINEAR:
		{
			vi2d center = (denorm - vf2d(0.5f, 0.5f)).floor();
			vf2d offset = (denorm - vf2d(0.5f, 0.5f)) - vf2d(center);

			struct Pixelf
			{
				float r = 0.0f;
				float g = 0.0f;
				float b = 0.0f;
				float a = 0.0f;
			};

			Pixelf splineX[4][4];

			vec2d<size_t> s;
			for (s.y = 0; s.y < 4; s.y++)
				for (s.x = 0; s.x < 4; s.x++)
				{
					Pixel p = GetPixel(center + s - 1, wrap);
					splineX[s.y][s.x] = { (float)p.r, (float)p.g, (float)p.b, (float)p.a };
				}

			vf2d t = offset;
			vf2d tt = t * t;
			vf2d ttt = tt * tt;

			vf2d q[4];
			q[0] = 0.5f * (-1.0f * ttt + 2.0f * tt - t);
			q[1] = 0.5f * (3.0f * ttt - 5.0f * tt + 2.0f);
			q[2] = 0.5f * (-3.0f * ttt + 4.0f * tt + t);
			q[3] = 0.5f * (ttt - tt);

			Pixelf splineY[4];

			for (size_t s = 0; s < 4; s++)
				for (size_t i = 0; i < 4; i++)
				{
					splineY[s].r += splineX[s][i].r * q[i].x;
					splineY[s].g += splineX[s][i].g * q[i].x;
					splineY[s].b += splineX[s][i].b * q[i].x;
					splineY[s].a += splineX[s][i].a * q[i].x;
				}

			Pixelf pix;
			for (size_t i = 0; i < 4; i++)
			{
				pix.r += splineY[i].r * q[i].y;
				pix.g += splineY[i].g * q[i].y;
				pix.b += splineY[i].b * q[i].y;
				pix.a += splineY[i].a * q[i].y;
			}

			return Pixel(
				(uint8_t)std::clamp(pix.r, 0.0f, 255.0f),
				(uint8_t)std::clamp(pix.g, 0.0f, 255.0f),
				(uint8_t)std::clamp(pix.b, 0.0f, 255.0f),
				(uint8_t)std::clamp(pix.a, 0.0f, 255.0f)
			);
		}

		}

		return def::NONE;
	}

	Texture::Texture(Sprite* sprite)
	{
		Construct(sprite, false);
	}

	Texture::Texture(std::string_view fileName)
	{
		Construct(new Sprite(fileName), true);
	}

	void Texture::Construct(Sprite* sprite, bool deleteSprite)
	{
		Load(sprite);

		uvScale = 1.0f / vf2d(sprite->size);
		size = sprite->size;

		if (deleteSprite)
			delete sprite;
	}

	void Texture::Load(Sprite* sprite)
	{
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(
			GL_TEXTURE_2D,
			0, GL_RGBA,
			sprite->size.x,
			sprite->size.y,
			0, GL_RGBA,
			GL_UNSIGNED_BYTE,
			sprite->pixels.data()
		);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::Update(Sprite* sprite)
	{
		glBindTexture(GL_TEXTURE_2D, id);

		glTexImage2D(
			GL_TEXTURE_2D,
			0, GL_RGBA,
			sprite->size.x,
			sprite->size.y,
			0, GL_RGBA,
			GL_UNSIGNED_BYTE,
			sprite->pixels.data()
		);
		
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	Graphic::Graphic(std::string_view fileName)
	{
		Load(fileName);
	}

	Graphic::Graphic(const vi2d& size)
	{
		Load(size);
	}

	Graphic::~Graphic()
	{
		delete texture;
		delete sprite;
	}

	void Graphic::Load(std::string_view fileName)
	{
		sprite = new Sprite(fileName);
		texture = new Texture(sprite);
	}

	void Graphic::Load(const vi2d& size)
	{
		sprite = new Sprite(size);
		texture = new Texture(sprite);
	}

	void Graphic::Save(std::string_view fileName, const Sprite::FileType type) const
	{
		return sprite->Save(fileName, type);
	}

	void Graphic::UpdateTexture()
	{
		texture->Update(sprite);
	}

	GameEngine::GameEngine()
	{
		m_AppName = "Undefined";
		m_MousePos = { -1, -1 };

		m_Window = nullptr;
		m_Monitor = nullptr;

		m_Font = nullptr;
		m_DrawTarget = nullptr;

		m_Tint = { 255, 255, 255, 255 };

		m_PixelMode = Pixel::Mode::DEFAULT;
		m_TextureStructure = Texture::Structure::FAN;

		m_TickTimer = 0.0f;

		m_Shader = nullptr;
		s_Engine = this;
	}

	GameEngine::~GameEngine()
	{
		Destroy();
	}

	void GameEngine::Destroy()
	{
		delete m_Screen;
		delete m_Font;

		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	void GameEngine::AppThread()
	{
		if (!OnUserCreate())
			m_IsAppRunning = false;

		auto startTime = std::chrono::system_clock::now();
		auto endTime = startTime;

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
			endTime = std::chrono::system_clock::now();

			float deltaTime = std::chrono::duration<float>(endTime - startTime).count();
			startTime = endTime;

			m_TickTimer += deltaTime;

			if (glfwWindowShouldClose(m_Window))
				m_IsAppRunning = false;

			// Check def::Key for keycodes
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

			// Check def::Button for button 
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

			m_MousePos.x = (int)mouseX / m_PixelSize.x;
			m_MousePos.y = (int)mouseY / m_PixelSize.y;

			if (!OnUserUpdate(deltaTime))
				m_IsAppRunning = false;

			ClearBuffer(BLACK);

			m_Offset.x = 0.0f;
			m_Offset.y = 0.0f;

			m_Scale.x = 0.0f;
			m_Scale.y = 0.0f;

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			m_DrawTarget->UpdateTexture();
			glBindTexture(GL_TEXTURE_2D, m_DrawTarget->texture->id);

			DrawQuad(m_Tint);

			for (const auto& texture : m_Textures)
				DrawTexture(texture);

			m_Textures.clear();

			if (!OnAfterDraw())
				m_IsAppRunning = false;

			if (m_IsVSync)
				glfwSwapBuffers(m_Window);
			else
				glFlush();

			glfwPollEvents();

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
		glBindTexture(GL_TEXTURE_2D, ti.texture ? ti.texture->id : 0);

		switch (ti.structure)
		{
		case Texture::Structure::DEFAULT:	glBegin(GL_TRIANGLES);		break;
		case Texture::Structure::FAN:		glBegin(GL_TRIANGLE_FAN);	break;
		case Texture::Structure::STRIP:		glBegin(GL_TRIANGLE_STRIP);	break;
		}

		for (int i = 0; i < ti.points; i++)
		{
			glColor4ub(ti.tint[i].r, ti.tint[i].g, ti.tint[i].b, ti.tint[i].a);
			glTexCoord2f(ti.uv[i].x, ti.uv[i].y);
			glVertex2f(ti.vertices[i].x, ti.vertices[i].y);
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

	void GameEngine::ErrorCallback(int errorCode, const char* description)
	{
		if (errorCode != GLFW_INVALID_ENUM)
		{
			std::cout << "[GLFW Error] Code: "
				<< "0x000" << std::hex << errorCode
				<< ", text: " << description << std::endl;

			s_Engine->Destroy();

			exit(1);
		}
	}

	void GameEngine::DropCallback(GLFWwindow* window, int pathCount, const char* paths[])
	{
		auto& cache = s_Engine->GetDropped();

		cache.clear();
		cache.reserve(pathCount);

		for (int i = 0; i < pathCount; i++)
			cache[i] = paths[i];
	}

	bool GameEngine::OnAfterDraw()
	{
		return true;
	}

	bool GameEngine::Construct(int screenWidth, int screenHeight, int pixelWidth, int pixelHeight, bool fullScreen, bool vsync, bool dirtyPixel)
	{
		glfwSetErrorCallback(ErrorCallback);

		glfwInit();

		m_ScreenSize = { screenWidth, screenHeight };
		m_InvScreenSize = 1.0f / vf2d(m_ScreenSize);
		m_PixelSize = { pixelWidth, pixelHeight };
		m_WindowSize = m_ScreenSize * m_PixelSize;

		m_IsFullScreen = fullScreen;
		m_IsVSync = vsync;

		m_IsDirtyPixel = dirtyPixel;

		m_Monitor = glfwGetPrimaryMonitor();

		if (!m_Monitor)
			return false;

		const GLFWvidmode* videoMode = glfwGetVideoMode(m_Monitor);

		if (!videoMode)
			return false;

		m_MaxWindowSize = { videoMode->width, videoMode->height };

		if (!m_IsVSync)
			glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE);

		if (m_IsFullScreen)
		{
			m_WindowSize = m_MaxWindowSize;
			m_ScreenSize = m_WindowSize / m_PixelSize;

			m_Window = glfwCreateWindow(m_WindowSize.x, m_WindowSize.y, "", m_Monitor, NULL);
			if (!m_Window) return false;

			glfwSetWindowMonitor(
				m_Window,
				m_Monitor,
				0, 0,
				m_WindowSize.x, m_WindowSize.y,
				videoMode->refreshRate
			);
		}
		else
		{
			m_Window = glfwCreateWindow(m_WindowSize.x, m_WindowSize.y, "", NULL, NULL);

			if (!m_Window)
				return false;
		}

		glfwMakeContextCurrent(m_Window);
		glViewport(0, 0, m_WindowSize.x, m_WindowSize.y);

		glEnable(GL_TEXTURE_2D);

		if (!m_IsDirtyPixel)
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		if (m_IsVSync)
		{
			glfwSwapInterval(1);
			glfwWindowHint(GLFW_REFRESH_RATE, videoMode->refreshRate);
		}

		m_Screen = new Graphic(m_ScreenSize);
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

		m_Font = new Sprite({ 128, 48 });

		int px = 0;
		int py = 0;

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

				m_Font->SetPixel(px, py, { k, k, k, k });

				if (++py == 48)
				{
					px++;
					py = 0;
				}
			}
		}

		glfwSetDropCallback(m_Window, DropCallback);

		return true;
	}

	bool GameEngine::Draw(int x, int y, Pixel p)
	{
		if (!m_DrawTarget)
			return false;

		Sprite* target = m_DrawTarget->sprite;

		switch (m_PixelMode)
		{
		case Pixel::Mode::CUSTOM:
			return target->SetPixel(x, y, m_Shader({ x, y }, target->GetPixel(x, y), p));

		case Pixel::Mode::DEFAULT:
			return target->SetPixel(x, y, p);
			
		case Pixel::Mode::MASK:
		{
			if (p.a == 255)
				return target->SetPixel(x, y, p);
		}
		break;

		case Pixel::Mode::ALPHA:
		{
			Pixel d = target->GetPixel(x, y);

			uint8_t r = uint8_t(std::lerp(d.r, p.r, (float)p.a / 255.0f));
			uint8_t g = uint8_t(std::lerp(d.g, p.g, (float)p.a / 255.0f));
			uint8_t b = uint8_t(std::lerp(d.b, p.b, (float)p.a / 255.0f));

			return target->SetPixel(x, y, { r, g, b });
		}

		}

		return false;
	}

	void GameEngine::DrawHorizontalLine(int startX, int endX, int y, const Pixel& p)
	{
		for (int i = startX; i <= endX; i++)
			Draw(i, y, p);
	}

	void GameEngine::DrawLine(int x1, int y1, int x2, int y2, const Pixel& p)
	{
		int dx = x2 - x1;
		int dy = y2 - y1;

		int dx1 = abs(dx);
		int dy1 = abs(dy);

		int px = 2 * dy1 - dx1;
		int py = 2 * dx1 - dy1;

		int x, y, xe, ye;

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

			while (x < xe)
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

			while (y < ye)
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

	void GameEngine::DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const Pixel& p)
	{
		DrawLine(x1, y1, x2, y2, p);
		DrawLine(x2, y2, x3, y3, p);
		DrawLine(x3, y3, x1, y1, p);
	}

	void GameEngine::FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const Pixel& p)
	{
		int t1x, t2x, y, minx, maxx, t1xp, t2xp;

		bool changed1 = false;
		bool changed2 = false;

		int signx1, signx2, dx1, dy1, dx2, dy2;
		int e1, e2;

		if (y1 > y2)
		{
			std::swap(y1, y2);
			std::swap(x1, x2);
		}

		if (y1 > y3)
		{
			std::swap(y1, y3);
			std::swap(x1, x3);
		}

		if (y2 > y3)
		{
			std::swap(y2, y3);
			std::swap(x2, x3);
		}

		t1x = t2x = x1;
		y = y1;
		dx1 = x2 - x1;

		if (dx1 < 0)
		{
			dx1 = -dx1;
			signx1 = -1;
		}
		else
			signx1 = 1;

		dy1 = y2 - y1;
		dx2 = x3 - x1;

		if (dx2 < 0)
		{
			dx2 = -dx2;
			signx2 = -1;
		}
		else
			signx2 = 1;

		dy2 = y3 - y1;

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

		e2 = dx2 >> 1;

		if (y1 == y2)
			goto next;

		e1 = dx1 >> 1;

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

			DrawHorizontalLine(minx, maxx, y, p);

			if (!changed1)
				t1x += signx1;

			t1x += t1xp;

			if (!changed2)
				t2x += signx2;

			t2x += t2xp;
			y++;

			if (y == y2)
				break;
		}

	next:
		dx1 = x3 - x2;

		if (dx1 < 0)
		{
			dx1 = -dx1;
			signx1 = -1;
		}
		else
			signx1 = 1;

		dy1 = y3 - y2;
		t1x = x2;

		if (dy1 > dx1)
		{
			std::swap(dy1, dx1);
			changed1 = true;
		}
		else
			changed1 = false;

		e1 = dx1 >> 1;

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

			DrawHorizontalLine(minx, maxx, y, p);

			if (!changed1)
				t1x += signx1;

			t1x += t1xp;

			if (!changed2)
				t2x += signx2;

			t2x += t2xp;
			y++;

			if (y > y3)
				return;
		}
	}

	void GameEngine::DrawRectangle(int x, int y, int sx, int sy, const Pixel& p)
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

	void GameEngine::FillRectangle(int x, int y, int sx, int sy, const Pixel& p)
	{
		for (int i = 0; i < sx; i++)
			for (int j = 0; j < sy; j++)
				Draw(x + i, y + j, p);
	}

	void GameEngine::DrawCircle(int x, int y, int r, const Pixel& p)
	{
		int x1 = 0;
		int y1 = r;
		int p1 = 3 - 2 * r;

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

			if (p1 < 0)
				p1 += 4 * x1++ + 6;
			else
				p1 += 4 * (x1++ - y1--) + 10;
		}
	}

	void GameEngine::FillCircle(int x, int y, int r, const Pixel& p)
	{
		int x1 = 0;
		int y1 = r;
		int p1 = 3 - 2 * r;

		while (y1 >= x1)
		{
			DrawHorizontalLine(x - x1, x + x1, y - y1, p);
			DrawHorizontalLine(x - y1, x + y1, y - x1, p);
			DrawHorizontalLine(x - x1, x + x1, y + y1, p);
			DrawHorizontalLine(x - y1, x + y1, y + x1, p);

			if (p1 < 0)
				p1 += 4 * x1++ + 6;
			else
				p1 += 4 * (x1++ - y1--) + 10;
		}
	}

	void GameEngine::DrawEllipse(int x, int y, int sx, int sy, const Pixel& p)
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

		if (y > y1)
			y = y1;

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

	void GameEngine::FillEllipse(int x, int y, int sx, int sy, const Pixel& p)
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

		if (y > y1)
			y = y1;

		y += (b + 1) / 2;
		y1 = y - b1;

		a *= 8 * a;
		b1 = 8 * b * b;

		int z = 0;

		do
		{
			DrawHorizontalLine(x, x1, y, p);
			DrawHorizontalLine(x, x1, y1, p);

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
			DrawHorizontalLine(x - 1, x1 + 1, y++, p);
			DrawHorizontalLine(x - 1, x1 + 1, y1--, p);
		}
	}

	void GameEngine::DrawSprite(int x, int y, const Sprite* sprite)
	{
		for (int i = 0; i < sprite->size.x; i++)
			for (int j = 0; j < sprite->size.y; j++)
				Draw(x + i, y + j, sprite->GetPixel(i, j));
	}

	void GameEngine::DrawPartialSprite(int x, int y, int fx, int fy, int fsx, int fsy, const Sprite* sprite)
	{
		for (int i = 0, x1 = 0; i < fsx; i++, x1++)
			for (int j = 0, y1 = 0; j < fsy; j++, y1++)
				Draw(x + x1, y + y1, sprite->GetPixel(fx + i, fy + j));
	}

	void GameEngine::DrawTexture(float x, float y, const Texture* tex, float scaleX, float scaleY, const Pixel& tint)
	{
		vf2d screenPos =
		{
			(x * m_InvScreenSize.x) * 2.0f - 1.0f,
			((y * m_InvScreenSize.y) * 2.0f - 1.0f) * -1.0f
		};

		vf2d screenSize =
		{
			screenPos.x + (2.0f * (float(tex->size.x) * m_InvScreenSize.x)) * scaleX,
			screenPos.y - (2.0f * (float(tex->size.y) * m_InvScreenSize.y)) * scaleY
		};

		TextureInstance ti;
		ti.texture = tex;
		ti.points = 4;
		ti.structure = m_TextureStructure;
		ti.tint = { tint, tint, tint, tint };
		ti.vertices = { screenPos, { screenPos.x, screenSize.y }, screenSize, { screenSize.x, screenPos.y } };
		ti.uv = { { 0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f} };

		m_Textures.push_back(ti);
	}

	void GameEngine::DrawPartialTexture(float x, float y, float filePosX, float filePosY, float fileSizeX, float fileSizeY, const Texture* tex, float scaleX, float scaleY, const Pixel& tint)
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

		vf2d tl = (vf2d(filePosX, filePosY) + vf2d(0.0001f, 0.0001f)) * tex->uvScale;
		vf2d br = (vf2d(filePosX, filePosY) + vf2d(fileSizeX, fileSizeY) - vf2d(0.0001f, 0.0001f)) * tex->uvScale;

		TextureInstance ti;
		ti.texture = tex;
		ti.points = 4;
		ti.structure = m_TextureStructure;
		ti.tint = { tint, tint, tint, tint };
		ti.vertices = { quantisedPos, { quantisedPos.x, quantisedSize.y }, quantisedSize, { quantisedSize.x, quantisedPos.y } };
		ti.uv = { tl, { tl.x, br.y }, br, { br.x, tl.y } };
		m_Textures.push_back(ti);
	}

	void GameEngine::DrawRotatedTexture(float x, float y, float r, const Texture* tex, float centerX, float centerY, float scaleX, float scaleY, const Pixel& tint)
	{
		TextureInstance ti;
		ti.texture = tex;
		ti.points = 4;
		ti.structure = m_TextureStructure;
		ti.tint = { tint, tint, tint, tint };

		vf2d center = vf2d(centerX, centerY) * vf2d(tex->size);
		vf2d scale = vf2d(scaleX, scaleY);

		ti.vertices = {
			-1.0f * center * scale,
			(vf2d(0.0f, (float)tex->size.y) - center) * scale,
			(vf2d((float)tex->size.x, (float)tex->size.y) - center) * scale,
			(vf2d((float)tex->size.x, 0.0f) - center) * scale
		};

		float c = cos(r), s = sin(r);
		for (int i = 0; i < ti.points; i++)
		{
			vf2d o = { ti.vertices[i].x * c - ti.vertices[i].y * s, ti.vertices[i].x * s + ti.vertices[i].y * c };
			ti.vertices[i] = vf2d(x, y) + o;
			ti.vertices[i] = ti.vertices[i] * m_InvScreenSize * 2.0f - vf2d(1.0f, 1.0f);
			ti.vertices[i].y *= -1.0f;
		}

		ti.uv = { { 0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f} };
		m_Textures.push_back(ti);
	}

	void GameEngine::DrawPartialRotatedTexture(float x, float y, float fx, float fy, float fw, float fh, float r, const Texture* tex, float centerX, float centerY, float scaleX, float scaleY, const Pixel& tint)
	{
		TextureInstance ti;
		ti.texture = tex;
		ti.points = 4;
		ti.structure = m_TextureStructure;
		ti.tint = { tint, tint, tint, tint };

		vf2d screenSpacePos =
		{
			  ((x - centerX * tex->size.x) * m_InvScreenSize.x) * 2.0f - 1.0f,
			-(((y - centerY * tex->size.y) * m_InvScreenSize.y) * 2.0f - 1.0f)
		};

		vf2d screenSpaceSize =
		{
			  ((x - centerX * tex->size.x + fw * scaleX) * m_InvScreenSize.x) * 2.0f - 1.0f,
			-(((y - centerY * tex->size.y + fh * scaleY) * m_InvScreenSize.y) * 2.0f - 1.0f)
		};

		vf2d quantisedPos = ((screenSpacePos * vf2d(m_WindowSize)) + vf2d(0.5f, 0.5f)).floor() / vf2d(m_WindowSize);
		vf2d quantisedSize = ((screenSpaceSize * vf2d(m_WindowSize)) + vf2d(0.5f, -0.5f)).ceil() / vf2d(m_WindowSize);

		vf2d tl = (vf2d(fx, fy) + vf2d(0.0001f, 0.0001f)) * tex->uvScale;
		vf2d br = (vf2d(fx, fy) + vf2d(fx, fy) - vf2d(0.0001f, 0.0001f)) * tex->uvScale;

		ti.vertices = { quantisedPos, { quantisedPos.x, quantisedSize.y }, quantisedSize, { quantisedSize.x, quantisedPos.y } };

		float c = cos(r), s = sin(r);
		for (int i = 0; i < ti.points; i++)
		{
			vf2d o = { ti.vertices[i].x * c - ti.vertices[i].y * s, ti.vertices[i].x * s + ti.vertices[i].y * c };
			ti.vertices[i] = vf2d(x, y) + o;
			ti.vertices[i] = ti.vertices[i] * m_InvScreenSize * 2.0f - vf2d(1.0f, 1.0f);
			ti.vertices[i].y *= -1.0f;
		}

		ti.uv = { tl, { tl.x, br.y }, br, { br.x, tl.y } };
		m_Textures.push_back(ti);
	}

	void GameEngine::DrawWarpedTexture(const std::vector<vf2d>& points, const Texture* tex, const Pixel& tint)
	{
		TextureInstance ti;
		ti.texture = tex;
		ti.structure = m_TextureStructure;
		ti.points = 4;
		ti.tint = { tint, tint, tint, tint };
		ti.vertices.resize(ti.points);
		ti.uv = { { 0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f} };

		float rd = ((points[2].x - points[0].x) * (points[3].y - points[1].y) - (points[3].x - points[1].x) * (points[2].y - points[0].y));

		if (rd != 0.0f)
		{
			rd = 1.0f / rd;

			float rn = ((points[3].x - points[1].x) * (points[0].y - points[1].y) - (points[3].y - points[1].y) * (points[0].x - points[1].x)) * rd;
			float sn = ((points[2].x - points[0].x) * (points[0].y - points[1].y) - (points[2].y - points[0].y) * (points[0].x - points[1].x)) * rd;

			vf2d center;
			if (!(rn < 0.0f || rn > 1.0f || sn < 0.0f || sn > 1.0f))
				center = points[0] + rn * (points[2] - points[0]);

			float d[4];

			for (int i = 0; i < 4; i++)
				d[i] = (points[i] - center).mag();

			for (int i = 0; i < 4; i++)
			{
				float q = d[i] == 0.0f ? 1.0f : (d[i] + d[(i + 2) & 3]) / d[(i + 2) & 3];
				ti.uv[i] *= q;
				ti.vertices[i] = { (points[i].x * m_InvScreenSize.x) * 2.0f - 1.0f, ((points[i].y * m_InvScreenSize.y) * 2.0f - 1.0f) * -1.0f };
			}

			m_Textures.push_back(ti);
		}
	}

	void GameEngine::DrawWireFrameModel(const std::vector<vf2d>& modelCoordinates, float x, float y, float r, float s, const Pixel& p)
	{
		size_t verts = modelCoordinates.size();

		std::vector<vf2d> coordinates(verts);
		float cs = cosf(r), sn = sinf(r);

		for (size_t i = 0; i < verts; i++)
		{
			coordinates[i].x = (modelCoordinates[i].x * cs - modelCoordinates[i].y * sn) * s + x;
			coordinates[i].y = (modelCoordinates[i].x * sn + modelCoordinates[i].y * cs) * s + y;
		}

		for (size_t i = 0; i < verts + 1; i++)
			DrawLine(coordinates[i % verts], coordinates[(i + 1) % verts], p);
	}

	void GameEngine::FillWireFrameModel(const std::vector<vf2d>& modelCoordinates, float x, float y, float r, float s, const Pixel& p)
	{
		size_t verts = modelCoordinates.size();

		std::vector<vf2d> coordinates;
		coordinates.resize(verts);

		for (size_t i = 0; i < verts; i++)
		{
			coordinates[i].x = (modelCoordinates[i].x * cosf(r) - modelCoordinates[i].y * sinf(r)) * s + x;
			coordinates[i].y = (modelCoordinates[i].x * sinf(r) + modelCoordinates[i].y * cosf(r)) * s + y;
		}

		auto GetAngle = [](const vf2d& p1, const vf2d& p2)
			{
				float a = atan2(p2.y, p2.x) - atan2(p1.y, p1.x);
				while (a > 3.14159f) a -= 3.14159f * 2.0f;
				while (a < -3.14159f) a += 3.14159f * 2.0f;
				return a;
			};

		auto PointInPolygon = [&](const vf2d& p)
			{
				float angle = 0.0f;

				vf2d p1, p2;
				for (int i = 0; i < verts; i++)
				{
					p1 = coordinates[i] - p;
					p2 = coordinates[(i + 1) % verts] - p;
					angle += GetAngle(p1, p2);
				}

				return std::abs(angle) < 3.14159f;
			};

		vf2d min = coordinates.front();
		vf2d max = coordinates.front();

		for (size_t i = 1; i < verts; i++)
		{
			if (min.x > coordinates[i].x) min.x = coordinates[i].x;
			if (min.y > coordinates[i].y) min.y = coordinates[i].y;

			if (max.x < coordinates[i].x) max.x = coordinates[i].x;
			if (max.y < coordinates[i].y) max.y = coordinates[i].y;
		}

		vf2d point;
		for (point.x = min.x; point.x < max.x; point.x++)
			for (point.y = min.y; point.y < max.y; point.y++)
			{
				if (PointInPolygon(point))
					Draw(point, p);
			}
	}

	void GameEngine::DrawString(int x, int y, std::string_view s, const Pixel& p)
	{
		int sx = 0;
		int sy = 0;

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
				int ox = (c - 32) % 16;
				int oy = (c - 32) / 16;

				for (int i = 0; i < 8; i++)
					for (int j = 0; j < 8; j++)
					{
						if (m_Font->GetPixel(i + ox * 8, j + oy * 8).r > 0)
							Draw(x + sx + i, y + sy + j, p);
					}

				sx += 8;
			}
		}
	}

	void GameEngine::Clear(const Pixel& p)
	{
		m_DrawTarget->sprite->SetPixelData(p);
	}

	KeyState GameEngine::GetKey(Key k) const { return m_Keys[static_cast<size_t>(k)]; }
	KeyState GameEngine::GetMouse(Button k) const { return m_Mouse[static_cast<size_t>(k)]; }

	int GameEngine::MouseX() const { return m_MousePos.x; }
	int GameEngine::MouseY() const { return m_MousePos.y; }

	int GameEngine::ScreenWidth() const { return m_ScreenSize.x; }
	int GameEngine::ScreenHeight() const { return m_ScreenSize.y; }

	bool GameEngine::IsFullScreen() const { return m_IsFullScreen; }
	bool GameEngine::IsVSync() const { return m_IsVSync; }
	bool GameEngine::IsFocused() const { return static_cast<bool>(glfwGetWindowAttrib(m_Window, GLFW_FOCUSED)); }

	void GameEngine::SetIcon(std::string_view fileName)
	{
		Sprite icon(fileName);

		GLFWimage img;
		img.width = icon.size.x;
		img.height = icon.size.y;
		img.pixels = (uint8_t*)icon.pixels.data();

		glfwSetWindowIcon(m_Window, 1, &img);
	}

	void GameEngine::SetDrawTarget(Graphic* target)
	{
		m_DrawTarget = target ? target : m_Screen;
		m_DrawTarget->UpdateTexture();
	}

	Graphic* GameEngine::GetDrawTarget()
	{
		return m_DrawTarget;
	}

	void GameEngine::SetTitle(std::string_view title)
	{
		m_AppName = title;
	}

	WindowState GameEngine::GetWindowState() const
	{
		int f = static_cast<int>(WindowState::NONE);
		if (glfwGetWindowAttrib(m_Window, GLFW_FOCUSED)) f |= static_cast<int>(WindowState::FOCUSED);
		if (glfwGetWindowAttrib(m_Window, GLFW_MAXIMIZED)) f |= static_cast<int>(WindowState::MAXIMIZED);
		return static_cast<WindowState>(f);
	}

	GLFWwindow* GameEngine::GetWindow() const
	{
		return m_Window;
	}

	std::vector<std::string>& GameEngine::GetDropped()
	{
		return m_DropCache;
	}

	void GameEngine::SetPixelMode(Pixel::Mode pixelMode)
	{
		m_PixelMode = pixelMode;
	}

	Pixel::Mode GameEngine::GetPixelMode() const
	{
		return m_PixelMode;
	}

	void GameEngine::SetTextureStructure(Texture::Structure textureStructure)
	{
		m_TextureStructure = textureStructure;
	}

	Texture::Structure GameEngine::GetTextureStructure() const
	{
		return m_TextureStructure;
	}

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

	void GameEngine::DrawCircle(const vi2d& pos, int r, const Pixel& p)
	{
		DrawCircle(pos.x, pos.y, r, p);
	}

	void GameEngine::FillCircle(const vi2d& pos, int r, const Pixel& p)
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

	void GameEngine::DrawSprite(const vi2d& pos, const Sprite* spr)
	{
		DrawSprite(pos.x, pos.y, spr);
	}

	void GameEngine::DrawPartialSprite(const vi2d& pos, const vi2d& fpos, const vi2d& fsize, const Sprite* spr)
	{
		DrawPartialSprite(pos.x, pos.y, fpos.x, fpos.y, fsize.x, fsize.y, spr);
	}

	void GameEngine::DrawTexture(const vf2d& pos, const Texture* tex, const vf2d& scale, const Pixel& tint)
	{
		DrawTexture(pos.x, pos.y, tex, scale.x, scale.y, tint);
	}

	void GameEngine::DrawPartialTexture(const vf2d& pos, const vf2d& filePos, const vf2d& fileSize, const Texture* tex, const vf2d& scale, const Pixel& tint)
	{
		DrawPartialTexture(pos.x, pos.y, filePos.x, filePos.y, fileSize.x, fileSize.y, tex, scale.x, scale.y, tint);
	}

	void GameEngine::DrawRotatedTexture(const vf2d& pos, float r, const Texture* tex, const vf2d& center, const vf2d& scale, const Pixel& tint)
	{
		DrawRotatedTexture(pos.x, pos.y, r, tex, center.x, center.y, scale.x, scale.y, tint);
	}

	void GameEngine::DrawPartialRotatedTexture(const vf2d& pos, const vf2d& fpos, const vf2d& fsize, float r, const Texture* tex, const vf2d& center, const vf2d& scale, const Pixel& tint)
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

	void GameEngine::DrawString(const vi2d& pos, std::string_view text, const Pixel& p)
	{
		DrawString(pos.x, pos.y, text, p);
	}

	vi2d GameEngine::ScreenSize() const
	{
		return m_ScreenSize;
	}

	vi2d GameEngine::MaxWindowSize() const
	{
		return m_MaxWindowSize;
	}

	vi2d GameEngine::GetMouse() const
	{
		return m_MousePos;
	}

	void GameEngine::ClearBuffer(const Pixel& p)
	{
		glClearColor((float)p.r / 255.0f, (float)p.g / 255.0f, (float)p.b / 255.0f, (float)p.a / 255.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void GameEngine::SetTint(const Pixel& p)
	{
		m_Tint = p;
	}

	void GameEngine::SetShader(Pixel(*func)(const vi2d& pos, const Pixel& prev, const Pixel& cur))
	{
		m_Shader = func;
		m_PixelMode = m_Shader ? Pixel::Mode::CUSTOM : Pixel::Mode::DEFAULT;
	}

	Key GameEngine::AnyKey(bool pressed, bool held, bool released)
	{
		for (uint32_t i = 0; i < 512U; i++)
		{
			if (m_Keys[i].pressed && pressed || m_Keys[i].held && held || m_Keys[i].released && released)
				return Key(i);
		}

		return Key::NONE;
	}

	std::vector<Key> GameEngine::AnyKeys(bool pressed, bool held, bool released)
	{
		std::vector<Key> keys;

		for (uint32_t i = 0; i < 512U; i++)
		{
			if (m_Keys[i].pressed && pressed || m_Keys[i].held && held || m_Keys[i].released && released)
				keys.push_back(Key(i));
		}

		return keys;
	}

#endif

}

#endif
