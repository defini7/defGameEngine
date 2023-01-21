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

#include "defGameEngine.h"

#pragma warning(disable : 4996)

#include <fstream>
#include <strstream>

template <class T>
struct vec3d_basic
{
	vec3d_basic() = default;
	vec3d_basic(T x, T y, T z, T w = 1.0f)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	T x = 0.0f;
	T y = 0.0f;
	T z = 0.0f;
	T w = 1.0f;

	vec3d_basic& operator=(const vec3d_basic& v) = default;
	vec3d_basic<T> operator+(const vec3d_basic<T>& v) { return vec3d_basic<T>(this->x + v.x, this->y + v.y, this->z + v.z); }
	vec3d_basic<T> operator-(const vec3d_basic<T>& v) { return vec3d_basic<T>(this->x - v.x, this->y - v.y, this->z - v.z); }
	vec3d_basic<T> operator*(const vec3d_basic<T>& v) { return vec3d_basic<T>(this->x * v.x, this->y * v.y, this->z * v.z); }
	vec3d_basic<T> operator/(const vec3d_basic<T>& v) { return vec3d_basic<T>(this->x / v.x, this->y / v.y, this->z / v.z); }
	vec3d_basic<T> operator+(const T& v) { return vec3d_basic<T>(this->x + v, this->y + v, this->z + v); }
	vec3d_basic<T> operator-(const T& v) { return vec3d_basic<T>(this->x - v, this->y - v, this->z - v); }
	vec3d_basic<T> operator*(const T& v) { return vec3d_basic<T>(this->x * v, this->y * v, this->z * v); }
	vec3d_basic<T> operator/(const T& v) { return vec3d_basic<T>(this->x / v, this->y / v, this->z / v); }

	vec3d_basic<T>& operator+=(const vec3d_basic<T>& v)
	{
		this->x += v.x;
		this->y += v.y;
		this->z += v.z;
		return *this;
	}

	vec3d_basic<T>& operator-=(const vec3d_basic<T>& v)
	{
		this->x -= v.x;
		this->y -= v.y;
		this->z -= v.z;
		return *this;
	}

	vec3d_basic<T>& operator*=(const vec3d_basic<T>& v)
	{
		this->x *= v.x;
		this->y *= v.y;
		this->z *= v.z;
		return *this;
	}

	vec3d_basic<T>& operator/=(const vec3d_basic<T>& v)
	{
		this->x /= v.x;
		this->y /= v.y;
		this->z /= v.z;
		return *this;
	}

	vec3d_basic<T>& operator+=(const T& v)
	{
		this->x += v;
		this->y += v;
		this->z += v;
		return *this;
	}

	vec3d_basic<T>& operator-=(const T& v)
	{
		this->x -= v;
		this->y -= v;
		this->z -= v;
		return *this;
	}

	vec3d_basic<T>& operator*=(const T& v)
	{
		this->x *= v;
		this->y *= v;
		this->z *= v;
		return *this;
	}

	vec3d_basic<T>& operator/=(const T& v)
	{
		this->x /= v;
		this->y /= v;
		this->z /= v;
		return *this;
	}

	bool operator==(const vec3d_basic<T>& v) { return this->x == v.x && this->y == v.y && this->z == v.z; }
	bool operator!=(const vec3d_basic<T>& v) { return this->x != v.x || this->y != v.y || this->z != v.z; }

	friend vec3d_basic<T> operator*(const float& lhs, const vec3d_basic<T>& rhs)
	{
		return vec3d_basic<T>((T)(lhs * (float)rhs.x), (T)(lhs * (float)rhs.y), (T)(lhs * (float)rhs.z));
	}

	friend vec3d_basic<T> operator*(const double& lhs, const vec3d_basic<T>& rhs)
	{
		return vec3d_basic<T>((T)(lhs * (double)rhs.x), (T)(lhs * (double)rhs.y), (T)(lhs * (double)rhs.z));
	}

	friend vec3d_basic<T> operator*(const int& lhs, const vec3d_basic<T>& rhs)
	{
		return vec3d_basic<T>((T)(lhs * (int)rhs.x), (T)(lhs * (int)rhs.y), (T)(lhs * (int)rhs.z));
	}

	friend vec3d_basic<T> operator/(const float& lhs, const vec3d_basic<T>& rhs)
	{
		return vec3d_basic<T>((T)(lhs / (float)rhs.x), (T)(lhs / (float)rhs.y), (T)(lhs / (float)rhs.z));
	}

	friend vec3d_basic<T> operator/(const double& lhs, const vec3d_basic<T>& rhs)
	{
		return vec3d_basic<T>((T)(lhs / (double)rhs.x), (T)(lhs / (double)rhs.y), (T)(lhs / (double)rhs.z));
	}

	friend vec3d_basic<T> operator/(const int& lhs, const vec3d_basic<T>& rhs)
	{
		return vec3d_basic<T>((T)(lhs / (int)rhs.x), (T)(lhs / (int)rhs.y), (T)(lhs / (int)rhs.z));
	}

