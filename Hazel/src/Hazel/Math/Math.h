#pragma once
#include<glm/glm.hpp>
#include"Hazel/Renderer/Mesh/StaticMesh.h"
namespace Hazel::Math{
	bool DeComposeTransform(const glm::mat4& transform, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale);

	void EulerToDirectionVectors(const glm::vec3 eulerAngles, glm::vec3& front, glm::vec3& right, glm::vec3& up);

	void DirectionVectorsToEuler(const glm::vec3 front, const glm::vec3 right, const glm::vec3 up, glm::vec3& eulerAngles);

	//Geometry
	struct GeometryData {
		struct Data
		{
			std::vector<StaticMeshVertex>vertices;
			std::vector<uint32_t>indices;
		};
		struct SphereConfig
		{
			uint32_t numLatitudeLines;
			uint32_t numLongitudeLines;
		};

		Data s_CubeData, s_SphereData;
		SphereConfig s_SphereConfig;
	};

	static GeometryData s_GeometryData;

	GeometryData::Data GetCubeData(uint32_t id);
	GeometryData::Data GetSphereData(uint32_t id, uint32_t numLatitudeLines,uint32_t numLongitudeLines);

}


