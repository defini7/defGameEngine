#ifndef IMAGE_PROCESSING_HPP
#define IMAGE_PROCESSING_HPP

#include <algorithm>
#include <array>
#include <functional>

namespace def::img
{
	constexpr float CONVO_RIDGE_KERNEL[9] =
	{
		0.0f, -1.0f, 0.0f,
		-1.0f, 4.0f, -1.0f,
		0.0f, -1.0f, 0.0f
	};

	constexpr float CONVO_EDGE_KERNEL[9] =
	{
		-1.0f, -1.0f, -1.0f,
		-1.0f,  8.0f, -1.0f,
		-1.0f, -1.0f, -1.0f
	};

	constexpr float CONVO_SHARPEN_KERNEL[9] =
	{
		0.0f, -1.0f, 0.0f,
		-1.0f, 5.0f, -1.0f,
		0.0f, -1.0f, 0.0f
	};

	constexpr float CONVO_BLUR_KERNEL[9] =
	{
		1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f,
		1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f,
		1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f
	};

	constexpr float SOBEL_KERNEL_X[9] =
	{
		-1.0f, 0.0f, 1.0f,
		-2.0f, 0.0f, 2.0f,
		-1.0f, 0.0f, 1.0f
	};

	constexpr float SOBEL_KERNEL_Y[9] =
	{
		-1.0f, -2.0f, -1.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 2.0f, 1.0f
	};

	constexpr float DITHERING_FLOYDSTEINBERG_KERNEL[4] =
	{
		7.0f / 16.0f, 3.0f / 16.0f,
		5.0f / 16.0f, 1.0f / 16.0f
	};

	const std::vector<float> DITHERING_ORDERED_2X2 = { 0, 2, 3, 1 };
	const std::vector<float> DITHERING_ORDERED_4X4 = { 0, 8, 2, 10, 12, 4, 14, 6, 3, 11, 1, 9, 15, 7, 13, 5 };

	struct Pixelf
	{
		Pixelf(float r = 0.0f, float g = 0.0f, float b = 0.0f) : r(r), g(g), b(b) {}
		
		union
		{
			struct { float r, g, b; };
			float rgb[3];
		};

		std::string str() const
		{
			return std::to_string(r) + ", " + std::to_string(g) + ", " + std::to_string(b);
		}

		float& operator[](const size_t i)
		{
			return rgb[i];
		}
	};

	struct Frame
	{
		Pixelf* pixels = nullptr;
		size_t width, height;

		Frame() = default;
		Frame(const size_t w, const size_t h);
		~Frame();

		void create(const size_t w, const size_t h);

		Pixelf get(const size_t x, const size_t y) const;
		void set(const size_t x, const size_t y, const Pixelf& p);

		Frame& operator=(const Frame& f);

	};

	void Greyscale(const Frame& input, Frame& output);
	void Sepia(const Frame& input, Frame& output);

	void Threshold(const Frame& input, Frame& output, const float factor);
	void Motion(const Frame& input, const Frame& prevInput, Frame& output);
	void Convolution(const Frame& input, Frame& output, const float kernel[9]);
	void LowPass(const Frame& input, Frame& output, const float factor);
	void Adaptive(const Frame& input, Frame& output, const float factor);
	void Sobel(const Frame& input, Frame& output, const float kernelX[9] = SOBEL_KERNEL_X, const float kernelY[9] = SOBEL_KERNEL_Y);
	void Median(const Frame& input, Frame& output);
	void Dithering_FloydSteinberg(const Frame& input, Frame& output, const std::function<void(const def::Pixel&, def::Pixel& p)>& quantise);
	void Dithering_Ordered(const Frame& input, Frame& output, const std::vector<float>& kernel, const size_t kernelWidth, const size_t kernelHeight);
	void Ripple(const Frame& input, Frame& output, const float accTime, const float centerX = 0.5f, const float centerY = 0.5f);
	void CRT(const Frame& input, Frame& output, const float brightness, const float contrast, const float vertFill1 = 0.0f, const float vertFill2 = 0.0f);

#ifdef DEF_IMAGE_PROCESSING
#undef DEF_IMAGE_PROCESSING

	Frame::Frame(const size_t w, const size_t h)
	{
		create(w, h);
	}

	Frame::~Frame()
	{
		delete[] pixels;
	}

	void Frame::create(const size_t w, const size_t h)
	{
		width = w;
		height = h;
		pixels = new Pixelf[w * h];
	}

	Pixelf Frame::get(const size_t x, const size_t y) const
	{
		if (x < width && y < height)
			return pixels[y * width + x];

		return { 0.0f, 0.0f, 0.0f };
	}

	void Frame::set(const size_t x, const size_t y, const Pixelf& p)
	{
		if (x < width && y < height)
			pixels[y * width + x] = p;
	}

	Frame& Frame::operator=(const Frame& f)
	{
		memcpy(pixels, f.pixels, width * height * sizeof(Pixelf));
		return *this;
	}

