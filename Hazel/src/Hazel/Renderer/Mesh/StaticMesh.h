#pragma once
#include"glm/glm.hpp"
#include<memory>
#include<vector>

#include"Hazel/Core/Core.h"

namespace Hazel {
	class Shader;
	class VertexArray;
	class VertexBuffer;
	class Texture2D;
	class Material;
	struct StaticMeshVertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
		glm::vec3 Tangent;
		glm::vec3 BiTangent;
		uint32_t	EntityID;
	};

	class StaticMesh
	{
	public:
		Ref<VertexArray> m_VertexArray;
		Ref<VertexBuffer> m_VertexBuffer;
		Ref< Material>m_Material;
		std::string name;

		
		StaticMesh(std::vector<StaticMeshVertex>& vertices, std::vector<uint32_t>& indices, std::vector<Ref<Texture2D>>&& textures);


		void DrawStaticMesh(Ref<Shader>& shader);
	private:
		void SetupMesh(std::vector<StaticMeshVertex>& vertices, std::vector<uint32_t>& indices);
	};

}
