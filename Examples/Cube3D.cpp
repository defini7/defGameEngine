#define DGE_APPLICATION
#include "../defGameEngine.hpp"

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

void Vector_CrossProduct(const Vector3& v1, const Vector3& v2, Vector3& out)
{
	out.x = v1.y * v2.z - v2.y * v1.z;
	out.y = v1.z * v2.x - v2.z * v1.x;
	out.z = v1.x * v2.y - v2.x * v1.y;
}

float Vector_Length(const Vector3& v)
{
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

void Vector_Normalise(Vector3& v)
{
	float length = Vector_Length(v);

	v.x /= length;
	v.y /= length;
	v.z /= length;
}

float Vector_DotProduct(const Vector3& v1, const Vector3& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

class Engine3D : public def::GameEngine
{
public:
	Engine3D()
	{
		SetTitle("Engine 3D");
	}

	Matrix4x4 projMatrix;
	Mesh cube;

	float angle = 0.0f;

	Vector3 camera;

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

			Vector3 vector1, vector2, normal, cameraRay;
			
			// Get the first vector that's on the triangle
			vector1.x = translated.p[1].x - translated.p[0].x;
			vector1.y = translated.p[1].y - translated.p[0].y;
			vector1.z = translated.p[1].z - translated.p[0].z;

			// Get the second vector that's on the triangle
			vector2.x = translated.p[2].x - translated.p[0].x;
			vector2.y = translated.p[2].y - translated.p[0].y;
			vector2.z = translated.p[2].z - translated.p[0].z;

			// Get the vector from the camera position to the triangle vertex
			cameraRay.x = translated.p[0].x - camera.x;
			cameraRay.y = translated.p[0].y - camera.y;
			cameraRay.z = translated.p[0].z - camera.z;

			// Get a vector that's perpendicular to the vector1 and vector2
			Vector_CrossProduct(vector1, vector2, normal);
			
			// Normalise the normal, yep!
			Vector_Normalise(normal);

			// If the angle between normal and camera ray is less than 90 degrees
			// then we can draw it
			if (Vector_DotProduct(normal, cameraRay) < 0.0f)
			{
				projected.p[0] = Matrix_MultiplyVector(projMatrix, translated.p[0]);
				projected.p[1] = Matrix_MultiplyVector(projMatrix, translated.p[1]);
				projected.p[2] = Matrix_MultiplyVector(projMatrix, translated.p[2]);

				projected.p[0].x += 0.5f; projected.p[0].y += 0.5f;
				projected.p[1].x += 0.5f; projected.p[1].y += 0.5f;
				projected.p[2].x += 0.5f; projected.p[2].y += 0.5f;

				projected.p[0].x *= (float)ScreenWidth(); projected.p[0].y *= (float)ScreenHeight();
				projected.p[1].x *= (float)ScreenWidth(); projected.p[1].y *= (float)ScreenHeight();
				projected.p[2].x *= (float)ScreenWidth(); projected.p[2].y *= (float)ScreenHeight();

				Vector3 lightDir = { 0.0f, 0.0f, -1.0f };

				Vector_Normalise(lightDir);
				float factor = Vector_DotProduct(lightDir, normal);

				FillTriangle(
					projected.p[0].x, projected.p[0].y,
					projected.p[1].x, projected.p[1].y,
					projected.p[2].x, projected.p[2].y,
					def::Pixel::Float(factor, factor, factor)
				);
			}
		}

		angle += deltaTime;

		return true;
	}

};

int main()
{
	Engine3D demo;

	demo.Construct(256, 240, 4, 4);
	demo.Run();

	return 0;
}
