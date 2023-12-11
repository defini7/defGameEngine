#define DGE_APPLICATION
#include "defGameEngine.hpp"

#define SOL_ALL_SAFETIES_ON 1
#include "sol/sol.hpp"

#ifdef _WIN32
#pragma comment(lib, "lua54.lib")
#endif

int g_IotaCount = 0;

int Iota(bool resetAtStart = false)
{
	if (resetAtStart)
		g_IotaCount = 0;

	return g_IotaCount++;
}

void LuaPanic(std::optional<std::string> message)
{
	std::cerr << "[LUA] Panic:\n";

	if (message)
		std::cerr << message.value() << std::endl;
}

sol::state lua(sol::c_call<decltype(&LuaPanic), &LuaPanic>);

class Application : public def::GameEngine
{
public:
	Application()
	{

	}

protected:
	bool OnUserCreate() override
	{
		if (!lua["OnUserCreate"].valid())
		{
			std::cerr << "[LUA] OnUserCreate function must be provided" << std::endl;
			return false;
		}

		if (!lua["OnUserUpdate"].valid())
		{
			std::cerr << "[LUA] OnUserUpdate function must be provided" << std::endl;
			return false;
		}

		return lua["OnUserCreate"]();
	}

	bool OnUserUpdate(float deltaTime) override
	{
		return lua["OnUserUpdate"](deltaTime);
	}
};

template <class T>
void RegisterVec2D(std::string_view name)
{
	auto vecType = lua.new_usertype<T>(name,
		sol::constructors<T(), T(decltype(T::x), decltype(T::y))>(),
		"x", sol::property(&T::x, &T::x),
		"y", sol::property(&T::y, &T::y)
	);

	vecType["clamp"] = &T::clamp;
	vecType["dot"] = &T::dot;
	vecType["length"] = &T::length;
	vecType["mag"] = &T::mag;
	vecType["mag2"] = &T::mag2;
	vecType["man"] = &T::man;
	vecType["min"] = &T::max;
	vecType["max"] = &T::min;
	vecType["swap"] = &T::swap;
	vecType["norm"] = &T::norm;
	vecType["abs"] = &T::abs;
	vecType["perp"] = &T::perp;
	vecType["floor"] = &T::floor;
	vecType["ceil"] = &T::ceil;
	vecType["round"] = &T::round;
	vecType["cart"] = &T::cart;
	vecType["polar"] = &T::polar;
	vecType["str"] = &T::str;
}

bool CreateApp(def::vi2d& screenSize, def::vi2d& pixelSize, std::string& title, bool& fullScreen, bool& vsync, bool& dirtyPixel)
{
	sol::function luaCreateApp = lua["CreateApp"];

	if (!luaCreateApp.valid())
	{
		std::cerr << "[LUA] CreateApp function must be provided" << std::endl;
		return false;
	}

	auto funcResult = luaCreateApp();

	if (funcResult.get_type() != sol::type::table)
	{
		std::cerr << "[LUA] CreateApp function must return a table" << std::endl;
		return false;
	}

	sol::table init = funcResult;

	title = init["title"].get_or<std::string_view>("Undefined");

	if (init["size"].valid())
	{
		screenSize.x = init["size"][1].get_or(256);
		screenSize.y = init["size"][2].get_or(240);

		pixelSize.x = init["size"][3].get_or(4);
		pixelSize.y = init["size"][4].get_or(4);
	}
	else
	{
		screenSize = { 256, 240 };
		pixelSize = { 4, 4 };
	}

	fullScreen = init["full_screen"].get_or(false);
	vsync = init["vsync"].get_or(false);
	dirtyPixel = init["dirty_pixel"].get_or(false);

	return true;
}

void RegisterKeyState()
{
	auto keyType = lua.new_usertype<def::KeyState>("KeyState",
		sol::constructors<def::KeyState(), def::KeyState(bool, bool, bool)>(),
		"held", sol::property(&def::KeyState::held, &def::KeyState::held),
		"released", sol::property(&def::KeyState::released, &def::KeyState::released),
		"pressed", sol::property(&def::KeyState::pressed, &def::KeyState::pressed)
	);
}