	operator vec3d_basic<int>()		const { return { static_cast<int32_t>(this->x), static_cast<int32_t>(this->y), static_cast<int32_t>(this->z) }; }
	operator vec3d_basic<float>()	const { return { static_cast<float>(this->x), static_cast<float>(this->y), static_cast<float>(this->z) }; }
	operator vec3d_basic<double>()	const { return { static_cast<double>(this->x), static_cast<double>(this->y), static_cast<double>(this->z) }; }

	float dot(vec3d_basic<T> v) { return this->x * v.x + this->y * v.y + this->z * v.z; }

	vec3d_basic<T> cross(vec3d_basic<T>& v) { return { this->y * v.z - this->z * v.y, this->z * v.x - this->x * v.z, this->x * v.y - this->y * v.x }; }

	T mag() { return static_cast<T>(sqrtf(this->dot(*this))); }
	T mag2() { return static_cast<T>(this->dot(*this)); }

	vec3d_basic<T> norm() { float n = 1.0f / mag(); return vec3d_basic<T>(this->x * n, this->y * n, this->z * n); }
	vec3d_basic<T> abs() { return vec3d_basic<T>(std::abs(this->x), std::abs(this->y), std::abs(this->z)); }
	vec3d_basic<T> perp() { return vec3d_basic<T>(-this->y, this->x, this->z); }
	vec3d_basic<T> floor() { return vec3d_basic<T>(std::floor(this->x), std::floor(this->y), std::floor(this->z)); }
	vec3d_basic<T> ceil() { return vec3d_basic<T>(std::ceil(this->x), std::ceil(this->y), std::ceil(this->z)); }
	vec3d_basic<T> cart() { return vec3d_basic<T>(cos(this->y) * this->x, sin(this->y) * this->x, this->z); }
	vec3d_basic<T> polar() { return vec3d_basic<T>(mag(), atan2(this->y, this->x), this->z); }
	vec3d_basic<T>& ref() { return *this; }

	std::string str() { return "(" + std::to_string(this->x) + ", " + std::to_string(this->y) + ")"; }
};

typedef vec3d_basic<int> vi3d;
typedef vec3d_basic<float> vf3d;
typedef vec3d_basic<double> vd3d;

struct mat4x4
{
	mat4x4() = default;
	mat4x4(float* &mat)
	{
		for (int x = 0; x < 4; x++)
			for (int y = 0; y < 4; y++)
				m[x][y] = mat[y * 4 + x];
	}

	float m[4][4] = { 0 };

	void projection(float fFovDeg, float fAspectRatio, float fNear, float fFar)
	{
		float fov = 1.0f / tanf(fFovDeg * 0.5f / 180.0f * 3.14159f);

		m[0][0] = fAspectRatio * fov;
		m[1][1] = fov;
		m[2][2] = fFar / (fFar - fNear);
		m[3][2] = (-fFar * fNear) / (fFar - fNear);
		m[2][3] = 1.0f;
		m[3][3] = 0.0f;
	}

	void identity()
	{
		m[0][0] = 1.0f;
		m[1][1] = 1.0f;
		m[2][2] = 1.0f;
		m[3][3] = 1.0f;
	}

	void translate(float x, float y, float z)
	{
		m[0][0] = 1.0f;
		m[1][1] = 1.0f;
		m[2][2] = 1.0f;
		m[3][3] = 1.0f;
		m[3][0] = x;
		m[3][1] = y;
		m[3][2] = z;
	}

	void point_at(vf3d& pos, vf3d& target, vf3d& up)
	{
		vf3d newForward = (target - pos).norm();
		vf3d newUp = (up - newForward * up.dot(newForward)).norm();

		vf3d newRight = newUp.cross(newForward);

		m[0][0] = newRight.x;	m[0][1] = newRight.y;	m[0][2] = newRight.z;	m[0][3] = 0.0f;
		m[1][0] = newUp.x;		m[1][1] = newUp.y;		m[1][2] = newUp.z;		m[1][3] = 0.0f;
		m[2][0] = newForward.x;	m[2][1] = newForward.y;	m[2][2] = newForward.z;	m[2][3] = 0.0f;
		m[3][0] = pos.x;		m[3][1] = pos.y;		m[3][2] = pos.z;		m[3][3] = 1.0f;
	}

	void quick_inv(mat4x4& mat)
	{
		m[0][0] = mat.m[0][0]; m[0][1] = mat.m[1][0]; m[0][2] = mat.m[2][0]; m[0][3] = 0.0f;
		m[1][0] = mat.m[0][1]; m[1][1] = mat.m[1][1]; m[1][2] = mat.m[2][1]; m[1][3] = 0.0f;
		m[2][0] = mat.m[0][2]; m[2][1] = mat.m[1][2]; m[2][2] = mat.m[2][2]; m[2][3] = 0.0f;
		m[3][0] = -(mat.m[3][0] * m[0][0] + mat.m[3][1] * m[1][0] + mat.m[3][2] * m[2][0]);
		m[3][1] = -(mat.m[3][0] * m[0][1] + mat.m[3][1] * m[1][1] + mat.m[3][2] * m[2][1]);
		m[3][2] = -(mat.m[3][0] * m[0][2] + mat.m[3][1] * m[1][2] + mat.m[3][2] * m[2][2]);
		m[3][3] = 1.0f;
	}

