#define DGE_APPLICATION
#include "defGameEngine.hpp"

#include <array>

class Jackpot : public def::GameEngine
{
public:
	Jackpot()
	{
		SetTitle("Jackpot");
	}

	def::vi2d machinePos = { 30, 60 };
	def::vi2d machineCellSize = { 16, 16 };

	enum CellType : uint8_t
	{
		RED,
		BLUE,
		GREEN
	};

	template <size_t itemsPerCol>
	struct Line
	{
		float offset = 0.0f;
		float maxOffset = 0.0f;
		std::array<uint8_t, itemsPerCol> content;
	};

	std::vector<Line<3>> lines;

	def::Graphic* field;

	int yBorder = 0;
	int score = 0;

protected:
	int RandInt(int rmin, int rmax)
	{
		return rmin + rand() % (rmax - rmin);
	}

	bool OnUserCreate() override
	{
		srand(time(0));

		lines.resize(3);
		for (auto& l : lines)
		{
			l.offset = (float)RandInt(0, 3);
			l.maxOffset = (float)RandInt(15, 20);
			for (int i = 0; i < l.content.size(); i++) l.content[i] = i;
		}

		field = new def::Graphic(3 * machineCellSize.x + 1, 3 * machineCellSize.y + 1);

		return true;
	}

	bool OnUserUpdate(float dt) override
	{
		// Spin wheels again if they stopped
		if (lines.back().offset >= lines.back().maxOffset)
		{
			if (GetKey(def::Key::SPACE).pressed)
			{
				for (auto& l : lines)
				{
					l.offset = float((int)l.offset % l.content.size());
					l.maxOffset = (float)RandInt(15, 20);
				}
			}
			else
				return true;
		}

		for (auto& l : lines)
		{
			// Update current line offset
			// until it reaches maximum
			if (l.offset <= l.maxOffset)
			{
				l.offset += 5.0f * dt;
				break;
			}
		}

		yBorder = machineCellSize.y * 3;
		SetDrawTarget(field);

		Clear(def::BROWN);

		def::vi2d p;
		for (auto& l : lines)
		{
			for (p.y = 0; p.y < l.content.size(); p.y++)
			{
				def::Pixel col;

				switch (l.content[p.y])
				{
				case RED: col = def::RED; break;
				case BLUE: col = def::BLUE; break;
				case GREEN: col = def::GREEN; break;
				}

				def::vi2d pos = (p + def::vi2d(0.0f, l.offset)) * machineCellSize;

				FillRectangle(pos, machineCellSize, col);
				DrawRectangle(pos, machineCellSize, def::BROWN);
			}

			p.x++;
		}

		for (int i = 0; i < 3; i++)
			DrawRectangle({ machineCellSize.x * i, machineCellSize.y }, machineCellSize, def::YELLOW);

		SetDrawTarget(nullptr);
		yBorder = ScreenHeight();

		if (lines.back().offset >= lines.back().maxOffset)
		{
			def::Pixel p0 = field->sprite->GetPixel({ machineCellSize.x * 0 + 1, machineCellSize.y + 1 });
			def::Pixel p1 = field->sprite->GetPixel({ machineCellSize.x * 1 + 1, machineCellSize.y + 1 });
			def::Pixel p2 = field->sprite->GetPixel({ machineCellSize.x * 2 + 1, machineCellSize.y + 1 });

			if (p0 == p1 && p1 == p2)
			{
				if (p0 == def::RED) score += 20;
				if (p0 == def::BLUE) score += 40;
				if (p0 == def::GREEN) score += 100;
			}
		}

		Clear(def::DARK_CYAN);

		DrawString(machinePos - def::vi2d(0, 12), "Score: " + std::to_string(score));
		DrawSprite(machinePos, field->sprite);

		int y = 0;

		FillRectangle(machinePos + def::vi2d(3 * machineCellSize.x + 6, y), { 5, 5 }, def::RED);
		DrawString(machinePos + def::vi2d(3 * machineCellSize.x + 14, y), "20");
		y += 12;

		FillRectangle(machinePos + def::vi2d(3 * machineCellSize.x + 6, y), { 5, 5 }, def::BLUE);
		DrawString(machinePos + def::vi2d(3 * machineCellSize.x + 14, y), "40");
		y += 12;

		FillRectangle(machinePos + def::vi2d(3 * machineCellSize.x + 6, y), { 5, 5 }, def::GREEN);
		DrawString(machinePos + def::vi2d(3 * machineCellSize.x + 14, y), "100");
		y += 12;

		return true;
	}

	virtual bool Draw(int32_t x, int32_t y, def::Pixel p) override
	{
		return def::GameEngine::Draw(x, y % yBorder, p);
	}

};

int main()
{
	Jackpot demo;
	demo.Construct(256, 240, 4, 4);
	demo.Run();
	return 0;
}
