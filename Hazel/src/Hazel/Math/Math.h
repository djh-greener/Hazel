#pragma once
#include<glm/glm.hpp>
#include"Hazel/Renderer/Mesh/StaticMesh.h"
namespace Hazel::Math{
	bool DeComposeTransform(const glm::mat4& transform, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale);

	void EulerToDirectionVectors(const glm::vec3 eulerAngles, glm::vec3& front, glm::vec3& right, glm::vec3& up);

	void DirectionVectorsToEuler(const glm::vec3 front, const glm::vec3 right, const glm::vec3 up, glm::vec3& eulerAngles);

	struct GeometryData {
		struct CubeData
		{
			std::vector<StaticMeshVertex>vertices;
			std::vector<uint32_t>indices;
		}s_CubeData;

	};

	static GeometryData s_GeometryData;
	GeometryData::CubeData GetCubeData(uint32_t id);
	

}