	void inverse(mat4x4& mat)
	{
		m[0][0] = mat.m[1][1] * mat.m[2][2] * mat.m[3][3] - mat.m[1][1] * mat.m[2][3] * mat.m[3][2] - mat.m[2][1] * mat.m[1][2] * mat.m[3][3] + mat.m[2][1] * mat.m[1][3] * mat.m[3][2] + mat.m[3][1] * mat.m[1][2] * mat.m[2][3] - mat.m[3][1] * mat.m[1][3] * mat.m[2][2];
		m[1][0] = -mat.m[1][0] * mat.m[2][2] * mat.m[3][3] + mat.m[1][0] * mat.m[2][3] * mat.m[3][2] + mat.m[2][0] * mat.m[1][2] * mat.m[3][3] - mat.m[2][0] * mat.m[1][3] * mat.m[3][2] - mat.m[3][0] * mat.m[1][2] * mat.m[2][3] + mat.m[3][0] * mat.m[1][3] * mat.m[2][2];
		m[2][0] = mat.m[1][0] * mat.m[2][1] * mat.m[3][3] - mat.m[1][0] * mat.m[2][3] * mat.m[3][1] - mat.m[2][0] * mat.m[1][1] * mat.m[3][3] + mat.m[2][0] * mat.m[1][3] * mat.m[3][1] + mat.m[3][0] * mat.m[1][1] * mat.m[2][3] - mat.m[3][0] * mat.m[1][3] * mat.m[2][1];
		m[3][0] = -mat.m[1][0] * mat.m[2][1] * mat.m[3][2] + mat.m[1][0] * mat.m[2][2] * mat.m[3][1] + mat.m[2][0] * mat.m[1][1] * mat.m[3][2] - mat.m[2][0] * mat.m[1][2] * mat.m[3][1] - mat.m[3][0] * mat.m[1][1] * mat.m[2][2] + mat.m[3][0] * mat.m[1][2] * mat.m[2][1];
		m[0][1] = -mat.m[0][1] * mat.m[2][2] * mat.m[3][3] + mat.m[0][1] * mat.m[2][3] * mat.m[3][2] + mat.m[2][1] * mat.m[0][2] * mat.m[3][3] - mat.m[2][1] * mat.m[0][3] * mat.m[3][2] - mat.m[3][1] * mat.m[0][2] * mat.m[2][3] + mat.m[3][1] * mat.m[0][3] * mat.m[2][2];
		m[1][1] = mat.m[0][0] * mat.m[2][2] * mat.m[3][3] - mat.m[0][0] * mat.m[2][3] * mat.m[3][2] - mat.m[2][0] * mat.m[0][2] * mat.m[3][3] + mat.m[2][0] * mat.m[0][3] * mat.m[3][2] + mat.m[3][0] * mat.m[0][2] * mat.m[2][3] - mat.m[3][0] * mat.m[0][3] * mat.m[2][2];
		m[2][1] = -mat.m[0][0] * mat.m[2][1] * mat.m[3][3] + mat.m[0][0] * mat.m[2][3] * mat.m[3][1] + mat.m[2][0] * mat.m[0][1] * mat.m[3][3] - mat.m[2][0] * mat.m[0][3] * mat.m[3][1] - mat.m[3][0] * mat.m[0][1] * mat.m[2][3] + mat.m[3][0] * mat.m[0][3] * mat.m[2][1];
		m[3][1] = mat.m[0][0] * mat.m[2][1] * mat.m[3][2] - mat.m[0][0] * mat.m[2][2] * mat.m[3][1] - mat.m[2][0] * mat.m[0][1] * mat.m[3][2] + mat.m[2][0] * mat.m[0][2] * mat.m[3][1] + mat.m[3][0] * mat.m[0][1] * mat.m[2][2] - mat.m[3][0] * mat.m[0][2] * mat.m[2][1];
		m[0][2] = mat.m[0][1] * mat.m[1][2] * mat.m[3][3] - mat.m[0][1] * mat.m[1][3] * mat.m[3][2] - mat.m[1][1] * mat.m[0][2] * mat.m[3][3] + mat.m[1][1] * mat.m[0][3] * mat.m[3][2] + mat.m[3][1] * mat.m[0][2] * mat.m[1][3] - mat.m[3][1] * mat.m[0][3] * mat.m[1][2];
		m[1][2] = -mat.m[0][0] * mat.m[1][2] * mat.m[3][3] + mat.m[0][0] * mat.m[1][3] * mat.m[3][2] + mat.m[1][0] * mat.m[0][2] * mat.m[3][3] - mat.m[1][0] * mat.m[0][3] * mat.m[3][2] - mat.m[3][0] * mat.m[0][2] * mat.m[1][3] + mat.m[3][0] * mat.m[0][3] * mat.m[1][2];
		m[2][2] = mat.m[0][0] * mat.m[1][1] * mat.m[3][3] - mat.m[0][0] * mat.m[1][3] * mat.m[3][1] - mat.m[1][0] * mat.m[0][1] * mat.m[3][3] + mat.m[1][0] * mat.m[0][3] * mat.m[3][1] + mat.m[3][0] * mat.m[0][1] * mat.m[1][3] - mat.m[3][0] * mat.m[0][3] * mat.m[1][1];
		m[3][2] = -mat.m[0][0] * mat.m[1][1] * mat.m[3][2] + mat.m[0][0] * mat.m[1][2] * mat.m[3][1] + mat.m[1][0] * mat.m[0][1] * mat.m[3][2] - mat.m[1][0] * mat.m[0][2] * mat.m[3][1] - mat.m[3][0] * mat.m[0][1] * mat.m[1][2] + mat.m[3][0] * mat.m[0][2] * mat.m[1][1];
		m[0][3] = -mat.m[0][1] * mat.m[1][2] * mat.m[2][3] + mat.m[0][1] * mat.m[1][3] * mat.m[2][2] + mat.m[1][1] * mat.m[0][2] * mat.m[2][3] - mat.m[1][1] * mat.m[0][3] * mat.m[2][2] - mat.m[2][1] * mat.m[0][2] * mat.m[1][3] + mat.m[2][1] * mat.m[0][3] * mat.m[1][2];
		m[1][3] = mat.m[0][0] * mat.m[1][2] * mat.m[2][3] - mat.m[0][0] * mat.m[1][3] * mat.m[2][2] - mat.m[1][0] * mat.m[0][2] * mat.m[2][3] + mat.m[1][0] * mat.m[0][3] * mat.m[2][2] + mat.m[2][0] * mat.m[0][2] * mat.m[1][3] - mat.m[2][0] * mat.m[0][3] * mat.m[1][2];
		m[2][3] = -mat.m[0][0] * mat.m[1][1] * mat.m[2][3] + mat.m[0][0] * mat.m[1][3] * mat.m[2][1] + mat.m[1][0] * mat.m[0][1] * mat.m[2][3] - mat.m[1][0] * mat.m[0][3] * mat.m[2][1] - mat.m[2][0] * mat.m[0][1] * mat.m[1][3] + mat.m[2][0] * mat.m[0][3] * mat.m[1][1];
		m[3][3] = mat.m[0][0] * mat.m[1][1] * mat.m[2][2] - mat.m[0][0] * mat.m[1][2] * mat.m[2][1] - mat.m[1][0] * mat.m[0][1] * mat.m[2][2] + mat.m[1][0] * mat.m[0][2] * mat.m[2][1] + mat.m[2][0] * mat.m[0][1] * mat.m[1][2] - mat.m[2][0] * mat.m[0][2] * mat.m[1][1];

		float det = 1.0f / float(mat.m[0][0] * m[0][0] + mat.m[0][1] * m[1][0] + mat.m[0][2] * m[2][0] + mat.m[0][3] * m[3][0]);

		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				m[i][j] *= (float)det;
	}