	void Greyscale(const Frame& input, Frame& output)
	{
		for (int y = 0; y < input.height; y++)
			for (int x = 0; x < input.width; x++)
			{
				Pixelf in = input.get(x, y);

				float avg = (in.r + in.g + in.b) / 3.0f;
				output.set(x, y, Pixelf(avg, avg, avg));
			}
	}

	void Sepia(const Frame& input, Frame& output)
	{
		for (int y = 0; y < input.height; y++)
			for (int x = 0; x < input.width; x++)
			{
				Pixelf in = input.get(x, y), out;

				out.r = std::min(0.393f * in.r + 0.769f * in.g + 0.189f * in.b, 1.0f);
				out.g = std::min(0.349f * in.r + 0.686f * in.g + 0.168f * in.b, 1.0f);
				out.b = std::min(0.272f * in.r + 0.534f * in.g + 0.131f * in.b, 1.0f);

				output.set(x, y, out);
			}
	}

	void Threshold(const Frame& input, Frame& output, const float factor)
	{
		for (int y = 0; y < input.height; y++)
			for (int x = 0; x < input.width; x++)
			{
				Pixelf out, in = input.get(x, y);

				float col = ((in.r + in.g + in.b) / 3.0f) >= factor ? 1.0f : 0.0f;
				out.r = col; out.g = col; out.b = col;

				output.set(x, y, out);
			}
	}

	void Motion(const Frame& input, const Frame& prevInput, Frame& output)
	{
		for (int y = 0; y < input.height; y++)
			for (int x = 0; x < input.width; x++)
			{
				Pixelf in = input.get(x, y);
				Pixelf prev = prevInput.get(x, y);

				Pixelf out;
				out.r = fabs(in.r - prev.r);
				out.g = fabs(in.g - prev.g);
				out.b = fabs(in.b - prev.b);

				output.set(x, y, out);
			}
	}

	void Convolution(const Frame& input, Frame& output, const float kernel[9])
	{
		for (int y = 0; y < input.height; y++)
			for (int x = 0; x < input.width; x++)
			{
				float sumR = 0.0f, sumG = 0.0f, sumB = 0.0f;

				for (int i = 0; i < 3; i++)
					for (int j = 0; j < 3; j++)
					{
						Pixelf in = input.get(x + i - 1, y + j - 1);
						sumR += in.r * kernel[j * 3 + i];
						sumG += in.g * kernel[j * 3 + i];
						sumB += in.b * kernel[j * 3 + i];
					}

				output.set(x, y, { sumR, sumG, sumB });
			}
	}

	void LowPass(const Frame& input, Frame& output, const float factor)
	{
		for (int y = 0; y < input.height; y++)
			for (int x = 0; x < input.width; x++)
			{
				Pixelf in1 = input.get(x, y);
				Pixelf in2 = output.get(x, y);

				Pixelf out;
				out.r = (in1.r - in2.r) * factor + in2.r;
				out.g = (in1.g - in2.g) * factor + in2.g;
				out.b = (in1.b - in2.b) * factor + in2.b;

				output.set(x, y, out);
			}
	}

	void Adaptive(const Frame& input, Frame& output, const float factor)
	{
		for (int y = 0; y < input.height; y++)
			for (int x = 0; x < input.width; x++)
			{
				float sumR = 0.0f, sumG = 0.0f, sumB = 0.0f;

				for (int i = -2; i <= 2; i++)
					for (int j = -2; j <= 2; j++)
					{
						Pixelf in = input.get(x + i, y + j);
						sumR += in.r; sumG += in.g; sumB += in.b;
					}
				sumR /= 25.0f; sumG /= 25.0f; sumB /= 25.0f;

				Pixelf out, in = input.get(x, y);
				out.r = (in.r > sumR * factor) ? 1.0f : 0.0f;
				out.g = (in.g > sumG * factor) ? 1.0f : 0.0f;
				out.b = (in.b > sumB * factor) ? 1.0f : 0.0f;

				output.set(x, y, out);
			}
	}

	void Sobel(const Frame& input, Frame& output, const float kernelX[9], const float kernelY[9])
	{
		for (int y = 0; y < input.height; y++)
			for (int x = 0; x < input.width; x++)
			{
				float sumXr = 0.0f, sumXg = 0.0f, sumXb = 0.0f;
				float sumYr = 0.0f, sumYg = 0.0f, sumYb = 0.0f;

				for (int i = 0; i < 3; i++)
					for (int j = 0; j < 3; j++)
					{
						Pixelf in = input.get(x + i - 1, y + j - 1);
						size_t k = j * 3 + i;

						sumXr += in.r * kernelX[k];
						sumXg += in.g * kernelX[k];
						sumXb += in.b * kernelX[k];

						sumYr += in.r * kernelY[k];
						sumYg += in.g * kernelY[k];
						sumYb += in.b * kernelY[k];
					}

				Pixelf out;
				out.r = fabs((sumXr + sumYr) * 0.5f);
				out.g = fabs((sumXg + sumYg) * 0.5f);
				out.b = fabs((sumXb + sumYb) * 0.5f);

				output.set(x, y, out);
			}
	}

