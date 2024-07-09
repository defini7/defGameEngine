/*
*   BSD 3-Clause License
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

#define DGE_APPLICATION
#include "defGameEngine.hpp"

struct Point
{
	def::vf2d coord;
	float length;
};

struct Spline
{
	static constexpr float STEP_SIZE = 0.005f;

	std::vector<Point> points;
	float fLength = 0.0f;

	Point GetPoint(float fPoint, bool bLooped = false, float fCurvation = 0.5f, float fTension = 0.0f)
	{
		std::vector<Point> seg;
		Get(seg, fPoint, bLooped, fCurvation, fTension);

		fPoint -= (int)fPoint;

		Point p = { seg[0].coord * fPoint * fPoint * fPoint + seg[1].coord * fPoint * fPoint + seg[2].coord * fPoint + seg[3].coord };
		return p;
	}

	Point GetGradient(float fPoint, bool bLooped = false, float fCurvation = 0.5f, float fTension = 0.0f)
	{
		std::vector<Point> grad;
		Get(grad, fPoint, bLooped, fCurvation, fTension);

		fPoint -= (int)fPoint;

		Point p = { 3.0f * grad[0].coord * fPoint * fPoint + 2.0f * grad[1].coord * fPoint + grad[2].coord };
		return p;
	}

	float CalculateLength(int nNode, bool bLooped = false)
	{
		float fLength = 0.0f;

		Point old_point, new_point;
		old_point = GetPoint((float)nNode, bLooped);

		for (float t = 0.0f; t < 1.0f; t += STEP_SIZE)
		{
			new_point = GetPoint((float)nNode + t, bLooped);
			fLength += (new_point.coord - old_point.coord).mag();
			old_point = new_point;
		}

		return fLength;
	}

	float GetOffset(float fPoint)
	{
		int i = 0;
		while (fPoint > points[i].length)
		{
			fPoint -= points[i].length;
			i++;
		}

		return (float)i + fPoint / points[i].length;
	}

private:
	void Get(std::vector<Point>& data, float fPoint, bool bLooped = false, float fCurvation = 0.5f, float fTension = 0.0f)
	{
		int p[4];

		if (bLooped)
		{
			p[0] = (int)fPoint;
			p[1] = (p[0] + 1) % points.size();
			p[2] = (p[1] + 1) % points.size();
			p[3] = (p[0] >= 1.0f) ? p[0] - 1.0f : points.size() - 1;
		}
		else
		{
			for (int i = 0; i < 4; i++)
			{
				p[i] = (int)fPoint + i;

				if (p[i] >= points.size())
					p[i] = points.size() - 1;
			}
		}

		std::vector<float> t =
		{
			pow((points[p[0]].coord - points[p[1]].coord).mag(), fCurvation),
			pow((points[p[1]].coord - points[p[2]].coord).mag(), fCurvation),
			pow((points[p[2]].coord - points[p[3]].coord).mag(), fCurvation),
		};

		std::vector<def::vf2d> m =
		{
			(1.0f - fTension) * (points[p[2]].coord - points[p[1]].coord + t[1] * ((points[p[1]].coord - points[p[0]].coord) / t[0] - (points[p[2]].coord - points[p[0]].coord) / (t[0] + t[1]))),
			(1.0f - fTension) * (points[p[2]].coord - points[p[1]].coord + t[1] * ((points[p[3]].coord - points[p[2]].coord) / t[2] - (points[p[3]].coord - points[p[1]].coord) / (t[1] + t[2]))),
		};

		data =
		{
			{ 2.0f * (points[p[1]].coord - points[p[2]].coord) + m[0] + m[1] },
			{ -3.0f * (points[p[1]].coord - points[p[2]].coord) - 2.0f * m[0] - m[1] },
			{ m[0] },
			{ points[p[1]] }
		};
	}
};

class Example : public def::GameEngine
{
public:
	Example()
	{
		SetTitle("Splines!");
	}

	Spline path;
	size_t nSelectedPoint = 0;

	float fMoveSpeed = 50.0f;
	float fMarker = 0.0f;

protected:
	bool OnUserCreate() override
	{
		/*path.points =
		{
			{10.0f, 100.0f},
			{20.0f, 100.0f},
			{30.0f, 100.0f},
			{40.0f, 100.0f},
			{50.0f, 100.0f},
			{60.0f, 100.0f},
			{70.0f, 100.0f},
			{80.0f, 100.0f},
			{90.0f, 100.0f},
		};*/

		for (int i = 0; i < 10; i++)
		{
			float x = 30.0f * sinf((float)i / 10.0f * 2.0f * 3.14159f) + ScreenWidth() / 2;
			float y = 30.0f * cosf((float)i / 10.0f * 2.0f * 3.14159f) + ScreenHeight() / 2;

			path.points.push_back({ { x, y } });
		}

		return true;
	}

	bool OnUserUpdate(float fDeltaTime) override
	{
		if (GetKey(def::Key::Z).bPressed)
			nSelectedPoint--;

		if (GetKey(def::Key::X).bPressed)
			nSelectedPoint++;
		
		nSelectedPoint %= path.points.size();

		if (GetKey(def::Key::UP).bHeld)
			path.points[nSelectedPoint].coord.y -= fMoveSpeed * fDeltaTime;

		if (GetKey(def::Key::DOWN).bHeld)
			path.points[nSelectedPoint].coord.y += fMoveSpeed * fDeltaTime;

		if (GetKey(def::Key::LEFT).bHeld)
			path.points[nSelectedPoint].coord.x -= fMoveSpeed * fDeltaTime;

		if (GetKey(def::Key::RIGHT).bHeld)
			path.points[nSelectedPoint].coord.x += fMoveSpeed * fDeltaTime;

		fMarker += fMoveSpeed * fDeltaTime;

		Clear(def::GREEN);

		for (float t = 0.0f; t < (float)path.points.size(); t += Spline::STEP_SIZE)
		{
			Point p = path.GetPoint(t, true);
			Point g = path.GetGradient(t, true);
			
			float r = atan2f(-g.coord.y, g.coord.x);

			def::vf2d vDir = 5.0f * def::vf2d(sin(r), cos(r));

			DrawLine(p.coord + vDir, p.coord - vDir, def::GREY);
			Draw(p.coord, def::WHITE);
		}

		path.fLength = 0.0f;

		for (int i = 0; i < path.points.size(); i++)
		{
			path.points[i].length = path.CalculateLength(i, true);
			path.fLength += path.points[i].length;
		}

		for (const auto& p : path.points)
			DrawCircle(p.coord, 1, def::RED);

		DrawCircle(path.points[nSelectedPoint].coord, 1, def::YELLOW);

		if (fMarker >= path.fLength)
			fMarker -= path.fLength;

		if (fMarker < 0.0f)
			fMarker += path.fLength;

		float fOffset = path.GetOffset(fMarker);
		Point marker = path.GetPoint(fOffset, true);
		Point gradient = path.GetGradient(fOffset, true);

		float r = atan2f(-gradient.coord.y, gradient.coord.x);

		def::vf2d vDir = 5.0f * def::vf2d(sin(r), cos(r));

		DrawLine(marker.coord + vDir, marker.coord - vDir, def::DARK_BLUE);

		return true;
	}
};

int main()
{
	Example demo;

	demo.Construct(160, 120, 8, 8);
	demo.Run();

	return 0;
}
