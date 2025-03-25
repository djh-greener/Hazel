#include "hzpch.h"
#include "Math.h"
#define GLM_ENABLE_EXPERIMENTAL
#define M_PI 3.1415926
#include <glm/gtx/matrix_decompose.hpp>


namespace Hazel::Math {
	bool DeComposeTransform(const glm::mat4& transform, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale)
	{
		using namespace glm;
		using T = float;

		mat4 LocalMatrix(transform);

		// Normalize the matrix.
		if (epsilonEqual(LocalMatrix[3][3], static_cast<float>(0), epsilon<T>()))
			return false;

		// First, isolate perspective.  This is the messiest.
		if (
			epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
			epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
			epsilonNotEqual(LocalMatrix[2][3], static_cast<T>(0), epsilon<T>()))
		{
			// Clear the perspective partition
			LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
			LocalMatrix[3][3] = static_cast<T>(1);
		}

		// Next take care of translation (easy).
		position = vec3(LocalMatrix[3]);
		LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);

		vec3 Row[3];

		// Now get scale and shear.
		for (length_t i = 0; i < 3; ++i)
			for (length_t j = 0; j < 3; ++j)
				Row[i][j] = LocalMatrix[i][j];

		// Compute X scale factor and normalize first row.
		scale.x = length(Row[0]);
		Row[0] = detail::scale(Row[0], static_cast<T>(1));
		scale.y = length(Row[1]);
		Row[1] = detail::scale(Row[1], static_cast<T>(1));
		scale.z = length(Row[2]);
		Row[2] = detail::scale(Row[2], static_cast<T>(1));

		// At this point, the matrix (in rows[]) is orthonormal.
		// Check for a coordinate system flip.  If the determinant
		// is -1, then negate the matrix and the scaling factors.
#if 0
		Pdum3 = cross(Row[1], Row[2]); // v3Cross(row[1], row[2], Pdum3);
		if (dot(Row[0], Pdum3) < 0)
		{
			for (length_t i = 0; i < 3; i++)
			{
				scale[i] *= static_cast<T>(-1);
				Row[i] *= static_cast<T>(-1);
			}
		}
#endif

		rotation.y = asin(-Row[0][2]);
		if (cos(rotation.y) != 0) {
			rotation.x = atan2(Row[1][2], Row[2][2]);
			rotation.z = atan2(Row[0][1], Row[0][0]);
		}
		else {
			rotation.x = atan2(-Row[2][0], Row[1][1]);
			rotation.z = 0;
		}


