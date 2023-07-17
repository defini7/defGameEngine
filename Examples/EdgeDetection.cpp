#define DGE_APPLICATION
#include "defGameEngine.h"
#include "escapi.h"

#include <algorithm>
#include <array>

constexpr int FRAME_WIDTH = 320;
constexpr int FRAME_HEIGHT = 240;

struct Frame
{
	float* pixels;

	Frame()
	{
		pixels = new float[FRAME_WIDTH * FRAME_HEIGHT] { 0.0f };
	}

	float get(int x, int y)
	{
		if (x >= 0 && y >= 0 && x < FRAME_WIDTH && y < FRAME_HEIGHT)
			return pixels[y * FRAME_WIDTH + x];
		return 0.0f;
	}

	void set(int x, int y, float p)
	{
		if (x >= 0 && y >= 0 && x < FRAME_WIDTH && y < FRAME_HEIGHT)
			pixels[y * FRAME_WIDTH + x] = p;
	}

	void operator=(const Frame& f)
	{
		memcpy(pixels, f.pixels, sizeof(f.pixels));
	}

};

class EdgeDetection : public def::GameEngine
{
public:
	EdgeDetection()
	{
		SetTitle("Edge Detection");
	}

private:
	SimpleCapParams capture;

	Frame input, output, prevInput;

	union RGBint
	{
		int rgb;
		uint8_t c[4];
	};

protected:
	void DrawFrame(Frame& frame, int x, int y)
	{
		for (int j = 0; j < FRAME_HEIGHT; j++)
			for (int i = 0; i < FRAME_WIDTH; i++)
			{
				uint8_t c = (uint8_t)std::min(std::max(0.0f, frame.pixels[j * FRAME_WIDTH + i] * 255.0f), 255.0f);
				Draw(x + i, y + j, def::Pixel(c, c, c));
			}
	}

	bool OnUserCreate() override
	{
		// Initialising ESCAPI

		if (setupESCAPI() == 0) return false;
		capture.mWidth = FRAME_WIDTH;
		capture.mHeight = FRAME_HEIGHT;
		capture.mTargetBuf = new int[FRAME_WIDTH * FRAME_HEIGHT];
		return initCapture(0, &capture);
	}

	bool OnUserUpdate(float deltaTime) override
	{
		// Capture camera image
		prevInput = input;
		doCapture(0); while (isCaptureDone(0) == 0);
		for (int y = 0; y < capture.mHeight; y++)
			for (int x = 0; x < capture.mWidth; x++)
			{
				int i = y * capture.mWidth + x;
				RGBint col; col.rgb = capture.mTargetBuf[i];
				input.pixels[i] = (float)col.c[1] / 255.0f;
			}
		
		// Update output frame with differences between frames
		for (int y = 0; y < FRAME_HEIGHT; y++)
			for (int x = 0; x < FRAME_WIDTH; x++)
				output.set(x, y, fabs(input.get(x, y) - prevInput.get(x, y)));

		// Find top left and bottom right points of area
		// where a motion was occured

		int x1 = -1, y1 = -1;
		int x2 = -1, y2 = -1;
		
		for (int y = 0; y < FRAME_HEIGHT; y++)
			for (int x = 0; x < FRAME_WIDTH; x++)
			{
				if (output.get(x, y) == 1.0f)
				{
					if (x1 == -1) x1 = x;
					if (y1 == -1) y1 = y;
					if (x1 && y1) break;
				}
			}

		for (int y = FRAME_HEIGHT - 1; y >= 0; y--)
			for (int x = FRAME_WIDTH - 1; x >= 0; x--)
			{
				if (output.get(x, y) == 1.0f)
				{
					if (x2 == -1) x2 = x;
					if (y2 == -1) y2 = y;
					if (x2 && y2) break;
				}
			}

		Clear(def::DARK_BLUE);

		DrawFrame(input, 250, 10);

		DrawRectangle(x1 + 250, y1 + 10, x2 - x1, y2 - y1, def::RED);

		return true;
	}

};

int main()
{
	EdgeDetection demo;

	demo.Construct(800, 500, 2, 2);
	demo.Run();

	return 0;
}
