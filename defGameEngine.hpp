#ifndef DEF_GAME_ENGINE_HPP
#define DEF_GAME_ENGINE_HPP

#pragma GCC diagnostic ignored "-Wunknown-pragmas"

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
					Draw(i, j, def::Pixel(rand() % 256, rand() % 256, rand() % 256));

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
#include <string_view>
#include <chrono>
#include <vector>
#include <cmath>
#include <algorithm>
#include <functional>
#include <list>

#ifdef __EMSCRIPTEN__
	#define PLATFORM_EMSCRIPTEN
#else
	#define PLATFORM_GL
	#define PLATFORM_GLFW3
#endif

#ifdef PLATFORM_GLFW3
	#include "GLFW/glfw3.h"
#endif

#ifdef PLATFORM_EMSCRIPTEN
	#include <EGL/egl.h>
	#include <GLES2/gl2.h>

	#define GL_GLEXT_PROTOTYPES
	#include <GLES2/gl2ext.h>

	#include <emscripten/emscripten.h>
	#include <emscripten/key_codes.h>
	#include <emscripten/html5.h>
#endif

#ifdef STB_IMAGE_IMPLEMENTATION
#undef STB_IMAGE_IMPLEMENTATION
#endif
#include "stb/stb_image.h"

#ifdef STB_IMAGE_WRITE_IMPLEMENTATION
#undef STB_IMAGE_WRITE_IMPLEMENTATION
#endif
#include "stb/stb_image_write.h"

#pragma warning(disable : 4996)

// Oh, dear stb_image...
#define SAFE_STBI_FAILURE_REASON() (stbi_failure_reason() ? stbi_failure_reason() : "")

#ifdef _WIN32

#define _CRT_SECURE_NO_WARNINGS

#ifndef __MINGW32__
	#ifdef PLATFORM_GL
		#pragma comment(lib, "opengl32.lib")
	#endif

	#ifdef PLATFORM_GLFW3
		#pragma comment(lib, "glfw3.lib")
	#endif
#endif

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
		NONE = -1,

		SPACE, APOSTROPHE, COMMA, MINUS, PERIOD, SLASH,
		K0, K1, K2, K3, K4, K5, K6, K7, K8, K9,
		SEMICOLON, EQUAL,

		A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S,
		T, U, V, W, X, Y, Z,

		LEFT_BRACKET, BACKSLASH, RIGHT_BRACKET,

		ESCAPE, ENTER, TAB, BACKSPACE, INSERT, DEL, RIGHT, LEFT,
		DOWN, UP, PAGE_UP, PAGE_DOWN, HOME, END,
		CAPS_LOCK, SCROLL_LOCK, NUM_LOCK, PRINT_SCREEN, PAUSE,

		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13,
		F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24,

		NP_0, NP_1, NP_2, NP_3, NP_4, NP_5, NP_6, NP_7, NP_8,
		NP_9, NP_DECIMAL, NP_DIVIDE, NP_MULTIPLY, NP_SUBTRACT, NP_ADD,
		NP_ENTER, NP_EQUAL,

		LEFT_SHIFT, LEFT_CONTROL, LEFT_ALT, LEFT_SUPER, RIGHT_SHIFT,
		RIGHT_CONTROL, RIGHT_ALT, RIGHT_SUPER, MENU,

		KEYS_COUNT
	};

	enum class Button
	{
		LEFT, RIGHT, WHEEL,
		MOUSE4, MOUSE5, MOUSE6,
		MOUSE7, MOUSE8
	};

