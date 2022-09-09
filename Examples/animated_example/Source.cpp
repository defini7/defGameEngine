#include "defGameEngine.h"

#define DGE_ANIMATED
#include "DGE_Animated.h"

class Sample : public def::GameEngine
{
public:
	Sample()
	{
		SetTitle("Sample");
	}

protected:
	bool OnUserCreate() override
	{
		sprDemo = new def::Sprite("Mario.png");
		gfxDemo = new def::GFX(sprDemo, GetRenderer());

		anim = new Animated(gfxDemo);

		id = anim->AddAnimation(def::vi2d(0, 0).ref(), def::vi2d(32, 32).ref(), 8);

		Clear();

		return true;
	}

	bool OnUserUpdate(float fDeltaTime) override
	{
		def::vi2d vFilePos;
		def::vi2d vFileSize;

		bool bFrameReady = anim->Animate(id, vFilePos, vFileSize, fDeltaTime);

		if (bFrameReady)
		{
			Clear();
			DrawPartialGFX(0, 0, vFilePos.x, vFilePos.y, vFileSize.x, vFileSize.y, gfxDemo, 0.0f, 4.0f);
		}

		return true;
	}

private:
	Animated* anim = nullptr;

	def::Sprite* sprDemo = nullptr;
	def::GFX* gfxDemo = nullptr;

	uint32_t id;
};

int main()
{
	Sample demo;
	def::rcode err = demo.Construct(1024, 960);

	if (err.ok)
		demo.Run();
	else
		std::cerr << err.info << "\n";

	return 0;
}
