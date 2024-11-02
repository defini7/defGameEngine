#define DGE_APPLICATION
#include "../defGameEngine.hpp"

class Timer final : public def::GameEngine
{
public:
	Timer()
	{
		SetTitle("Timer");
	}

	std::chrono::system_clock::time_point begin;
	std::chrono::system_clock::time_point pausedTime;

	bool enableMilliseconds = false;
	bool isPaused = false;

protected:
	bool OnUserCreate() override
	{
		begin = std::chrono::system_clock::now();
		
		return true;
	}

	bool OnUserUpdate(float deltaTime) override
	{
		if (GetKey(def::Key::SPACE).pressed)
		{
			isPaused = !isPaused;

			if (isPaused)
				pausedTime = std::chrono::system_clock::now();
			else
				begin += std::chrono::system_clock::now() - pausedTime;
		}

		if (GetKey(def::Key::ENTER).pressed)
			enableMilliseconds = !enableMilliseconds;

		if (!isPaused)
		{
			auto diff = std::chrono::system_clock::now() - begin;

			auto GetTime = [&diff](auto t)
				{
					auto time = std::chrono::duration_cast<decltype(t)>(diff);
					diff -= time;

					return time;
				};

			auto years = GetTime(std::chrono::years());
			auto days = GetTime(std::chrono::days());
			auto hours = GetTime(std::chrono::hours());
			auto minutes = GetTime(std::chrono::minutes());
			auto seconds = GetTime(std::chrono::seconds());

			auto FixValue = [](int value)
				{
					if (value < 10)
						return "0" + std::to_string(value);

					if (value > 10)
						return std::to_string(value).substr(0, 2);

					return std::to_string(value);
				};

			std::string s;

			if (years.count() != 0)
				s += FixValue(years.count()) + ":";

			if (days.count() != 0)
				s += FixValue(days.count()) + ":";

			s += FixValue(hours.count()) + ":";
			s += FixValue(minutes.count()) + ":";
			s += FixValue(seconds.count());

			if (enableMilliseconds)
			{
				auto milliseconds = GetTime(std::chrono::milliseconds());
				s += "." + FixValue(milliseconds.count());
			}

			Clear(def::BLACK);

			DrawString(10, 10, s);
		}

		return true;
	}
};

int main()
{
	Timer timer;

	if (timer.Construct(106, 30, 6, 6))
		timer.Run();

	return 0;
}