void RegisterPixel()
{
	auto pixelType = lua.new_usertype<def::Pixel>("Pixel",
		sol::constructors<def::Pixel(), def::Pixel(uint8_t, uint8_t, uint8_t, uint8_t)>(),
		"r", sol::property(&def::Pixel::r, &def::Pixel::r),
		"g", sol::property(&def::Pixel::g, &def::Pixel::g),
		"b", sol::property(&def::Pixel::b, &def::Pixel::b),
		"a", sol::property(&def::Pixel::a, &def::Pixel::a)
	);

	pixelType["Mode"] = lua.create_table_with(
		"DEFAULT", Iota(true), "ALPHA", Iota(), "MASK", Iota(), "CUSTOM", Iota()
	);

	pixelType["mix"] = &def::Pixel::mix;
	pixelType["str"] = &def::Pixel::str;

	lua["Colors"] = lua.create_table_with(
		"BLACK", def::BLACK,
		"DARK_BLUE", def::DARK_BLUE,
		"DARK_GREEN", def::DARK_GREEN,
		"DARK_CYAN", def::DARK_CYAN,
		"DARK_RED", def::DARK_RED,
		"DARK_MAGENTA", def::DARK_MAGENTA,
		"DARK_GREY", def::DARK_GREY,
		"DARK_ORANGE", def::DARK_ORANGE,
		"DARK_BROWN", def::DARK_BROWN,
		"DARK_PURPLE", def::DARK_PURPLE,
		"ORANGE", def::ORANGE,
		"GREY", def::GREY,
		"BLUE", def::BLUE,
		"GREEN", def::GREEN,
		"CYAN", def::CYAN,
		"RED", def::RED,
		"MAGENTA", def::MAGENTA,
		"YELLOW", def::YELLOW,
		"WHITE", def::WHITE,
		"GOLD", def::GOLD,
		"PINK", def::PINK,
		"MAROON", def::MAROON,
		"LIME", def::LIME,
		"BROWN", def::BROWN,
		"BEIGE", def::BEIGE,
		"VIOLET", def::VIOLET,
		"PURPLE", def::PURPLE,
		"NONE", def::NONE
	);

	lua["PixelF"] = &def::PixelF;
	lua["RandomPixel"] = &def::RandomPixel;
}

void RegisterSprite()
{
	auto spriteType = lua.new_usertype<def::Sprite>("Sprite",
		sol::constructors<def::Sprite(), def::Sprite(const def::vi2d&), def::Sprite(std::string_view)>(),
		"size", sol::property(&def::Sprite::size, &def::Sprite::size),
		"pixels", sol::property(&def::Sprite::pixels, &def::Sprite::pixels)
	);

	spriteType["Create"] = &def::Sprite::Create;
	spriteType["Load"] = &def::Sprite::Load;
	spriteType["Save"] = &def::Sprite::Save;
	spriteType["SetPixel"] = &def::Sprite::SetPixel;
	spriteType["GetPixel"] = &def::Sprite::GetPixel;
	spriteType["SetPixelData"] = &def::Sprite::SetPixelData;
	spriteType["Sample"] = &def::Sprite::Sample;

	spriteType["FileType"] = lua.create_table_with(
		"BMP", Iota(true), "PNG", Iota(), "JPG", Iota(),
		"TGA", Iota(), "TGA_RLE", Iota()
	);

	spriteType["SampleMethod"] = lua.create_table_with(
		"LINEAR", Iota(true), "BILINEAR", Iota(), "TRILINEAR", Iota()
	);

	spriteType["WrapMethod"] = lua.create_table_with(
		"NONE", Iota(true), "REPEAT", Iota(), "MIRROR", Iota(), "CLAMP", Iota()
	);
}

