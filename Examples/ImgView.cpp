#define DGE_APPLICATION
#include "defGameEngine.h"

std::unique_ptr<def::Sprite> g_Image;

class ImageViewer : public def::GameEngine
{
public:
	ImageViewer()
	{
		SetTitle("Image Viewer");
	}

public:
	bool OnUserCreate() override
	{
		DrawSprite(0, 0, g_Image.get());
		return true;
	}

	bool OnUserUpdate(float deltaTime) override
	{
		return true;
	}
};

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cerr << "Usage: imgview <filename>" << std::endl;
		return 1;
	}

	g_Image = std::make_unique<def::Sprite>(argv[1]);

	ImageViewer demo;
	demo.Construct(g_Image->width, g_Image->height, 1, 1);
	demo.Run();

	return 0;
}