	mat4x4 operator*(mat4x4 mat)
	{
		mat4x4 res;

		for (int c = 0; c < 4; c++)
			for (int r = 0; r < 4; r++)
				res.m[r][c] = m[r][0] * mat.m[0][c] + m[r][1] * mat.m[1][c] + m[r][2] * mat.m[2][c] + m[r][3] * mat.m[3][c];

		return res;
	}


	vf3d operator*(vf3d& v)
	{
		vf3d res;

		res.x = v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + v.w * m[3][0];
		res.y = v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + v.w * m[3][1];
		res.z = v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + v.w * m[3][2];
		res.w = v.x * m[0][3] + v.y * m[1][3] + v.z * m[2][3] + v.w * m[3][3];

		return res;
	}

	void rotateX(float a)
	{
		m[0][0] = 1.0f;
		m[1][1] = cosf(a);
		m[1][2] = sinf(a);
		m[2][1] = -sinf(a);
		m[2][2] = cosf(a);
		m[3][3] = 1.0f;
	}

	void rotateY(float a)
	{
		m[0][0] = cosf(a);
		m[0][2] = -sinf(a);
		m[1][1] = 1.0f;
		m[2][0] = sinf(a);
		m[2][2] = cosf(a);
		m[3][3] = 1.0f;
	}

	void rotateZ(float a)
	{
		m[0][0] = cosf(a);
		m[0][1] = sinf(a);
		m[1][0] = -sinf(a);
		m[1][1] = cosf(a);
		m[2][2] = 1.0f;
		m[3][3] = 1.0f;
	}

	void scale(float x, float y, float z)
	{
		m[0][0] = x;
		m[1][1] = y;
		m[2][2] = z;
		m[3][3] = 1.0f;
	}
};

struct tex
{
	float u = 0.0f;
	float v = 0.0f;
	float w = 1.0f;
};

struct triangle
{
	vf3d p[3];
	tex t[3];

	def::Pixel col;
};

struct mesh
{
	std::vector<triangle> tris;
};

