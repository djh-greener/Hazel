#include"hzpch.h"
#include "StaticMesh.h"


#include"Hazel/Renderer/Material/Material.h"
#include "Hazel/Renderer/Texture.h"
#include "Hazel/Renderer/VertexArray.h"
#include "Hazel/Renderer/Shader.h"
#include"Hazel/Renderer/RenderCommand.h"


namespace Hazel {

	StaticMesh::StaticMesh(std::vector<StaticMeshVertex>& vertices, std::vector<uint32_t>& indices, std::vector<Ref<Texture2D>>&& textures):
		m_Material(CreateRef<Material>(std::move(textures)))
	{
		SetupMesh(vertices, indices);
	}

	void StaticMesh::DrawStaticMesh(Ref<Shader> &shader)
	{
		m_Material->Apply(shader);
		m_VertexArray->Bind();
		RenderCommand::DrawIndexed(m_VertexArray);
		m_VertexArray->UnBind();
		m_Material->UnApply();
	}

	void StaticMesh::SetupMesh(std::vector<StaticMeshVertex>& vertices, std::vector<uint32_t>& indices)
	{
		m_VertexArray = VertexArray::Create();

		m_VertexBuffer = VertexBuffer::Create((uint32_t)(vertices.size() * sizeof(StaticMeshVertex)));
		m_VertexBuffer->SetLayout({
			{ ShaderDataType::Float3,		"a_Position"			},
			{ ShaderDataType::Float3,		"a_Normal"			},
			{ ShaderDataType::Float2,		"a_TexCoord"		},
			{ ShaderDataType::Float3,		"a_Tangent"			},
			{ ShaderDataType::Float3,		"a_BiTangent"		},
			{ ShaderDataType::Int,				"a_EntityID"			},
			});
		m_VertexBuffer->SetData(vertices.data(), (uint32_t)(vertices.size() * sizeof(StaticMeshVertex)));
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		Ref<IndexBuffer> quadIB = IndexBuffer::Create(indices.data(), (uint32_t)(indices.size()));
		m_VertexArray->SetIndexBuffer(quadIB);
		vertices.clear();
		indices.clear();
	}

}
