#include "defGameEngine.h"

class Lerp : public def::GameEngine
{
public:
	Lerp()
	{
		SetTitle("Lerp");
	}

protected:
	bool OnUserCreate() override
	{
		return true;
	}

	bool OnUserUpdate(float fDeltaTime) override
	{
		float t = 0.0f;
		for (int x = 0; x < 84; x += 8)
		{
			FillRectangle(x + 20, 20 + 8 * 0, 8, 8, def::BLUE * t);
			FillRectangle(x + 20, 20 + 8 * 1, 8, 8, def::CYAN * t);
			FillRectangle(x + 20, 20 + 8 * 2, 8, 8, def::DARK_BLUE * t);
			FillRectangle(x + 20, 20 + 8 * 3, 8, 8, def::DARK_CYAN * t);
			FillRectangle(x + 20, 20 + 8 * 4, 8, 8, def::DARK_GREEN * t);
			FillRectangle(x + 20, 20 + 8 * 5, 8, 8, def::DARK_GREY * t);
			FillRectangle(x + 20, 20 + 8 * 6, 8, 8, def::DARK_MAGENTA * t);
			FillRectangle(x + 20, 20 + 8 * 7, 8, 8, def::DARK_RED * t);
			FillRectangle(x + 20, 20 + 8 * 8, 8, 8, def::ORANGE * t);
			FillRectangle(x + 20, 20 + 8 * 9, 8, 8, def::GREEN * t);
			FillRectangle(x + 20, 20 + 8 * 10, 8, 8, def::GREY * t);
			FillRectangle(x + 20, 20 + 8 * 11, 8, 8, def::MAGENTA * t);
			FillRectangle(x + 20, 20 + 8 * 12, 8, 8, def::RED * t);
			FillRectangle(x + 20, 20 + 8 * 13, 8, 8, def::WHITE * t);
			FillRectangle(x + 20, 20 + 8 * 14, 8, 8, def::YELLOW * t);
			t += 0.1f;
		}

		return true;
	}

};

int main(int argc, char* argv[])
{
	Lerp demo;
	def::rcode err = demo.Construct(256, 240, 4, 4);

	if (err.ok)
		demo.Run();
	else
		std::cerr << err.info << "\n";

	return 0;
}
