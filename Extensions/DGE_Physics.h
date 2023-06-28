#pragma once

#pragma region license
/***
*	BSD 3-Clause License
	Copyright (c) 2021, 2022, 2023 Alex
	All rights reserved.
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
***/
#pragma endregion

namespace def
{
	struct Shape
	{
		vf2d velocity;
		vf2d pos;
	};

	struct Point : Shape {};

	struct Rectangle : Shape
	{
		Rectangle() = default;
		Rectangle(const vf2d& v, const vf2d& p, const vf2d& s);

		vf2d size;

		Rectangle* contact[4];
	};

	struct Circle : Shape
	{
		Circle() = default;
		Circle(const vf2d& v, const vf2d& p, float r);

		float radius;
	};

	struct Line : Shape
	{
		Line() = default;
		Line(const vf2d& v, const vf2d& p, const vf2d& s);

		vf2d size;
	};

	class Physics
	{
	public:
		bool PointVsCircle(const Point& p, const Circle& c);
		bool PointVsRect(const Point& p, const Rectangle& r);
		bool RectVsRect(const Rectangle& r1, const Rectangle& r2);
		bool LineVsRect(const Line& l, const Rectangle& r, vf2d& contactPoint, vf2d& contactNormal, float& hitTime);

		bool ResolveLineVsRect(const Line& l, const Rectangle& r, vf2d& contactPoint, vf2d& contactNormal);
		bool DynamicRectVsRect(const Rectangle& dynamicRect, const float timeStep, const Rectangle& staticRect,
			vf2d& contactPoint, vf2d& contactNormal, float& contactTime);
		bool ResolveDynamicRectVsRect(Rectangle& dynamicRect, const float timeStep, Rectangle* staticRect);
	};

#ifdef DGE_PHYSICS
#undef DGE_PHYSICS

	Rectangle::Rectangle(const vf2d& v, const vf2d& p, const vf2d& s)
	{
		velocity = v;
		pos = p;
		size = s;

		contact[0] = nullptr;
		contact[1] = nullptr;
		contact[2] = nullptr;
		contact[3] = nullptr;
	}

	Circle::Circle(const vf2d& v, const vf2d& p, float r)
	{
		velocity = v;
		pos = p;
		radius = r;
	}

	Line::Line(const vf2d& v, const vf2d& p, const vf2d& s)
	{
		velocity = v;
		pos = p;
		size = s;
	}

	bool Physics::PointVsCircle(const Point& p, const Circle& c)
	{
		return (c.pos - p.pos).mag2() <= c.radius * c.radius;
	}

	bool Physics::PointVsRect(const Point& p, const Rectangle& r)
	{
		return p.pos > r.pos - 1 && p.pos <= r.pos + r.size;
	}

	bool Physics::RectVsRect(const Rectangle& r1, const Rectangle& r2)
	{
		return r1.pos - 1 < r2.pos + r2.size && r1.pos + r1.size + 1 > r2.pos;
	}

	bool Physics::LineVsRect(const Line& l, const Rectangle& r, vf2d& contactPoint, vf2d& contactNormal, float& hitTime)
	{
		contactNormal = { 0,0 };
		contactPoint = { 0,0 };

		vf2d invdir = vf2d(1.0f, 1.0f) / l.size;

		vf2d timeNear = (r.pos - l.pos) * invdir;
		vf2d timeFar = (r.pos + r.size - l.pos) * invdir;

		if (isnan(timeFar.y) || isnan(timeFar.x)) return false;
		if (isnan(timeNear.y) || isnan(timeNear.x)) return false;

		if (timeNear.x > timeFar.x) std::swap(timeNear.x, timeFar.x);
		if (timeNear.y > timeFar.y) std::swap(timeNear.y, timeFar.y);

		if (timeNear.x > timeFar.y || timeNear.y > timeFar.x) return false;

		hitTime = std::max(timeNear.x, timeNear.y);

		float t_hitimeFar = std::min(timeFar.x, timeFar.y);

		if (t_hitimeFar < 0.0f)
			return false;

		contactPoint = l.pos + l.size * hitTime;

		if (timeNear.x > timeNear.y)
			if (invdir.x < 0)
				contactNormal = { 1, 0 };
			else
				contactNormal = { -1, 0 };
		else if (timeNear.x < timeNear.y)
			if (invdir.y < 0)
				contactNormal = { 0, 1 };
			else
				contactNormal = { 0, -1 };

		return true;
	}

	bool Physics::ResolveLineVsRect(const Line& l, const Rectangle& r, vf2d& contactPoint, vf2d& contactNormal)
	{
		float hitTime;
		return LineVsRect(l, r, contactPoint, contactNormal, hitTime) && hitTime < 1.0f;
	}

	bool Physics::DynamicRectVsRect(const Rectangle& dynamicRect, const float timeStep, const Rectangle& staticRect,
		vf2d& contactPoint, vf2d& contactNormal, float& contact_time)
	{
		if (dynamicRect.velocity.x == 0 && dynamicRect.velocity.y == 0)
			return false;

		Rectangle expanded_target;

		expanded_target.pos = staticRect.pos - dynamicRect.size / 2;
		expanded_target.size = staticRect.size + dynamicRect.size;

		Line l;
		l.pos = dynamicRect.pos + dynamicRect.size / 2;
		l.size = dynamicRect.velocity * timeStep;

		if (LineVsRect(l, expanded_target, contactPoint, contactNormal, contact_time))
			return (contact_time >= 0.0f && contact_time < 1.0f);
		else
			return false;
	}

	bool Physics::ResolveDynamicRectVsRect(Rectangle& dynamicRect, const float timeStep, Rectangle* staticRect)
	{
		vf2d contactPoint, contactNormal;
		float contactTime = 0.0f;

		if (DynamicRectVsRect(dynamicRect, timeStep, *staticRect, contactPoint, contactNormal, contactTime))
		{
			if (contactNormal.y > 0) dynamicRect.contact[0] = staticRect; else nullptr;
			if (contactNormal.x < 0) dynamicRect.contact[1] = staticRect; else nullptr;
			if (contactNormal.y < 0) dynamicRect.contact[2] = staticRect; else nullptr;
			if (contactNormal.x > 0) dynamicRect.contact[3] = staticRect; else nullptr;

			dynamicRect.velocity += contactNormal * dynamicRect.velocity.abs() * (1 - contactTime);

			return true;
		}

		return false;
	}
	
#endif

}