		return true;
	}
	
	void EulerToDirectionVectors(const glm::vec3 eulerAngles, glm::vec3& front, glm::vec3& right, glm::vec3& up) {
		// 计算 front 向量
		front.x = cos(eulerAngles.y) * cos(eulerAngles.x);
		front.y = sin(eulerAngles.x);
		front.z = sin(eulerAngles.y) * cos(eulerAngles.x);
		front = glm::normalize(front);

		// 计算 right 向量
		right = glm::normalize(glm::cross(front, glm::vec3(0, 1, 0)));

		// 计算 up 向量
		up = glm::normalize(glm::cross(right, front));
	}


	void DirectionVectorsToEuler(const glm::vec3 front, const glm::vec3 right, const glm::vec3 up, glm::vec3& eulerAngles) {
		eulerAngles.x = atan2(front.y, glm::sqrt(front.x * front.x + front.z * front.z));
		eulerAngles.y = atan2(front.z, front.x);
		eulerAngles.z = atan2(up.y, right.y);
	}

	GeometryData::Data GetCubeData(uint32_t id)
    {
		auto& vertices = s_GeometryData.s_CubeData.vertices;
		auto& indices = s_GeometryData.s_CubeData.indices;

		if (indices.empty()||vertices.empty())
		{
			vertices.clear();
			vertices.resize(36);
			indices.clear();
			indices.resize(36);
			float data[]= 
			{
				// positions				// normals				// texture coords
				-0.5f, -0.5f, -0.5f,		 0.0f,  0.0f, -1.0f,		 0.0f,  0.0f,
				 0.5f, -0.5f, -0.5f,		 0.0f,  0.0f, -1.0f,		 1.0f,  0.0f,
				 0.5f,  0.5f, -0.5f,		0.0f,  0.0f, -1.0f,		 1.0f,  1.0f,
				 0.5f,  0.5f, -0.5f,		0.0f,  0.0f, -1.0f,		 1.0f,  1.0f,
				-0.5f,  0.5f, -0.5f,		 0.0f,  0.0f, -1.0f,		 0.0f,  1.0f,
				-0.5f, -0.5f, -0.5f,		 0.0f,  0.0f, -1.0f,		 0.0f,  0.0f,

				-0.5f, -0.5f,  0.5f,		 0.0f,  0.0f,  1.0f,		 0.0f,  0.0f,
				 0.5f, -0.5f,  0.5f,		 0.0f,  0.0f,  1.0f,		 1.0f,  0.0f,
				 0.5f,  0.5f,  0.5f,		0.0f,  0.0f,  1.0f,		 1.0f,  1.0f,
				 0.5f,  0.5f,  0.5f,		0.0f,  0.0f,  1.0f,		 1.0f,  1.0f,
				-0.5f,  0.5f,  0.5f,		 0.0f,  0.0f,  1.0f,		 0.0f,  1.0f,
				-0.5f, -0.5f,  0.5f,		 0.0f,  0.0f,  1.0f,		 0.0f,  0.0f,

				-0.5f,  0.5f,  0.5f,		-1.0f,  0.0f,  0.0f,		1.0f,  0.0f,
				-0.5f,  0.5f, -0.5f,		-1.0f,  0.0f,  0.0f,		1.0f,  1.0f,
				-0.5f, -0.5f, -0.5f,		 -1.0f,  0.0f,  0.0f,		 0.0f,  1.0f,
				-0.5f, -0.5f, -0.5f,		 -1.0f,  0.0f,  0.0f,		 0.0f,  1.0f,
				-0.5f, -0.5f,  0.5f,		-1.0f,  0.0f,  0.0f,		0.0f,  0.0f,
				-0.5f,  0.5f,  0.5f,		-1.0f,  0.0f,  0.0f,		1.0f,  0.0f,

				 0.5f,  0.5f,  0.5f,		1.0f,  0.0f,  0.0f,		1.0f,  0.0f,
				 0.5f,  0.5f, -0.5f,		1.0f,  0.0f,  0.0f,		1.0f,  1.0f,
				 0.5f, -0.5f, -0.5f,		 1.0f,  0.0f,  0.0f,		0.0f,  1.0f,
				 0.5f, -0.5f, -0.5f,		 1.0f,  0.0f,  0.0f,		0.0f,  1.0f,
				 0.5f, -0.5f,  0.5f,		1.0f,  0.0f,  0.0f,		0.0f,  0.0f,
				 0.5f,  0.5f,  0.5f,		1.0f,  0.0f,  0.0f,		1.0f,  0.0f,

				-0.5f, -0.5f, -0.5f,		  0.0f, -1.0f,  0.0f,		 0.0f,  1.0f,
				 0.5f, -0.5f, -0.5f,		  0.0f, -1.0f,  0.0f,		 1.0f,  1.0f,
				 0.5f, -0.5f,  0.5f,		 0.0f, -1.0f,  0.0f,		1.0f,  0.0f,
				 0.5f, -0.5f,  0.5f,		 0.0f, -1.0f,  0.0f,		1.0f,  0.0f,
				-0.5f, -0.5f,  0.5f,		  0.0f, -1.0f,  0.0f,		 0.0f,  0.0f,
				-0.5f, -0.5f, -0.5f,		  0.0f, -1.0f,  0.0f,		 0.0f,  1.0f,

				-0.5f,  0.5f, -0.5f,		 0.0f,  1.0f,  0.0f,		 0.0f,  1.0f,
				 0.5f,  0.5f, -0.5f,		 0.0f,  1.0f,  0.0f,		 1.0f,  1.0f,
				 0.5f,  0.5f,  0.5f,		0.0f,  1.0f,  0.0f,		 1.0f,  0.0f,
				 0.5f,  0.5f,  0.5f,		0.0f,  1.0f,  0.0f,		 1.0f,  0.0f,
				-0.5f,  0.5f,  0.5f,		 0.0f,  1.0f,  0.0f,		 0.0f,  0.0f,
				-0.5f,  0.5f, -0.5f,		 0.0f,  1.0f,  0.0f,		 0.0f,  1.0f
			};

			for (size_t i = 0; i < indices.size(); i++)
				indices[i] = i;

			for (int i = 0; i < 36; i++)
			{
				int numColumn = 3 + 3 + 2;
				vertices[i].Position = glm::vec3(data[numColumn * i], data[numColumn * i + 1], data[numColumn * i + 2]);
				vertices[i].Normal = glm::vec3(data[numColumn * i+3], data[numColumn * i + 4], data[numColumn * i + 5]);
				vertices[i].TexCoords = glm::vec2(data[numColumn * i + 6], data[numColumn * i + 7]);
				vertices[i].Tangent = glm::vec3(0);
				vertices[i].BiTangent = glm::vec3(0);
				vertices[i].EntityID = id;
			}
		}
		else
			for (int i = 0; i < 36; i++)
				vertices[i].EntityID = id;
		
		return { vertices ,indices };
    }

	GeometryData::Data GetSphereData(uint32_t id, uint32_t numLatitudeLines, uint32_t numLongitudeLines)
	{
		auto& vertices = s_GeometryData.s_SphereData.vertices;
		auto& indices = s_GeometryData.s_SphereData.indices;

		if (vertices.empty()||indices.empty()|| 
			numLatitudeLines!=s_GeometryData.s_SphereConfig.numLatitudeLines|| 
			numLongitudeLines != s_GeometryData.s_SphereConfig.numLongitudeLines)
		{
			vertices.clear();
			vertices.resize(numLatitudeLines * numLongitudeLines * 2);
			indices.clear();
			indices.resize(numLatitudeLines * numLongitudeLines * 6);
		}
		vertices.reserve(numLatitudeLines * numLongitudeLines * 2);

		float latitudeSpacing = 1.0f / (numLatitudeLines + 1.0f);
		float longitudeSpacing = 2.0f * 3.1415 / numLongitudeLines;

		for (int latitude = 0; latitude <= numLatitudeLines; ++latitude) {
			float phi = (latitude * M_PI) / numLatitudeLines;
			float sinPhi = sin(phi);
			float cosPhi = cos(phi);

			for (int longitude = 0; longitude < numLongitudeLines; ++longitude) {
				float theta = longitude * longitudeSpacing;
				float sinTheta = sin(theta);
				float cosTheta = cos(theta);

				// 顶点位置
				glm::vec3 position = glm::vec3(sinPhi * cosTheta, cosPhi, sinPhi * sinTheta);
				vertices.push_back({ position, position, glm::vec2(longitude * longitudeSpacing, 1.0f - (latitude + 1) * latitudeSpacing) });

				// 顶点法线（与顶点位置相同）
				if (latitude != 0 && latitude != numLatitudeLines) {
					float nextPhi = (latitude + 1) * M_PI / numLatitudeLines;
					float nextSinPhi = sin(nextPhi);
					float nextCosPhi = cos(nextPhi);

					glm::vec3 nextPosition = glm::vec3(nextSinPhi * cosTheta, nextCosPhi, nextSinPhi * sinTheta);
					glm::vec3 edge1 = position - glm::vec3(sinPhi * cosTheta, cosPhi, sinPhi * sinTheta);
					glm::vec3 edge2 = nextPosition - position;
					glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));
					vertices.push_back({ nextPosition, normal, glm::vec2(longitude * longitudeSpacing, 1.0f - (latitude + 1) * latitudeSpacing) });
				}
			}
		}

		return { vertices,indices };
	}




}
