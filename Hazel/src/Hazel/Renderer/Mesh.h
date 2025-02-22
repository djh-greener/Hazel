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

	struct Vertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
		glm::vec3 Tangent;
		glm::vec3 BiTangent;
		uint32_t	EntityID;
	};

	class Mesh
	{
	public:
		Ref<VertexArray> m_VertexArray;
		Ref<VertexBuffer> m_VertexBuffer;

		std::vector<Vertex>vertices;
		std::vector<uint32_t>indices;
		std::vector<Ref<Texture2D>> textures;


		//Mesh(std::vector<Vertex>&& vertices, std::vector<uint32_t>&& indices, std::vector<Ref<Texture2D>>&& textures);
		Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Ref<Texture2D>> textures);

		void SetupMesh();
		void DrawMesh(Ref<Shader> shader);

	};

}
