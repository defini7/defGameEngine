#pragma once

#pragma region license
/***
*	BSD 3-Clause License
	Copyright (c) 2021, Alex
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

#pragma region includes

#include "defGameEngine.h"

#include <algorithm>

#pragma endregion

namespace def
{
	struct sShape
	{
		vf2d vVelocity;
		vf2d vStart;
	};

	struct sPoint : sShape {};

	struct sRectangle : sShape
	{
		vf2d vEnd;

		sRectangle* contact[4];
	};

	struct sCircle : sShape
	{
		float fRadius;
	};

	struct sLine : sShape
	{
		vf2d vEnd;
	};

	class DGE_Physics
	{
	public:
		bool PointVsCircle(const sPoint& p, const sCircle& c)
		{
			return (c.vStart - p.vStart).mag2() <= c.fRadius * c.fRadius;
		}

		bool PointVsRectangle(const sPoint& p, const sRectangle& r)
		{
			return p.vStart > r.vStart - 1 && p.vStart < r.vEnd + 1;
		}

		bool RectVsRect(const sRectangle& r1, const sRectangle& r2)
		{
			return r1.vStart - 1 < r2.vEnd && r1.vEnd + 1 > r2.vStart;
		}

		bool LineVsRect(const sLine& l, const sRectangle& r, vf2d& contact_point, vf2d& contact_normal, float& t_hit_near)
		{
			contact_normal = { 0,0 };
			contact_point = { 0,0 };

			vf2d invdir = vf2d(1.0f, 1.0f) / l.vEnd;

			vf2d t_near = (r.vStart - l.vStart) * invdir;
			vf2d t_far = (r.vEnd - l.vStart) * invdir;

			if (isnan(t_far.y) || isnan(t_far.x)) return false;
			if (isnan(t_near.y) || isnan(t_near.x)) return false;

			if (t_near.x > t_far.x) std::swap(t_near.x, t_far.x);
			if (t_near.y > t_far.y) std::swap(t_near.y, t_far.y);

			if (t_near.x > t_far.y || t_near.y > t_far.x) return false;

			t_hit_near = std::max(t_near.x, t_near.y);

			float t_hit_far = std::min(t_far.x, t_far.y);

			if (t_hit_far < 0)
				return false;

			contact_point = l.vStart + vf2d(t_hit_near, t_hit_near) * l.vEnd;

			if (t_near.x > t_near.y)
				if (invdir.x < 0)
					contact_normal = { 1, 0 };
				else
					contact_normal = { -1, 0 };
			else if (t_near.x < t_near.y)
				if (invdir.y < 0)
					contact_normal = { 0, 1 };
				else
					contact_normal = { 0, -1 };

			return true;
		}

		bool ResolveLineVsRect(const sLine& l, const sRectangle& r, vf2d& contact_point, vf2d& contact_normal)
		{
			float t_hit_near;
			return LineVsRect(l, r, contact_point, contact_normal, t_hit_near) && t_hit_near < 0.9f && t_hit_near > 0.0f;
		}

		bool DynamicRectVsRect(const sRectangle& r_dynamic, const float fTimeStep, const sRectangle& r_static,
			vf2d& contact_point, vf2d& contact_normal, float& contact_time)
		{
			if (r_dynamic.vVelocity.x == 0 && r_dynamic.vVelocity.y == 0)
				return false;

			sRectangle expanded_target;

			expanded_target.vStart = r_static.vStart - (r_dynamic.vEnd - r_dynamic.vStart) / 2;
			expanded_target.vEnd = expanded_target.vStart + (r_static.vEnd - r_static.vStart) + (r_dynamic.vEnd - r_dynamic.vStart);

			sLine l;
			l.vStart = r_dynamic.vStart + (r_dynamic.vEnd - r_dynamic.vStart) / 2;
			l.vEnd = r_dynamic.vVelocity * fTimeStep;

			if (LineVsRect(l, expanded_target, contact_point, contact_normal, contact_time))
				return (contact_time >= 0.0f && contact_time < 1.0f);
			else
				return false;
		}

		bool ResolveDynamicRectVsRect(sRectangle& r_dynamic, const float fTimeStep, sRectangle* r_static)
		{
			vf2d contact_point, contact_normal;
			float contact_time = 0.0f;

			if (DynamicRectVsRect(r_dynamic, fTimeStep, *r_static, contact_point, contact_normal, contact_time))
			{
				if (contact_normal.y > 0) r_dynamic.contact[0] = r_static; else nullptr;
				if (contact_normal.x < 0) r_dynamic.contact[1] = r_static; else nullptr;
				if (contact_normal.y < 0) r_dynamic.contact[2] = r_static; else nullptr;
				if (contact_normal.x > 0) r_dynamic.contact[3] = r_static; else nullptr;

				r_dynamic.vVelocity += contact_normal * r_dynamic.vVelocity.abs() * (1 - contact_time);

				return true;
			}

			return false;
		}

	private:
		std::vector<sPoint> vecPoints;
		std::vector<sRectangle> vecRectangles;
		std::vector<sCircle> vecCircles;
		std::vector<sLine> vecLines;

	public:
		std::vector<sPoint>* GetPoints()
		{
			return &vecPoints;
		}

		std::vector<sRectangle>* GetRectangles()
		{
			return &vecRectangles;
		}

		std::vector<sCircle>* GetCircles()
		{
			return &vecCircles;
		}

		std::vector<sLine>* GetLines()
		{
			return &vecLines;
		}

		void AddPoint(sPoint p)
		{
			return vecPoints.push_back(p);
		}

		void AddRectangle(sRectangle r)
		{
			return vecRectangles.push_back(r);
		}

		void AddCircle(sCircle c)
		{
			return vecCircles.push_back(c);
		}

		void AddLine(sLine l)
		{
			return vecLines.push_back(l);
		}

		sPoint& GetPoint(uint32_t id)
		{
			return vecPoints[id];
		}

		sRectangle& GetRectangle(uint32_t id)
		{
			return vecRectangles[id];
		}

		sCircle& GetCircle(uint32_t id)
		{
			return vecCircles[id];
		}

		sLine& GetLine(uint32_t id)
		{
			return vecLines[id];
		}
	};
}
