#define DGE_APPLICATION
#include "../defGameEngine.hpp"

#include <algorithm>

class Sample : public def::GameEngine
{
public:
	Sample()
	{
		SetTitle("Sample");
	}

	def::vf2d scale = { 1.0f, 1.0f };
	def::vf2d offset = { 0, 0 };

	def::vi2d panStart;

	std::vector<def::vf2d> data;

	def::vf2d ScreenToWorld(const def::vi2d& p)
	{
		return (def::vf2d(p) + offset) / scale;
	}

	def::vi2d WorldToScreen(const def::vf2d& p)
	{
		return p * scale - offset;
	}

protected:
	bool OnUserCreate() override
	{
		scale = def::vf2d(ScreenWidth(), ScreenHeight());

		auto randf = [](float min, float max)
		{
			return min + (float)rand() / float(max - min);
		};

		data.resize(100);

		for (int i = 0; i < data.size(); i++)
		{
			data[i] = def::vf2d(i, 1.0f + sin(i));
		}

		return true;
	}

	void Plot(const def::vf2d& pos, const def::vf2d& size, const std::vector<def::vf2d>& data, const def::Pixel& col = def::WHITE)
	{
		auto minX = std::min_element(data.begin(), data.end(), [](const def::vf2d& a, const def::vf2d& b) { return a.x < b.x; });
		auto maxX = std::max_element(data.begin(), data.end(), [](const def::vf2d& a, const def::vf2d& b) { return a.x < b.x; });

		auto minY = std::min_element(data.begin(), data.end(), [](const def::vf2d& a, const def::vf2d& b) { return a.y < b.y; });
		auto maxY = std::max_element(data.begin(), data.end(), [](const def::vf2d& a, const def::vf2d& b) { return a.y < b.y; });

		def::vf2d diff(maxX->x - minX->x, maxY->y - minY->y);
		def::vf2d step(1.0f / diff);

		for (float x = 0.05f; x < size.x - 0.05f; x += step.x)
		{
			def::vi2d p = WorldToScreen({ pos.x + x, pos.y });
			DrawLine(p.x, p.y - 2, p.x, p.y, col);
		}

		for (float y = 0.05f; y < size.x - 0.05f; y += step.y)
		{
			def::vi2d p = WorldToScreen({ pos.x, pos.y + y });
			DrawLine(p.x - 2, p.y, p.x, p.y, col);
		}

		for (const auto& p : data)
		{
			def::vi2d wp = WorldToScreen(pos + def::vf2d(0.05f, 0.05f) + p / diff * (size - 0.1f));
			FillCircle(wp, 2, col);
		}

		def::vi2d p = WorldToScreen(pos);
		DrawRectangle(p, WorldToScreen(pos + size) - p, col);
	}

	void Scatter(const def::vf2d& pos, const def::vf2d& size, std::vector<def::vf2d> data, const def::Pixel& col = def::WHITE)
	{
		auto minX = std::min_element(data.begin(), data.end(), [](const def::vf2d& a, const def::vf2d& b) { return a.x < b.x; });
		auto maxX = std::max_element(data.begin(), data.end(), [](const def::vf2d& a, const def::vf2d& b) { return a.x < b.x; });

		auto minY = std::min_element(data.begin(), data.end(), [](const def::vf2d& a, const def::vf2d& b) { return a.y < b.y; });
		auto maxY = std::max_element(data.begin(), data.end(), [](const def::vf2d& a, const def::vf2d& b) { return a.y < b.y; });

		def::vf2d diff(maxX->x - minX->x, maxY->y - minY->y);
		def::vf2d step(1.0f / diff);

		for (float x = 0.05f; x < size.x - 0.05f; x += step.x)
		{
			def::vi2d p = WorldToScreen({ pos.x + x, pos.y });
			DrawLine(p.x, p.y - 2, p.x, p.y, col);
		}

		for (float y = 0.05f; y < size.x - 0.05f; y += step.y)
		{
			def::vi2d p = WorldToScreen({ pos.x, pos.y + y });
			DrawLine(p.x - 2, p.y, p.x, p.y, col);
		}

		std::sort(data.begin(), data.end(), [](const def::vf2d& a, const def::vf2d& b) { return b.x < a.x; });

		def::vf2d op = WorldToScreen(pos + def::vf2d(0.05f, 0.05f) + data[0] / diff * (size - 0.1f));
		for (size_t i = 1; i < data.size(); i++)
		{
			def::vi2d np = WorldToScreen(pos + def::vf2d(0.05f, 0.05f) + data[i] / diff * (size - 0.1f));
			DrawLine(op, np, col);
			op = np;
		}

		def::vi2d p = WorldToScreen(pos);
		DrawRectangle(p, WorldToScreen(pos + size) - p, col);
	}

	template <typename T>
	void Bar(const def::vf2d& pos, const def::vf2d& size, const std::vector<T>& data, const def::Pixel& col = def::WHITE)
	{
		auto min = std::min_element(data.begin(), data.end(), [](const T& a, const T& b) { return a < b; });
		auto max = std::max_element(data.begin(), data.end(), [](const T& a, const T& b) { return a < b; });

		float diff = *max - *min;
		def::vf2d step = 1.0f / size;

		for (float x = 0.05f; x < size.x - 0.05f; x += step.x)
		{
			def::vi2d p = WorldToScreen({ pos.x + x, pos.y });
			DrawLine(p.x, p.y - 2, p.x, p.y, col);
		}

		for (float y = 0.05f; y < size.x - 0.05f; y += step.y)
		{
			def::vi2d p = WorldToScreen({ pos.x, pos.y + y });
			DrawLine(p.x - 2, p.y, p.x, p.y, col);
		}

		float barWidth = (size.x - 0.1f) / data.size();

		for (size_t i = 0; i < data.size(); i++)
		{
			def::vi2d wp = WorldToScreen(pos + def::vf2d(0.05f, 0.05f) + barWidth * i + def::vf2d(0.0f, size.y));
			FillRectangle(wp, def::vi2d(barWidth, data[i] / diff * size.y), col);
		}

		def::vi2d p = WorldToScreen(pos);
		DrawRectangle(p, WorldToScreen(pos + size) - p, col);
	}

	bool OnUserUpdate(float deltaTime) override
	{
		if (GetMouse(def::Button::LEFT).pressed) panStart = offset + def::vf2d(GetMousePos());
		if (GetMouse(def::Button::LEFT).held) offset = panStart - GetMousePos();

		if (GetKey(def::Key::UP).held) scale *= 1.01f;
		if (GetKey(def::Key::DOWN).held) scale *= 0.99f;

		Clear(def::WHITE);
		//Bar<float>({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.5f, 2.0f, 20.0f, 31.0f, 10.0f, 15.0f }, def::BLACK);
		Plot({ 0.0f, 1.1f }, { 1.0f, 1.0f }, data, def::BLACK);
		Scatter({ 0.0f, 2.2f }, { 1.0f, 1.0f }, data, def::BLACK);

		return true;
	}
};

int main()
{
	Sample demo;

	demo.Construct(800, 600, 1, 1);
	demo.Run();

	return 0;
}
