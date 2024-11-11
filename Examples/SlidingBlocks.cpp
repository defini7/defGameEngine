#define DGE_APPLICATION
#include "defGameEngine.hpp"

enum SideType
{
	SIDE_NONE,
	SIDE_LEFT,
	SIDE_RIGHT,
	SIDE_UP,
	SIDE_DOWN
};

constexpr int TILE_SIZE = 32;

struct Block
{
	virtual void Draw(def::GameEngine* engine, const def::vi2d& pos) const
	{
		if (engine)
			engine->DrawPartialSprite(pos * TILE_SIZE, filePos, { TILE_SIZE, TILE_SIZE }, s_Gfx);
	}

	virtual bool Push(SideType touchSide)
	{
		return false;
	}

	def::vi2d filePos;

	static def::Sprite* s_Gfx;
};

def::Sprite* Block::s_Gfx = nullptr;

struct Block_Stop : Block
{
	Block_Stop()
	{
		filePos = { 0, 0 };
	}
};

struct Block_Anydir : Block
{
	Block_Anydir()
	{
		filePos = { TILE_SIZE, 0 };
	}

	bool Push(SideType touchSide) override
	{
		return touchSide != SIDE_NONE;
	}
};

struct Block_Horizontal : Block
{
	Block_Horizontal()
	{
		filePos = { 2 * TILE_SIZE, 0 };
	}

	bool Push(SideType touchSide) override
	{
		return touchSide == SIDE_LEFT || touchSide == SIDE_RIGHT;
	}
};

struct Block_Vertical : Block
{
	Block_Vertical()
	{
		filePos = { 3 * TILE_SIZE, 0 };
	}

	bool Push(SideType touchSide) override
	{
		return touchSide == SIDE_UP || touchSide == SIDE_DOWN;
	}
};

struct Block_Count : Block
{
	Block_Count(int allowedMoves = 0)
	{
		remainingMoves = allowedMoves;
		filePos = { 4 * TILE_SIZE, 0 };
	}

	bool Push(SideType touchSide) override
	{
		if (touchSide != SIDE_NONE && remainingMoves > 0)
		{
			remainingMoves--;
			return true;
		}

		return false;
	}

	void Draw(def::GameEngine* engine, const def::vi2d& pos) const override
	{
		Block::Draw(engine, pos);
		engine->DrawString(pos * TILE_SIZE + TILE_SIZE / 8, std::to_string(remainingMoves), def::WHITE);
	}

	int remainingMoves;
};

struct Block_Obstacle : Block
{
	Block_Obstacle()
	{
		filePos = { 5 * TILE_SIZE, 0 };
	}
};

class SlidingBlocks : public def::GameEngine
{
public:
	SlidingBlocks()
	{
		SetTitle("Sliding Blocks");
	}

	virtual ~SlidingBlocks()
	{
		delete Block::s_Gfx;

		for (auto block : map)
		{
			if (block)
				delete block;
		}
	}

	std::vector<Block*> map;
	def::vi2d mapSize;

	def::vi2d playerPos = { 1, 1 };

protected:
	void PushBlocks(SideType touchSide)
	{
		std::function<bool(int, int, SideType)> push = [&](int x, int y, SideType touchSide) -> bool
			{
				size_t i = y * mapSize.x + x;

				if (map[i])
				{
					if (map[i]->Push(touchSide))
					{
						switch (touchSide)
						{
						case SIDE_RIGHT: x--; break;
						case SIDE_LEFT:	 x++; break;
						case SIDE_DOWN:  y--; break;
						case SIDE_UP:    y++; break;
						}

						if (push(x, y, touchSide))
						{
							map[y * mapSize.x + x] = map[i];
							map[i] = nullptr;

							return true;
						}
					}

					return false;
				}

				return true;
			};

		if (touchSide != SIDE_NONE)
		{
			def::vi2d possiblePos = playerPos;

			switch (touchSide)
			{
			case SIDE_RIGHT: possiblePos.x--; break;
			case SIDE_LEFT:	 possiblePos.x++; break;
			case SIDE_DOWN:  possiblePos.y--; break;
			case SIDE_UP:    possiblePos.y++; break;
			}

			if (push(possiblePos.x, possiblePos.y, touchSide))
				playerPos = possiblePos;
		}
	}

	bool OnUserCreate() override
	{
		Block::s_Gfx = new def::Sprite("Assets/blocks.png");

		std::string s;
		s += "################";
		s += "#........5.....#";
		s += "#.+........4...#";
		s += "#....@@@@@.....#";
		s += "#...@..........#";
		s += "#...@.......+..#";
		s += "#...@..9.......#";
		s += "#...@..........#";
		s += "#........-..|..#";
		s += "#...+..........#";
		s += "#........@.....#";
		s += "#.|.........7..#";
		s += "#....@...-.....#";
		s += "#..............#";
		s += "#........-.....#";
		s += "################";

		mapSize = GetScreenSize() / TILE_SIZE;
		map.resize(mapSize.x * mapSize.y);

		def::vi2d pos;
		for (; pos.y < mapSize.y; pos.y++)
		{
			for (; pos.x < mapSize.x; pos.x++)
			{
				size_t i = pos.y * mapSize.x + pos.x;

				switch (s[i])
				{
				case '@': map[i] = new Block_Stop();	   break;
				case '+': map[i] = new Block_Anydir();	   break;
				case '-': map[i] = new Block_Vertical();   break;
				case '|': map[i] = new Block_Horizontal(); break;
				case '#': map[i] = new Block_Obstacle();   break;
				default:
				{
					if (isdigit(s[i]))
						map[i] = new Block_Count(s[i] - '0');
				}

				}
			}

			pos.x = 0;
		}

		return true;
	}

	bool OnUserUpdate(float deltaTime) override
	{
		SideType touchSide = SIDE_NONE;

		if (GetKey(def::Key::LEFT).pressed)	 touchSide = SIDE_RIGHT;
		if (GetKey(def::Key::RIGHT).pressed) touchSide = SIDE_LEFT;
		if (GetKey(def::Key::UP).pressed)	 touchSide = SIDE_DOWN;
		if (GetKey(def::Key::DOWN).pressed)  touchSide = SIDE_UP;

		PushBlocks(touchSide);

		Clear(def::BLACK);

		SetPixelMode(def::Pixel::Mode::MASK);

		for (int y = 0; y < mapSize.y; y++)
			for (int x = 0; x < mapSize.x; x++)
			{
				Block* block = map[y * mapSize.x + x];

				if (block)
					block->Draw(this, { x, y });
			}

		SetPixelMode(def::Pixel::Mode::DEFAULT);

		FillRectangle(playerPos * TILE_SIZE, { TILE_SIZE, TILE_SIZE }, def::WHITE);

		return true;
	}
};

int main()
{
	SlidingBlocks demo;

	if (demo.Construct(512, 512, 2, 2))
		demo.Run();

	return 0;
}