#ifndef DGE_IGNORE_VEC2D

	template <class T>
	struct vec2d
	{
		static_assert(std::is_arithmetic<T>::value, "vec2d<T> must be numeric");

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

		std::string str() const;

		template <class F>
		constexpr operator vec2d<F>() const
		{
			return { static_cast<F>(this->x), static_cast<F>(this->y) };
		}
	};

	typedef vec2d<int> vi2d;
	typedef vec2d<float> vf2d;
	typedef vec2d<double> vd2d;

	template <class T1, class T2>
	constexpr vec2d<T1>& operator+=(vec2d<T1>& v1, const vec2d<T2>& v2);

	template <class T1, class T2>
	constexpr vec2d<T1>& operator-=(vec2d<T1>& v1, const vec2d<T2>& v2);

	template <class T1, class T2>
	constexpr vec2d<T1>& operator*=(vec2d<T1>& v1, const vec2d<T2>& v2);

	template <class T1, class T2>
	constexpr vec2d<T1>& operator/=(vec2d<T1>& v1, const vec2d<T2>& v2);

	template <class T1, class T2>
	constexpr vec2d<T1>& operator%=(vec2d<T1>& v1, const vec2d<T2>& v2);

	template <class T1, class T2>
	constexpr vec2d<T1>& operator+=(vec2d<T1>& v1, const T2& v2);

	template <class T1, class T2>
	constexpr vec2d<T1>& operator-=(vec2d<T1>& v1, const T2& v2);

	template <class T1, class T2>
	constexpr vec2d<T1>& operator*=(vec2d<T1>& v1, const T2& v2);

	template <class T1, class T2>
	constexpr vec2d<T1>& operator/=(vec2d<T1>& v1, const T2& v2);

	template <class T1, class T2>
	constexpr vec2d<T1>& operator%=(vec2d<T1>& v1, const T2& v2);

	template <class T1, class T2>
	constexpr vec2d<decltype(T1(1) + T2(1))> operator+(const vec2d<T1>& v1, const vec2d<T2>& v2);

	template <class T1, class T2>
	constexpr vec2d<decltype(T1(1) + T2(1))> operator-(const vec2d<T1>& v1, const vec2d<T2>& v2);

	template <class T1, class T2>
	constexpr vec2d<decltype(T1(1) + T2(1))> operator*(const vec2d<T1>& v1, const vec2d<T2>& v2);

	template <class T1, class T2>
	constexpr vec2d<decltype(T1(1) + T2(1))> operator/(const vec2d<T1>& v1, const vec2d<T2>& v2);

	template <class T1, class T2>
	constexpr vec2d<decltype(T1(1) + T2(1))> operator%(const vec2d<T1>& v1, const vec2d<T2>& v2);

	template <class T1, class T2>
	constexpr vec2d<decltype(T1(1) + T2(1))> operator+(const vec2d<T1>& v1, const T2& v2);

	template <class T1, class T2>
	constexpr vec2d<decltype(T1(1) + T2(1))> operator-(const vec2d<T1>& v1, const T2& v2);

	template <class T1, class T2>
	constexpr vec2d<decltype(T1(1) + T2(1))> operator*(const vec2d<T1>& v1, const T2& v2);

	template <class T1, class T2>
	constexpr vec2d<decltype(T1(1) + T2(1))> operator/(const vec2d<T1>& v1, const T2& v2);

	template <class T1, class T2>
	constexpr vec2d<decltype(T1(1) + T2(1))> operator+(const T1& v1, const vec2d<T2>& v2);

	template <class T1, class T2>
	constexpr vec2d<decltype(T1(1) + T2(1))> operator-(const T1& v1, const vec2d<T2>& v2);

	template <class T1, class T2>
	constexpr vec2d<decltype(T1(1) + T2(1))> operator*(const T1& v1, const vec2d<T2>& v2);

	template <class T1, class T2>
	constexpr vec2d<decltype(T1(1) + T2(1))> operator/(const T1& v1, const vec2d<T2>& v2);

	template <class T1, class T2>
	constexpr vec2d<decltype(T1(1) + T2(1))> operator%(const T1& v1, const vec2d<T2>& v2);

	template <class T>
	constexpr vec2d<T> operator-(const vec2d<T>& v);

	template <class T1, class T2>
	constexpr bool operator==(const vec2d<T1>& v1, const vec2d<T2>& v2);

	template <class T1, class T2>
	constexpr bool operator<=(const vec2d<T1>& v1, const vec2d<T2>& v2);

	template <class T1, class T2>
	constexpr bool operator>=(const vec2d<T1>& v1, const vec2d<T2>& v2);

	template <class T1, class T2>
	constexpr bool operator<(const vec2d<T1>& v1, const vec2d<T2>& v2);

	template <class T1, class T2>
	constexpr bool operator>(const vec2d<T1>& v1, const vec2d<T2>& v2);

	template <class T1, class T2>
	constexpr bool operator!=(const vec2d<T1>& v1, const vec2d<T2>& v2);

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
		constexpr Pixel(uint32_t rgba = 0x000000FF);
		constexpr Pixel(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF);

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
			uint32_t rgba_n;
			uint8_t rgba_v[4];
		};

		constexpr Pixel lerp(const Pixel& rhs, const float factor) const;
		std::string str() const;

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

		bool SetPixel(int x, int y, const Pixel& col);
		bool SetPixel(const vi2d& pos, const Pixel& col);

		Pixel GetPixel(int x, int y, const WrapMethod wrap = WrapMethod::NONE) const;
		Pixel GetPixel(const vi2d& pos, const WrapMethod wrap = WrapMethod::NONE) const;

		void SetPixelData(const Pixel& col);

		Pixel Sample(float x, float y, const SampleMethod sampleMethod, const WrapMethod wrapMethod) const;
		Pixel Sample(const vf2d& pos, const SampleMethod sampleMethod, const WrapMethod wrapMethod) const;
	};

	struct Texture
	{
		enum class Structure
		{
			DEFAULT,
			FAN,
			STRIP,
			WIREFRAME
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
		TextureInstance();

		const Texture* texture;

		Texture::Structure structure;
		uint32_t points;

		std::vector<Pixel> tint;
		std::vector<vf2d> vertices;
		std::vector<vf2d> uv;
	};

	class GameEngine;

	class Platform
	{
	public:
		virtual void Destroy() const = 0;
		virtual void SetTitle(const std::string& text) const = 0;

		virtual bool IsWindowClose() const = 0;
		virtual bool IsWindowFocused() const = 0;

		virtual void ClearBuffer(const Pixel& col) const = 0;

		virtual void OnBeforeDraw() = 0;
		virtual void OnAfterDraw() = 0;

		virtual void FlushScreen(bool vsync) const = 0;
		virtual void PollEvents() const = 0;

		virtual void DrawQuad(const Pixel& tint) const = 0;
		virtual void DrawTexture(const TextureInstance& texInst) const = 0;

		virtual void BindTexture(int id) const = 0;

		virtual bool ConstructWindow(vi2d& screenSize, const vi2d pixelSize, vi2d& windowSize, bool vsync, bool fullscreen, bool dirtypixel) = 0;
		
		virtual void SetIcon(Sprite& icon) const = 0;
	};

#ifdef PLATFORM_GL

	class Platform_GL : public Platform
	{
	public:
		void ClearBuffer(const Pixel& col) const override;

		void OnBeforeDraw() override;
		void OnAfterDraw() override;

		void DrawQuad(const Pixel& tint) const override;
		void DrawTexture(const TextureInstance& texInst) const override;

		void BindTexture(int id) const override;

		void Destroy() const override;
		void SetTitle(const std::string& text) const override;

		bool IsWindowClose() const override;
		bool IsWindowFocused() const override;

		void FlushScreen(bool vsync) const override;
		void PollEvents() const override;

		bool ConstructWindow(vi2d& screenSize, const vi2d pixelSize, vi2d& windowSize, bool vsync, bool fullscreen, bool dirtypixel) override;

		void SetIcon(Sprite& icon) const override;
	};

#endif

#ifdef PLATFORM_GLFW3

	class Platform_GLFW3 : public Platform_GL
	{
	public:
		Platform_GLFW3();

		friend class GameEngine;

	private:
		GLFWmonitor* m_Monitor;
		GLFWwindow* m_Window;

	public:
		static void ErrorCallback(int errorCode, const char* description);
		static void DropCallback(GLFWwindow* window, int pathCount, const char* paths[]);
		static void ScrollCallback(GLFWwindow* window, double x, double y);
		static void MousePosCallback(GLFWwindow* window, double x, double y);
		static void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

		void Destroy() const override;
		void SetTitle(const std::string& text) const override;

		bool IsWindowClose() const override;
		bool IsWindowFocused() const override;

		void FlushScreen(bool vsync) const override;
		void PollEvents() const override;

		bool ConstructWindow(vi2d& screenSize, const vi2d pixelSize, vi2d& windowSize, bool vsync, bool fullscreen, bool dirtypixel) override;
		
		void SetIcon(Sprite& icon) const override;
	};

#endif

#ifdef PLATFORM_EMSCRIPTEN
	
	class Platform_Emscripten : public Platform
	{
	public:
		friend class GameEngine;

		virtual void Destroy() const override;
		virtual void SetTitle(const std::string& text) const override;

		virtual bool IsWindowClose() const override;
		virtual bool IsWindowFocused() const override;

		virtual void ClearBuffer(const Pixel& col) const override;

		virtual void OnBeforeDraw() override;
		virtual void OnAfterDraw() override;

		virtual void FlushScreen(bool vsync) const override;
		virtual void PollEvents() const override;

		virtual void DrawQuad(const Pixel& tint) const override;
		virtual void DrawTexture(const TextureInstance& texInst) const override;

		virtual void BindTexture(int id) const override;

		virtual bool ConstructWindow(vi2d& screenSize, const vi2d pixelSize, vi2d& windowSize, bool vsync, bool fullscreen, bool dirtypixel) override;
		
		virtual void SetIcon(Sprite& icon) const override;

		static void MainLoop();

	private:
		static EM_BOOL FocusCallback(int eventType, const EmscriptenFocusEvent* event, void* userData);
		static EM_BOOL KeyboardCallback(int eventType, const EmscriptenKeyboardEvent* event, void* userData);
		static EM_BOOL WheelCallback(int eventType, const EmscriptenWheelEvent* event, void* userData);
		static EM_BOOL TouchCallback(int eventType, const EmscriptenTouchEvent* event, void* userData);
		static EM_BOOL MouseCallback(int eventType, const EmscriptenMouseEvent* event, void* userData);

	public:
		static bool s_IsWindowFocused;

	private:
		EGLDisplay m_Display;
		EGLConfig m_Config;
		EGLContext m_Context;
		EGLSurface m_Surface;

		uint32_t m_FragmentShader = 0;
		uint32_t m_VertexShader = 0;
		uint32_t m_QuadShader = 0;
		uint32_t m_VbQuad = 0;
		uint32_t m_VaQuad = 0;

		struct Vertex
		{
			float pos[3];
			vf2d uv;
			Pixel col;
		};

		mutable Vertex m_VertexMemory[128];

		Graphic m_BlankQuad;
	};

	bool Platform_Emscripten::s_IsWindowFocused = false;

#endif

	class GameEngine
	{
	public:
		GameEngine();
		virtual ~GameEngine();

#ifdef PLATFORM_GLFW3
		friend class Platform_GLFW3;
#endif

#ifdef PLATFORM_EMSCRIPTEN
		friend class Platform_Emscripten;
#endif

	private:
		std::string m_AppName;

		vi2d m_WindowSize;
		vi2d m_ScreenSize;
		vf2d m_InvScreenSize;
		vi2d m_PixelSize;

		bool m_IsAppRunning;
		bool m_IsFullScreen;
		bool m_IsDirtyPixel;
		bool m_IsVSync;
		bool m_OnlyTextures;

		KeyState m_Keys[(size_t)Key::KEYS_COUNT];
		KeyState m_Mouse[8];

		bool m_KeyOldState[(size_t)Key::KEYS_COUNT];
		bool m_KeyNewState[(size_t)Key::KEYS_COUNT];

		bool m_MouseOldState[8];
		bool m_MouseNewState[8];

		vi2d m_MousePos;

		Graphic m_Font;
		int m_TabSize = 4;

		Graphic* m_DrawTarget;
		Graphic* m_Screen;

		std::vector<TextureInstance> m_Textures;

		Pixel m_ConsoleBackgroundColour;
		Pixel m_ClearBufferColour;

		Texture::Structure m_TextureStructure;
		Pixel::Mode m_PixelMode;

		std::vector<std::string> m_DropCache;
		int m_ScrollDelta;

		std::string m_TextInput;
		size_t m_CursorPos;

		bool m_CaptureText;
		bool m_ShowConsole;
		bool m_Caps;

		struct ConsoleEntry
		{
			std::string command;
			std::string output;

			Pixel outputColour;
		};

		std::vector<ConsoleEntry> m_ConsoleHistory;
		size_t m_PickedConsoleHistoryCommand;

		float m_DeltaTime;
		float m_TickTimer;

		Pixel (*m_Shader)(const vi2d&, const Pixel&, const Pixel&);

		Platform* m_Platform;

		std::chrono::system_clock::time_point m_TimeStart;
		std::chrono::system_clock::time_point m_TimeEnd;

#ifndef PLATFORM_EMSCRIPTEN
		uint32_t m_FramesCount;
#endif

	public:
		static GameEngine* s_Engine;
		static std::unordered_map<Key, std::pair<char, char>> s_KeyboardUS;
		static std::unordered_map<int, def::Key> s_KeysTable;
		inline static std::vector<vf2d> s_UnitCircle;

		virtual bool OnUserCreate() = 0;
		virtual bool OnUserUpdate(float deltaTime) = 0;
		virtual bool OnAfterDraw();

		virtual void OnTextCapturingComplete(const std::string& text);
		virtual bool OnConsoleCommand(const std::string& command, std::stringstream& output, Pixel& colour);

		bool Construct(int screenWidth, int screenHeight, int pixelWidth, int pixelHeight, bool fullScreen = false, bool vsync = false, bool dirtyPixel = true);
		void Run();

	private:
		void Destroy();
		void ScanHardware(KeyState* data, bool* newState, bool* oldState, size_t count);
		void MainLoop();

		static void MakeUnitCircle(std::vector<vf2d>& circle, const size_t verts);

	public:
		bool Draw(const vi2d& pos, const Pixel& col = WHITE);
		virtual bool Draw(int x, int y, const Pixel& col = WHITE);

		void DrawLine(const vi2d& pos1, const vi2d& pos2, const Pixel& col = WHITE);
		virtual void DrawLine(int x1, int y1, int x2, int y2, const Pixel& col = WHITE);

		void DrawTriangle(const vi2d& pos1, const vi2d& pos2, const vi2d& pos3, const Pixel& col = WHITE);
		virtual void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const Pixel& col = WHITE);

		void FillTriangle(const vi2d& pos1, const vi2d& pos2, const vi2d& pos3, const Pixel& col = WHITE);
		virtual void FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const Pixel& col = WHITE);

		void DrawRectangle(const vi2d& pos, const vi2d& size, const Pixel& col = WHITE);
		virtual void DrawRectangle(int x, int y, int sizeX, int sizeY, const Pixel& col = WHITE);

		void FillRectangle(const vi2d& pos, const vi2d& size, const Pixel& col = WHITE);
		virtual void FillRectangle(int x, int y, int sizeX, int sizeY, const Pixel& col = WHITE);

		void DrawCircle(const vi2d& pos, int radius, const Pixel& col = WHITE);
		virtual void DrawCircle(int x, int y, int radius, const Pixel& col = WHITE);

		void FillCircle(const vi2d& pos, int radius, const Pixel& col = WHITE);
		virtual void FillCircle(int x, int y, int radius, const Pixel& col = WHITE);

		void DrawEllipse(const vi2d& pos, const vi2d& size, const Pixel& col = WHITE);
		virtual void DrawEllipse(int x, int y, int sizeX, int sizeY, const Pixel& col = WHITE);

		void FillEllipse(const vi2d& pos, const vi2d& size, const Pixel& col = WHITE);
		virtual void FillEllipse(int x, int y, int sizeX, int sizeY, const Pixel& col = WHITE);

		void DrawSprite(const vi2d& pos, const Sprite* sprite);
		virtual void DrawSprite(int x, int y, const Sprite* sprite);

		void DrawPartialSprite(const vi2d& pos, const vi2d& filePos, const vi2d& fileSize, const Sprite* sprite);
		virtual void DrawPartialSprite(int x, int y, int fileX, int fileY, int fileSizeX, int fileSizeY, const Sprite* sprite);

		void DrawWireFrameModel(const std::vector<vf2d>& modelCoordinates, const vf2d& pos, float rotation = 0.0f, float scale = 1.0f, const Pixel& col = WHITE);
		virtual void DrawWireFrameModel(const std::vector<vf2d>& modelCoordinates, float x, float y, float rotation = 0.0f, float scale = 1.0f, const Pixel& col = WHITE);

		void FillWireFrameModel(const std::vector<vf2d>& modelCoordinates, const vf2d& pos, float rotation = 0.0f, float scale = 1.0f, const Pixel& col = WHITE);
		virtual void FillWireFrameModel(const std::vector<vf2d>& modelCoordinates, float x, float y, float rotation = 0.0f, float scale = 1.0f, const Pixel& col = WHITE);

		void DrawString(const vi2d& pos, std::string_view text, const Pixel& col = WHITE, const vec2d<uint32_t>& scale = { 1, 1 });
		virtual void DrawString(int x, int y, std::string_view text, const Pixel& col = WHITE, uint32_t scaleX = 1, uint32_t scaleY = 1);

		virtual void Clear(const Pixel& col);
		void ClearTexture(const Pixel& col);

		void DrawTexture(const vf2d& pos, const Texture* tex, const vf2d& scale = { 1.0f, 1.0f }, const Pixel& tint = WHITE);
		void DrawPartialTexture(const vf2d& pos, const Texture* tex, const vf2d& filePos, const vf2d& fileSize, const vf2d& scale = { 1.0f, 1.0f }, const Pixel& tint = WHITE);

		void DrawWarpedTexture(const std::vector<vf2d>& points, const Texture* tex, const Pixel& tint = WHITE);

		void DrawRotatedTexture(const vf2d& pos, const Texture* tex, float rotation, const vf2d& center = { 0.0f, 0.0f }, const vf2d& scale = { 1.0f, 1.0f }, const Pixel& tint = WHITE);
		void DrawPartialRotatedTexture(const vf2d& pos, const Texture* tex, const vf2d& filePos, const vf2d& fileSize, float rotation, const vf2d& center = { 0.0f, 0.0f }, const vf2d& scale = { 1.0f, 1.0f }, const Pixel& tint = WHITE);
		
		void DrawTexturePolygon(const std::vector<vf2d>& verts, const std::vector<Pixel>& cols, Texture::Structure structure);

		void DrawTextureLine(const vi2d& pos1, const vi2d& pos2, const Pixel& col = WHITE);

		void DrawTextureTriangle(const vi2d& pos1, const vi2d& pos2, const vi2d& pos3, const Pixel& col = WHITE);
		void DrawTextureRectangle(const vi2d& pos, const vi2d& size, const Pixel& col = WHITE);
		void DrawTextureCircle(const vi2d& pos, int radius, const Pixel& col = WHITE);

		void FillTextureTriangle(const vi2d& pos1, const vi2d& pos2, const vi2d& pos3, const Pixel& col = WHITE);
		void FillTextureRectangle(const vi2d& pos, const vi2d& size, const Pixel& col = WHITE);
		void FillTextureCircle(const vi2d& pos, int radius, const Pixel& col = WHITE);

		void GradientTextureTriangle(const vi2d& pos1, const vi2d& pos2, const vi2d& pos3, const Pixel& col1 = WHITE, const Pixel& col2 = WHITE, const Pixel& col3 = WHITE);
		void GradientTextureRectangle(const vi2d& pos, const vi2d& size, const Pixel& colTL = WHITE, const Pixel& colTR = WHITE, const Pixel& colBR = WHITE, const Pixel& colBL = WHITE);

		void DrawTextureString(const vi2d& pos, std::string_view text, const Pixel& col = def::WHITE, const vf2d& scale = { 1.0f, 1.0f });

		KeyState GetKey(Key key) const;
		KeyState GetMouse(Button button) const;

		vi2d GetMousePos() const;
		int GetMouseWheelDelta() const;

		int GetMouseX() const;
		int GetMouseY() const;

		void SetTitle(std::string_view title);

		vi2d GetScreenSize() const;
		vi2d GetWindowSize() const;

		int ScreenWidth() const;
		int ScreenHeight() const;

		bool IsFullScreen() const;
		bool IsVSync() const;
		bool IsFocused() const;

		void SetIcon(std::string_view fileName);

		void SetDrawTarget(Graphic* target);
		Graphic* GetDrawTarget();

		std::vector<std::string>& GetDropped();

		void SetPixelMode(Pixel::Mode pixelMode);
		Pixel::Mode GetPixelMode() const;

		void SetTextureStructure(Texture::Structure textureStructure);
		Texture::Structure GetTextureStructure() const;

		void SetShader(Pixel (*func)(const vi2d&, const Pixel&, const Pixel&));

		void CaptureText(bool enable);
		bool IsCapturingText() const;

		std::string GetCapturedText() const;
		size_t GetCursorPos() const;

		void SetConsoleBackgroundColour(const Pixel& col);
		void ShowConsole(bool enable);
		bool IsConsoleEnabled() const;
		void ClearCapturedText();
		void ClearConsole();

		bool IsCaps() const;

		void UseOnlyTextures(bool enable);

		float GetDeltaTime() const;

		auto GetWindow()
		{
#if defined(PLATFORM_GLFW3)
			return ((Platform_GLFW3*)m_Platform)->m_Window;
#elif defined(PLATFORM_EMSCRIPTEN)
			return ((Platform_Emscripten*)m_Platform)->m_Display;
#endif
		}
	};

