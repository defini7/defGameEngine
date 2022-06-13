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
		def::vf2d vStart;
		def::vf2d vVelocity;
	};

	struct sPoint : sShape {};

	struct sRectangle : sShape
	{
		def::vf2d vEnd;
	};

	struct sCircle : sShape
	{
		float fRadius;
	};

	struct sLine : sShape
	{
		def::vf2d vEnd;
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

		bool LineVsRect(const sLine& l, const sRectangle& r, def::vf2d& contact_point, def::vf2d& contact_normal, float& t_hit_near)
		{
			contact_normal = { 0,0 };
			contact_point = { 0,0 };

			def::vf2d invdir = def::vf2d(1.0f, 1.0f) / l.vEnd;

			def::vf2d t_near = (r.vStart - l.vStart) * invdir;
			def::vf2d t_far = (r.vEnd - l.vStart) * invdir;

			if (isnan(t_far.y) || isnan(t_far.x)) return false;
			if (isnan(t_near.y) || isnan(t_near.x)) return false;

			if (t_near.x > t_far.x) std::swap(t_near.x, t_far.x);
			if (t_near.y > t_far.y) std::swap(t_near.y, t_far.y);

			if (t_near.x > t_far.y || t_near.y > t_far.x) return false;

			t_hit_near = std::max(t_near.x, t_near.y);

			float t_hit_far = std::min(t_far.x, t_far.y);

			if (t_hit_far < 0)
				return false;

			contact_point = l.vStart + def::vf2d(t_hit_near, t_hit_near) * l.vEnd;

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

		bool ResolveLineVsRect(const sLine& l, const sRectangle& r, def::vf2d& contact_point, def::vf2d& contact_normal)
		{
			float t_hit_near;
			return LineVsRect(l, r, contact_point, contact_normal, t_hit_near) && t_hit_near < 0.9f && t_hit_near > 0.0f;
		}
	};
}
