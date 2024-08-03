/*
*	BSD 3-Clause License

	Copyright (c) 2022 - 2024, Alex

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
*/

#define DGE_APPLICATION
#include "defGameEngine.hpp"

class Raycasting : public def::GameEngine
{
public:
	Raycasting()
	{
		SetTitle("Raycasting");
	}

	virtual ~Raycasting()
	{
		delete[] map;
	}

private:
	def::vi2d tileSize;
	def::vi2d tilesCount;

	def::vf2d start;
	def::vf2d end;

	bool* map;

	float pointSpeed = 100.0f;

protected:
	void ControlPoint(def::vf2d& point, const float deltaTime, const def::Key up, const def::Key down, const def::Key left, const def::Key right)
	{
		if (GetKey(up).held)
			point.y -= pointSpeed * deltaTime;

		if (GetKey(down).held)
			point.y += pointSpeed * deltaTime;

		if (GetKey(left).held)
			point.x -= pointSpeed * deltaTime;

		if (GetKey(right).held)
			point.x += pointSpeed * deltaTime;
	}

	void SetTile(const def::vi2d& pos, const bool value)
	{
		map[pos.y * tilesCount.x + pos.x] = value;
	}

	bool GetTile(const def::vi2d& pos)
	{
		return map[pos.y * tilesCount.x + pos.x];
	}

	bool OnUserCreate() override
	{
		tileSize = { 8, 8 };
		tilesCount = ScreenSize() / tileSize;

		map = new bool[tilesCount.x * tilesCount.y] { false };

		start = { 10, 10 };
		end = { 20, 10 };

		return true;
	}

	bool OnUserUpdate(float deltaTime) override
	{
		ControlPoint(start, deltaTime, def::Key::W, def::Key::S, def::Key::A, def::Key::D);
		ControlPoint(end, deltaTime, def::Key::UP, def::Key::DOWN, def::Key::LEFT, def::Key::RIGHT);

		if (GetMouse(def::Button::RIGHT).pressed)
		{
			def::vi2d tilePos = GetMouse() / tileSize;

			SetTile(tilePos, !GetTile(tilePos));
		}

		// Perform DDA algorithm
		// thank you, https://lodev.org/cgtutor/raycasting.html

		def::vf2d rayStart = start / tileSize;
		def::vf2d rayDirection = (end / tileSize - rayStart).norm();
		def::vf2d stepSize = (1.0f / rayDirection).abs();

		def::vf2d side, step;
		def::vi2d mapPos = rayStart;

		if (rayDirection.x < 0)
		{
			step.x = -1;
			side.x = (rayStart.x - (float)mapPos.x) * stepSize.x;
		}
		else
		{
			step.x = 1;
			side.x = (float(mapPos.x + 1) - rayStart.x) * stepSize.x;
		}

		if (rayDirection.y < 0)
		{
			step.y = -1;
			side.y = (rayStart.y - (float)mapPos.y) * stepSize.y;
		}
		else
		{
			step.y = 1;
			side.y = (float(mapPos.y + 1) - rayStart.y) * stepSize.y;
		}

		bool tileFound = false;

		float distance = 0.0f;
		float maxDistance = 64.0f;

		while (!tileFound && distance < maxDistance)
		{
			if (side.x < side.y)
			{
				mapPos.x += step.x;
				distance = side.x;
				side.x += stepSize.x;
			}
			else
			{
				mapPos.y += step.y;
				distance = side.y;
				side.y += stepSize.y;
			}

			if (mapPos.x >= 0 && mapPos.y >= 0 && mapPos.x < tilesCount.x && mapPos.y < tilesCount.y)
			{
				if (GetTile(mapPos))
					tileFound = true;
			}
		}

		def::vf2d intersectionPoint;

		if (tileFound)
			intersectionPoint = rayStart + rayDirection * distance;

		Clear(def::BLACK);

		def::vi2d p;
		for (; p.y < tilesCount.y; p.y++)
			for (p.x = 0; p.x < tilesCount.x; p.x++)
			{
				if (GetTile(p))
					FillRectangle(p * tileSize, tileSize, def::BLUE);
			}

		DrawLine(start, end, def::GREY);

		if (tileFound)
			DrawCircle(intersectionPoint * tileSize, 3, def::CYAN);

		FillCircle(start, 3, def::RED);
		FillCircle(end, 3, def::GREEN);

		return true;
	}
};

int main()
{
	Raycasting demo;

	if (demo.Construct(241, 241, 4, 4))
		demo.Run();

	return 0;
}