void RegisterTexture()
{
	auto textureType = lua.new_usertype<def::Texture>("Texture",
		sol::constructors<def::Texture(def::Sprite*), def::Texture(std::string_view)>(),
		"id", sol::property(&def::Texture::id, &def::Texture::id),
		"uv_scale", sol::property(&def::Texture::uvScale, &def::Texture::uvScale),
		"size", sol::property(&def::Texture::size, &def::Texture::size)
	);

	textureType["Load"] = &def::Texture::Load;
	textureType["Update"] = &def::Texture::Update;

	textureType["Structure"] = lua.create_table_with(
		"DEFAULT", Iota(true), "FAN", Iota(), "STRIP", Iota()
	);
}

void RegisterGraphic()
{
	auto graphicType = lua.new_usertype<def::Graphic>("Graphic",
		sol::constructors<def::Graphic(), def::Graphic(std::string_view), def::Graphic(const def::vi2d&)>(),
		"texture", sol::property(&def::Graphic::texture, &def::Graphic::texture),
		"sprite", sol::property(&def::Graphic::sprite, &def::Graphic::sprite)
	);

	graphicType["Load"] = [](def::Graphic* self, std::string_view fileName) { self->Load(fileName); };
	graphicType["Save"] = &def::Graphic::Save;
	graphicType["UpdateTexture"] = &def::Graphic::UpdateTexture;
}

void RegisterWindowState()
{
	lua["WindowState"] = lua.create_table_with(
		"NONE", Iota(true), "MAXIMIZED", Iota(), "FOCUSED", Iota()
	);
}

