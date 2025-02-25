#include"hzpch.h"
#include "StaticMesh.h"



#include "Hazel/Renderer/Texture.h"
#include "Hazel/Renderer/VertexArray.h"
#include "Hazel/Renderer/Shader.h"
#include"Hazel/Renderer/RenderCommand.h"


namespace Hazel {

	StaticMesh::StaticMesh(std::vector<StaticMeshVertex>&& vertices, std::vector<uint32_t>&& indices, std::vector<Ref<Texture2D>>&& textures):
		vertices(std::move(vertices)),
		indices(std::move(indices)),
		textures(std::move(textures))
	{
		SetupStaticMesh();
	}
	void StaticMesh::SetupStaticMesh()
	{
		m_VertexArray = VertexArray::Create();

		m_VertexBuffer = VertexBuffer::Create(vertices.size() * sizeof(StaticMeshVertex));
		m_VertexBuffer->SetLayout({
			{ ShaderDataType::Float3,		"a_Position"			},
			{ ShaderDataType::Float3,		"a_Normal"			},
			{ ShaderDataType::Float2,		"a_TexCoord"		},
			{ ShaderDataType::Float3,		"a_Tangent"			},
			{ ShaderDataType::Float3,		"a_BiTangent"		},
			{ ShaderDataType::Int,				"a_EntityID"			},
			});
		m_VertexBuffer->SetData(vertices.data(), vertices.size() * sizeof(StaticMeshVertex));
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		Ref<IndexBuffer> quadIB = IndexBuffer::Create(indices.data(), indices.size());
		m_VertexArray->SetIndexBuffer(quadIB);

		//TODO: Set vertices¡¢indices empty
	}
	void StaticMesh::DrawStaticMesh(Ref<Shader> shader)
	{
		unsigned int diffuseNr = 0, specularNr = 0, ambientNr = 0, normalNr = 0;
		for (int i = 0; i < textures.size(); i++) {
			textures[i]->Bind(i);
			shader->Bind();
			std::string varName = "material." + textures[i]->GetShaderUniformName();
			if (textures[i]->GetShaderUniformName() == "diffuse")
				varName += std::to_string(++diffuseNr);
			else if (textures[i]->GetShaderUniformName() == "specular")
				varName += std::to_string(++specularNr);
			else if (textures[i]->GetShaderUniformName() == "ambient")
				varName += std::to_string(++ambientNr);
			else if (textures[i]->GetShaderUniformName() == "normal")
				varName += std::to_string(++normalNr);
			else
				HZ_ERROR("Error Texture Type!");
			shader->SetInt(varName, i);
		}
		m_VertexArray->Bind();
		RenderCommand::DrawIndexed(m_VertexArray);
		m_VertexArray->UnBind();
	}

}
