#define DGE_APPLICATION
#include "defGameEngine.h"

class AxisKiller : public def::GameEngine
{
public:
	AxisKiller()
	{
		SetTitle("Axis Killer");
	}

	int border1_start;
	int border1_end;

	int border2_start;
	int border2_end;

	def::vf2d pos1, pos2;

	def::vf2d enemyPos;
	int objRadius = 5;

	struct Bullet
	{
		def::vf2d vel;
		def::vf2d pos;
		bool redundant = false;
	};

	std::list<Bullet> bullets;
	int bulletsShot = 0;
	int bulletsHit = 0;

	int score = 0;

protected:
	void NewEnemy()
	{
		auto rndInt = [](int rmin, int rmax)
		{
			return rmin + rand() % (rmax - rmin);
		};

		enemyPos = { (float)rndInt(border1_start, border1_end * 0.75f), (float)rndInt(border2_start, border2_end * 0.75f) };
	}

	bool PointVsCircle(const def::vf2d& point, const def::vf2d& circle, float r)
	{
		return (circle - point).mag2() <= r * r;
	}

	bool OnUserCreate() override
	{
		border1_start = ScreenWidth() * 0.15f;
		border1_end = ScreenWidth() * 0.95f;

		border2_start = ScreenHeight() * 0.15f;
		border2_end = ScreenHeight() * 0.9f;

		pos1 = { (float)border1_start, ScreenHeight() * 0.85f };
		pos2 = { ScreenWidth() * 0.9f, (float)border2_start };

		NewEnemy();

		return true;
	}

	bool OnUserUpdate(float dt) override
	{
		// Change position of players
		if (GetMouse(0).held)
		{
			if (PointVsCircle(GetMouse(), pos1, objRadius))
				pos1.x = std::max(border1_start, std::min(border1_end, MouseX()));

			if (PointVsCircle(GetMouse(), pos2, objRadius))
				pos2.y = std::max(border2_start, std::min(border2_end, MouseY()));
		}

		// Shoot
		if (GetKey(def::Key::SPACE).pressed)
		{
			bullets.push_back({ { 0.0f, -1.0f }, { pos1.x, pos1.y - objRadius - 1 } });
			bullets.push_back({ { -1.0f, 0.0f }, { pos2.x - objRadius - 1, pos2.y } });
			bulletsShot = 2;
		}

		// Check collision and update bullets' positions
		for (auto& b : bullets)
		{
			b.pos += b.vel * dt * 150.0f;

			if (PointVsCircle(b.pos, enemyPos, objRadius))
			{
				bulletsHit++;
				b.redundant = true;
			}

			if (b.pos.x < -objRadius || b.pos.y < -objRadius) b.redundant = true;
		}

		bullets.remove_if([](const Bullet& b) { return b.redundant; });

		// If all bullets hit the enemy
		// destroy the enemy ship and spawn new one
		if (bulletsHit == 2)
		{
			NewEnemy();

			// Add score, to keep player's attention even further
			score++;

			bullets.clear();
			bulletsShot = 0;
			bulletsHit = 0;
		}
		else if (bulletsShot == 2 && bullets.empty())
		{
			// If bullets missed the enemy, then remove 1 point from score,
			// and if score is less than 0, player loses the game
			if (--score < 0) return false;

			bulletsShot = 0;
			bulletsHit = 0;
		}

		Clear(def::BLACK);
		
		// Draw Enemy
		FillCircle(enemyPos, objRadius, def::RED);

		// Draw axis
		DrawLine(border1_start, pos1.y, border1_end, pos1.y, def::WHITE);
		DrawLine(pos2.x, border2_start, pos2.x, border2_end, def::WHITE);

		// Draw players
		DrawCircle(pos1, objRadius, def::ORANGE);
		DrawCircle(pos2, objRadius, def::ORANGE);

		// Draw bullets
		for (const auto& b : bullets)
			Draw(b.pos, def::WHITE);

		// Draw score
		DrawString(2, 2, "Score: " + std::to_string(score), def::WHITE);

		return true;
	}
};

int main()
{
	AxisKiller demo;

	demo.Construct(256, 240, 4, 4);
	demo.Run();

	return 0;
}