void RegisterFunctions(Application& app)
{
	lua["Draw"] = [&app = app](const def::vi2d& pos, const def::Pixel& p) { return app.Draw(pos, p); };
	lua["DrawLine"] = [&app = app](const def::vi2d& pos1, const def::vi2d& pos2, const def::Pixel& p) { app.DrawLine(pos1, pos2, p); };

	lua["DrawTriangle"] = [&app = app](const def::vi2d& pos1, const def::vi2d& pos2, const def::vi2d& pos3, const def::Pixel& p) { app.DrawTriangle(pos1, pos2, pos3, p); };
	lua["FillTriangle"] = [&app = app](const def::vi2d& pos1, const def::vi2d& pos2, const def::vi2d& pos3, const def::Pixel& p) { app.FillTriangle(pos1, pos2, pos3, p); };

	lua["DrawRectangle"] = [&app = app](const def::vi2d& pos, const def::vi2d& size, const def::Pixel& p) { app.DrawRectangle(pos, size, p); };
	lua["FillRectangle"] = [&app = app](const def::vi2d& pos, const def::vi2d& size, const def::Pixel& p) { app.FillRectangle(pos, size, p); };

	lua["DrawCircle"] = [&app = app](const def::vi2d& pos, int32_t radius, const def::Pixel& p) { app.DrawCircle(pos, radius, p); };
	lua["FillCircle"] = [&app = app](const def::vi2d& pos, int32_t radius, const def::Pixel& p) { app.FillCircle(pos, radius, p); };

	lua["DrawEllipse"] = [&app = app](const def::vi2d& pos, const def::vi2d& size, const def::Pixel& p) { app.DrawEllipse(pos, size, p); };
	lua["FillEllipse"] = [&app = app](const def::vi2d& pos, const def::vi2d& size, const def::Pixel& p) { app.FillEllipse(pos, size, p); };

	lua["DrawSprite"] = [&app = app](const def::vi2d& pos, const def::Sprite& sprite) { app.DrawSprite(pos, &sprite); };
	lua["DrawPartialSprite"] = [&app = app](const def::vi2d& pos, const def::vi2d& fpos, const def::vi2d& fsize, const def::Sprite& sprite) { app.DrawPartialSprite(pos, fpos, fsize, &sprite); };

	lua["DrawTexture"] = [&app = app](const def::vf2d& pos, const def::Texture& tex, const def::vf2d& scale, const def::Pixel& tint) { app.DrawTexture(pos, &tex, scale, tint); };
	lua["DrawPartialTexture"] = [&app = app](const def::vf2d& pos, const def::vi2d& filePos, const def::vi2d& fileSize, const def::Texture& tex, const def::vf2d& scale, const def::Pixel& tint) { app.DrawPartialTexture(pos, filePos, fileSize, &tex, scale, tint); };

	lua["DrawWarpedTexture"] = [&app = app](const std::vector<def::vf2d>& points, const def::Texture& tex, const def::Pixel& tint) { app.DrawWarpedTexture(points, &tex, tint); };

	lua["DrawRotatedTexture"] = [&app = app](const def::vf2d& pos, float r, const def::Texture& tex, const def::vf2d& center, const def::vf2d& scale, const def::Pixel& tint) { app.DrawRotatedTexture(pos, r, &tex, center, scale, tint); };
	lua["DrawRotatedPartialTexture"] = [&app = app](const def::vf2d& pos, const def::vi2d& filePos, const def::vi2d& fileSize, float r, const def::Texture& tex, const def::vf2d& center, const def::vf2d& scale, const def::Pixel& tint) { app.DrawPartialRotatedTexture(pos, filePos, fileSize, r, &tex, center, scale, tint); };

	lua["DrawWireFrameModel"] = [&app = app](sol::table modelCoordinates, const def::vf2d& pos, float r, float s, const def::Pixel& p)
		{
			std::vector<def::vf2d> coords;

			for (const auto& [_, coord] : modelCoordinates)
				coords.push_back(coord.as<def::vf2d>());

			app.DrawWireFrameModel(coords, pos, r, s, p);
		};

	lua["FillWireFrameModel"] = [&app = app](sol::table modelCoordinates, const def::vf2d& pos, float r, float s, const def::Pixel& p)
		{
			std::vector<def::vf2d> coords;

			for (const auto& [_, coord] : modelCoordinates)
				coords.push_back(coord.as<def::vf2d>());

			app.FillWireFrameModel(coords, pos, r, s, p);
		};

	lua["DrawString"] = [&app = app](const def::vi2d& pos, const char* text, const def::Pixel& p) { app.DrawString(pos, text, p); };
	lua["Clear"] = [&app = app](const def::Pixel& p) { app.Clear(p); };

	lua["GetKey"] = [&app = app](uint32_t k) { return app.GetKey(k); };
	lua["GetMouse"] = [&app = app](uint32_t k) { return app.GetMouse(k); };

	lua["MousePos"] = [&app = app]() { return app.GetMouse(); };
	lua["MouseX"] = [&app = app]() { return app.MouseX(); };
	lua["MouseY"] = [&app = app]() { return app.MouseY(); };

	lua["SetTitle"] = [&app = app](const char* title) { app.SetTitle(title); };

	lua["ScreenSize"] = [&app = app]() { return app.ScreenSize(); };
	lua["GetMaxWindowSize"] = [&app = app]() { return app.MaxWindowSize(); };

	lua["ScreenWidth"] = [&app = app]() { return app.ScreenWidth(); };
	lua["ScreenHeight"] = [&app = app]() { return app.ScreenHeight(); };

	lua["IsFullScreen"] = [&app = app]() { return app.IsFullScreen(); };
	lua["IsVSync"] = [&app = app]() { return app.IsVSync(); };
	lua["IsFocused"] = [&app = app]() { return app.IsFocused(); };
	lua["SetIcon"] = [&app = app](const char* fileName) { app.SetIcon(fileName); };

	lua["SetDrawTarget"] = [&app = app](def::Graphic* target) { app.SetDrawTarget(target); };
	lua["GetDrawTarget"] = [&app = app]() { return app.GetDrawTarget(); };

	lua["GetWindowState"] = [&app = app]() { return (uint32_t)app.GetWindowState(); };

	lua["GetDropped"] = []() { return Application::GetDropped(); };

	lua["SetPixelMode"] = [&app = app](def::Pixel::Mode pixelMode) { app.SetPixelMode(pixelMode); };
	lua["GetPixelMode"] = [&app = app]() { return (uint32_t)app.GetPixelMode(); };

	lua["SetTextureStructure"] = [&app = app](def::Texture::Structure structure) { app.SetTextureStructure(structure); };
	lua["GetTextureStructure"] = [&app = app]() { return (uint32_t)app.GetTextureStructure(); };

	lua["SetTint"] = [&app = app](const def::Pixel& p) { app.SetTint(p); };
	lua["SetShader"] = [&app = app](def::Pixel(*func)(const def::vi2d&, const def::Pixel&, const def::Pixel&)) { app.SetShader(func); };

	lua["AnyKey"] = [&app = app](bool pressed, bool held, bool released) { return uint32_t(app.AnyKey(pressed, held, released)); };
	lua["AnyKeys"] = [&app = app](bool pressed, bool held, bool released)
		{
			sol::table res;

			auto keys = app.AnyKeys(pressed, held, released);

			for (const auto& key : keys)
				res.add(uint32_t(key));

			return res;
		};
}

