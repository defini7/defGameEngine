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

#define DGE_APPLICATION
#include "defGameEngine.hpp"

struct Vector3
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
};

struct Matrix4x4
{
	float m[4][4]{ 0.0f };
};

struct Triangle
{
	Vector3 p[3];
};

struct Mesh
{
	std::vector<Triangle> model;
};

Vector3 Matrix_MultiplyVector(const Matrix4x4& matrix, const Vector3& vector)
{
	Vector3 out =
	{
		vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + matrix.m[3][0],
		vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + matrix.m[3][1],
		vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + matrix.m[3][2]
	};

	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + matrix.m[3][3];

	if (w != 0.0f)
	{
		out.x /= w;
		out.y /= w;
		out.z /= w;
	}

	return out;
}

// https://en.wikipedia.org/wiki/Rotation_matrix

void Matrix_RotateX(Matrix4x4& matrix, float angle)
{
	matrix.m[0][0] = 1.0f;
	matrix.m[1][1] = cosf(angle);
	matrix.m[1][2] = sinf(angle);
	matrix.m[2][1] = -sinf(angle);
	matrix.m[2][2] = cosf(angle);
}

void Matrix_RotateZ(Matrix4x4& matrix, float angle)
{
	matrix.m[0][0] = cosf(angle);
	matrix.m[0][1] = sinf(angle);
	matrix.m[1][0] = -sinf(angle);
	matrix.m[1][1] = cosf(angle);
	matrix.m[2][2] = 1.0f;
}

class RayCasting : public def::GameEngine
{
public:
	RayCasting()
	{
		SetTitle("Ray Casting");
	}

	Matrix4x4 projMatrix;
	Mesh cube;

	float angle = 0.0f;

protected:
	bool OnUserCreate() override
	{
		cube.model =
		{
			{ 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

			{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
			{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

			{ 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
			{ 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

			{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

			{ 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
			{ 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

			{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
			{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },
		};

		// https://stackoverflow.com/questions/53245632/general-formula-for-perspective-projection-matrix

		float fov = 3.14159f * 0.5f;
		float near = 0.001f;
		float far = 1000.0f;
		float aspectRatio = (float)ScreenWidth() / (float)ScreenHeight();

		projMatrix.m[0][0] = 1.0f / (aspectRatio * tanf(fov * 0.5f));
		projMatrix.m[1][1] = 1.0f / tanf(fov * 0.5f);
		projMatrix.m[2][2] = (far + near) / (near - far);
		projMatrix.m[2][3] = -1.0f;
		projMatrix.m[3][2] = 2.0f * far * near / (near - far);

		return true;
	}

	bool OnUserUpdate(float deltaTime) override
	{
		Clear(def::BLACK);

		for (const auto& triangle : cube.model)
		{
			Triangle translated, rotated, projected;
			Matrix4x4 rotatedMatX, rotatedMatZ;

			Matrix_RotateX(rotatedMatX, angle);
			Matrix_RotateZ(rotatedMatZ, angle);

			rotated.p[0] = Matrix_MultiplyVector(rotatedMatZ, triangle.p[0]);
			rotated.p[1] = Matrix_MultiplyVector(rotatedMatZ, triangle.p[1]);
			rotated.p[2] = Matrix_MultiplyVector(rotatedMatZ, triangle.p[2]);

			rotated.p[0] = Matrix_MultiplyVector(rotatedMatX, rotated.p[0]);
			rotated.p[1] = Matrix_MultiplyVector(rotatedMatX, rotated.p[1]);
			rotated.p[2] = Matrix_MultiplyVector(rotatedMatX, rotated.p[2]);

			translated = rotated;
			translated.p[0].z += 5.0f;
			translated.p[1].z += 5.0f;
			translated.p[2].z += 5.0f;

			projected.p[0] = Matrix_MultiplyVector(projMatrix, translated.p[0]);
			projected.p[1] = Matrix_MultiplyVector(projMatrix, translated.p[1]);
			projected.p[2] = Matrix_MultiplyVector(projMatrix, translated.p[2]);

			projected.p[0].x += 0.5f; projected.p[0].y += 0.5f;
			projected.p[1].x += 0.5f; projected.p[1].y += 0.5f;
			projected.p[2].x += 0.5f; projected.p[2].y += 0.5f;

			projected.p[0].x *= (float)ScreenWidth(); projected.p[0].y *= (float)ScreenHeight();
			projected.p[1].x *= (float)ScreenWidth(); projected.p[1].y *= (float)ScreenHeight();
			projected.p[2].x *= (float)ScreenWidth(); projected.p[2].y *= (float)ScreenHeight();

			DrawTriangle(
				projected.p[0].x, projected.p[0].y,
				projected.p[1].x, projected.p[1].y,
				projected.p[2].x, projected.p[2].y,
				def::WHITE
			);
		}

		angle += deltaTime;

		return true;
	}

};

int main()
{
	RayCasting demo;

	demo.Construct(256, 240, 4, 4);
	demo.Run();

	return 0;
}
