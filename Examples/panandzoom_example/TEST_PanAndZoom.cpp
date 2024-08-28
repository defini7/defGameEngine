/*
*	BSD 3-Clause License

	Copyright (c) 2024, Alex

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

#define DEF_GEOMETRY2D_IMPL
#include "defGeometry2D.hpp"

#define DGE_IGNORE_VEC2D
#define DGE_APPLICATION
#include "defGameEngine.hpp"

#define DGE_PAN_AND_ZOOM
#include "DGE_PanAndZoom.hpp"

class Engine3D : public def::GameEngine
{
public:
	Engine3D()
	{
		SetTitle("Math library");
		UseOnlyTextures(true);
	}

	def::circle<float> c;
	def::line<float> l;

	def::PanAndZoom pz;

protected:
	bool OnUserCreate() override
	{
		c.pos = { 512, 480 };
		c.radius = 150.0f;

		l.start = { 200, 300 };
		l.end = { 700, 700 };

		return true;
	}

	bool OnUserUpdate(float dt) override
	{
		if (GetMouse(def::Button::WHEEL).pressed)
			pz.StartPan(GetMousePos());

		if (GetMouse(def::Button::WHEEL).held)
			pz.UpdatePan(GetMousePos());

		if (GetMouseWheelDelta() > 0)
			pz.Zoom(1.1f, GetMousePos());

		if (GetMouseWheelDelta() < 0)
			pz.Zoom(0.9f, GetMousePos());

		def::vf2d mouse = pz.ScreenToWorld(GetMousePos());

		l.end = mouse;

		/*for (int i = 0; i < 4; i++)
		{
			def::line<float> l = r.side(i);

			DrawLine(l.start, l.end, def::YELLOW);
		}*/

		ClearTexture(def::DARK_BLUE);

		auto res = intersects(c, l);

		/*if (res)
		{
			DrawString(0, 0, "It does!");
		}*/

		pz.DrawTextureLine(l.start, l.end);
		pz.DrawTextureCircle(c.pos, c.radius);

		if (res.size() > 0)
		{
			for (const auto& p : res)
				pz.FillTextureCircle(p, 10, def::RED);
		}

		DrawTextureString({ 0, 0 }, "Screen Mouse: " + mouse.str(), def::YELLOW, {3.0f, 3.0f});
		DrawTextureString({ 0, 60 }, "Offset: " + pz.GetOffset().str(), def::YELLOW, { 3.0f, 3.0f });
		DrawTextureString({ 0, 120 }, "Scale: " + pz.GetScale().str(), def::YELLOW, { 3.0f, 3.0f });
		DrawTextureString({ 0, 180 }, pz.IsVisible(c.pos) ? "Visible" : "Invisible", def::YELLOW, {3.0f, 3.0f});

		pz.FillTextureCircle(mouse, 10, def::PURPLE);

		return true;
	}
};

int main()
{
	Engine3D demo;

	demo.Construct(1024, 960, 1, 1);
	demo.Run();

	return 0;
}
