#pragma once

#pragma region license
/***
*	BSD 3-Clause License
	Copyright (c) 2021, 2022 Alex
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

#ifdef DGE_PHYSICS

namespace def
{
	struct sShape
	{
		vf2d vVelocity;
		vf2d vPos;
	};

	struct sPoint : sShape {};

	struct sRectangle : sShape
	{
		sRectangle() = default;
		sRectangle(vf2d& v, vf2d& p, vf2d& s)
		{
			vVelocity = v;
			vPos = p;
			vSize = s;

			contact[0] = nullptr;
			contact[1] = nullptr;
			contact[2] = nullptr;
			contact[3] = nullptr;
		}

		vf2d vSize;

		sRectangle* contact[4];
	};

	struct sCircle : sShape
	{
		sCircle() = default;
		sCircle(vf2d& v, vf2d& p, float r)
		{
			vVelocity = v;
			vPos = p;
			fRadius = r;
		}

		float fRadius;
	};

	struct sLine : sShape
	{
		sLine() = default;
		sLine(vf2d& v, vf2d& p, vf2d& s)
		{
			vVelocity = v;
			vPos = p;
			vSize = s;
		}

		vf2d vSize;
	};

	class DGE_Physics
	{
	public:
		bool PointVsCircle(const sPoint& p, const sCircle& c)
		{
			return (c.vPos - p.vPos).mag2() <= c.fRadius * c.fRadius;
		}

		bool PointVsRectangle(const sPoint& p, const sRectangle& r)
		{
			return p.vPos > r.vPos - 1 && p.vPos <= r.vPos + r.vSize;
		}

		bool RectVsRect(const sRectangle& r1, const sRectangle& r2)
		{
			return r1.vPos - 1 < r2.vPos + r2.vSize && r1.vPos + r1.vSize + 1 > r2.vPos;
		}

		bool LineVsRect(const sLine& l, const sRectangle& r, vf2d& contact_point, vf2d& contact_normal, float& t_hit_near)
		{
			contact_normal = { 0,0 };
			contact_point = { 0,0 };

			vf2d invdir = vf2d(1.0f, 1.0f) / l.vSize;

			vf2d t_near = (r.vPos - l.vPos) * invdir;
			vf2d t_far = (r.vPos + r.vSize - l.vPos) * invdir;

			if (isnan(t_far.y) || isnan(t_far.x)) return false;
			if (isnan(t_near.y) || isnan(t_near.x)) return false;

			if (t_near.x > t_far.x) std::swap(t_near.x, t_far.x);
			if (t_near.y > t_far.y) std::swap(t_near.y, t_far.y);

			if (t_near.x > t_far.y || t_near.y > t_far.x) return false;

			t_hit_near = std::max(t_near.x, t_near.y);

			float t_hit_far = std::min(t_far.x, t_far.y);

			if (t_hit_far < 0.0f)
				return false;

			contact_point = l.vPos + l.vSize * t_hit_near;

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
			return LineVsRect(l, r, contact_point, contact_normal, t_hit_near) && t_hit_near < 1.0f;
		}

		bool DynamicRectVsRect(const sRectangle& r_dynamic, const float fTimeStep, const sRectangle& r_static,
			vf2d& contact_point, vf2d& contact_normal, float& contact_time)
		{
			if (r_dynamic.vVelocity.x == 0 && r_dynamic.vVelocity.y == 0)
				return false;

			sRectangle expanded_target;

			expanded_target.vPos = r_static.vPos - r_dynamic.vSize / 2;
			expanded_target.vSize = r_static.vSize + r_dynamic.vSize;

			sLine l;
			l.vPos = r_dynamic.vPos + r_dynamic.vSize / 2;
			l.vSize = r_dynamic.vVelocity * fTimeStep;

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
	};
}

#endif
