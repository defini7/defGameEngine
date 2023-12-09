/* TODO:
* - Enums for keys
*/

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

sol::state lua;

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
void AddVec2D(std::string_view name)
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

	sol::protected_function_result funcResult = luaCreateApp();

	if (!funcResult.valid())
	{
		std::cerr << "[LUA] CreateApp function must return a table" << std::endl;
		return false;
	}

	sol::table init = funcResult;
	int32_t dimensions[4]{ 256, 240, 4, 4 };

	if (init["title"].valid())
		title = init["title"].get<std::string_view>();

	fullScreen = init["full_screen"].valid() && init["full_screen"].get<bool>();
	vsync = init["vsync"].valid() && init["vsync"].get<bool>();
	dirtyPixel = init["dirty_pixel"].valid() && init["dirty_pixel"].get<bool>();

	if (init["dimensions"].valid())
	{
		for (int i = 0; i < 4; i++)
		{
			if (init["dimensions"][i + 1].valid())
				dimensions[i] = init["dimensions"][i + 1];
		}
	}

	screenSize.x = dimensions[0];
	screenSize.y = dimensions[1];

	pixelSize.x = dimensions[2];
	pixelSize.y = dimensions[3];

	return true;
}

void RegisterKeyState()
{
	lua.new_usertype<def::KeyState>("KeyState",
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
	lua["DrawLine"] = [&app = app](const def::vi2d& pos1, const def::vi2d& pos2, const def::Pixel& p) { return app.DrawLine(pos1, pos2, p); };

	lua["DrawTriangle"] = [&app = app](const def::vi2d& pos1, const def::vi2d& pos2, const def::vi2d& pos3, const def::Pixel& p) { return app.DrawTriangle(pos1, pos2, pos3, p); };
	lua["FillTriangle"] = [&app = app](const def::vi2d& pos1, const def::vi2d& pos2, const def::vi2d& pos3, const def::Pixel& p) { return app.FillTriangle(pos1, pos2, pos3, p); };

	lua["DrawRectangle"] = [&app = app](const def::vi2d& pos, const def::vi2d& size, const def::Pixel& p) { return app.DrawRectangle(pos, size, p); };
	lua["FillRectangle"] = [&app = app](const def::vi2d& pos, const def::vi2d& size, const def::Pixel& p) { return app.FillRectangle(pos, size, p); };

	lua["DrawCircle"] = [&app = app](const def::vi2d& pos, int32_t radius, const def::Pixel& p) { return app.DrawCircle(pos, radius, p); };
	lua["FillCircle"] = [&app = app](const def::vi2d& pos, int32_t radius, const def::Pixel& p) { return app.FillCircle(pos, radius, p); };

	lua["DrawEllipse"] = [&app = app](const def::vi2d& pos, const def::vi2d& size, const def::Pixel& p) { return app.DrawEllipse(pos, size, p); };
	lua["FillEllipse"] = [&app = app](const def::vi2d& pos, const def::vi2d& size, const def::Pixel& p) { return app.FillEllipse(pos, size, p); };

	lua["DrawSprite"] = [&app = app](const def::vi2d& pos, const def::Sprite& sprite) { return app.DrawSprite(pos, &sprite); };
	lua["DrawPartialSprite"] = [&app = app](const def::vi2d& pos, const def::vi2d& fpos, const def::vi2d& fsize, const def::Sprite& sprite) { return app.DrawPartialSprite(pos, fpos, fsize, &sprite); };

	lua["DrawTexture"] = [&app = app](const def::vf2d& pos, const def::Texture& tex, const def::vf2d& scale, const def::Pixel& tint) { return app.DrawTexture(pos, &tex, scale, tint); };
	lua["DrawPartialTexture"] = [&app = app](const def::vf2d& pos, const def::vi2d& filePos, const def::vi2d& fileSize, const def::Texture& tex, const def::vf2d& scale, const def::Pixel& tint) { return app.DrawPartialTexture(pos, filePos, fileSize, &tex, scale, tint); };

	lua["DrawWarpedTexture"] = [&app = app](const std::vector<def::vf2d>& points, const def::Texture& tex, const def::Pixel& tint) { return app.DrawWarpedTexture(points, &tex, tint); };

	lua["DrawRotatedTexture"] = [&app = app](const def::vf2d& pos, float r, const def::Texture& tex, const def::vf2d& center, const def::vf2d& scale, const def::Pixel& tint) { return app.DrawRotatedTexture(pos, r, &tex, center, scale, tint); };
	lua["DrawRotatedPartialTexture"] = [&app = app](const def::vf2d& pos, const def::vi2d& filePos, const def::vi2d& fileSize, float r, const def::Texture& tex, const def::vf2d& center, const def::vf2d& scale, const def::Pixel& tint) { return app.DrawPartialRotatedTexture(pos, filePos, fileSize, r, &tex, center, scale, tint); };

	lua["DrawWireFrameModel"] = [&app = app](const std::vector<def::vf2d>& modelCoordinates, const def::vf2d& pos, float r, float s, const def::Pixel& p) { return app.DrawWireFrameModel(modelCoordinates, pos, r, s, p); };
	lua["FillWireFrameModel"] = [&app = app](const std::vector<def::vf2d>& modelCoordinates, const def::vf2d& pos, float r, float s, const def::Pixel& p) { return app.FillWireFrameModel(modelCoordinates, pos, r, s, p); };

	lua["DrawString"] = [&app = app](const def::vi2d& pos, std::string_view text, const def::Pixel& p) { return app.DrawString(pos, text, p); };
	lua["Clear"] = [&app = app](const def::Pixel& p) { return app.Clear(p); };

	lua["GetKey"] = [&app = app](uint32_t k) { return app.GetKey(k); };
	lua["GetMouse"] = [&app = app](uint32_t k) { return app.GetMouse(k); };

	lua["MousePos"] = [&app = app]() { return app.GetMouse(); };
	lua["MouseX"] = [&app = app]() { return app.MouseX(); };
	lua["MouseY"] = [&app = app]() { return app.MouseY(); };

	lua["SetTitle"] = [&app = app](std::string_view title) { return app.SetTitle(title); };

	lua["ScreenSize"] = [&app = app]() { return app.ScreenSize(); };
	lua["GetMaxWindowSize"] = [&app = app]() { return app.MaxWindowSize(); };

	lua["ScreenWidth"] = [&app = app]() { return app.ScreenWidth(); };
	lua["ScreenHeight"] = [&app = app]() { return app.ScreenHeight(); };

	lua["IsFullScreen"] = [&app = app]() { return app.IsFullScreen(); };
	lua["IsVSync"] = [&app = app]() { return app.IsVSync(); };
	lua["IsFocused"] = [&app = app]() { return app.IsFocused(); };
	lua["SetIcon"] = [&app = app](std::string_view fileName) { return app.SetIcon(fileName); };

	lua["SetDrawTarget"] = [&app = app](def::Graphic* target) { return app.SetDrawTarget(target); };
	lua["GetDrawTarget"] = [&app = app]() { return app.GetDrawTarget(); };

	lua["GetWindowState"] = [&app = app]() { return app.GetWindowState(); };

	lua["GetDropped"] = []() { return Application::GetDropped(); };

	lua["SetPixelMode"] = [&app = app](def::Pixel::Mode pixelMode) { app.SetPixelMode(pixelMode); };
	lua["GetPixelMode"] = [&app = app]() { return app.GetPixelMode(); };

	lua["SetTextureStructure"] = [&app = app](def::Texture::Structure structure) { app.SetTextureStructure(structure); };
	lua["GetTextureStructure"] = [&app = app]() { return app.GetTextureStructure(); };

	lua["SetTint"] = [&app = app](const def::Pixel& p) { app.SetTint(p); };
	lua["SetShader"] = [&app = app](def::Pixel(*func)(const def::vi2d&, const def::Pixel&, const def::Pixel&)) { app.SetShader(func); };

	lua["AnyKey"] = [&app = app](bool pressed, bool held, bool released) { return app.AnyKey(pressed, held, released); };
	lua["AnyKeys"] = [&app = app](bool pressed, bool held, bool released) { return app.AnyKeys(pressed, held, released); };
}

void RegisterAll(Application& app)
{
	AddVec2D<def::vi2d>("vi2d");
	AddVec2D<def::vf2d>("vf2d");

	RegisterKeyState();
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

	if (!title.empty())
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

	if (!lua.script_file(argv[1]).valid())
		return 1;

	Application app;
	
	RegisterAll(app);
	return RunApplication(app) ? 0 : 1;
}