#ifdef DGE_APPLICATION
#undef DGE_APPLICATION

	std::unordered_map<Key, std::pair<char, char>> GameEngine::s_KeyboardUS =
	{
		{ Key::SPACE, { ' ', ' ' } }, { Key::APOSTROPHE, { '\'', '"' } },
		{ Key::COMMA, { ',', '<' } }, { Key::MINUS, { '-', '_' } },
		{ Key::PERIOD, { '.', '>' } }, { Key::SLASH, { '/', '?' } },
		{ Key::K0, { '0', ')' } }, { Key::K1, { '1', '!' } },
		{ Key::K2, { '2', '@' } }, { Key::K3, { '3', '#' } },
		{ Key::K4, { '4', '$' } }, { Key::K5, { '5', '%' } },
		{ Key::K6, { '6', '^' } }, { Key::K7, { '7', '&' } },
		{ Key::K8, { '8', '*' } }, { Key::K9, { '9', '(' } },
		{ Key::SEMICOLON, { ';', ':' } }, { Key::EQUAL, { '=', '+' } },
		{ Key::A, { 'a', 'A' } }, { Key::B, { 'b', 'B' } },
		{ Key::C, { 'c', 'C' } }, { Key::D, { 'd', 'D' } },
		{ Key::E, { 'e', 'E' } }, { Key::F, { 'f', 'F' } },
		{ Key::G, { 'g', 'G' } }, { Key::H, { 'h', 'H' } },
		{ Key::I, { 'i', 'I' } }, { Key::J, { 'j', 'J' } },
		{ Key::K, { 'k', 'K' } }, { Key::L, { 'l', 'L' } },
		{ Key::M, { 'm', 'M' } }, { Key::N, { 'n', 'N' } },
		{ Key::O, { 'o', 'O' } }, { Key::P, { 'p', 'P' } },
		{ Key::Q, { 'q', 'Q' } }, { Key::R, { 'r', 'R' } },
		{ Key::S, { 's', 'S' } }, { Key::T, { 't', 'T' } },
		{ Key::U, { 'u', 'U' } }, { Key::V, { 'v', 'V' } },
		{ Key::W, { 'w', 'W' } }, { Key::X, { 'x', 'X' } },
		{ Key::Y, { 'y', 'Y' } }, { Key::Z, { 'z', 'Z' } },
		{ Key::LEFT_BRACKET, { '[', '{' } }, { Key::BACKSLASH, { '\\', '|' } },
		{ Key::RIGHT_BRACKET, { ']', '}' } },
		{ Key::NP_0, { '0', '0' } }, { Key::NP_1, { '1', '1' } },
		{ Key::NP_2, { '2', '2' } }, { Key::NP_3, { '3', '3' } },
		{ Key::NP_4, { '4', '4' } }, { Key::NP_5, { '5', '5' } },
		{ Key::NP_6, { '6', '6' } }, { Key::NP_7, { '7', '7' } },
		{ Key::NP_8, { '8', '8' } }, { Key::NP_9, { '9', '9' } },
		{ Key::NP_DIVIDE, { '/', '/' } }, { Key::NP_MULTIPLY, { '*', '*' } },
		{ Key::NP_SUBTRACT, { '-', '-' } }, { Key::NP_ADD, { '+', '+' } },
		{ Key::NP_EQUAL, { '=', '+' } }
	};

#ifdef PLATFORM_GLFW3

	std::unordered_map<int, def::Key> GameEngine::s_KeysTable =
	{
		{ GLFW_KEY_SPACE, Key::SPACE }, { GLFW_KEY_APOSTROPHE, Key::APOSTROPHE }, { GLFW_KEY_COMMA, Key::COMMA },
		{ GLFW_KEY_MINUS, Key::MINUS }, { GLFW_KEY_PERIOD, Key::PERIOD }, { GLFW_KEY_SLASH, Key::SLASH },

		{ GLFW_KEY_0, Key::K0 }, { GLFW_KEY_1, Key::K1 }, { GLFW_KEY_2, Key::K2 }, { GLFW_KEY_3, Key::K3 }, { GLFW_KEY_4, Key::K4 },
		{ GLFW_KEY_5, Key::K5 }, { GLFW_KEY_6, Key::K6 }, { GLFW_KEY_7, Key::K7 }, { GLFW_KEY_8, Key::K8 }, { GLFW_KEY_9, Key::K9 },

		{ GLFW_KEY_SEMICOLON, Key::SEMICOLON }, { GLFW_KEY_EQUAL, Key::EQUAL },

		{ GLFW_KEY_A, Key::A }, { GLFW_KEY_B, Key::B }, { GLFW_KEY_C, Key::C }, { GLFW_KEY_D, Key::D },
		{ GLFW_KEY_E, Key::E }, { GLFW_KEY_F, Key::F }, { GLFW_KEY_G, Key::G }, { GLFW_KEY_H, Key::H },
		{ GLFW_KEY_I, Key::I }, { GLFW_KEY_J, Key::J }, { GLFW_KEY_K, Key::K }, { GLFW_KEY_L, Key::L },
		{ GLFW_KEY_M, Key::M }, { GLFW_KEY_N, Key::N }, { GLFW_KEY_O, Key::O }, { GLFW_KEY_P, Key::P },
		{ GLFW_KEY_Q, Key::Q }, { GLFW_KEY_R, Key::R }, { GLFW_KEY_S, Key::S }, { GLFW_KEY_T, Key::T },
		{ GLFW_KEY_U, Key::U }, { GLFW_KEY_V, Key::V }, { GLFW_KEY_W, Key::W }, { GLFW_KEY_X, Key::X },
		{ GLFW_KEY_Y, Key::Y }, { GLFW_KEY_Z, Key::Z },
		{ GLFW_KEY_LEFT_BRACKET, Key::LEFT_BRACKET }, { GLFW_KEY_BACKSLASH, Key::BACKSLASH }, { GLFW_KEY_RIGHT_BRACKET, Key::RIGHT_BRACKET },

		{ GLFW_KEY_ESCAPE, Key::ESCAPE }, { GLFW_KEY_ENTER, Key::ENTER }, { GLFW_KEY_TAB, Key::TAB }, { GLFW_KEY_BACKSPACE, Key::BACKSPACE },
		{ GLFW_KEY_INSERT, Key::INSERT }, { GLFW_KEY_DELETE, Key::DEL }, { GLFW_KEY_RIGHT, Key::RIGHT }, { GLFW_KEY_LEFT, Key::LEFT },
		{ GLFW_KEY_DOWN, Key::DOWN }, { GLFW_KEY_UP, Key::UP }, { GLFW_KEY_PAGE_UP, Key::PAGE_UP }, { GLFW_KEY_PAGE_DOWN, Key::PAGE_DOWN },
		{ GLFW_KEY_HOME, Key::HOME }, { GLFW_KEY_END, Key::END },
		{ GLFW_KEY_CAPS_LOCK, Key::CAPS_LOCK }, { GLFW_KEY_SCROLL_LOCK, Key::SCROLL_LOCK }, { GLFW_KEY_NUM_LOCK, Key::NUM_LOCK },
		{ GLFW_KEY_PRINT_SCREEN, Key::PRINT_SCREEN }, { GLFW_KEY_PAUSE, Key::PAUSE },

		{ GLFW_KEY_F1, Key::F1 }, { GLFW_KEY_F2, Key::F2 }, { GLFW_KEY_F3, Key::F3 }, { GLFW_KEY_F4, Key::F4 }, { GLFW_KEY_F5, Key::F5 }, { GLFW_KEY_F6, Key::F6 },
		{ GLFW_KEY_F7, Key::F7 }, { GLFW_KEY_F8, Key::F8 }, { GLFW_KEY_F9, Key::F9 }, { GLFW_KEY_F10, Key::F10 }, { GLFW_KEY_F11, Key::F11 }, { GLFW_KEY_F12, Key::F12 },
		{ GLFW_KEY_F13, Key::F13 }, { GLFW_KEY_F14, Key::F14 }, { GLFW_KEY_F15, Key::F15 }, { GLFW_KEY_F16, Key::F16 }, { GLFW_KEY_F17, Key::F17 }, { GLFW_KEY_F18, Key::F18 },
		{ GLFW_KEY_F19, Key::F19 }, { GLFW_KEY_F20, Key::F20 }, { GLFW_KEY_F21, Key::F21 }, { GLFW_KEY_F22, Key::F22 }, { GLFW_KEY_F23, Key::F23 }, { GLFW_KEY_F24, Key::F24 },

		{ GLFW_KEY_KP_0, Key::NP_0 }, { GLFW_KEY_KP_1, Key::NP_1 }, { GLFW_KEY_KP_2, Key::NP_2 },
		{ GLFW_KEY_KP_3, Key::NP_3 }, { GLFW_KEY_KP_4, Key::NP_4 }, { GLFW_KEY_KP_5, Key::NP_5 },
		{ GLFW_KEY_KP_6, Key::NP_6 }, { GLFW_KEY_KP_7, Key::NP_7 }, { GLFW_KEY_KP_8, Key::NP_8 },
		{ GLFW_KEY_KP_9, Key::NP_9 }, { GLFW_KEY_KP_DECIMAL, Key::NP_DECIMAL },
		{ GLFW_KEY_KP_DIVIDE, Key::NP_DIVIDE }, { GLFW_KEY_KP_MULTIPLY, Key::NP_MULTIPLY },
		{ GLFW_KEY_KP_SUBTRACT, Key::NP_SUBTRACT }, { GLFW_KEY_KP_ADD, Key::NP_ADD },
		{ GLFW_KEY_KP_ENTER, Key::NP_ENTER }, { GLFW_KEY_KP_EQUAL, Key::NP_EQUAL },
		
		{ GLFW_KEY_LEFT_SHIFT, Key::LEFT_SHIFT }, { GLFW_KEY_LEFT_CONTROL, Key::LEFT_CONTROL },
		{ GLFW_KEY_LEFT_ALT, Key::LEFT_ALT }, { GLFW_KEY_LEFT_SUPER, Key::LEFT_SUPER },
		{ GLFW_KEY_RIGHT_SHIFT, Key::RIGHT_SHIFT }, { GLFW_KEY_RIGHT_CONTROL, Key::RIGHT_CONTROL },
		{ GLFW_KEY_RIGHT_ALT, Key::RIGHT_ALT }, { GLFW_KEY_RIGHT_SUPER, Key::RIGHT_SUPER }, { GLFW_KEY_MENU, Key::MENU },

		{ GLFW_KEY_UNKNOWN, Key::NONE }
	};

#endif

