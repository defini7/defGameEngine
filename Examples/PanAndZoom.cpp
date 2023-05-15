/*
* BSD 3-Clause License
Copyright (c) 2023, Алекс
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#define DGE_APPLICATION
#include "defGameEngine.h"

class Sample : public def::GameEngine
{
public:
	Sample()
	{
		SetTitle("Sample");
		ShowFPS();
	}

private:
	def::vf2d vScale;
	def::vf2d vOffset;

	def::vf2d vStartPan;

protected:
	bool OnUserCreate() override
	{
		vScale = { 1.0f, 1.0f };
		vOffset = { 0.0f, 0.0f };

		return true;
	}

	def::vi2d WorldToScreen(const def::vf2d& vWorld)
	{
		return ((def::vf2d)vWorld - vOffset) * vScale;
	}

	def::vf2d ScreenToWorld(const def::vi2d& vScreen)
	{
		return (def::vf2d)vScreen / vScale + vOffset;
	}

	bool OnUserUpdate(float fDeltaTime) override
	{
		def::vf2d vMouse = GetMouse();

		if (GetMouse(2).bPressed)
			vStartPan = vMouse;

		if (GetMouse(2).bHeld)
		{
			vOffset -= (vMouse - vStartPan) / vScale;
			vStartPan = vMouse;
		}

		def::vf2d vBeforeZoom = ScreenToWorld(vMouse);

		if (GetKey(def::Key::EQUAL).bHeld)
			vScale *= 1.001f;

		if (GetKey(def::Key::MINUS).bHeld)
			vScale *= 0.999f;

		vScale.x = std::max(vScale.x, 1.0f);
		vScale.y = std::max(vScale.y, 1.0f);

		def::vf2d vAfterZoom = ScreenToWorld(vMouse);
		vOffset += vBeforeZoom - vAfterZoom;

		Clear(def::BLACK);

		def::vi2d vLineLeft = WorldToScreen({ 0.0f, 0.5f * (float)ScreenHeight() });
		def::vi2d vLineRight = WorldToScreen({ (float)ScreenWidth(), 0.5f * (float)ScreenHeight() });

		def::vi2d vLineTop = WorldToScreen({ 0.5f * (float)ScreenWidth(), 0.0f });
		def::vi2d vLineBottom = WorldToScreen({ 0.5f * (float)ScreenWidth(), (float)ScreenHeight() });

		DrawLine(vLineLeft, vLineRight);
		DrawLine(vLineTop, vLineBottom);

		def::vf2d vWorldTopLeft = ScreenToWorld({ 0, 0 });
		def::vf2d vWorldTopRight = ScreenToWorld(ScreenSize());

		def::vf2d vWorldMouse = vAfterZoom;

		def::vf2d vWorldPerScreenSizePixel = (vWorldTopRight - vWorldTopLeft) / def::vf2d(ScreenSize());

		auto func = [](float x)
		{
			return sinf(x);
		};

		def::vi2d p, op;
		op = WorldToScreen({ vWorldTopLeft.x - vWorldPerScreenSizePixel.x, -func(vWorldTopLeft.x - vWorldPerScreenSizePixel.x - 0.5f * (float)ScreenWidth()) + 0.5f * (float)ScreenHeight() });

		for (float x = vWorldTopLeft.x; x < vWorldTopRight.x; x += vWorldPerScreenSizePixel.x)
		{
			float y = -func(x - 0.5f * (float)ScreenWidth()) + 0.5f * (float)ScreenHeight();
			p = WorldToScreen({ x, y });
			DrawLine(op, p, def::GREEN);
			op = p;
		}

		DrawString(5, 8, "Offset: " + std::string(vOffset));
		DrawString(5, 18, "Scale: " + std::string(vScale));

		return true;
	}
};

int main()
{
	Sample demo;

	def::rcode err = demo.Construct(256, 240, 4, 4);

	if (err.ok)
		demo.Run();
	else
		std::cerr << err.info << "\n";

	return 0;
}