	void Median(const Frame& input, Frame& output)
	{
		for (int y = 0; y < input.height; y++)
			for (int x = 0; x < input.width; x++)
			{
				std::array<Pixelf, 25> col;

				for (int i = 0; i < 5; i++)
					for (int j = 0; j < 5; j++)
						col[j * 5 + i] = input.get(x + i - 2, y + j - 2);

				std::sort(col.begin(), col.end(), [](const Pixelf& c1, const Pixelf& c2) {
					return c2.r < c1.r && c2.g < c1.g && c2.b < c1.b;
					});

				output.set(x, y, col[12]);
			}
	}

	// takes 2 by 2 dithering values
	void Dithering_FloydSteinberg(const Frame& input, Frame& output, const std::function<void(const Pixelf&, Pixelf&)>& quantise)
	{
		for (int y = 0; y < input.height; y++)
			for (int x = 0; x < input.width; x++)
			{
				Pixelf oldPix = input.get(x, y), newPix;

				quantise(oldPix, newPix);
				output.set(x, y, newPix);

				Pixelf err;
				err.r = oldPix.r - newPix.r;
				err.g = oldPix.g - newPix.g;
				err.b = oldPix.b - newPix.b;

				auto updatePixel = [&](int ox, int oy, size_t i)
				{
					int newX = x + ox;
					int newY = y + oy;

					Pixelf out = input.get(newX, newY);
					out.r += err.r * DITHERING_FLOYDSTEINBERG_KERNEL[i];
					out.g += err.g * DITHERING_FLOYDSTEINBERG_KERNEL[i];
					out.b += err.b * DITHERING_FLOYDSTEINBERG_KERNEL[i];

					output.set(newX, newY, out);
				};

				updatePixel(1, 0, 0);
				updatePixel(-1, 1, 1);
				updatePixel(0, 1, 2);
				updatePixel(1, 1, 3);
			}
	}

	void Dithering_Ordered(const Frame& input, Frame& output, const std::vector<float>& kernel, const size_t kernelWidth, const size_t kernelHeight)
	{
		float kernelSize = float(kernelWidth * kernelHeight);

		for (int y = 0; y < input.height; y++)
			for (int x = 0; x < input.width; x++)
			{
				int wx = x % kernelWidth;
				int wy = y % kernelHeight;

				Pixelf oldPix = input.get(x, y), newPix;

				newPix.r = round(kernelSize * sqrt(oldPix.r)) * kernel[wy * kernelWidth + wx];
				newPix.g = round(kernelSize * sqrt(oldPix.g)) * kernel[wy * kernelWidth + wx];
				newPix.b = round(kernelSize * sqrt(oldPix.b)) * kernel[wy * kernelWidth + wx];

				output.set(x, y, newPix);
			}
	}

	void Ripple(const Frame& input, Frame& output, const float accTime, const float centerX, const float centerY)
	{
		auto lerp = [](float x, float y, float t)
		{
			return x * (1.0f - t) + y * t;
		};

		float fw = (float)input.width;
		float fh = (float)input.height;

		for (int y = 0; y < input.height; y++)
			for (int x = 0; x < input.width; x++)
			{
				float u = (float)x / fw;
				float v = (float)y / fh;

				float cx = u * centerX;
				float cy = v * centerY;

				float l = sqrt(cx * cx + cy * cy);

				u += (cx / l) * lerp(cos(l * 12.0f - accTime * 4.0f) * 0.03f, 0.0f, l / 0.25f);
				v += (cy / l) * lerp(cos(l * 12.0f - accTime * 4.0f) * 0.03f, 0.0f, l / 0.25f);

				output.set(x, y, input.get(int(u * fw), int(v * fh)));
			}
	}

	void CRT(const Frame& input, Frame& output, const Pixelf& gun, const Pixelf& skew, const Pixelf& align)
	{
		Pixelf workingGun = gun;

		for (int scanline = 0; scanline < input.height; scanline++)
		{
			workingGun = gun;
			
			for (int pixel = 0; pixel < input.width; pixel++)
			{
				Pixelf inRed = input.get(pixel + align.r, scanline);
				Pixelf inGreen = input.get(pixel + align.g, scanline);
				Pixelf inBlue = input.get(pixel + align.b, scanline);

				workingGun.r += (inRed.r - workingGun.r) * skew.r;
				workingGun.g += (inGreen.g - workingGun.g) * skew.g;
				workingGun.b += (inBlue.b - workingGun.b) * skew.b;

				output.set(pixel, scanline, workingGun);
			}
		}
	}

#endif
}

#endif
