#define DGE_APPLICATION
#include "../defGameEngine.hpp"

#define DGE_AFFINE_TRANSFORMS
#include "../Extensions/DGE_AffineTransforms.hpp"

#define DGE_CAMERA2D
#include "../Extensions/DGE_Camera2D.hpp"

class AffineTransforms : public def::GameEngine
{
public:
	AffineTransforms()
	{
		SetTitle("Affine Transforms");
	}

	def::AffineTransforms at;
	def::Camera2D camera;

	def::vf2d pos;
	def::vf2d size;

	def::Sprite* spr;

protected:
	bool OnUserCreate() override
	{
		camera.Initialise({ 0, 0 }, GetScreenSize(), def::Camera2D::Mode::BorderLock);

		size = { 20, 20 };

		spr = new def::Sprite("assets/kart.png");

		return true;
	}

	bool OnUserUpdate(float deltaTime) override
	{
		if (GetMouseWheelDelta() > 0) at.Zoom(1.1f, pos);
		if (GetMouseWheelDelta() < 0) at.Zoom(0.9f, pos);

		Clear(def::DARK_BLUE);

		//pos += (at.ScreenToWorld(GetMousePos()) - pos) * deltaTime;

		if (GetKey(def::Key::A).held) pos.x -= 500.0f * deltaTime;
		if (GetKey(def::Key::D).held) pos.x += 500.0f * deltaTime;
		if (GetKey(def::Key::W).held) pos.y -= 500.0f * deltaTime;
		if (GetKey(def::Key::S).held) pos.y += 500.0f * deltaTime;

		at.SetOffset(camera.Update(pos, deltaTime));
		
		at.DrawSprite({ 0, 0 }, spr);
		at.FillRectangle(pos - size / 2, size, def::RED);

		return true;
	}

};

int main()
{
	AffineTransforms app;

	if (app.Construct(1280, 720, 1, 1))
		app.Run();

	return 0;
}
