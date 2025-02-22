#include"hzpch.h"
#include "Mesh.h"



#include "Hazel/Renderer/Texture.h"
#include "Hazel/Renderer/VertexArray.h"
#include "Hazel/Renderer/Shader.h"
#include"Hazel/Renderer/RenderCommand.h"


namespace Hazel {

	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Ref<Texture2D>> textures):
		vertices((vertices)),
		indices((indices)),
		textures((textures))
	{
		SetupMesh();
	}
	void Mesh::SetupMesh()
	{
		m_VertexArray = VertexArray::Create();

		m_VertexBuffer = VertexBuffer::Create(vertices.size() * sizeof(Vertex));
		m_VertexBuffer->SetLayout({
			{ ShaderDataType::Float3,		"a_Position"			},
			{ ShaderDataType::Float3,		"a_Normal"			},
			{ ShaderDataType::Float2,		"a_TexCoord"		},
			{ ShaderDataType::Float3,		"a_Tangent"			},
			{ ShaderDataType::Float3,		"a_BiTangent"		},
			{ ShaderDataType::Int,				"a_EntityID"			},
			});
		m_VertexBuffer->SetData(vertices.data(), vertices.size() * sizeof(Vertex));
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		Ref<IndexBuffer> quadIB = IndexBuffer::Create(indices.data(), indices.size());
		m_VertexArray->SetIndexBuffer(quadIB);
		m_VertexArray->UnBind();
		m_VertexBuffer->UnBind();
		quadIB->UnBind();
		//TODO: Set vertices¡¢indices empty
	}
	void Mesh::DrawMesh(Ref<Shader> shader)
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
