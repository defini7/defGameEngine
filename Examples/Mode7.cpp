/*
* BSD 3-Clause License
Copyright (c) 2023, Alex
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
#include "../defGameEngine.h"

class Mode7 : public def::GameEngine
{
public:
	Mode7()
	{
		SetTitle("Mode7");
	}

private:
	def::Sprite* kart = nullptr;
	def::Sprite* sky = nullptr;

public:
	bool OnUserCreate() override
	{
		kart = new def::Sprite("kart.png");
		sky = new def::Sprite("sky.png");

		return true;
	}

	bool OnUserUpdate(float deltaTime) override
	{
		if (GetKey(def::Key::UP).held) scale += 10.0f * deltaTime;
		if (GetKey(def::Key::DOWN).held) scale -= 10.0f * deltaTime;

		if (GetKey(def::Key::LEFT).held) theta -= deltaTime;
		if (GetKey(def::Key::RIGHT).held) theta += deltaTime;

		def::vf2d velocity = { 0.0f, 0.0f };

		if (GetKey(def::Key::W).held) velocity += def::vf2d(-sin(theta), cos(theta));
		if (GetKey(def::Key::A).held) velocity += def::vf2d(cos(theta), sin(theta));
		if (GetKey(def::Key::S).held) velocity += def::vf2d(sin(theta), -cos(theta));
		if (GetKey(def::Key::D).held) velocity += def::vf2d(-cos(theta), -sin(theta));

		camera += velocity * deltaTime;

		def::vf2d screen;
		for (screen.x = 0; screen.x < ScreenWidth(); screen.x++)
		{
			for (screen.y = ScreenHeight() / 2; screen.y < ScreenHeight(); screen.y++)
			{
				def::vf2d window = def::vf2d(ScreenSize()) / 2.0f + screen * def::vf2d(-1.0f, 1.0f);
				float windowZ = screen.y - ScreenHeight() / 2;

				float rotatedX = window.x * cos(theta) - window.y * sin(theta);
				float rotatedY = window.x * sin(theta) + window.y * cos(theta);

				def::vi2d vPixel = (def::vf2d(rotatedX, rotatedY) / windowZ + camera) * scale;

				def::Pixel kartCol = kart->GetPixel(vPixel.x % kart->width, vPixel.y % kart->height);
				def::Pixel skyCol = sky->GetPixel(vPixel.x % sky->width, vPixel.y % sky->height);

				Draw(screen, kartCol);
				Draw(def::vf2d(screen.x, ScreenHeight() - screen.y - 1), skyCol);
			}
		}

		for (int x = 0; x < ScreenWidth(); x++) Draw(x, ScreenHeight() / 2, def::CYAN);

		return true;
	}

private:
	def::vf2d camera = { 0.0f, 0.0f };
	def::vf2d scale = { 100.0f, 100.0f };
	float theta = 0.0f;

};

int main()
{
	Mode7 app;

	app.Construct(256, 240, 4, 4);
	app.Run();

	return 0;
}