bool LoadFromObjectFile(mesh& m, const std::string& sObjFilename, bool bHasTexture = false)
{
	std::ifstream f(sObjFilename);
	if (!f.is_open()) return false;

	// Local cache of verts
	std::vector<vf3d> verts;
	std::vector<tex> texs;

	while (!f.eof())
	{
		char line[128];
		f.getline(line, 128);

		std::strstream s;
		s << line;

		char junk;

		if (line[0] == 'v')
		{
			if (line[1] == 't')
			{
				tex v;
				s >> junk >> junk >> v.u >> v.v >> v.w;
				v.v = 1.0f - v.v;
				texs.push_back(v);
			}
			else
			{
				vf3d v;
				s >> junk >> v.x >> v.y >> v.z;
				verts.push_back(v);
			}
		}

		if (bHasTexture)
		{
			if (line[0] == 'f')
			{
				s >> junk;

				std::string tokens[6];
				int nTokenCount = -1;
				while (!s.eof())
				{
					char c = s.get();
					if (c == ' ' || c == '/')
					{
						if (tokens[nTokenCount].size() > 0)
							nTokenCount++;
					}
					else
						tokens[nTokenCount].append(1, c);
				}

				tokens[nTokenCount].pop_back();

				m.tris.push_back({
					{
						verts[stoi(tokens[0]) - 1],
						verts[stoi(tokens[2]) - 1],
						verts[stoi(tokens[4]) - 1]
					},
					{
						texs[stoi(tokens[1]) - 1],
						texs[stoi(tokens[3]) - 1],
						texs[stoi(tokens[5]) - 1],
					}
					});
			}
		}
		else
		{
			if (line[0] == 'f')
			{
				int f[3];
				s >> junk >> f[0] >> f[1] >> f[2];
				m.tris.push_back({ verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1] });
			}
		}
	}

	return true;
}

class Engine3D : public def::GameEngine
{
public:
	Engine3D()
	{
		SetTitle("3D Engine");
		ShowFPS();
	}

private:
	mesh meshCube;
	mat4x4 matProj;

	vf3d vCamera;
	vf3d vLookDir;

	float fYaw = 0.0f;

	def::Sprite* sprTex = nullptr;

	float* pDepthBuffer = nullptr;

protected:
	vf3d IntersectPlane(vf3d& plane_p, vf3d& plane_n, vf3d& start, vf3d& end, float& t)
	{
		plane_n = plane_n.norm();
		float plane_d = -plane_n.dot(plane_p);
		float ad = start.dot(plane_n);
		float bd = end.dot(plane_n);
		t = (-plane_d - ad) / (bd - ad);

		vf3d startToEnd = end - start;
		vf3d toIntersect = startToEnd * t;

		return start + toIntersect;
	}

	int ClipAgainstPlane(vf3d plane_p, vf3d plane_n, triangle& in_tri, triangle& out_tri1, triangle& out_tri2)
	{
		plane_n = plane_n.norm();

		auto dist = [&](vf3d& p)
		{
			return plane_n.dot(p) - plane_n.dot(plane_p);
		};

		int nInsidePoints = 0;
		int nOutsidePoints = 0;

		vf3d* inside_points[3];
		vf3d* outside_points[3];

		int nInsideTextures = 0;
		int nOutsideTextures = 0;

		tex* inside_textures[3];
		tex* outside_textures[3];

		float d0 = dist(in_tri.p[0]);
		float d1 = dist(in_tri.p[1]);
		float d2 = dist(in_tri.p[2]);

		if (d0 >= 0)
		{
			inside_points[nInsidePoints++] = &in_tri.p[0];
			inside_textures[nInsideTextures++] = &in_tri.t[0];
		}
		else
		{
			outside_points[nOutsidePoints++] = &in_tri.p[0];
			outside_textures[nOutsideTextures++] = &in_tri.t[0];
		}

		if (d1 >= 0)
		{
			inside_points[nInsidePoints++] = &in_tri.p[1];
			inside_textures[nInsideTextures++] = &in_tri.t[1];
		}
		else
		{
			outside_points[nOutsidePoints++] = &in_tri.p[1];
			outside_textures[nOutsideTextures++] = &in_tri.t[1];
		}

		if (d2 >= 0)
		{
			inside_points[nInsidePoints++] = &in_tri.p[2];
			inside_textures[nInsideTextures++] = &in_tri.t[2];
		}
		else
		{
			outside_points[nOutsidePoints++] = &in_tri.p[2];
			outside_textures[nOutsideTextures++] = &in_tri.t[2];
		}

		// No triangles
		if (nInsidePoints == 0)
		{
			return 0;
		}

		// Full triangle
		if (nInsidePoints == 3)
		{
			// Full triangle lies in plane
			out_tri1 = in_tri;
			return 1;
		}

		// New triangle
		if (nInsidePoints == 1 && nOutsidePoints == 2)
		{
			out_tri1.col = in_tri.col;

			out_tri1.p[0] = *inside_points[0];
			out_tri1.t[0] = *inside_textures[0];

			float t;

			out_tri1.p[1] = IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0], t);
			out_tri1.t[1].u = t * (outside_textures[0]->u - inside_textures[0]->u) + inside_textures[0]->u;
			out_tri1.t[1].v = t * (outside_textures[0]->v - inside_textures[0]->v) + inside_textures[0]->v;
			out_tri1.t[1].w = t * (outside_textures[0]->w - inside_textures[0]->w) + inside_textures[0]->w;