#ifdef PLATFORM_EMSCRIPTEN

	std::unordered_map<int, Key> GameEngine::s_KeysTable =
	{
		{ DOM_PK_SPACE, Key::SPACE }, { DOM_PK_QUOTE, Key::APOSTROPHE }, { DOM_PK_COMMA, Key::COMMA },
		{ DOM_PK_MINUS, Key::MINUS }, { DOM_PK_PERIOD, Key::PERIOD }, { DOM_PK_SLASH, Key::SLASH },

		{ DOM_PK_0, Key::K0 }, { DOM_PK_1, Key::K1 }, { DOM_PK_2, Key::K2 }, { DOM_PK_3, Key::K3 }, { DOM_PK_4, Key::K4 },
		{ DOM_PK_5, Key::K5 }, { DOM_PK_6, Key::K6 }, { DOM_PK_7, Key::K7 }, { DOM_PK_8, Key::K8 }, { DOM_PK_9, Key::K9 },

		{ DOM_PK_SEMICOLON, Key::SEMICOLON }, { DOM_PK_EQUAL, Key::EQUAL },

		{ DOM_PK_A, Key::A }, { DOM_PK_B, Key::B }, { DOM_PK_C, Key::C }, { DOM_PK_D, Key::D },
		{ DOM_PK_E, Key::E }, { DOM_PK_F, Key::F }, { DOM_PK_G, Key::G }, { DOM_PK_H, Key::H },
		{ DOM_PK_I, Key::I }, { DOM_PK_J, Key::J }, { DOM_PK_K, Key::K }, { DOM_PK_L, Key::L },
		{ DOM_PK_M, Key::M }, { DOM_PK_N, Key::N }, { DOM_PK_O, Key::O }, { DOM_PK_P, Key::P },
		{ DOM_PK_Q, Key::Q }, { DOM_PK_R, Key::R }, { DOM_PK_S, Key::S }, { DOM_PK_T, Key::T },
		{ DOM_PK_U, Key::U }, { DOM_PK_V, Key::V }, { DOM_PK_W, Key::W }, { DOM_PK_X, Key::X },
		{ DOM_PK_Y, Key::Y }, { DOM_PK_Z, Key::Z },

		{ DOM_PK_BRACKET_LEFT, Key::LEFT_BRACKET }, { DOM_PK_BACKSLASH, Key::BACKSLASH }, { DOM_PK_BRACKET_RIGHT, Key::RIGHT_BRACKET },

		{ DOM_PK_ESCAPE, Key::ESCAPE }, { DOM_PK_ENTER, Key::ENTER }, { DOM_PK_TAB, Key::TAB }, { DOM_PK_BACKSPACE, Key::BACKSPACE },
		{ DOM_PK_INSERT, Key::INSERT }, { DOM_PK_DELETE, Key::DEL }, { DOM_PK_ARROW_RIGHT, Key::RIGHT }, { DOM_PK_ARROW_LEFT, Key::LEFT },
		{ DOM_PK_ARROW_DOWN, Key::DOWN }, { DOM_PK_ARROW_UP, Key::UP }, { DOM_PK_PAGE_UP, Key::PAGE_UP }, { DOM_PK_PAGE_DOWN, Key::PAGE_DOWN },
		{ DOM_PK_HOME, Key::HOME }, { DOM_PK_END, Key::END },
		{ DOM_PK_CAPS_LOCK, Key::CAPS_LOCK }, { DOM_PK_SCROLL_LOCK, Key::SCROLL_LOCK }, { DOM_PK_NUM_LOCK, Key::NUM_LOCK },
		{ DOM_PK_PRINT_SCREEN, Key::PRINT_SCREEN }, { DOM_PK_PAUSE, Key::PAUSE },

		{ DOM_PK_F1, Key::F1 }, { DOM_PK_F2, Key::F2 }, { DOM_PK_F3, Key::F3 }, { DOM_PK_F4, Key::F4 }, { DOM_PK_F5, Key::F5 }, { DOM_PK_F6, Key::F6 },
		{ DOM_PK_F7, Key::F7 }, { DOM_PK_F8, Key::F8 }, { DOM_PK_F9, Key::F9 }, { DOM_PK_F10, Key::F10 }, { DOM_PK_F11, Key::F11 }, { DOM_PK_F12, Key::F12 },
		{ DOM_PK_F13, Key::F13 }, { DOM_PK_F14, Key::F14 }, { DOM_PK_F15, Key::F15 }, { DOM_PK_F16, Key::F16 }, { DOM_PK_F17, Key::F17 }, { DOM_PK_F18, Key::F18 },
		{ DOM_PK_F19, Key::F19 }, { DOM_PK_F20, Key::F20 }, { DOM_PK_F21, Key::F21 }, { DOM_PK_F22, Key::F22 }, { DOM_PK_F23, Key::F23 }, { DOM_PK_F24, Key::F24 },

		{ DOM_PK_NUMPAD_0, Key::NP_0 }, { DOM_PK_NUMPAD_1, Key::NP_1 }, { DOM_PK_NUMPAD_2, Key::NP_2 },
		{ DOM_PK_NUMPAD_3, Key::NP_3 }, { DOM_PK_NUMPAD_4, Key::NP_4 }, { DOM_PK_NUMPAD_5, Key::NP_5 },
		{ DOM_PK_NUMPAD_6, Key::NP_6 }, { DOM_PK_NUMPAD_7, Key::NP_7 }, { DOM_PK_NUMPAD_8, Key::NP_8 },
		{ DOM_PK_NUMPAD_9, Key::NP_9 }, { DOM_PK_NUMPAD_DECIMAL, Key::NP_DECIMAL },
		{ DOM_PK_NUMPAD_DIVIDE, Key::NP_DIVIDE }, { DOM_PK_NUMPAD_MULTIPLY, Key::NP_MULTIPLY },
		{ DOM_PK_NUMPAD_SUBTRACT, Key::NP_SUBTRACT }, { DOM_PK_NUMPAD_ADD, Key::NP_ADD },
		{ DOM_PK_NUMPAD_ENTER, Key::NP_ENTER }, { DOM_PK_NUMPAD_EQUAL, Key::NP_EQUAL },

		{ DOM_PK_SHIFT_LEFT, Key::LEFT_SHIFT }, { DOM_PK_CONTROL_LEFT, Key::LEFT_CONTROL },
		{ DOM_PK_ALT_LEFT, Key::LEFT_ALT }, { DOM_PK_OS_LEFT, Key::LEFT_SUPER },
		{ DOM_PK_SHIFT_RIGHT, Key::RIGHT_SHIFT }, { DOM_PK_CONTROL_RIGHT, Key::RIGHT_CONTROL },
		{ DOM_PK_ALT_RIGHT, Key::RIGHT_ALT }, { DOM_PK_OS_RIGHT, Key::RIGHT_SUPER }, { DOM_PK_CONTEXT_MENU, Key::MENU },

		{ DOM_PK_UNKNOWN, Key::NONE }
	};

