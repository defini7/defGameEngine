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

// https://github.com/defini7/defGeometry2D
#define DEF_GEOMETRY2D_IMPL
#include "defGeometry2D.hpp"

// https://github.com/defini7/defGameEngine
#define DGE_APPLICATION
#include "defGameEngine.hpp"

template <class T>
struct Dynamic
{
	T model;
	def::vf2d velocity;
};

struct Ball : Dynamic<def::circle<float>> {};

struct Tile : Dynamic<def::rect<float>>
{
	bool isRedundant = false;
};

class BreakOut : public def::GameEngine
{
public:
	BreakOut()
	{
		SetTitle("Break Out");
	}

private:
	Ball ball;

	std::list<Tile> tiles;
	std::list<Tile>::iterator bat;

protected:
	bool OnUserCreate() override
	{
		ball.model.pos = GetScreenSize() / 2;
		ball.velocity = { 0.5f, 0.8f };
		ball.model.radius = 5.0f;

		Tile b;
		b.model.pos = GetScreenSize() * def::vf2d(0.5f, 0.75f);
		b.model.size = { 20.0f, 5.0f };
	
		tiles.push_back(b);
		bat = tiles.begin();

		def::vf2d origin = GetScreenSize() * 0.25f;
		def::vf2d tileSize = { 20.0f, 10.0f };

		def::vi2d count = GetScreenSize() * def::vf2d(0.5f, 0.2f) / tileSize;

		// Add tiles
		for (int i = 0; i < count.x; i++)
			for (int j = 0; j < count.y; j++)
			{
				Tile tile;

				tile.model.pos = origin + (tileSize + 1) * def::vf2d(i, j);
				tile.model.size = tileSize;

				tiles.push_back(tile);
			}

		return true;
	}

	bool OnUserUpdate(float deltaTime) override
	{
		if (GetKey(def::Key::LEFT).held) bat->model.pos.x -= 150.0f * deltaTime;
		if (GetKey(def::Key::RIGHT).held) bat->model.pos.x += 150.0f * deltaTime;

		def::vf2d potentialPos = ball.model.pos + ball.velocity * deltaTime * 200.0f;

		if (potentialPos.x < ball.model.radius)
			ball.velocity.x *= -1.0f;

		if (potentialPos.x >= (float)ScreenWidth() - ball.model.radius)
			ball.velocity.x *= -1.0f;

		if (potentialPos.y < ball.model.radius)
			ball.velocity.y *= -1.0f;

		if (potentialPos.y >= (float)ScreenHeight() - ball.model.radius)
			ball.velocity.y *= -1.0f;

		for (auto it = tiles.begin(); it != tiles.end(); it++)
		{
			uint32_t i = 0;

			while (i < def::rect<float>::SIDES)
			{
				if (def::intersects(ball.model, it->model.side(i)).size() > 0)
				{
					if (it != bat)
						it->isRedundant = true;

					break;
				}

				i++;
			}

			switch (i)
			{
			case def::rect<float>::side::LEFT: ball.velocity.x *= -1.0f; break;
			case def::rect<float>::side::TOP: ball.velocity.y *= -1.0f; break;
			case def::rect<float>::side::RIGHT: ball.velocity.x *= -1.0f; break;
			case def::rect<float>::side::BOTTOM: ball.velocity.y *= -1.0f; break;
			}
		}

		ball.model.pos += ball.velocity * deltaTime * 150.0f;

		tiles.remove_if([](const Tile& t) { return t.isRedundant; });

		Clear(def::BLACK);

		FillCircle(ball.model.pos, ball.model.radius, def::YELLOW);
		FillRectangle(bat->model.pos, bat->model.size, def::BLUE);

		for (auto it = std::next(tiles.begin()); it != tiles.end(); it++)
			FillRectangle(it->model.pos, it->model.size, def::GREEN);

		return true;
	}
};

int main()
{
	BreakOut app;

	app.Construct(512, 480, 2, 2);
	app.Run();

	return 0;
}