			out_tri1.p[2] = IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[1], t);
			out_tri1.t[2].u = t * (outside_textures[1]->u - inside_textures[0]->u) + inside_textures[0]->u;
			out_tri1.t[2].v = t * (outside_textures[1]->v - inside_textures[0]->v) + inside_textures[0]->v;
			out_tri1.t[2].w = t * (outside_textures[1]->w - inside_textures[0]->w) + inside_textures[0]->w;

			return 1;
		}

		// Quad
		if (nInsidePoints == 2 && nOutsidePoints == 1)
		{
			out_tri1.col = in_tri.col;
			out_tri2.col = in_tri.col;

			out_tri1.p[0] = *inside_points[0];
			out_tri1.p[1] = *inside_points[1];

			out_tri1.t[0] = *inside_textures[0];
			out_tri1.t[1] = *inside_textures[1];

			float t;

			out_tri1.p[2] = IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0], t);
			out_tri1.t[2].u = t * (outside_textures[0]->u - inside_textures[0]->u) + inside_textures[0]->u;
			out_tri1.t[2].v = t * (outside_textures[0]->v - inside_textures[0]->v) + inside_textures[0]->v;
			out_tri1.t[2].w = t * (outside_textures[0]->w - inside_textures[0]->w) + inside_textures[0]->w;

			out_tri2.p[0] = *inside_points[1];
			out_tri2.p[1] = out_tri1.p[2];

			out_tri2.t[0] = *inside_textures[1];
			out_tri2.t[1] = out_tri1.t[2];

			out_tri2.p[2] = IntersectPlane(plane_p, plane_n, *inside_points[1], *outside_points[0], t);

			out_tri2.t[2].u = t * (outside_textures[0]->u - inside_textures[1]->u) + inside_textures[1]->u;
			out_tri2.t[2].v = t * (outside_textures[0]->v - inside_textures[1]->v) + inside_textures[1]->v;
			out_tri2.t[2].w = t * (outside_textures[0]->w - inside_textures[1]->w) + inside_textures[1]->w;

			return 2;
		}

	}

	void DrawTexturedTriangle(int x1, int y1, float u1, float v1, float w1,
		int x2, int y2, float u2, float v2, float w2,
		int x3, int y3, float u3, float v3, float w3,
		def::Sprite* tex)
	{
		if (y2 < y1)
		{
			std::swap(y1, y2);
			std::swap(x1, x2);
			std::swap(u1, u2);
			std::swap(v1, v2);
			std::swap(w1, w2);
		}

		if (y3 < y1)
		{
			std::swap(y1, y3);
			std::swap(x1, x3);
			std::swap(u1, u3);
			std::swap(v1, v3);
			std::swap(w1, w3);
		}

		if (y3 < y2)
		{
			std::swap(y2, y3);
			std::swap(x2, x3);
			std::swap(u2, u3);
			std::swap(v2, v3);
			std::swap(w2, w3);
		}

		int dy1 = y2 - y1;
		int dx1 = x2 - x1;

		float du1 = u2 - u1;
		float dv1 = v2 - v1;
		float dw1 = w2 - w1;

		int dy2 = y3 - y1;
		int dx2 = x3 - x1;

		float du2 = u3 - u1;
		float dv2 = v3 - v1;
		float dw2 = w3 - w1;

		float tex_u, tex_v, tex_w;

		float dax_step = 0.0f, dbx_step = 0.0f,
			du1_step = 0.0f, dv1_step = 0.0f,
			du2_step = 0.0f, dv2_step = 0.0f,
			dw1_step = 0.0f, dw2_step = 0.0f;

		if (dy1) dax_step = dx1 / (float)abs(dy1);
		if (dy2) dbx_step = dx2 / (float)abs(dy2);

		if (dy1) du1_step = du1 / (float)abs(dy1);
		if (dy1) dv1_step = dv1 / (float)abs(dy1);
		if (dy1) dw1_step = dw1 / (float)abs(dy1);

		if (dy2) du2_step = du2 / (float)abs(dy2);
		if (dy2) dv2_step = dv2 / (float)abs(dy2);
		if (dy2) dw2_step = dw2 / (float)abs(dy2);

		if (dy1)
		{
			for (int i = y1; i <= y2; i++)
			{
				int ax = x1 + (float)(i - y1) * dax_step;
				int bx = x1 + (float)(i - y1) * dbx_step;

				float tex_su = u1 + (float)(i - y1) * du1_step;
				float tex_sv = v1 + (float)(i - y1) * dv1_step;
				float tex_sw = w1 + (float)(i - y1) * dw1_step;

				float tex_eu = u1 + (float)(i - y1) * du2_step;
				float tex_ev = v1 + (float)(i - y1) * dv2_step;
				float tex_ew = w1 + (float)(i - y1) * dw2_step;

				if (ax > bx)
				{
					std::swap(ax, bx);
					std::swap(tex_su, tex_eu);
					std::swap(tex_sv, tex_ev);
					std::swap(tex_sw, tex_ew);
				}

				tex_u = tex_su;
				tex_v = tex_sv;
				tex_w = tex_sw;

				float tstep = 1.0f / ((float)(bx - ax));
				float t = 0.0f;

				for (int j = ax; j < bx; j++)
				{
					tex_u = (1.0f - t) * tex_su + t * tex_eu;
					tex_v = (1.0f - t) * tex_sv + t * tex_ev;
					tex_w = (1.0f - t) * tex_sw + t * tex_ew;

					if (tex_w > pDepthBuffer[i * GetScreenWidth() + j])
					{
						Draw(j, i, tex->Sample(tex_u / tex_w, tex_v / tex_w));
						pDepthBuffer[i * GetScreenWidth() + j] = tex_w;
					}

					t += tstep;
				}

			}
		}

		dy1 = y3 - y2;
		dx1 = x3 - x2;
		dv1 = v3 - v2;
		du1 = u3 - u2;
		dw1 = w3 - w2;

		if (dy1) dax_step = dx1 / (float)abs(dy1);
		if (dy2) dbx_step = dx2 / (float)abs(dy2);

		du1_step = 0.0f, dv1_step = 0.0f;
		if (dy1) du1_step = du1 / (float)abs(dy1);
		if (dy1) dv1_step = dv1 / (float)abs(dy1);
		if (dy1) dw1_step = dw1 / (float)abs(dy1);

		if (dy1)
		{
			for (int i = y2; i <= y3; i++)
			{
				int ax = x2 + int((float)(i - y2) * dax_step);
				int bx = x1 + int((float)(i - y1) * dbx_step);

				float tex_su = u2 + (float)(i - y2) * du1_step;
				float tex_sv = v2 + (float)(i - y2) * dv1_step;
				float tex_sw = w2 + (float)(i - y2) * dw1_step;

				float tex_eu = u1 + (float)(i - y1) * du2_step;
				float tex_ev = v1 + (float)(i - y1) * dv2_step;
				float tex_ew = w1 + (float)(i - y1) * dw2_step;

				if (ax > bx)
				{
					std::swap(ax, bx);
					std::swap(tex_su, tex_eu);
					std::swap(tex_sv, tex_ev);
					std::swap(tex_sw, tex_ew);
				}

				tex_u = tex_su;
				tex_v = tex_sv;
				tex_w = tex_sw;

				float tstep = 1.0f / ((float)(bx - ax));
				float t = 0.0f;

				for (int j = ax; j < bx; j++)
				{
					tex_u = (1.0f - t) * tex_su + t * tex_eu;
					tex_v = (1.0f - t) * tex_sv + t * tex_ev;
					tex_w = (1.0f - t) * tex_sw + t * tex_ew;

					if (tex_w > pDepthBuffer[i * GetScreenWidth() + j])
					{
						Draw(j, i, tex->Sample(tex_u / tex_w, tex_v / tex_w));
						pDepthBuffer[i * GetScreenWidth() + j] = tex_w;
					}

					t += tstep;
				}
			}
		}
	}


	bool OnUserCreate() override
	{
		pDepthBuffer = new float[GetScreenWidth() * GetScreenHeight()];

		sprTex = new def::Sprite("castle.png");

		if (!LoadFromObjectFile(meshCube, "castle.obj", true))
			return false;

		matProj.projection(90.0f, (float)GetScreenHeight() / (float)GetScreenWidth(), 0.1f, 1000.0f);

		return true;
	}

	bool OnUserUpdate(float dt) override
	{
		if (GetKey(def::Key::UP).bHeld)
			vCamera.y += 8.0f * dt;

		if (GetKey(def::Key::DOWN).bHeld)
			vCamera.y -= 8.0f * dt;

		vf3d vForward = vLookDir * (8.0f * dt);

		if (GetKey(def::Key::W).bHeld)
			vCamera = vCamera + vForward;

		if (GetKey(def::Key::S).bHeld)
			vCamera = vCamera - vForward;

		if (GetKey(def::Key::A).bHeld)
			fYaw += 4.0f * dt;

		if (GetKey(def::Key::D).bHeld)
			fYaw -= 4.0f * dt;

		mat4x4 matWorld;
		matWorld.identity();

		vf3d vUp = { 0.0f, 1.0f, 0.0f };
		vf3d vTarget = { 0.0f, 0.0f, 1.0f };

		mat4x4 matCameraRot;
		matCameraRot.rotateY(fYaw);

		vLookDir = matCameraRot * vTarget;
		vTarget = vCamera + vLookDir;

		mat4x4 matCamera;
		matCamera.point_at(vCamera, vTarget, vUp);

		mat4x4 matView;
		matView.quick_inv(matCamera);

		std::vector<triangle> vecTrianglesToRaster;

		// Draw triangles
		for (triangle triTransformed : meshCube.tris)
		{
			triangle triProjected, triViewed;

			triTransformed.p[0] = matWorld * triTransformed.p[0];
			triTransformed.p[1] = matWorld * triTransformed.p[1];
			triTransformed.p[2] = matWorld * triTransformed.p[2];

			triTransformed.t[0] = triTransformed.t[0];
			triTransformed.t[1] = triTransformed.t[1];
			triTransformed.t[2] = triTransformed.t[2];

			vf3d vNormal, vLine1, vLine2, vDir;

			vLine1 = triTransformed.p[1] - triTransformed.p[0];
			vLine2 = triTransformed.p[2] - triTransformed.p[0];

			vNormal = vLine1.cross(vLine2).norm();

			vDir = triTransformed.p[0] - vCamera;

			if (vNormal.dot(vDir) < 0.0f)
			{
				triViewed.p[0] = matView * triTransformed.p[0];
				triViewed.p[1] = matView * triTransformed.p[1];
				triViewed.p[2] = matView * triTransformed.p[2];

				triViewed.t[0] = triTransformed.t[0];
				triViewed.t[1] = triTransformed.t[1];
				triViewed.t[2] = triTransformed.t[2];

				vf3d vLightDir = { 0.0f, 1.0f, -1.0f };

				float dp = vLightDir.dot(vNormal);
				triViewed.col = def::PixelF(dp, dp, dp);

				triangle clipped[2];
				int nClipped = ClipAgainstPlane({ 0.0f, 0.0f, 0.1f }, { 0.0f, 0.0f, 1.0f }, triViewed, clipped[0], clipped[1]);

				for (int n = 0; n < nClipped; n++)
				{
					triProjected.p[0] = matProj * clipped[n].p[0];
					triProjected.p[1] = matProj * clipped[n].p[1];
					triProjected.p[2] = matProj * clipped[n].p[2];

					triProjected.t[0] = clipped[n].t[0];
					triProjected.t[1] = clipped[n].t[1];
					triProjected.t[2] = clipped[n].t[2];

					triProjected.col = clipped[n].col;

					triProjected.t[0].u = triProjected.t[0].u / triProjected.p[0].w;
					triProjected.t[1].u = triProjected.t[1].u / triProjected.p[1].w;
					triProjected.t[2].u = triProjected.t[2].u / triProjected.p[2].w;

					triProjected.t[0].v = triProjected.t[0].v / triProjected.p[0].w;
					triProjected.t[1].v = triProjected.t[1].v / triProjected.p[1].w;
					triProjected.t[2].v = triProjected.t[2].v / triProjected.p[2].w;

					triProjected.t[0].w = 1.0f / triProjected.p[0].w;
					triProjected.t[1].w = 1.0f / triProjected.p[1].w;
					triProjected.t[2].w = 1.0f / triProjected.p[2].w;

					triProjected.p[0] = triProjected.p[0] / triProjected.p[0].w;
					triProjected.p[1] = triProjected.p[1] / triProjected.p[1].w;
					triProjected.p[2] = triProjected.p[2] / triProjected.p[2].w;

					vf3d vInvertion = { -1.0f, -1.0f, 1.0f };
					triProjected.p[0] = triProjected.p[0] * vInvertion;
					triProjected.p[1] = triProjected.p[1] * vInvertion;
					triProjected.p[2] = triProjected.p[2] * vInvertion;

					vf3d vOffsetView = { 1.0f, 1.0f, 0.0f };
					triProjected.p[0] = triProjected.p[0] + vOffsetView;
					triProjected.p[1] = triProjected.p[1] + vOffsetView;
					triProjected.p[2] = triProjected.p[2] + vOffsetView;

					triProjected.p[0].x *= 0.5f * (float)GetScreenWidth();
					triProjected.p[0].y *= 0.5f * (float)GetScreenHeight();
					triProjected.p[1].x *= 0.5f * (float)GetScreenWidth();
					triProjected.p[1].y *= 0.5f * (float)GetScreenHeight();
					triProjected.p[2].x *= 0.5f * (float)GetScreenWidth();
					triProjected.p[2].y *= 0.5f * (float)GetScreenHeight();

					vecTrianglesToRaster.push_back(triProjected);
				}
			}
		}

		Clear(def::BLACK);

		for (int i = 0; i < GetScreenWidth() * GetScreenHeight(); i++)
			pDepthBuffer[i] = 0.0f;

		for (auto& triToRaster : vecTrianglesToRaster)
		{
			triangle clipped[2];

			std::list<triangle> tris = { triToRaster };

			int nNewTris = 1;

			for (int p = 0; p < 4; p++)
			{
				int nTrisToAdd = 0;

				while (nNewTris > 0)
				{
					triangle t = tris.front();
					tris.pop_front();
					nNewTris--;

					switch (p)
					{
					case 0:	nTrisToAdd = ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, t, clipped[0], clipped[1]);								break;
					case 1:	nTrisToAdd = ClipAgainstPlane({ 0.0f, (float)GetScreenHeight() - 1.0f, 0.0f }, { 0.0f, -1.0f, 0.0f }, t, clipped[0], clipped[1]);	break;
					case 2:	nTrisToAdd = ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, t, clipped[0], clipped[1]);								break;
					case 3:	nTrisToAdd = ClipAgainstPlane({ (float)GetScreenWidth(), 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, t, clipped[0], clipped[1]);	break;
					}

					for (int i = 0; i < nTrisToAdd; i++)
						tris.push_back(clipped[i]);
				}

				nNewTris = tris.size();
			}

			for (auto& t : tris)
			{
				DrawTexturedTriangle(t.p[0].x, t.p[0].y, t.t[0].u, t.t[0].v, t.t[0].w,
					t.p[1].x, t.p[1].y, t.t[1].u, t.t[1].v, t.t[1].w,
					t.p[2].x, t.p[2].y, t.t[2].u, t.t[2].v, t.t[2].w, sprTex);
			}
		}

		return true;
	}
};

int main()
{
	Engine3D demo;

	if (demo.Construct(1280, 720, 1, 1))
		demo.Run();

	return 0;
}