#endif

	GameEngine* GameEngine::s_Engine = nullptr;

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
	constexpr vec2d<T>::vec2d(const T& x, const T& y) : x(x), y(y)
	{

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
	constexpr vec2d<decltype(T1(1) + T2(1))> operator+(const vec2d<T1>& v1, const vec2d<T2>& v2)
	{
		return vec2d(v1.x + v2.x, v1.y + v2.y);
	}

	template <class T1, class T2>
	constexpr vec2d<decltype(T1(1) + T2(1))> operator-(const vec2d<T1>& v1, const vec2d<T2>& v2)
	{
		return vec2d(v1.x - v2.x, v1.y - v2.y);
	}

	template <class T1, class T2>
	constexpr vec2d<decltype(T1(1) + T2(1))> operator*(const vec2d<T1>& v1, const vec2d<T2>& v2)
	{
		return vec2d(v1.x * v2.x, v1.y * v2.y);
	}

	template <class T1, class T2>
	constexpr vec2d<decltype(T1(1) + T2(1))> operator/(const vec2d<T1>& v1, const vec2d<T2>& v2)
	{
		return vec2d(v1.x / v2.x, v1.y / v2.y);
	}

	template <class T1, class T2>
	constexpr vec2d<decltype(T1(1) + T2(1))> operator%(const vec2d<T1>& v1, const vec2d<T2>& v2)
	{
		return vec2d(v1.x % v2.x, v1.y % v2.y);
	}

	template <class T1, class T2>
	constexpr vec2d<decltype(T1(1) + T2(1))> operator+(const vec2d<T1>& v1, const T2& v2)
	{
		return vec2d(v1.x + v2, v1.y + v2);
	}

	template <class T1, class T2>
	constexpr vec2d<decltype(T1(1) + T2(1))> operator-(const vec2d<T1>& v1, const T2& v2)
	{
		return vec2d(v1.x - v2, v1.y - v2);
	}

	template <class T1, class T2>
	constexpr vec2d<decltype(T1(1) + T2(1))> operator*(const vec2d<T1>& v1, const T2& v2)
	{
		return vec2d(v1.x * v2, v1.y * v2);
	}

	template <class T1, class T2>
	constexpr vec2d<decltype(T1(1) + T2(1))> operator/(const vec2d<T1>& v1, const T2& v2)
	{
		return vec2d(v1.x / v2, v1.y / v2);
	}

	template <class T1, class T2>
	constexpr vec2d<decltype(T1(1) + T2(1))> operator+(const T1& v1, const vec2d<T2>& v2)
	{
		return vec2d(v1 + v2.x, v1 + v2.y);
	}

	template <class T1, class T2>
	constexpr vec2d<decltype(T1(1) + T2(1))> operator-(const T1& v1, const vec2d<T2>& v2)
	{
		return vec2d(v1 - v2.x, v1 - v2.y);
	}

	template <class T1, class T2>
	constexpr vec2d<decltype(T1(1) + T2(1))> operator*(const T1& v1, const vec2d<T2>& v2)
	{
		return vec2d(v1 * v2.x, v1 * v2.y);
	}

	template <class T1, class T2>
	constexpr vec2d<decltype(T1(1) + T2(1))> operator/(const T1& v1, const vec2d<T2>& v2)
	{
		return vec2d(v1 / v2.x, v1 / v2.y);
	}

	template <class T1, class T2>
	constexpr vec2d<decltype(T1(1) + T2(1))> operator%(const T1& v1, const vec2d<T2>& v2)
	{
		return vec2d(v1 % v2.x, v1 % v2.y);
	}

	template <class T>
	constexpr vec2d<T> operator-(const vec2d<T>& v)
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
	std::string vec2d<T>::str() const
	{
		return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
	}

#endif

	constexpr KeyState::KeyState() : held(false), released(false), pressed(false)
	{

	}

	constexpr KeyState::KeyState(bool held, bool released, bool pressed)
		: held(held), released(released), pressed(pressed)
	{

	}

	constexpr Pixel::Pixel(uint32_t rgba) : rgba_n(rgba)
	{

	}

	constexpr Pixel::Pixel(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a)
	{

	}

	constexpr Pixel Pixel::lerp(const Pixel& rhs, const float factor) const
	{
		return Pixel(
			(uint8_t)std::lerp(r, rhs.r, factor),
			(uint8_t)std::lerp(g, rhs.g, factor),
			(uint8_t)std::lerp(b, rhs.b, factor),
			(uint8_t)std::lerp(a, rhs.a, factor)
		);
	}

	std::string Pixel::str() const
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

	bool Sprite::SetPixel(int x, int y, const Pixel& col)
	{
		if (x >= 0 && y >= 0 && x < size.x && y < size.y)
		{
			pixels[y * size.x + x] = col;
			return true;
		}

		return false;
	}

	bool Sprite::SetPixel(const vi2d& pos, const Pixel& col)
	{
		return SetPixel(pos.x, pos.y, col);
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

	Pixel Sprite::Sample(float x, float y, const SampleMethod sample, const WrapMethod wrap) const
	{
		return Sample({ x, y }, sample, wrap);
	}

	Pixel Sprite::Sample(const vf2d& pos, const SampleMethod sample, const WrapMethod wrap) const
	{
		vf2d denorm = pos * vf2d(size);

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

		return NONE;
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
#if defined(PLATFORM_GL) || defined(PLATFORM_EMSCRIPTEN)
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
#else
#error Consider defining PLATFORM_GL macro
#endif
	}

	void Texture::Update(Sprite* sprite)
	{
#if defined(PLATFORM_GL) || defined(PLATFORM_EMSCRIPTEN)
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
#else
#error Consider defining PLATFORM_GL macro
#endif
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

	TextureInstance::TextureInstance()
	{
		texture = nullptr;

		structure = Texture::Structure::FAN;
		points = 0;

		uv = { { 0.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f } };
	}

#ifdef PLATFORM_GL

	void Platform_GL::ClearBuffer(const Pixel& col) const
	{
		glClearColor((float)col.r / 255.0f, (float)col.g / 255.0f, (float)col.b / 255.0f, (float)col.a / 255.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Platform_GL::OnBeforeDraw()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void Platform_GL::OnAfterDraw()
	{
	}

	void Platform_GL::DrawQuad(const Pixel& tint) const
	{
		glBegin(GL_QUADS);
		glColor4ub(tint.r, tint.g, tint.b, tint.a);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, 1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, 1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, -1.0f);
		glEnd();
	}

	void Platform_GL::DrawTexture(const TextureInstance& texInst) const
	{
		BindTexture(texInst.texture ? texInst.texture->id : 0);

		switch (texInst.structure)
		{
		case Texture::Structure::DEFAULT:	glBegin(GL_TRIANGLES);		break;
		case Texture::Structure::FAN:		glBegin(GL_TRIANGLE_FAN);	break;
		case Texture::Structure::STRIP:		glBegin(GL_TRIANGLE_STRIP);	break;
		case Texture::Structure::WIREFRAME:	glBegin(GL_LINE_LOOP);		break;
		}

		for (uint32_t i = 0; i < texInst.points; i++)
		{
			glColor4ub(texInst.tint[i].r, texInst.tint[i].g, texInst.tint[i].b, texInst.tint[i].a);
			glTexCoord2f(texInst.uv[i].x, texInst.uv[i].y);
			glVertex2f(texInst.vertices[i].x, texInst.vertices[i].y);
		}

		glEnd();
	}

	void Platform_GL::BindTexture(int id) const
	{
		glBindTexture(GL_TEXTURE_2D, id);
	}

	void Platform_GL::Destroy() const {}
	void Platform_GL::SetTitle(const std::string& text) const {}

	bool Platform_GL::IsWindowClose() const { return false; }
	bool Platform_GL::IsWindowFocused() const { return false; }
	void Platform_GL::FlushScreen(bool vsync) const {}
	void Platform_GL::PollEvents() const {}

	bool Platform_GL::ConstructWindow(vi2d& screenSize, const vi2d pixelSize, vi2d& windowSize, bool vsync, bool fullscreen, bool dirtypixel) { return false; }
	void Platform_GL::SetIcon(Sprite& icon) const {}

#endif

#ifdef PLATFORM_GLFW3

	Platform_GLFW3::Platform_GLFW3()
	{
		m_Window = nullptr;
		m_Monitor = nullptr;

		glfwSetErrorCallback(ErrorCallback);
		glfwInit();
	}

	void Platform_GLFW3::ErrorCallback(int errorCode, const char* description)
	{
		if (errorCode != GLFW_INVALID_ENUM)
		{
			std::cout << "[GLFW Error] Code: "
				<< "0x000" << std::hex << errorCode
				<< ", text: " << description << std::endl;

			GameEngine::s_Engine->Destroy();

			exit(1);
		}
	}

	void Platform_GLFW3::DropCallback(GLFWwindow* window, int pathCount, const char* paths[])
	{
		auto& cache = GameEngine::s_Engine->GetDropped();

		cache.clear();
		cache.reserve(pathCount);

		for (int i = 0; i < pathCount; i++)
			cache[i] = paths[i];
	}

	void Platform_GLFW3::ScrollCallback(GLFWwindow* window, double x, double y)
	{
		GameEngine::s_Engine->m_ScrollDelta = y;
	}

	void Platform_GLFW3::MousePosCallback(GLFWwindow* window, double x, double y)
	{
		GameEngine::s_Engine->m_MousePos.x = (int)x / GameEngine::s_Engine->m_PixelSize.x;
		GameEngine::s_Engine->m_MousePos.y = (int)y / GameEngine::s_Engine->m_PixelSize.y;
	}

	void Platform_GLFW3::KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		GameEngine::s_Engine->m_KeyNewState[size_t(GameEngine::s_KeysTable[key])] = (action == GLFW_PRESS || action == GLFW_REPEAT);
	}

	void Platform_GLFW3::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		GameEngine::s_Engine->m_MouseNewState[button] = (action == GLFW_PRESS || action == GLFW_REPEAT);
	}

	void Platform_GLFW3::Destroy() const
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	void Platform_GLFW3::SetTitle(const std::string& text) const
	{
		glfwSetWindowTitle(m_Window, text.c_str());
	}

	bool Platform_GLFW3::IsWindowClose() const
	{
		return glfwWindowShouldClose(m_Window);
	}

	bool Platform_GLFW3::IsWindowFocused() const
	{
		return glfwGetWindowAttrib(m_Window, GLFW_FOCUSED) == GLFW_TRUE;
	}

	void Platform_GLFW3::FlushScreen(bool vsync) const
	{
		if (vsync)
			glfwSwapBuffers(m_Window);
		else
			glFlush();
	}

	void Platform_GLFW3::PollEvents() const
	{
		glfwPollEvents();
	}

	bool Platform_GLFW3::ConstructWindow(vi2d& screenSize, const vi2d pixelSize, vi2d& windowSize, bool vsync, bool fullscreen, bool dirtypixel)
	{
		m_Monitor = glfwGetPrimaryMonitor();

		if (!m_Monitor)
			return false;

		if (!vsync)
			glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE);

		const GLFWvidmode* videoMode = glfwGetVideoMode(m_Monitor);
		if (!videoMode) return false;

		if (fullscreen)
		{
			windowSize = { videoMode->width, videoMode->height };
			screenSize = windowSize / pixelSize;

			m_Window = glfwCreateWindow(windowSize.x, windowSize.y, "", m_Monitor, NULL);
			if (!m_Window) return false;

			glfwSetWindowMonitor(
				m_Window,
				m_Monitor,
				0, 0,
				windowSize.x, windowSize.y,
				videoMode->refreshRate
			);
		}
		else
		{
			m_Window = glfwCreateWindow(windowSize.x, windowSize.y, "", NULL, NULL);

			if (!m_Window)
				return false;
		}

		glfwMakeContextCurrent(m_Window);
		glViewport(0, 0, windowSize.x, windowSize.y);

		glEnable(GL_TEXTURE_2D);

		if (!dirtypixel)
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		if (vsync)
		{
			glfwSwapInterval(1);
			glfwWindowHint(GLFW_REFRESH_RATE, videoMode->refreshRate);
		}

		glfwSetDropCallback(m_Window, DropCallback);

		glfwSetScrollCallback(m_Window, ScrollCallback);
		glfwSetCursorPosCallback(m_Window, MousePosCallback);
		glfwSetMouseButtonCallback(m_Window, MouseButtonCallback);
		glfwSetKeyCallback(m_Window, KeyboardCallback);

		return true;
	}

	void Platform_GLFW3::SetIcon(Sprite& icon) const
	{
		GLFWimage img;
		img.width = icon.size.x;
		img.height = icon.size.y;
		img.pixels = (uint8_t*)icon.pixels.data();

		glfwSetWindowIcon(m_Window, 1, &img);
	}

#endif

#ifdef PLATFORM_EMSCRIPTEN

	void Platform_Emscripten::Destroy() const
	{
		emscripten_cancel_main_loop();

		eglMakeCurrent(m_Display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		eglDestroyContext(m_Display, m_Context);
		eglDestroySurface(m_Display, m_Surface);
		eglTerminate(m_Display);
	}

	void Platform_Emscripten::SetTitle(const std::string& text) const
	{
		emscripten_set_window_title(text.c_str());
	}

	bool Platform_Emscripten::IsWindowClose() const
	{
		return !GameEngine::s_Engine->m_IsAppRunning;
	}

	bool Platform_Emscripten::IsWindowFocused() const
	{
		return Platform_Emscripten::s_IsWindowFocused;
	}

	void Platform_Emscripten::ClearBuffer(const Pixel& col) const
	{
		glClearColor((float)col.r / 255.0f, (float)col.g / 255.0f, (float)col.b / 255.0f, (float)col.a / 255.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Platform_Emscripten::OnBeforeDraw()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glUseProgram(m_QuadShader);
		glBindVertexArrayOES(m_VaQuad);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)(5 * sizeof(float)));
		glEnableVertexAttribArray(2);
	}

	void Platform_Emscripten::OnAfterDraw()
	{

	}

	void Platform_Emscripten::FlushScreen(bool vsync) const
	{
		if (vsync)
			eglSwapBuffers(m_Display, m_Surface);
		else
			glFlush();
	}

	void Platform_Emscripten::PollEvents() const
	{
		
	}

	void Platform_Emscripten::DrawQuad(const Pixel& tint) const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VbQuad);
		Vertex verts[4] = {
			{ { -1.0f, -1.0f, 1.0 }, { 0.0f, 1.0f }, tint },
			{ { +1.0f, -1.0f, 1.0 }, { 1.0f, 1.0f }, tint },
			{ { -1.0f, +1.0f, 1.0 }, { 0.0f, 0.0f }, tint },
			{ { +1.0f, +1.0f, 1.0 }, { 1.0f, 0.0f }, tint },
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4, verts, GL_STREAM_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	void Platform_Emscripten::DrawTexture(const TextureInstance& texInst) const
	{
		BindTexture(texInst.texture ? texInst.texture->id : 0);

		glBindBuffer(GL_ARRAY_BUFFER, m_VbQuad);

		for (uint32_t i = 0; i < texInst.points; i++)
		{
			m_VertexMemory[i].pos[0] = texInst.vertices[i].x;
			m_VertexMemory[i].pos[1] = texInst.vertices[i].y;
			m_VertexMemory[i].pos[2] = 1.0f;

			m_VertexMemory[i].uv.x = texInst.uv[i].x;
			m_VertexMemory[i].uv.y = texInst.uv[i].y;
			m_VertexMemory[i].col = texInst.tint[i];
		}

		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * texInst.points, m_VertexMemory, GL_STREAM_DRAW);

		switch (texInst.structure)
		{
			case Texture::Structure::WIREFRAME: glDrawArrays(GL_LINE_LOOP, 0, texInst.points); break;
			case Texture::Structure::FAN: glDrawArrays(GL_TRIANGLE_FAN, 0, texInst.points); break;
			case Texture::Structure::STRIP: glDrawArrays(GL_TRIANGLE_STRIP, 0, texInst.points); break;
			case Texture::Structure::DEFAULT: glDrawArrays(GL_TRIANGLES, 0, texInst.points); break;
		}
	}

	void Platform_Emscripten::BindTexture(int id) const
	{
		if (id > 0)
			glBindTexture(GL_TEXTURE_2D, id);
		else
			glBindTexture(GL_TEXTURE_2D, m_BlankQuad.texture->id);
	}

	bool Platform_Emscripten::ConstructWindow(vi2d& screenSize, const vi2d pixelSize, vi2d& windowSize, bool vsync, bool fullscreen, bool dirtypixel)
	{
		emscripten_set_canvas_element_size("#canvas", windowSize.x, windowSize.y);

		EGLint const attributes[] = { EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8, EGL_ALPHA_SIZE, 8, EGL_NONE };
		EGLint const configAttributes[] = { EGL_CONTEXT_CLIENT_VERSION , 2, EGL_NONE };
		EGLint configsCount;

		m_Display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

		eglInitialize(m_Display, NULL, NULL);
		eglChooseConfig(m_Display, attributes, &m_Config, 1, &configsCount);

		m_Context = eglCreateContext(m_Display, m_Config, EGL_NO_CONTEXT, configAttributes);
		m_Surface = eglCreateWindowSurface(m_Display, m_Config, NULL, NULL);

		eglMakeCurrent(m_Display, m_Surface, m_Surface, m_Context);
		eglSwapInterval(m_Display, vsync ? 1 : 0);

		m_FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		const GLchar* fragmentShader =
				"#version 300 es\n"
				"precision mediump float;"
				"out vec4 pixel;\n""in vec2 oTex;\n"
				"in vec4 oCol;\n""uniform sampler2D sprTex;\n""void main(){pixel = texture(sprTex, oTex) * oCol;}";

		glShaderSource(m_FragmentShader, 1, &fragmentShader, NULL);
		glCompileShader(m_FragmentShader);

		m_VertexShader = glCreateShader(GL_VERTEX_SHADER);

		const GLchar* vertexShader =
				"#version 300 es\n"
				"precision mediump float;"
				"layout(location = 0) in vec3 aPos;\n""layout(location = 1) in vec2 aTex;\n"
				"layout(location = 2) in vec4 aCol;\n""out vec2 oTex;\n""out vec4 oCol;\n"
				"void main(){ float p = 1.0 / aPos.z; gl_Position = p * vec4(aPos.x, aPos.y, 0.0, 1.0); oTex = p * aTex; oCol = aCol;}";

		glShaderSource(m_VertexShader, 1, &vertexShader, NULL);
		glCompileShader(m_VertexShader);

		m_QuadShader = glCreateProgram();

		glAttachShader(m_QuadShader, m_FragmentShader);
		glAttachShader(m_QuadShader, m_VertexShader);

		glLinkProgram(m_QuadShader);

		glGenBuffers(1, &m_VbQuad);
		glGenVertexArraysOES(1, &m_VaQuad);

		glBindVertexArrayOES(m_VaQuad);
		glBindBuffer(GL_ARRAY_BUFFER, m_VbQuad);

		Vertex vertices[128];
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 128, vertices, GL_STREAM_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)(5 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArrayOES(0);

		m_BlankQuad.Load({ 1, 1 });
		m_BlankQuad.sprite->pixels[0] = WHITE;
		m_BlankQuad.UpdateTexture();

		emscripten_set_keydown_callback("#canvas", 0, 1, KeyboardCallback);
		emscripten_set_keyup_callback("#canvas", 0, 1, KeyboardCallback);

		emscripten_set_wheel_callback("#canvas", 0, 1, WheelCallback);
		emscripten_set_mousedown_callback("#canvas", 0, 1, MouseCallback);

		emscripten_set_mouseup_callback("#canvas", 0, 1, MouseCallback);
		emscripten_set_mousemove_callback("#canvas", 0, 1, MouseCallback);

		emscripten_set_touchstart_callback("#canvas", 0, 1, TouchCallback);
		emscripten_set_touchmove_callback("#canvas", 0, 1, TouchCallback);
		emscripten_set_touchend_callback("#canvas", 0, 1, TouchCallback);

		emscripten_set_blur_callback("#canvas", 0, 1, FocusCallback);
		emscripten_set_focus_callback("#canvas", 0, 1, FocusCallback);

		return true;
	}

	void Platform_Emscripten::SetIcon(Sprite& icon) const
	{

	}

	void Platform_Emscripten::MainLoop()
	{
		GameEngine::s_Engine->MainLoop();
	}

	EM_BOOL Platform_Emscripten::FocusCallback(int eventType, const EmscriptenFocusEvent* event, void* userData)
	{
		switch (eventType)
		{
		case EMSCRIPTEN_EVENT_FOCUS: s_IsWindowFocused = true; break;
		case EMSCRIPTEN_EVENT_BLUR: s_IsWindowFocused = false; break;
		}

		return EM_FALSE;
	}

	EM_BOOL Platform_Emscripten::KeyboardCallback(int eventType, const EmscriptenKeyboardEvent* event, void* userData)
	{
		GameEngine* e = GameEngine::s_Engine;

		switch (eventType)
		{
		case EMSCRIPTEN_EVENT_KEYDOWN: e->m_KeyNewState[size_t(GameEngine::s_KeysTable[emscripten_compute_dom_pk_code(event->code)])] = true; break;
		case EMSCRIPTEN_EVENT_KEYUP: e->m_KeyNewState[size_t(GameEngine::s_KeysTable[emscripten_compute_dom_pk_code(event->code)])] = false; break;
		}

		return EM_TRUE;
	}

	EM_BOOL Platform_Emscripten::WheelCallback(int eventType, const EmscriptenWheelEvent* event, void* userData)
	{
		if (eventType == EMSCRIPTEN_EVENT_WHEEL)
			GameEngine::s_Engine->m_ScrollDelta = -1 * event->deltaY;

		return EM_TRUE;
	}

	EM_BOOL Platform_Emscripten::TouchCallback(int eventType, const EmscriptenTouchEvent* event, void* userData)
	{
		GameEngine* e = GameEngine::s_Engine;

		switch (eventType)
		{
		case EMSCRIPTEN_EVENT_TOUCHMOVE:
		{
			e->m_MousePos.x = event->touches->targetX / e->m_PixelSize.x;
			e->m_MousePos.y = event->touches->targetY / e->m_PixelSize.y;
		}
		break;

		case EMSCRIPTEN_EVENT_TOUCHSTART:
		{
			e->m_MousePos.x = event->touches->targetX;
			e->m_MousePos.y = event->touches->targetY;

			e->m_MouseNewState[0] = true;
		}
		break;

		case EMSCRIPTEN_EVENT_TOUCHEND:
			e->m_MouseNewState[0] = false;
		break;

		}

		return EM_TRUE;
	}

	EM_BOOL Platform_Emscripten::MouseCallback(int eventType, const EmscriptenMouseEvent* event, void* userData)
	{
		GameEngine* e = GameEngine::s_Engine;

		if (eventType == EMSCRIPTEN_EVENT_MOUSEMOVE)
		{
			e->m_MousePos.x = event->targetX / e->m_PixelSize.x;
			e->m_MousePos.y = event->targetY / e->m_PixelSize.y;
		}

		auto check = [&](int button, int index)
		{
			if (event->button == button)
			{
				switch (eventType)
				{
				case EMSCRIPTEN_EVENT_MOUSEDOWN: e->m_MouseNewState[index] = true; break;
				case EMSCRIPTEN_EVENT_MOUSEUP: e->m_MouseNewState[index] = false; break;
				}

				return true;
			}

			return false;
		};

		check(0, 0);
		check(2, 1);
		check(3, 3);
		check(4, 4);

		return check(1, 2) ? EM_TRUE : EM_FALSE;
	}

