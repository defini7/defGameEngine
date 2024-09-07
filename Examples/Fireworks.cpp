#define DGE_APPLICATION
#include "defGameEngine.hpp"

struct Particle
{
	def::vf2d offset;
	def::vf2d velocity;

	float speed;

	def::Pixel col;
};

struct Firework
{
	def::vf2d pos;

	def::vf2d explodedPos;

	// Current time in milliseconds
	float explosionTimer;

	// Time when firework must explode
	float explosionTime;

	// Local timer to safe current lifetime
	float lifeTimer;

	// How long particles will be alive
	float lifeTime;

	std::vector<Particle> particles;
};

class Fireworks : public def::GameEngine
{
public:
	Fireworks()
	{
		SetTitle("Fireworks");
	}

private:
	std::vector<Firework> fireworks;

	// Count time in milliseconds
	float timer;

	// When new firework will appear
	float appearTime;

	const float fireworkSpeed = 100.0f;
	const float particleSpeed = 10.0f;

private:
	void AddFirework()
	{
		Firework f;

		f.pos.x = ((rand() + int(float(ScreenWidth() - 1) * 0.25f)) % int(float(ScreenWidth() - 1) * 0.75f));
		f.pos.y = ScreenHeight() - 1;

		f.explodedPos = { -1.0f, -1.0f };

		f.explosionTimer = 0.0f;
		f.explosionTime = float((rand() % 2) + 1);

		f.lifeTimer = 0.0f;
		f.lifeTime = float((rand() % 4) + 2);

		int particles = (rand() + 20) % 40;

		for (int i = 0; i < particles; i++)
		{
			Particle p;
			p.offset = { 0.0f, 0.0f };
			p.speed = float((rand() % 10) + 5);
			p.col = def::Pixel(rand() % 256, rand() % 256, rand() % 256);

			float angle = 2.0f * 3.1415926f * ((float)rand() / (float)RAND_MAX);
			p.velocity = { cos(angle), sin(angle) };

			f.particles.push_back(p);
		}

		fireworks.push_back(f);
	}

public:
	bool OnUserCreate() override
	{
		srand(time(NULL));

		timer = 0.0f;
		appearTime = 0.5f;

		return true;
	}

	bool OnUserUpdate(float deltaTime) override
	{
		if (timer >= appearTime)
		{
			AddFirework();
			timer = 0.0f;
		}
		else
			timer += deltaTime;

		Clear(def::BLACK);

		size_t i = 0;
		for (auto& f : fireworks)
		{
			if (f.explosionTimer >= f.explosionTime)
			{
				if (f.explodedPos.x < 0.0f)
					f.explodedPos = f.pos;

				for (auto& p : f.particles)
				{
					p.offset += (particleSpeed + p.speed) * p.velocity * deltaTime;
					Draw(f.explodedPos + p.offset, p.col);
				}

				if (f.lifeTimer > f.lifeTime)
					fireworks.erase(fireworks.begin() + i);
				else
					f.lifeTimer += deltaTime;
			}
			else
			{
				f.explosionTimer += deltaTime;
				f.pos.y -= fireworkSpeed * deltaTime;

				Draw(f.pos, def::WHITE);
			}

			i++;
		}

		return true;
	}
};

int main()
{
	Fireworks demo;
	demo.Construct(256, 240, 4, 4);
	demo.Run();

	return 0;
}
