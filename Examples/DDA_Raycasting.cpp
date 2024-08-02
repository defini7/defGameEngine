/*
*	BSD 3-Clause License
	Copyright (c) 2023, Alex
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

#include <algorithm>

struct Object
{
	def::vf2d pos;
	def::vf2d vel;

	float speed;

	uint32_t type;
	uint32_t id;

	bool toRemove = false;
};

class RayCasting : public def::GameEngine
{
public:
	RayCasting()
	{
		SetTitle("Ray Casting");
	}

	virtual ~RayCasting()
	{
		delete tiles;
		delete[] depthBuffer;
	}

private:
	enum Objects
	{
		BLUESTONE,
		COLORSTONE,
		EAGLE,
		GREYSTONE,
		MOSSY,
		PILLAR,
		PURPLESTONE,
		REDBRICK,
		WOOD,
		BARREL,
		BULLET
	};

	enum Side
	{
		FLOOR,
		CEILING,
		WALL
	};

	def::Sprite* tiles = nullptr;

	int mapCellSize = 3;

	int floorId = Objects::GREYSTONE;
	int ceilingId = Objects::WOOD;

	std::string map;

	def::vi2d mapSize = { 32, 32 };
	def::vi2d texSize = { 64, 64 };

	def::vf2d playerPos = { 7.0f, 3.0f };
	def::vf2d playerVel = { -1.0, 0.0f };
	def::vf2d playerPlane = { 0.0f, 0.66f };

	float moveSpeed = 5.0f;
	float rotSpeed = 3.0f;
	float depth = 16.0f;

	std::vector<Object> objects;
	float* depthBuffer = nullptr;

protected:
	bool OnUserCreate() override
	{
		tiles = new def::Sprite("Assets/tileset.png");

		map =
			"77777777777777777.........777777"
			"7..............................7"
			"7...8...............8..........7"
			"7...8...............8..........7"
			"7...88..............88.........7"
			"7....8...............8.........7"
			"7..............................7"
			"7..88..............88..........7"
			"7.......8888888.........88888887"
			"7..............................7"
			"...........8...............8...7"
			".........888.............888...7"
			"...............................2"
			"...............................7"
			"...............................7"
			"............88888..............7"
			"...............................7"
			"7..............................7"
			"7...6...............6..........7"
			"7...6...............6..........7"
			"7...66..............66.........7"
			"7....6...............6.........7"
			"7..............................7"
			"7..66..............66..........7"
			"7.......6666666.........66666667"
			"7..............................."
			"7..........6...............6...."
			"7........666.............666...."
			"7..............................."
			"7..............................."
			"7..............................."
			"777777..................77777777";

		objects.push_back({ {8.5f, 8.5f}, {0.0f, 0.0f}, 0.0f, Objects::BARREL });
		objects.push_back({ {7.5f, 7.5f}, {0.0f, 0.0f}, 0.0f, Objects::BARREL });
		objects.push_back({ {10.0f, 3.0f}, {0.0f, 0.0f}, 0.0f, Objects::BARREL });

		depthBuffer = new float[ScreenWidth()];

		return true;
	}

	bool OnUserUpdate(float fDeltaTime) override
	{
		// Remove redundant objects
		auto removeIter = std::remove_if(objects.begin(), objects.end(), [](const Object& o) { return o.toRemove; });
		if (removeIter != objects.end()) objects.erase(removeIter);

		if (GetKey(def::Key::W).held)
		{
			def::vf2d newPlayerPos = playerPos + playerVel * moveSpeed * fDeltaTime;

			if (newPlayerPos >= def::vf2d(0, 0) && newPlayerPos < mapSize)
			{
				if (map[(int)newPlayerPos.y * mapSize.x + newPlayerPos.x] == '.')
					playerPos = newPlayerPos;
			}
		}

		if (GetKey(def::Key::S).held)
		{
			def::vf2d newPlayerPos = playerPos - playerVel * moveSpeed * fDeltaTime;

			if (newPlayerPos >= def::vf2d(0, 0) && newPlayerPos < mapSize)
			{
				if (map[(int)newPlayerPos.y * mapSize.x + newPlayerPos.x] == '.')
					playerPos = newPlayerPos;
			}
		}

		if (GetKey(def::Key::A).held)
		{
			float oldVelX = playerVel.x;
			float oldPlaneX = playerPlane.x;

			playerVel.x = playerVel.x * cos(rotSpeed * fDeltaTime) - playerVel.y * sin(rotSpeed * fDeltaTime);
			playerVel.y = oldVelX * sin(rotSpeed * fDeltaTime) + playerVel.y * cos(rotSpeed * fDeltaTime);

			playerPlane.x = playerPlane.x * cos(rotSpeed * fDeltaTime) - playerPlane.y * sin(rotSpeed * fDeltaTime);
			playerPlane.y = oldPlaneX * sin(rotSpeed * fDeltaTime) + playerPlane.y * cos(rotSpeed * fDeltaTime);
		}

		if (GetKey(def::Key::D).held)
		{
			float oldVelX = playerVel.x;
			float oldPlaneX = playerPlane.x;

			playerVel.x = playerVel.x * cos(-rotSpeed * fDeltaTime) - playerVel.y * sin(-rotSpeed * fDeltaTime);
			playerVel.y = oldVelX * sin(-rotSpeed * fDeltaTime) + playerVel.y * cos(-rotSpeed * fDeltaTime);

			playerPlane.x = playerPlane.x * cos(-rotSpeed * fDeltaTime) - playerPlane.y * sin(-rotSpeed * fDeltaTime);
			playerPlane.y = oldPlaneX * sin(-rotSpeed * fDeltaTime) + playerPlane.y * cos(-rotSpeed * fDeltaTime);
		}

		// Check for collision
		for (auto& o1 : objects)
		{
			for (auto& o2 : objects)
			{
				if (o1.id != o2.id)
				{
					if (o1.pos.round() == o2.pos.round())
					{
						if (o1.type == Objects::BULLET || o2.type == Objects::BULLET)
						{
							o1.toRemove = true;
							o2.toRemove = true;
						}
					}
				}
			}
		}

		Clear(def::BLACK);

		// Perform DDA raycast algorithm
		for (int x = 0; x < ScreenWidth(); x++)
		{
			float playerAngle = 2.0f * (float)x / (float)ScreenWidth() - 1.0f;

			def::vf2d rayDir = playerVel + playerPlane * playerAngle;
			def::vf2d distance = (1.0f / rayDir).abs();

			bool doesHitWall = false;
			bool noWall = false;
			int side = 0;

			def::vi2d step;
			def::vf2d fromCurrentDistance;
			def::vi2d mapPos = playerPos;

			float distanceToWall;

			if (rayDir.x < 0.0f)
			{
				step.x = -1;
				fromCurrentDistance.x = (playerPos.x - (float)mapPos.x) * distance.x;
			}
			else
			{
				step.x = 1;
				fromCurrentDistance.x = ((float)mapPos.x + 1.0f - playerPos.x) * distance.x;
			}

			if (rayDir.y < 0.0f)
			{
				step.y = -1;
				fromCurrentDistance.y = (playerPos.y - (float)mapPos.y) * distance.y;
			}
			else
			{
				step.y = 1;
				fromCurrentDistance.y = ((float)mapPos.y + 1.0f - playerPos.y) * distance.y;
			}

			while (!doesHitWall && !noWall)
			{
				if (fromCurrentDistance.x < fromCurrentDistance.y)
				{
					fromCurrentDistance.x += distance.x;
					mapPos.x += step.x;
					side = 0;
				}
				else
				{
					fromCurrentDistance.y += distance.y;
					mapPos.y += step.y;
					side = 1;
				}

				if (mapPos.y < 0 || mapPos.y >= mapSize.y || mapPos.x < 0 || mapPos.x >= mapSize.x)
				{
					distanceToWall = depth;
					noWall = true;
				}

				if (!noWall && std::isdigit(map[mapPos.y * mapSize.x + mapPos.x]))
					doesHitWall = true;
			}

			if (side == 0)
				distanceToWall = fromCurrentDistance.x - distance.x;
			else
				distanceToWall = fromCurrentDistance.y - distance.y;

			int lineHeight = int((float)ScreenHeight() / distanceToWall);

			int ceilingPos = std::max(-lineHeight + ScreenHeight() / 2, 0);
			int floorPos = std::min(lineHeight + ScreenHeight() / 2, ScreenHeight() - 1);

			float testPoint;
			float texStep, texPos;

			if (side == 0)
				testPoint = playerPos.y + rayDir.y * distanceToWall;
			else
				testPoint = playerPos.x + rayDir.x * distanceToWall;

			testPoint -= floorf(testPoint);

			def::vi2d tex = { int(testPoint * (float)texSize.x), 0 };

			if ((side == 0 && rayDir.x > 0.0f) || (side == 1 && rayDir.y < 0.0f))
				tex.x = texSize.x - tex.x - 1;

			texStep = (float)texSize.y / (float)lineHeight / 2;
			texPos = float(ceilingPos - ScreenHeight() / 2 + lineHeight) * texStep;

			for (int y = 0; y <= floorPos; y++)
			{
				if (y <= ceilingPos) // ceiling and floor
				{
					float planeZ = float(ScreenHeight() / 2) / float(ScreenHeight() / 2 - y);

					def::vf2d planePoint = playerPos + 2.0f * rayDir * planeZ;
					def::vf2d planeSample = planePoint - planePoint.floor();

					def::vi2d texPos = (planeSample * texSize).min(texSize);

					Draw(x, y, tiles->GetPixel({ ceilingId * texSize.x + texPos.x, texPos.y })); // ceiling
					Draw(x, ScreenHeight() - y, tiles->GetPixel({ floorId * texSize.x + texPos.x, texPos.y })); // floor
				}
				else if (y > ceilingPos && !noWall) // wall
				{
					if (distanceToWall < depth)
					{
						tex.y = (int)texPos % (texSize.y - 1);
						texPos += texStep;

						Draw(x, y, tiles->GetPixel({ ((int)map[mapPos.y * mapSize.x + mapPos.x] - 48) * texSize.x + tex.x, tex.y }));
					}
				}
			}

			depthBuffer[x] = distanceToWall;
		}

		// Update and draw textured objects
		for (auto& o : objects)
		{
			o.pos += o.vel * o.speed * fDeltaTime;

			if (o.pos.floor() >= def::vf2d(0, 0) && o.pos.floor() < mapSize && !std::isdigit(map[(int)o.pos.y * mapSize.x + (int)o.pos.x]))
			{
				def::vf2d objectPos = o.pos - playerPos;

				float invDet = 1.0f / (playerPlane.x * playerVel.y - playerPlane.y * playerVel.x);

				def::vf2d transform =
				{
					invDet * (playerVel.y * objectPos.x - playerVel.x * objectPos.y),
					invDet * (-playerPlane.y * objectPos.x + playerPlane.x * objectPos.y)
				};

				float aspectRatio = transform.x / transform.y;

				def::vi2d objectScreenPos = { int(float(ScreenWidth() / 2) * (1.0f + aspectRatio)), ScreenHeight() / 2 };
				int objectScreenSize = int((float)ScreenHeight() / transform.y);

				def::vi2d ceilingPos = def::vi2d(objectScreenSize, objectScreenSize) / -2 + objectScreenPos;
				def::vi2d floorPos = def::vi2d(objectScreenSize, objectScreenSize) / 2 + objectScreenPos;

				ceilingPos = ceilingPos.max(def::vi2d(0, 0)).min(ScreenSize());
				floorPos = floorPos.max(def::vi2d(0, 0)).min(ScreenSize());

				SetPixelMode(def::Pixel::Mode::MASK);

				for (int x = ceilingPos.x; x < floorPos.x; x++)
				{
					int nTexX = (ScreenWidth() * (x - (-objectScreenSize / 2 + objectScreenPos.x)) * texSize.x / objectScreenSize) / ScreenWidth();

					if (transform.y >= 0 && x >= 0 && x < ScreenWidth() && transform.y < depthBuffer[x])
					{
						for (int y = ceilingPos.y; y < floorPos.y; y++)
						{
							int d = y * ScreenWidth() - ScreenHeight() * ScreenWidth() / 2 + objectScreenSize * ScreenWidth() / 2;
							int nTexY = (d * texSize.y / objectScreenSize) / ScreenWidth();

							Draw(x, y, tiles->GetPixel({ (int)o.type * texSize.x + nTexX, nTexY }));
							depthBuffer[x] = transform.y;
						}
					}
				}

				SetPixelMode(def::Pixel::Mode::DEFAULT);
			}
			else
				o.toRemove = true;
		}

		// Draw map
		for (int x = 0; x < mapSize.x; x++)
			for (int y = 0; y < mapSize.y; y++)
			{
				if (map[y * mapSize.x + x] == '.')
					FillRectangle(x * 2, y * 2, 2, 2, def::GREY);
				else
					FillRectangle(x * 2, y * 2, 2, 2, def::WHITE);
			}

		FillRectangle((int)playerPos.x * 2, (int)playerPos.y * 2, 2, 2, def::YELLOW);

		if (GetMouse(def::Button::LEFT).pressed)
		{
			Object o;
			o.pos = playerPos;
			o.vel = playerVel;
			o.speed = 5.0f;
			o.type = Objects::BULLET;
			o.id = objects.size();
			objects.push_back(o);
		}

		return true;
	}

};

int main()
{
	RayCasting demo;

	demo.Construct(1024, 768, 1, 1);
	demo.Run();

	return 0;
}