void RegisterKeys()
{
	lua["Keys"] = lua.create_table_with(
		"SPACE", def::Key::SPACE,
		"APOSTROPHE", def::Key::APOSTROPHE,
		"COMMA", def::Key::COMMA,
		"MINUS", def::Key::MINUS,
		"PERIOD", def::Key::PERIOD,
		"SLASH", def::Key::SLASH,

		"K0", def::Key::K0,
		"K1", def::Key::K1,
		"K2", def::Key::K2,
		"K3", def::Key::K3,
		"K4", def::Key::K4,
		"K5", def::Key::K5,
		"K6", def::Key::K6,
		"K7", def::Key::K7,
		"K8", def::Key::K8,
		"K9", def::Key::K9,

		"SEMICOLON", def::Key::SEMICOLON,
		"EQUAL", def::Key::EQUAL,

		"A", def::Key::A,
		"B", def::Key::B,
		"C", def::Key::C,
		"D", def::Key::D,
		"E", def::Key::E,
		"F", def::Key::F,
		"G", def::Key::G,
		"H", def::Key::H,
		"I", def::Key::I,
		"J", def::Key::J,
		"K", def::Key::K,
		"L", def::Key::L,
		"M", def::Key::M,
		"N", def::Key::N,
		"O", def::Key::O,
		"P", def::Key::P,
		"Q", def::Key::Q,
		"R", def::Key::R,
		"S", def::Key::S,
		"T", def::Key::T,
		"U", def::Key::U,
		"V", def::Key::V,
		"W", def::Key::W,
		"X", def::Key::X,
		"Y", def::Key::Y,
		"Z", def::Key::Z,

		"LEFT_BRACKET", def::Key::LEFT_BRACKET,
		"BACKSLASH", def::Key::BACKSLASH,
		"RIGHT_BRACKET", def::Key::RIGHT_BRACKET,
		"GRAVE_ACCENT", def::Key::GRAVE_ACCENT,
		"WORLD_1", def::Key::WORLD_1,
		"WORLD_2", def::Key::WORLD_2,

		"ESCAPE", def::Key::ESCAPE,
		"ENTER", def::Key::ENTER,
		"TAB", def::Key::TAB,
		"BACKSPACE", def::Key::BACKSPACE,
		"INSERT", def::Key::INSERT,
		"DEL", def::Key::DEL,
		"RIGHT", def::Key::RIGHT,
		"LEFT", def::Key::LEFT,
		"DOWN", def::Key::DOWN,
		"UP", def::Key::UP,
		"PAGE_UP", def::Key::PAGE_UP,
		"PAGE_DOWN", def::Key::PAGE_DOWN,
		"HOME", def::Key::HOME,
		"END", def::Key::END,
		"CAPS_LOCK", def::Key::CAPS_LOCK,
		"SCROLL_LOCK", def::Key::SCROLL_LOCK,
		"NUM_LOCK", def::Key::NUM_LOCK,
		"PRINT_SCREEN", def::Key::PRINT_SCREEN,
		"PAUSE", def::Key::PAUSE,

		"F1", def::Key::F1,
		"F2", def::Key::F2,
		"F3", def::Key::F3,
		"F4", def::Key::F4,
		"F5", def::Key::F5,
		"F6", def::Key::F6,
		"F7", def::Key::F7,
		"F8", def::Key::F8,
		"F9", def::Key::F9,
		"F10", def::Key::F10,
		"F11", def::Key::F11,
		"F12", def::Key::F12,
		"F13", def::Key::F13,
		"F14", def::Key::F14,
		"F15", def::Key::F15,
		"F16", def::Key::F16,
		"F17", def::Key::F17,
		"F18", def::Key::F18,
		"F19", def::Key::F19,
		"F20", def::Key::F20,
		"F21", def::Key::F21,
		"F22", def::Key::F22,
		"F23", def::Key::F23,
		"F24", def::Key::F24,
		"F25", def::Key::F25,

		"NP_0", def::Key::NP_0,
		"NP_1", def::Key::NP_1,
		"NP_2", def::Key::NP_2,
		"NP_3", def::Key::NP_3,
		"NP_4", def::Key::NP_4,
		"NP_5", def::Key::NP_5,
		"NP_6", def::Key::NP_6,
		"NP_7", def::Key::NP_7,
		"NP_8", def::Key::NP_8,
		"NP_9", def::Key::NP_9,
		"NP_DECIMAL", def::Key::NP_DECIMAL,
		"NP_DIVIDE", def::Key::NP_DIVIDE,
		"NP_MULTIPLY", def::Key::NP_MULTIPLY,
		"NP_SUBTRACT", def::Key::NP_SUBTRACT,
		"NP_ADD", def::Key::NP_ADD,
		"NP_ENTER", def::Key::NP_ENTER,
		"NP_EQUAL", def::Key::NP_EQUAL,

		"LEFT_SHIFT", def::Key::LEFT_SHIFT,
		"LEFT_CONTROL", def::Key::LEFT_CONTROL,
		"LEFT_ALT", def::Key::LEFT_ALT,
		"LEFT_SUPER", def::Key::LEFT_SUPER,
		"RIGHT_SHIFT", def::Key::RIGHT_SHIFT,
		"RIGHT_CONTROL", def::Key::RIGHT_CONTROL,
		"RIGHT_ALT", def::Key::RIGHT_ALT,
		"RIGHT_SUPER", def::Key::RIGHT_SUPER,
		"MENU", def::Key::MENU,

		"NONE", def::Key::NONE
	);
}

