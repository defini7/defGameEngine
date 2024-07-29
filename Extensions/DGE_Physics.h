#pragma once

#pragma region License
/***
*	BSD 3-Clause License
	Copyright (c) 2021, 2022, 2023, 2024 Alex
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

#include <vector>

namespace def
{
	constexpr double epsilon = 0.001;

	namespace utils
	{
		template <class T1, class T2>
		constexpr auto equal(T1 lhs, T2 rhs);
	}

	template <class T>
	struct circle
	{
		constexpr circle() = default;
		constexpr circle(const vec2d<T>& p, float r);

		vec2d<T> pos;
		float radius;
	};

	template <class T>
	struct line
	{
		constexpr line() = default;
		constexpr line(const vec2d<T>& p1, const vec2d<T>& p2);

		constexpr vec2d<T> vector() const;

		vec2d<T> pos[2];
	};

	template <class T>
	struct rect
	{
		constexpr rect() = default;
		constexpr rect(const vec2d<T>& p, const vec2d<T>& s);

		enum side : uint32_t
		{
			LEFT,
			TOP,
			RIGHT,
			BOTTOM
		};

		constexpr line<T> left() const;
		constexpr line<T> top() const;
		constexpr line<T> right() const;
		constexpr line<T> bottom() const;
		
		constexpr line<T> side(uint32_t i) const;
		constexpr vec2d<T> end() const;

		vec2d<T> pos;
		vec2d<T> size;

		static constexpr uint32_t SIDES = 4;
	};

	// Checks if p1 and p2 have the same coordinates
	template <class T1, class T2>
	constexpr bool contains(const vec2d<T1>& p1, const vec2d<T2>& p2);

	// Checks if p is inside c
	template <class T1, class T2>
	constexpr bool contains(const circle<T1>& c, const vec2d<T2>& p);

	// Checks if r contains p
	template <class T1, class T2>
	constexpr bool contains(const rect<T1>& r, const vec2d<T2>& p);

	// Checks if r1 contains r2
	template <class T1, class T2>
	constexpr bool contains(const rect<T1>& r1, const rect<T2>& r2);

	// Checks if r contains l
	template <class T1, class T2>
	constexpr bool contains(const rect<T1>& r, const line<T2>& l);

	// Checks if l1 contains l2
	template <class T1, class T2>
	constexpr bool contains(const line<T1>& l1, const line<T2>& l2);

	// Checks if l contains p
	template <class T1, class T2>
	constexpr bool contains(const line<T1>& l, const vec2d<T2>& p);

	// Checks if p1 and p2 have the same coordinates
	template <class T1, class T2>
	constexpr std::vector<vec2d<T2>> intersects(const vec2d<T1>& p1, const vec2d<T2>& p2);

	// Checks if p intersects c
	template <class T1, class T2>
	constexpr std::vector<vec2d<T2>> intersects(const circle<T1>& c, const vec2d<T2>& p);

	// Checks if r intersects p
	template <class T1, class T2>
	constexpr std::vector<vec2d<T2>> intersects(const rect<T1>& r, const vec2d<T2>& p);

	// Checks if r1 intersects r2
	template <class T1, class T2>
	constexpr std::vector<vec2d<T2>> intersects(const rect<T1>& r1, const rect<T2>& r2);

	// Checks if l1 intersects l2
	template <class T1, class T2>
	constexpr std::vector<vec2d<T2>> intersects(const line<T1>& l1, const line<T2>& l2);

	// Checks if r intersects l
	template <class T1, class T2>
	constexpr std::vector<vec2d<T2>> intersects(const rect<T1>& r, const line<T2>& l);

	// Checks if l intersects p
	template <class T1, class T2>
	constexpr std::vector<vec2d<T2>> intersects(const line<T1>& l, const vec2d<T2>& p);

#ifdef DGE_PHYSICS
#undef DGE_PHYSICS

	template <class T1, class T2>
	constexpr auto utils::equal(T1 lhs, T2 rhs)
	{
		return abs(lhs - rhs) <= epsilon;
	}

	template <class T>
	constexpr rect<T>::rect(const vec2d<T>& p, const vec2d<T>& s)
	{
		pos = p;
		size = s;
	}

	template <class T>
	constexpr line<T> rect<T>::left() const
	{
		return { pos, { pos.x, pos.y + size.y } };
	}

	template <class T>
	constexpr line<T> rect<T>::top() const
	{
		return { pos, { pos.x + size.x, pos.y } };
	}

	template <class T>
	constexpr line<T> rect<T>::right() const
	{
		return { { pos.x + size.x, pos.y }, end() };
	}

	template <class T>
	constexpr line<T> rect<T>::bottom() const
	{
		return { { pos.x, pos.y + size.y }, end() };
	}

	template<class T>
	constexpr line<T> rect<T>::side(uint32_t i) const
	{
		switch (i)
		{
		case side::LEFT: return left();
		case side::TOP: return top();
		case side::RIGHT: return right();
		case side::BOTTOM: return bottom();
		}

		return {};
	}

	template<class T>
	constexpr vec2d<T> rect<T>::end() const
	{
		return pos + size;
	}

	template <class T>
	constexpr circle<T>::circle(const vec2d<T>& p, float r)
	{
		pos = p;
		radius = r;
	}

	template <class T>
	constexpr line<T>::line(const vec2d<T>& p1, const vec2d<T>& p2)
	{
		pos[0] = p1;
		pos[1] = p2;
	}

	template <class T>
	constexpr vec2d<T> line<T>::vector() const
	{
		return pos[1] - pos[0];
	}

	template <class T1, class T2>
	constexpr bool contains(const vec2d<T1>& p1, const vec2d<T2>& p2)
	{
		return utils::equal(p1.x, p2.x) && utils::equal(p1.y, p2.y);
	}

	template <class T1, class T2>
	constexpr bool contains(const circle<T1>& c, const vec2d<T2>& p)
	{
		return (c.pos - p).mag2() <= c.radius * c.radius;
	}

	template <class T1, class T2>
	constexpr bool contains(const rect<T1>& r, const vec2d<T2>& p)
	{
		return p >= r.pos && p <= r.end();
	}

	template <class T1, class T2>
	constexpr bool contains(const rect<T1>& r1, const rect<T2>& r2)
	{
		return r1.pos <= r2.pos && r1.end() >= r2.end();
	}

	template <class T1, class T2>
	constexpr bool contains(const rect<T1>& r, const line<T2>& l)
	{
		return l.pos[0] >= r.pos && l.pos[1] >= r.pos && l.pos[0] <= r.end() && l.pos[1] <= r.end();
	}

	template <class T1, class T2>
	constexpr bool contains(const line<T1>& l1, const line<T2>& l2)
	{
		return (contains(l1.pos[0], l2.pos[0]) && contains(l1.pos[1], l2.pos[1])) || (contains(l1.pos[0], l2.pos[1]) && contains(l1.pos[1], l2.pos[0]));
	}

	template<class T1, class T2>
	constexpr bool contains(const line<T1>& l, const vec2d<T2>& p)
	{
		vec2d<T1> vec = l.vector();

		auto dp = vec.dot(p - l.pos[0]) / vec.mag2();

		if (dp < 0 || dp > 1)
			return false;

		vec2d<T2> proj = l.pos[0].lerp(l.pos[1], dp);

		// We need to find a proper epsilon value
		return p.dist(proj) < 1;
	}

	template<class T1, class T2>
	constexpr std::vector<vec2d<T2>> intersects(const vec2d<T1>& p1, const vec2d<T2>& p2)
	{
		if (contains(p1, p2))
			return { p2 };

		return {};
	}

	template<class T1, class T2>
	constexpr std::vector<vec2d<T2>> intersects(const circle<T1>& c, const vec2d<T2>& p)
	{
		if (utils::equal((c.pos - p).mag2(), c.radius * c.radius))
			return { p };

		return {};
	}

	template<class T1, class T2>
	constexpr std::vector<vec2d<T2>> intersects(const rect<T1>& r, const vec2d<T2>& p)
	{
		for (uint32_t i = 0; i < r.SIDES; i++)
		{
			if (contains(r.side(i), p))
				return { p };
		}

		return {};
	}

	template<class T1, class T2>
	constexpr std::vector<vec2d<T2>> intersects(const rect<T1>& r1, const rect<T2>& r2)
	{
		std::vector<vec2d<T2>> intersections;

		for (uint32_t i = 0; i < r1.SIDES; i++)
		{
			line<T1> side = r1.side(i);

			for (uint32_t j = 0; j < r2.SIDES; j++)
			{
				auto points = intersects(side, r2.side(j));

				if (!points.empty())
					intersections.push_back(points[0]);
			}
		}

		return intersections;	
	}

	template<class T1, class T2>
	constexpr std::vector<vec2d<T2>> intersects(const line<T1>& l1, const line<T2>& l2)
	{
		// l1: a1 * x + b1 * y = -c1
		// l2: a2 * x + b2 * y = -c2

		auto a1 = l1.pos[0].y - l1.pos[1].y;
		auto b1 = l1.pos[1].x - l1.pos[0].x;
		auto c1 = l1.pos[0].x * l1.pos[1].y - l1.pos[1].x * l1.pos[0].y;

		auto a2 = l2.pos[0].y - l2.pos[1].y;
		auto b2 = l2.pos[1].x - l2.pos[0].x;
		auto c2 = l2.pos[0].x * l2.pos[1].y - l2.pos[1].x * l2.pos[0].y;

		auto det = a1 * b2 - b1 * a2;

		if (det == 0)
		{
			// Determinant is 0 so there are no intersection points and lines are parallel
			return {};
		}

		vec2d<T2> point = { (b1 * c2 - b2 * c1) / det, (a2 * c1 - a1 * c2) / det };

		if (contains(l1, point) && contains(l2, point))
			return { point };

		return {};
	}

	template<class T1, class T2>
	constexpr std::vector<vec2d<T2>> intersects(const rect<T1>& r, const line<T2>& l)
	{
		std::vector<vec2d<T2>> intersections;

		for (uint32_t i = 0; i < r.SIDES; i++)
		{
			auto points = intersects(l, r.side(i));

			if (!points.empty())
				intersections.push_back(points[0]);
		}

		return intersections;
	}

	template<class T1, class T2>
	constexpr std::vector<vec2d<T2>> intersects(const line<T1>& l, const vec2d<T2>& p)
	{
		if (contains(l, p))
			return { p };

		return {};
	}

#endif

}