#endif

	GameEngine::GameEngine()
	{
		m_AppName = "Undefined";
		m_MousePos = { -1, -1 };

		m_DrawTarget = nullptr;

		m_ClearBufferColour = { 255, 255, 255, 255 };
		m_ConsoleBackgroundColour = { 0, 0, 255, 100 };

		m_PixelMode = Pixel::Mode::DEFAULT;
		m_TextureStructure = Texture::Structure::FAN;

		m_CaptureText = false;
		m_Caps = false;
		m_ShowConsole = false;

		m_DeltaTime = 0.0f;
		m_TickTimer = 0.0f;

		m_Shader = nullptr;
		s_Engine = this;

		m_PickedConsoleHistoryCommand = 0;
		m_CursorPos = 0;

		MakeUnitCircle(s_UnitCircle, 64); // TODO: Make 64 (vertices count) as constant

		m_OnlyTextures = false;

#if defined(PLATFORM_GLFW3)
		m_Platform = new Platform_GLFW3();
#elif defined(PLATFORM_EMSCRIPTEN)
		m_Platform = new Platform_Emscripten();
#else
		#error No platform was selected
#endif
	}

	GameEngine::~GameEngine()
	{
		Destroy();
	}

	void GameEngine::Destroy()
	{
		delete m_Screen;
		m_Platform->Destroy();
	}

	void GameEngine::ScanHardware(KeyState* data, bool* newState, bool* oldState, size_t count)
	{
		for (size_t i = 0; i < count; i++)
		{
			data[i].pressed = false;
			data[i].released = false;

			if (newState[i] != oldState[i])
			{
				if (newState[i])
				{
					data[i].pressed = !data[i].held;
					data[i].held = true;
				}
				else
				{
					data[i].released = true;
					data[i].held = false;
				}
			}

			oldState[i] = newState[i];
		}
	}

	void GameEngine::MainLoop()
	{
		if (m_IsAppRunning)
		{
			m_TimeEnd = std::chrono::system_clock::now();

			m_DeltaTime = std::chrono::duration<float>(m_TimeEnd - m_TimeStart).count();
			m_TimeStart = m_TimeEnd;

			m_TickTimer += m_DeltaTime;

			if (m_Platform->IsWindowClose())
				m_IsAppRunning = false;

			ScanHardware(m_Keys, m_KeyNewState, m_KeyOldState, (size_t)Key::KEYS_COUNT);
			ScanHardware(m_Mouse, m_MouseNewState, m_MouseOldState, 8);

			if (m_Keys[(size_t)Key::CAPS_LOCK].pressed)
				m_Caps = !m_Caps;

			if (m_CaptureText)
			{
				bool isUp = m_Keys[(size_t)Key::LEFT_SHIFT].held || m_Keys[(size_t)Key::RIGHT_SHIFT].held;

				for (const auto& [key, chars] : s_KeyboardUS)
				{
					if (GetKey(key).pressed)
					{
						if (m_Caps || isUp)
							m_TextInput.insert(m_CursorPos, 1, chars.second);
						else
							m_TextInput.insert(m_CursorPos, 1, chars.first);

						m_CursorPos++;
					}
				}

				if (m_Keys[(size_t)Key::BACKSPACE].pressed)
				{
					if (m_CursorPos > 0)
					{
						m_TextInput.erase(m_CursorPos - 1, 1);
						m_CursorPos--;
					}
				}

				if (m_Keys[(size_t)Key::DEL].pressed)
				{
					if (m_CursorPos < m_TextInput.length())
						m_TextInput.erase(m_CursorPos, 1);
				}

				if (m_Keys[(size_t)Key::LEFT].pressed)
				{
					if (m_CursorPos > 0)
						m_CursorPos--;
				}

				if (m_Keys[(size_t)Key::RIGHT].pressed)
				{
					if (m_CursorPos < m_TextInput.length())
						m_CursorPos++;
				}

				if (m_Keys[(size_t)Key::ENTER].pressed)
				{
					OnTextCapturingComplete(m_TextInput);

					if (m_ShowConsole)
					{
						std::stringstream output;
						Pixel colour = WHITE;

						if (OnConsoleCommand(m_TextInput, output, colour))
						{
							m_ConsoleHistory.push_back({ m_TextInput, output.str(), colour });
							m_PickedConsoleHistoryCommand = m_ConsoleHistory.size();
						}
					}

					m_TextInput.clear();
					m_CursorPos = 0;
				}

				if (m_ShowConsole)
				{
					if (!m_ConsoleHistory.empty())
					{
						bool moved = false;

						if (m_Keys[(size_t)Key::UP].pressed)
						{
							if (m_PickedConsoleHistoryCommand > 0)
							{
								m_PickedConsoleHistoryCommand--;
								moved = true;
							}
						}

						if (m_Keys[(size_t)Key::DOWN].pressed)
						{
							if (m_PickedConsoleHistoryCommand < m_ConsoleHistory.size() - 1)
							{
								m_PickedConsoleHistoryCommand++;
								moved = true;
							}
						}

						if (moved)
						{
							m_TextInput = m_ConsoleHistory[m_PickedConsoleHistoryCommand].command;
							m_CursorPos = m_TextInput.length();
						}
					}
				}
			}

			if (!OnUserUpdate(m_DeltaTime))
				m_IsAppRunning = false;

			m_ScrollDelta = 0;

			if (m_ShowConsole)
			{
				FillTextureRectangle({ 0, 0 }, m_ScreenSize, m_ConsoleBackgroundColour);

				int printCount = std::min(ScreenHeight() / 22, (int)m_ConsoleHistory.size());
				int start = m_ConsoleHistory.size() - printCount;

				for (size_t i = start; i < m_ConsoleHistory.size(); i++)
				{
					auto& entry = m_ConsoleHistory[i];

					DrawTextureString({ 10, 10 + int(i - start) * 20 }, "> " + entry.command);
					DrawTextureString({ 10, 20 + int(i - start) * 20 }, entry.output, entry.outputColour);
				}

				int x = GetCursorPos() * 8 + 36;
				int y = ScreenHeight() - 18;

				DrawTextureString({ 20, y }, "> " + GetCapturedText(), YELLOW);
				DrawTextureLine({ x, y }, { x, y + 8 }, RED);
			}

			m_Platform->ClearBuffer(m_ClearBufferColour);
			m_Platform->OnBeforeDraw();

			if (!m_OnlyTextures)
			{
				m_DrawTarget->UpdateTexture();

				m_Platform->BindTexture(m_DrawTarget->texture->id);
				m_Platform->DrawQuad(m_ClearBufferColour);
			}

			for (const auto& texture : m_Textures)
				m_Platform->DrawTexture(texture);

			m_Textures.clear();

			if (!OnAfterDraw())
				m_IsAppRunning = false;

			m_Platform->OnAfterDraw();
			m_Platform->FlushScreen(m_IsVSync);
			m_Platform->PollEvents();

#ifndef PLATFORM_EMSCRIPTEN
			m_FramesCount++;

			if (m_TickTimer >= 1.0f)
			{
				m_Platform->SetTitle("github.com/defini7 - defGameEngine - " + m_AppName + " - FPS: " + std::to_string(m_FramesCount));

				m_TickTimer = 0.0f;
				m_FramesCount = 0;
			}
#endif
		}
	}

	void GameEngine::MakeUnitCircle(std::vector<vf2d>& circle, const size_t verts)
	{
		circle.resize(verts);
		float step = 2.0f * 3.14159f / float(verts - 1);

		for (size_t i = 0; i < verts; i++)
		{
			float angle = step * (float)i;

			circle[i].x = cos(angle);
			circle[i].y = sin(angle);
		}
	}

	void GameEngine::Run()
	{
		m_IsAppRunning = OnUserCreate();

		m_TimeStart = std::chrono::system_clock::now();
		m_TimeEnd = m_TimeStart;

		for (size_t i = 0; i < (size_t)Key::KEYS_COUNT; i++)
		{
			m_Keys[i] = { false, false, false };
			m_KeyOldState[i] = false;
			m_KeyNewState[i] = false;
		}

		for (int i = 0; i < 8; i++)
		{
			m_Mouse[i] = { false, false, false };
			m_MouseOldState[i] = false;
			m_MouseNewState[i] = false;
		}

#ifdef PLATFORM_EMSCRIPTEN
		m_Platform->SetTitle("github.com/defini7 - defGameEngine - " + m_AppName);

		emscripten_set_main_loop(&Platform_Emscripten::MainLoop, 0, 1);
#else
		m_Platform->SetTitle("github.com/defini7 - defGameEngine - " + m_AppName + " - FPS: 0");
		m_FramesCount = 0;

		while (m_IsAppRunning)
			MainLoop();
#endif
	}

	bool GameEngine::OnAfterDraw()
	{
		return true;
	}

	void GameEngine::OnTextCapturingComplete(const std::string& text)
	{

	}

	bool GameEngine::OnConsoleCommand(const std::string& command, std::stringstream& output, Pixel& colour)
	{
		return false;
	}

	bool GameEngine::Construct(int screenWidth, int screenHeight, int pixelWidth, int pixelHeight, bool fullScreen, bool vsync, bool dirtyPixel)
	{
		m_ScreenSize = { screenWidth, screenHeight };
		m_PixelSize = { pixelWidth, pixelHeight };
		m_WindowSize = m_ScreenSize * m_PixelSize;

		m_InvScreenSize = { 1.0f / (float)m_ScreenSize.x, 1.0f / (float)m_ScreenSize.y };

		m_IsFullScreen = fullScreen;
		m_IsVSync = vsync;

		m_IsDirtyPixel = dirtyPixel;

		if (!m_Platform->ConstructWindow(m_ScreenSize, m_PixelSize, m_WindowSize, vsync, fullScreen, dirtyPixel))
			return false;

		if (!m_OnlyTextures)
		{
			m_Screen = new Graphic(m_ScreenSize);
			m_DrawTarget = m_Screen;
		}

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

		m_Font.Load({ 128, 48 });

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

				m_Font.sprite->SetPixel(px, py, { k, k, k, k });

				if (++py == 48)
				{
					px++;
					py = 0;
				}
			}
		}

		m_Font.UpdateTexture();

		return true;
	}

	bool GameEngine::Draw(int x, int y, const Pixel& col)
	{
		if (!m_DrawTarget)
			return false;

		Sprite* target = m_DrawTarget->sprite;

		switch (m_PixelMode)
		{
		case Pixel::Mode::CUSTOM:
			return target->SetPixel(x, y, m_Shader({ x, y }, target->GetPixel(x, y), col));

		case Pixel::Mode::DEFAULT:
			return target->SetPixel(x, y, col);

		case Pixel::Mode::MASK:
		{
			if (col.a == 255)
				return target->SetPixel(x, y, col);
		}
		break;

		case Pixel::Mode::ALPHA:
		{
			Pixel d = target->GetPixel(x, y);

			uint8_t r = uint8_t(std::lerp(d.r, col.r, (float)col.a / 255.0f));
			uint8_t g = uint8_t(std::lerp(d.g, col.g, (float)col.a / 255.0f));
			uint8_t b = uint8_t(std::lerp(d.b, col.b, (float)col.a / 255.0f));

			return target->SetPixel(x, y, { r, g, b });
		}

		}

		return false;
	}

	void GameEngine::DrawLine(int x1, int y1, int x2, int y2, const Pixel& col)
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

			Draw(x, y, col);

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

				Draw(x, y, col);
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

			Draw(x, y, col);

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

				Draw(x, y, col);
			}
		}
	}

	void GameEngine::DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const Pixel& col)
	{
		DrawLine(x1, y1, x2, y2, col);
		DrawLine(x2, y2, x3, y3, col);
		DrawLine(x3, y3, x1, y1, col);
	}

	void GameEngine::FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const Pixel& col)
	{
		auto draw_line = [&](int start, int end, int y)
			{
				for (int i = start; i <= end; i++)
					Draw(i, y, col);
			};

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

			draw_line(minx, maxx, y);

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

			draw_line(minx, maxx, y);

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

	void GameEngine::DrawRectangle(int x, int y, int sizeX, int sizeY, const Pixel& col)
	{
		for (int i = 0; i < sizeX - 1; i++)
		{
			Draw(x + i, y, col);
			Draw(x + i, y + sizeY, col);
		}

		for (int i = 0; i < sizeY; i++)
		{
			Draw(x, y + i, col);
			Draw(x + sizeX - 1, y + i, col);
		}

		Draw(x + sizeX - 1, y + sizeY, col);
	}

	void GameEngine::FillRectangle(int x, int y, int sizeX, int sizeY, const Pixel& col)
	{
		for (int i = 0; i < sizeX; i++)
			for (int j = 0; j < sizeY; j++)
				Draw(x + i, y + j, col);
	}

	void GameEngine::DrawCircle(int x, int y, int radius, const Pixel& col)
	{
		int x1 = 0;
		int y1 = radius;
		int p1 = 3 - 2 * radius;

		while (y1 >= x1)
		{
			Draw(x - x1, y - y1, col);
			Draw(x - y1, y - x1, col);
			Draw(x + y1, y - x1, col);
			Draw(x + x1, y - y1, col);
			Draw(x - x1, y + y1, col);
			Draw(x - y1, y + x1, col);
			Draw(x + y1, y + x1, col);
			Draw(x + x1, y + y1, col);

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

	void GameEngine::FillCircle(int x, int y, int radius, const Pixel& col)
	{
		auto draw_line = [&](int start, int end, int y)
			{
				for (int i = start; i <= end; i++)
					Draw(i, y, col);
			};

		int x1 = 0;
		int y1 = radius;
		int p1 = 3 - 2 * radius;

		while (y1 >= x1)
		{
			draw_line(x - x1, x + x1, y - y1);
			draw_line(x - y1, x + y1, y - x1);
			draw_line(x - x1, x + x1, y + y1);
			draw_line(x - y1, x + y1, y + x1);

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

	void GameEngine::DrawEllipse(int x, int y, int sizeX, int sizeY, const Pixel& col)
	{
		int x1 = x + sizeX;
		int y1 = y + sizeY;

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
			Draw(x1, y, col);
			Draw(x, y, col);
			Draw(x, y1, col);
			Draw(x1, y1, col);

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
			Draw(x - 1, y, col);
			Draw(x1 + 1, y++, col);
			Draw(x - 1, y1, col);
			Draw(x1 + 1, y1--, col);
		}
	}

	void GameEngine::FillEllipse(int x, int y, int sizeX, int sizeY, const Pixel& col)
	{
		auto draw_line = [&](int start, int end, int y)
			{
				for (int i = start; i <= end; i++)
					Draw(i, y, col);
			};

		int x1 = x + sizeX;
		int y1 = y + sizeY;

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
			draw_line(x, x1, y);
			draw_line(x, x1, y1);

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
			draw_line(x - 1, x1 + 1, y++);
			draw_line(x - 1, x1 + 1, y1--);
		}
	}

	void GameEngine::DrawSprite(int x, int y, const Sprite* sprite)
	{
		for (int j = 0; j < sprite->size.y; j++)
			for (int i = 0; i < sprite->size.x; i++)
				Draw(x + i, y + j, sprite->GetPixel(i, j));
	}

	void GameEngine::DrawPartialSprite(int x, int y, int fileX, int fileY, int fileSizeX, int fileSizeY, const Sprite* sprite)
	{
		for (int i = 0, x1 = 0; i < fileSizeX; i++, x1++)
			for (int j = 0, y1 = 0; j < fileSizeY; j++, y1++)
				Draw(x + x1, y + y1, sprite->GetPixel(fileX + i, fileY + j));
	}

	void GameEngine::DrawWarpedTexture(const std::vector<vf2d>& points, const Texture* tex, const Pixel& tint)
	{
		TextureInstance texInst;

		texInst.texture = tex;
		texInst.structure = m_TextureStructure;
		texInst.points = 4;
		texInst.tint = { tint, tint, tint, tint };
		texInst.vertices.resize(texInst.points);
		texInst.uv = { { 0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f} };

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
				texInst.uv[i] *= q;
				texInst.vertices[i] = { (points[i].x * m_InvScreenSize.x) * 2.0f - 1.0f, ((points[i].y * m_InvScreenSize.y) * 2.0f - 1.0f) * -1.0f };
			}

			m_Textures.push_back(texInst);
		}
	}

	void GameEngine::DrawWireFrameModel(const std::vector<vf2d>& modelCoordinates, float x, float y, float rotation, float scale, const Pixel& col)
	{
		size_t verts = modelCoordinates.size();

		std::vector<vf2d> coordinates(verts);
		float cs = cosf(rotation), sn = sinf(rotation);

		for (size_t i = 0; i < verts; i++)
		{
			coordinates[i].x = (modelCoordinates[i].x * cs - modelCoordinates[i].y * sn) * scale + x;
			coordinates[i].y = (modelCoordinates[i].x * sn + modelCoordinates[i].y * cs) * scale + y;
		}

		for (size_t i = 0; i <= verts; i++)
			DrawLine(coordinates[i % verts], coordinates[(i + 1) % verts], col);
	}

	void GameEngine::FillWireFrameModel(const std::vector<vf2d>& modelCoordinates, float x, float y, float rotation, float scale, const Pixel& col)
	{
		size_t verts = modelCoordinates.size();

		std::vector<vf2d> coordinates(verts);
		float cs = cosf(rotation), sn = sinf(rotation);

		for (size_t i = 0; i < verts; i++)
		{
			coordinates[i].x = (modelCoordinates[i].x * cs - modelCoordinates[i].y * sn) * scale + x;
			coordinates[i].y = (modelCoordinates[i].x * sn + modelCoordinates[i].y * cs) * scale + y;
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

				for (size_t i = 0; i < verts; i++)
					angle += GetAngle(coordinates[i] - p, coordinates[(i + 1) % verts] - p);

				return std::abs(angle) >= 3.14159f;
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
					Draw(point, col);
			}
	}

	void GameEngine::DrawString(int x, int y, std::string_view s, const Pixel& col, uint32_t scaleX, uint32_t scaleY)
	{
		int sx = 0;
		int sy = 0;

		for (auto c : s)
		{
			if (c == '\n')
			{
				sx = 0;
				sy += 8 * scaleY;
			}
			else if (c == '\t')
				sx += 8 * m_TabSize * scaleX;
			else
			{
				int ox = (c - 32) % 16;
				int oy = (c - 32) / 16;

				if (scaleX > 1 || scaleY > 1)
				{
					for (uint32_t i = 0; i < 8; i++)
						for (uint32_t j = 0; j < 8; j++)
						{
							if (m_Font.sprite->GetPixel(i + ox * 8, j + oy * 8).r > 0)
							{
								for (uint32_t is = 0; is < scaleX; is++)
									for (uint32_t js = 0; js < scaleY; js++)
										Draw(x + sx + i * scaleX + is, y + sy + j * scaleY + js, col);
							}
						}
				}
				else
				{
					for (uint32_t i = 0; i < 8; i++)
						for (uint32_t j = 0; j < 8; j++)
						{
							if (m_Font.sprite->GetPixel(i + ox * 8, j + oy * 8).r > 0)
								Draw(x + sx + i, y + sy + j, col);
						}
				}

				sx += 8 * scaleX;
			}
		}
	}

	void GameEngine::Clear(const Pixel& col)
	{
		m_DrawTarget->sprite->SetPixelData(col);
	}

	KeyState GameEngine::GetKey(Key k) const { return m_Keys[static_cast<size_t>(k)]; }
	KeyState GameEngine::GetMouse(Button k) const { return m_Mouse[static_cast<size_t>(k)]; }

	int GameEngine::GetMouseX() const { return m_MousePos.x; }
	int GameEngine::GetMouseY() const { return m_MousePos.y; }

	int GameEngine::ScreenWidth() const { return m_ScreenSize.x; }
	int GameEngine::ScreenHeight() const { return m_ScreenSize.y; }

	bool GameEngine::IsFullScreen() const { return m_IsFullScreen; }
	bool GameEngine::IsVSync() const { return m_IsVSync; }

	bool GameEngine::IsFocused() const
	{
		return m_Platform->IsWindowFocused();
	}

	void GameEngine::SetIcon(std::string_view fileName)
	{
		Sprite icon(fileName);
		m_Platform->SetIcon(icon);
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

	bool GameEngine::Draw(const vi2d& pos, const Pixel& p)
	{
		return Draw(pos.x, pos.y, p);
	}

	void GameEngine::DrawLine(const vi2d& pos1, const vi2d& pos2, const Pixel& col)
	{
		DrawLine(pos1.x, pos1.y, pos2.x, pos2.y, col);
	}

	void GameEngine::DrawTexturePolygon(const std::vector<vf2d>& verts, const std::vector<Pixel>& cols, Texture::Structure structure)
	{
		TextureInstance texInst;

		texInst.texture = nullptr;
		texInst.points = verts.size();
		texInst.structure = structure;

		texInst.tint.resize(verts.size());

		if (cols.size() > 1)
		{
			std::copy(
				cols.begin(),
				cols.end(),
				texInst.tint.begin());
		}
		else
		{
			std::fill(
				texInst.tint.begin(),
				texInst.tint.end(),
				cols[0]);
		}

		texInst.uv.resize(verts.size());
		texInst.vertices.resize(verts.size());

		for (size_t i = 0; i < verts.size(); i++)
		{
			texInst.vertices[i].x = verts[i].x * m_InvScreenSize.x * 2.0f - 1.0f;
			texInst.vertices[i].y = 1.0f - verts[i].y * m_InvScreenSize.y * 2.0f;
		}

		m_Textures.push_back(texInst);
	}

	void GameEngine::DrawTextureLine(const vi2d& pos1, const vi2d& pos2, const Pixel& col)
	{
		DrawTexturePolygon({ pos1, pos2 }, { col, col }, Texture::Structure::WIREFRAME);
	}

	void GameEngine::DrawTextureTriangle(const vi2d& pos1, const vi2d& pos2, const vi2d& pos3, const Pixel& col)
	{
		DrawTexturePolygon({ pos1, pos2, pos3 }, { col, col, col }, Texture::Structure::WIREFRAME);
	}

	void GameEngine::FillTextureTriangle(const vi2d& pos1, const vi2d& pos2, const vi2d& pos3, const Pixel& col)
	{
		DrawTexturePolygon({ pos1, pos2, pos3 }, { col, col, col }, Texture::Structure::FAN);
	}

	void GameEngine::DrawTextureRectangle(const vi2d& pos, const vi2d& size, const Pixel& col)
	{
		DrawTexturePolygon({ pos, { float(pos.x + size.x), (float)pos.y }, pos + size, { (float)pos.x, float(pos.y + size.y) } }, { col, col, col, col }, Texture::Structure::WIREFRAME);
	}

	void GameEngine::FillTextureRectangle(const vi2d& pos, const vi2d& size, const Pixel& col)
	{
		DrawTexturePolygon({ pos, { float(pos.x + size.x), (float)pos.y }, pos + size, { (float)pos.x, float(pos.y + size.y) } }, { col, col, col, col }, Texture::Structure::FAN);
	}

	void GameEngine::DrawTextureCircle(const vi2d& pos, int radius, const Pixel& col)
	{
		std::vector<vf2d> verts(s_UnitCircle.size());

		for (size_t i = 0; i < verts.size(); i++)
			verts[i] = s_UnitCircle[i] * (float)radius + pos;

		DrawTexturePolygon(verts, { col }, Texture::Structure::WIREFRAME);
	}

	void GameEngine::FillTextureCircle(const vi2d& pos, int radius, const Pixel& col)
	{
		std::vector<vf2d> verts(s_UnitCircle.size());

		for (size_t i = 0; i < verts.size(); i++)
			verts[i] = s_UnitCircle[i] * (float)radius + pos;

		DrawTexturePolygon(verts, { col }, Texture::Structure::FAN);
	}

	void GameEngine::GradientTextureTriangle(const vi2d& pos1, const vi2d& pos2, const vi2d& pos3, const Pixel& col1, const Pixel& col2, const Pixel& col3)
	{
		DrawTexturePolygon({ pos1, pos2, pos3 }, { col1, col2, col3 }, Texture::Structure::FAN);
	}

	void GameEngine::GradientTextureRectangle(const vi2d& pos, const vi2d& size, const Pixel& colTL, const Pixel& colTR, const Pixel& colBR, const Pixel& colBL)
	{
		DrawTexturePolygon({ pos, { float(pos.x + size.x), (float)pos.y }, pos + size, { (float)pos.x, float(pos.y + size.y) } }, { colTL, colTR, colBR, colBL }, Texture::Structure::FAN);
	}

	void GameEngine::DrawTextureString(const vi2d& pos, std::string_view text, const Pixel& col, const vf2d& scale)
	{
		vf2d p = { 0.0f, 0.0f };

		for (auto c : text)
		{
			if (c == '\n')
			{
				p.x = 0;
				p.y += 8.0f * scale.y;
			}
			else if (c == '\t')
			{
				p.x += 8.0f * float(m_TabSize) * scale.x;
			}
			else
			{
				vf2d offset((c - 32) % 16, (c - 32) / 16);

				DrawPartialTexture(pos + p, m_Font.texture, offset * 8.0f, { 8.0f, 8.0f }, scale, col);
				p.x += 8.0f * scale.x;
			}
		}
	}

	void GameEngine::DrawTriangle(const vi2d& pos1, const vi2d& pos2, const vi2d& pos3, const Pixel& col)
	{
		DrawTriangle(pos1.x, pos1.y, pos2.x, pos2.y, pos3.x, pos3.y, col);
	}

	void GameEngine::FillTriangle(const vi2d& pos1, const vi2d& pos2, const vi2d& pos3, const Pixel& col)
	{
		FillTriangle(pos1.x, pos1.y, pos2.x, pos2.y, pos3.x, pos3.y, col);
	}

	void GameEngine::DrawRectangle(const vi2d& pos, const vi2d& size, const Pixel& col)
	{
		DrawRectangle(pos.x, pos.y, size.x, size.y, col);
	}

	void GameEngine::FillRectangle(const vi2d& pos, const vi2d& size, const Pixel& col)
	{
		FillRectangle(pos.x, pos.y, size.x, size.y, col);
	}

	void GameEngine::DrawCircle(const vi2d& pos, int radius, const Pixel& col)
	{
		DrawCircle(pos.x, pos.y, radius, col);
	}

	void GameEngine::FillCircle(const vi2d& pos, int radius, const Pixel& col)
	{
		FillCircle(pos.x, pos.y, radius, col);
	}

	void GameEngine::DrawEllipse(const vi2d& pos, const vi2d& size, const Pixel& col)
	{
		DrawEllipse(pos.x, pos.y, size.x, size.y, col);
	}

	void GameEngine::FillEllipse(const vi2d& pos, const vi2d& size, const Pixel& col)
	{
		FillEllipse(pos.x, pos.y, size.x, size.y, col);
	}

	void GameEngine::DrawSprite(const vi2d& pos, const Sprite* spr)
	{
		DrawSprite(pos.x, pos.y, spr);
	}

	void GameEngine::DrawPartialSprite(const vi2d& pos, const vi2d& filePos, const vi2d& fileSize, const Sprite* spr)
	{
		DrawPartialSprite(pos.x, pos.y, filePos.x, filePos.y, fileSize.x, fileSize.y, spr);
	}

	void GameEngine::DrawTexture(const vf2d& pos, const Texture* tex, const vf2d& scale, const Pixel& tint)
	{
		vf2d pos1 = (pos * m_InvScreenSize * 2.0f - 1.0f) * vf2d(1.0f, -1.0f);
		vf2d pos2 = pos1 + 2.0f * tex->size * m_InvScreenSize * scale * vf2d(1.0f, -1.0f);

		TextureInstance texInst;

		texInst.texture = tex;
		texInst.points = 4;
		texInst.structure = m_TextureStructure;
		texInst.tint = { tint, tint, tint, tint };
		texInst.vertices = { pos1, { pos1.x, pos2.y }, pos2, { pos2.x, pos1.y } };
		

		m_Textures.push_back(texInst);
	}

	void GameEngine::DrawPartialTexture(const vf2d& pos, const Texture* tex, const vf2d& filePos, const vf2d& fileSize, const vf2d& scale, const Pixel& tint)
	{
		vf2d screenPos1 = (pos * m_InvScreenSize * 2.0f - 1.0f) * vf2d(1.0f, -1.0f);
		vf2d screenPos2 = ((pos + fileSize * scale) * m_InvScreenSize * 2.0f - 1.0f) * vf2d(1.0f, -1.0f);

		vf2d quantPos1 = (screenPos1 * vf2d(m_WindowSize) + vf2d(0.5f, 0.5f)).floor() / vf2d(m_WindowSize);
		vf2d quantPos2 = (screenPos2 * vf2d(m_WindowSize) + vf2d(0.5f, -0.5f)).ceil() / vf2d(m_WindowSize);

		vf2d tl = (filePos + 0.0001f) * tex->uvScale;
		vf2d br = (filePos + fileSize - 0.0001f) * tex->uvScale;

		TextureInstance texInst;

		texInst.texture = tex;
		texInst.points = 4;
		texInst.structure = m_TextureStructure;
		texInst.tint = { tint, tint, tint, tint };
		texInst.vertices = { quantPos1, { quantPos1.x, quantPos2.y }, quantPos2, { quantPos2.x, quantPos1.y } };
		texInst.uv = { tl, { tl.x, br.y }, br, { br.x, tl.y } };
		

		m_Textures.push_back(texInst);
	}

	void GameEngine::DrawRotatedTexture(const vf2d& pos, const Texture* tex, float rotation, const vf2d& center, const vf2d& scale, const Pixel& tint)
	{
		TextureInstance texInst;

		texInst.texture = tex;
		texInst.points = 4;
		texInst.structure = m_TextureStructure;
		texInst.tint = { tint, tint, tint, tint };
		

		vf2d denormCenter = center * tex->size;

		texInst.vertices = {
			-denormCenter * scale,
			(vf2d(0.0f, tex->size.y) - denormCenter) * scale,
			(tex->size - denormCenter) * scale,
			(vf2d(tex->size.x, 0.0f) - denormCenter) * scale
		};

		float c = cos(rotation), s = sin(rotation);
		for (size_t i = 0; i < texInst.points; i++)
		{
			vf2d offset =
			{
				texInst.vertices[i].x * c - texInst.vertices[i].y * s,
				texInst.vertices[i].x * s + texInst.vertices[i].y * c
			};
			
			texInst.vertices[i] = pos + offset;
			texInst.vertices[i] = texInst.vertices[i] * m_InvScreenSize * 2.0f - 1.0f;
			texInst.vertices[i].y *= -1.0f;
		}

		m_Textures.push_back(texInst);
	}

	void GameEngine::DrawPartialRotatedTexture(const vf2d& pos, const Texture* tex, const vf2d& filePos, const vf2d& fileSize, float rotation, const vf2d& center, const vf2d& scale, const Pixel& tint)
	{
		TextureInstance texInst;

		texInst.texture = tex;
		texInst.points = 4;
		texInst.structure = m_TextureStructure;
		texInst.tint = { tint, tint, tint, tint };
		

		vf2d denormCenter = center * fileSize;

		texInst.vertices = {
			-denormCenter * scale,
			(vf2d(0.0f, fileSize.y) - denormCenter) * scale,
			(fileSize - denormCenter) * scale,
			(vf2d(fileSize.x, 0.0f) - denormCenter) * scale
		};

		float c = cos(rotation), s = sin(rotation);
		for (size_t i = 0; i < texInst.points; i++)
		{
			vf2d offset =
			{
				texInst.vertices[i].x * c - texInst.vertices[i].y * s,
				texInst.vertices[i].x * s + texInst.vertices[i].y * c
			};

			texInst.vertices[i] = pos + offset;
			texInst.vertices[i] = texInst.vertices[i] * m_InvScreenSize * 2.0f - 1.0f;
			texInst.vertices[i].y *= -1.0f;
		}

		vf2d tl = filePos * tex->uvScale;
		vf2d br = tl + fileSize * tex->uvScale;

		texInst.uv = { tl, { tl.x, br.y }, br, { br.x, tl.y } };

		m_Textures.push_back(texInst);
	}

	void GameEngine::DrawWireFrameModel(const std::vector<vf2d>& modelCoordinates, const vf2d& pos, float rotation, float scale, const Pixel& col)
	{
		DrawWireFrameModel(modelCoordinates, pos.x, pos.y, rotation, scale, col);
	}

	void GameEngine::FillWireFrameModel(const std::vector<vf2d>& modelCoordinates, const vf2d& pos, float rotation, float scale, const Pixel& col)
	{
		FillWireFrameModel(modelCoordinates, pos.x, pos.y, rotation, scale, col);
	}

	void GameEngine::DrawString(const vi2d& pos, std::string_view text, const Pixel& col, const vec2d<uint32_t>& scale)
	{
		DrawString(pos.x, pos.y, text, col, scale.x, scale.y);
	}

	vi2d GameEngine::GetScreenSize() const
	{
		return m_ScreenSize;
	}

	vi2d GameEngine::GetWindowSize() const
	{
		return m_WindowSize;
	}

	vi2d GameEngine::GetMousePos() const
	{
		return m_MousePos;
	}

	int GameEngine::GetMouseWheelDelta() const
	{
		return m_ScrollDelta;
	}

	void GameEngine::ClearTexture(const Pixel& col)
	{
		m_ClearBufferColour = col;
	}

	void GameEngine::SetShader(Pixel (*func)(const vi2d&, const Pixel&, const Pixel&))
	{
		m_Shader = func;
		m_PixelMode = m_Shader ? Pixel::Mode::CUSTOM : Pixel::Mode::DEFAULT;
	}

	void GameEngine::CaptureText(bool enable)
	{
		m_CaptureText = enable;
	}

	std::string GameEngine::GetCapturedText() const
	{
		return m_TextInput;
	}

	size_t GameEngine::GetCursorPos() const
	{
		return m_CursorPos;
	}

	void GameEngine::ClearCapturedText()
	{
		m_TextInput.clear();
		m_CursorPos = 0;
	}

	void GameEngine::ShowConsole(bool enable)
	{
		m_ShowConsole = enable;
		m_CaptureText = enable;
	}

	void GameEngine::SetConsoleBackgroundColour(const Pixel& col)
	{
		m_ConsoleBackgroundColour = col;
	}

	void GameEngine::ClearConsole()
	{
		m_PickedConsoleHistoryCommand = 0;
		m_ConsoleHistory.clear();
	}

	bool GameEngine::IsCapturingText() const
	{
		return m_CaptureText;
	}

	bool GameEngine::IsConsoleEnabled() const
	{
		return m_ShowConsole;
	}

	bool GameEngine::IsCaps() const
	{
		return m_Caps;
	}

	void GameEngine::UseOnlyTextures(bool enable)
	{
		m_OnlyTextures = enable;
	}

	float GameEngine::GetDeltaTime() const
	{
		return m_DeltaTime;
	}

#endif

}

#endif
