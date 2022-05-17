#define PLATFORM_SDL2
#include "defGameEngine.h"

struct sSpaceObject
{
	float x;
	float y;

	float dx;
	float dy;
	
	int size;

	float angle;
};

class Asteroids : public def::GameEngine
{
public:
	Asteroids()
	{
		SetTitle("Asteroids");
	}

protected:
	bool OnUserCreate() override
	{
		ResetGame();

		vecPlayerVerticies.push_back(std::make_pair(0.0f, -5.5f));
		vecPlayerVerticies.push_back(std::make_pair(-2.5f, 2.5f));
		vecPlayerVerticies.push_back(std::make_pair(2.5f, 2.5f));

		return true;
	}

	bool OnUserUpdate(float fDeltaTime) override
	{
		if (bDead)
			ResetGame();

		// CLEAR SCREEN

		Clear(def::BLACK);

		// DRAW ASTEROIDS

		for (auto& a : vecAsteroids)
		{
			a.x += a.dx * 5.0f * fDeltaTime;
			a.y += a.dy * 5.0f * fDeltaTime;

			WrapCoords(a.x, a.y, a.x, a.y);
			
			DrawCircle(a.x, a.y, a.size, def::YELLOW);
		}

		// DRAW PLAYER

		DrawWireFrameModel(vecPlayerVerticies, player.x, player.y, player.angle, 2.0f);

		// CHECK FOR CRASH
		
		for (auto& a : vecAsteroids)
		{
			if (IsPointInsideCircle(a.x, a.y, a.size, player.x, player.y))
				bDead = true;
		}

		// CONTROL PLAYER

		if (GetKey(SDL_SCANCODE_LEFT).bHeld)
			player.angle -= 5.0f * fDeltaTime;

		if (GetKey(SDL_SCANCODE_RIGHT).bHeld)
			player.angle += 5.0f * fDeltaTime;

		if (GetKey(SDL_SCANCODE_UP).bHeld)
		{
			player.dx += sinf(player.angle) * 20.0f * fDeltaTime;
			player.dy -= cosf(player.angle) * 20.0f * fDeltaTime;
		}

		player.x += player.dx * fDeltaTime;
		player.y += player.dy * fDeltaTime;

		WrapCoords(player.x, player.y, player.x, player.y);

		// DRAW BULLETS

		for (int j = 0; j < vecBullets.size(); j++)
		{
			vecBullets[j].x += vecBullets[j].dx * 6.0f * fDeltaTime;
			vecBullets[j].y += vecBullets[j].dy * 6.0f * fDeltaTime;
			
			WrapCoords(vecBullets[j].x, vecBullets[j].y, vecBullets[j].x, vecBullets[j].y);

			Draw(vecBullets[j].x, vecBullets[j].y);

			for (int i = 0; i < vecAsteroids.size(); i++)
			{
				if (IsPointInsideCircle(vecAsteroids[i].x, vecAsteroids[i].y, vecAsteroids[i].size, vecBullets[j].x, vecBullets[j].y))
				{
					sSpaceObject o = vecAsteroids[i];
					
					vecAsteroids.erase(vecAsteroids.begin() + i);

					if (o.size > 1)
					{
						vecAsteroids.push_back({
								o.x, o.y, 8.0f, -6.0f, o.size / 2, 0.0f
							});

						vecAsteroids.push_back({
								o.x, o.y, -8.0f, 6.0f, o.size / 2, 0.0f
							});
					}
					else
					{
						vecAsteroids.push_back({
								   float(rand() % 100 + 30), float(rand() % 100 + 30), -8.0f, 6.0f, rand() % 10, 0.0f
							});
					}
					
					vecBullets.erase(vecBullets.begin() + j);

					nScore++;

					break;
				}
			}
		}

		// FIRE BULLETS

		if (GetMouse(0).bPressed)
			vecBullets.push_back({ player.x, player.y, 50.0f * sinf(player.angle), -50.0f * cosf(player.angle), 0, 0.0f });

		if (vecBullets.size() > 0)
		{
			auto it = std::remove_if(vecBullets.begin(), vecBullets.end(), 
				[&](sSpaceObject& o)
				{
					return o.x < 1 || o.y < 1 || o.x > GetScreenWidth() || o.y > GetScreenHeight();
				});
				
			if (it != vecBullets.end())
				vecBullets.erase(it);
		}

		// DRAW SCORE

		DrawString(10, 10, "Score: " + std::to_string(nScore), def::WHITE, 1.0f);

		return true;
	}

	void ResetGame()
	{
		vecAsteroids.clear();
		vecBullets.clear();

		vecAsteroids.push_back({
				20.0f, 20.0f, 8.0f, -6.0f, 16, 0.0f
			});

		player.x = (float)GetScreenWidth() / 2.0f;
		player.y = (float)GetScreenHeight() / 2.0f;
		player.dx = 0.0f;
		player.dy = 0.0f;
		player.angle = 0.0f;

		bDead = false;
		nScore = 0;
	}

	void WrapCoords(float ix, float iy, float &ox, float &oy)
	{
		ox = ix;
		oy = iy;

		if (ix < 0.0f)
			ox = ix + (float)GetScreenWidth();

		if (ix > (float)GetScreenWidth())
			ox = ix - (float)GetScreenWidth();

		if (iy < 0.0f)
			oy = iy + (float)GetScreenHeight();

		if (iy > (float)GetScreenHeight())
			oy = iy - (float)GetScreenHeight();
	}

	bool IsPointInsideCircle(float cx, float cy, float r, float x, float y)
	{
		return sqrtf((x - cx)*(x - cx) + (y - cy)*(y - cy)) < r;
	}

	virtual void Draw(int32_t x, int32_t y, def::Pixel p = def::WHITE) override
	{
		float fx, fy;
		WrapCoords(x, y, fx, fy);
		def::GameEngine::Draw((int32_t)fx, (int32_t)fy, p);
	}

private:
	std::vector<sSpaceObject> vecAsteroids;
	std::vector<sSpaceObject> vecBullets;

	sSpaceObject player;

	std::vector<std::pair<float, float>> vecPlayerVerticies;

	int nScore = 0;
	bool bDead = false;

};

int main(int argc, char* argv[])
{
	Asteroids demo;
	def::rcode err = demo.Construct(160, 100, 8, 8);

	if (err.ok)
		demo.Run();
	else
		std::cerr << err.info << "\n";

	return 0;
}