void RegisterButtons()
{
	lua["Mouse"] = lua.create_table_with(
		"LEFT", def::Button::LEFT,
		"RIGHT", def::Button::RIGHT,
		"WHEEL", def::Button::WHEEL,
		"MOUSE4", def::Button::MOUSE4,
		"MOUSE5", def::Button::MOUSE5
	);
}

void RegisterAll(Application& app)
{
	RegisterVec2D<def::vi2d>("vi2d");
	RegisterVec2D<def::vf2d>("vf2d");

	RegisterKeyState();
	RegisterKeys();
	RegisterButtons();
	RegisterPixel();
	RegisterSprite();
	RegisterTexture();
	RegisterGraphic();
	RegisterWindowState();
	RegisterFunctions(app);
}

bool RunApplication(Application& app)
{
	def::vi2d screenSize, pixelSize;
	std::string title;

	bool fullScreen, vsync, dirtyPixel;

	if (!CreateApp(screenSize, pixelSize, title, fullScreen, vsync, dirtyPixel))
		return false;

	app.SetTitle(title);

	app.Construct(screenSize.x, screenSize.y, pixelSize.x, pixelSize.y, fullScreen, vsync, dirtyPixel);
	app.Run();

	return true;
}

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cerr << "[C++] No filename was provided" << std::endl;
		return 1;
	}

	lua.open_libraries();

	Application app;
	RegisterAll(app);

	if (!lua.script_file("Lua/Helpers.lua").valid())
		return 1;

	if (!lua.script_file(argv[1]).valid())
		return 1;

	return RunApplication(app) ? EXIT_SUCCESS : EXIT_FAILURE;
}
