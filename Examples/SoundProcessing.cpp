#include "defGameEngineSDL.h"

enum WAVE_TYPE
{
	WT_NONE,
	WT_SINE,
	WT_SQUARE,
	WT_TRIANGLE,
	WT_NOISE,
	WT_SAW_ANA,
	WT_SAW_DIG
};

class SoundProcessing : public def::GameEngine
{
public:
	SoundProcessing()
	{
		SetTitle("Sound Processing");
		EnableSound();
	}

private:
	WAVE_TYPE wtWave = WT_NONE;
	
	float fCustom = 50.0f;
	float fFrequency = 44100.0f;

protected:
	float ApproxSin(float x)
	{
		float t = x / (2.0f * 3.141592653f);
		t -= (int)t;

		return (t < 0.5f) ?
			-16 * t * t + 8 * t :
			16 * t * t - 16 * t - 8 * t + 8;
	}

	bool OnUserCreate() override
	{
		return true;
	}

	bool OnUserUpdate(float fDeltaTime) override
	{
		Clear(def::BLACK);

		wtWave = WT_NONE;

		if (GetKey(def::Key::S).bHeld)
			wtWave = WT_SINE;

		if (GetKey(def::Key::F).bHeld)
			wtWave = WT_SQUARE;

		if (GetKey(def::Key::G).bHeld)
			wtWave = WT_TRIANGLE;

		if (GetKey(def::Key::J).bHeld)
			wtWave = WT_NOISE;

		if (GetKey(def::Key::K).bHeld)
			wtWave = WT_SAW_ANA;
		
		if (GetKey(def::Key::L).bHeld)
			wtWave = WT_SAW_DIG;

		if (GetKey(def::Key::Z).bHeld)
			fCustom += fDeltaTime;

		if (GetKey(def::Key::X).bHeld)
			fCustom -= fDeltaTime;

		if (GetKey(def::Key::C).bHeld)
			fFrequency += 10.0f * fDeltaTime;

		if (GetKey(def::Key::V).bHeld)
			fFrequency -= 10.0f * fDeltaTime;

		DrawString(2, 8,  "|   |   |   |   |   | |   |   |   |   | |   | |   |   |   |  ");
		DrawString(2, 16, "|   | S |   |   | F | | G |   |   | J | | K | | L |   |   |  ");
		DrawString(2, 24, "|   |___|   |   |___| |___|   |   |___| |___| |___|   |   |__");
		DrawString(2, 32, "|     |     |     |     |     |     |     |     |     |     |");
		DrawString(2, 48, "|  Z  |  X  |  C  |  V  |     |     |     |     |     |     |");
		DrawString(2, 64, "|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|");

		DrawString(2, 96, "Frequency: " + std::to_string(fFrequency));
		DrawString(2, 104, "Custom: " + std::to_string(fCustom));

		return true;
	}

	virtual float OnUserSoundSample(int nChannel, float fGlobalTime, float fTimeStep) override
	{
		switch (wtWave)
		{
		case WT_NONE: return 0.0f;
		case WT_SINE: return ApproxSin(fFrequency * fGlobalTime);
		case WT_SQUARE: return ApproxSin(fFrequency * fGlobalTime) > 0.0f ? 1.0f : -1.0f;
		case WT_TRIANGLE: return asinf(ApproxSin(fFrequency * fGlobalTime)) * (2.0f / 3.1415926535f);
		case WT_NOISE: return (2.0f * ((float)rand() / (float)RAND_MAX) - 1.0f);
		case WT_SAW_ANA:
		{
			float fOut = 0.0f;
			for (float n = 1.0f; n < fCustom; n++)
				fOut += ApproxSin(n * fFrequency * fGlobalTime) / n;
			return fOut * (2.0f / 3.1415926535f) * fGlobalTime;
		}
		case WT_SAW_DIG:
		{
			return (2.0f / 3.1415926535f) * (fFrequency * fGlobalTime * 3.1415926535f * fmod(fTimeStep, 1.0f / fFrequency) - (3.1415926535f / 2.0f));
		}
		}
	}

	virtual float OnUserSoundFilter(int nChannel, float fGlobalTime, float fSample) override
	{
		return fSample;
	}

};

int main()
{
	SoundProcessing demo;
	def::rcode err = demo.Construct(640, 480, 1, 1);

	if (err.ok)
		demo.Run();
	else
		std::cerr << err.info << "\n";

	return 0;
}
