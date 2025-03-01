#include"hzpch.h"
#include"BaseGeometryComponent.h"
#include"Hazel/Renderer/Mesh/StaticMesh.h"
#include"Hazel/Renderer/Texture.h"
#include"Hazel/Renderer/Shader.h"
#include"Hazel/Math/Math.h"
namespace Hazel {

	void BaseGeometryComponent::SetTypeName(std::string TypeName)
	{
		if (TypeName == "None")
			SetType(GeometryType::None);
		else if (TypeName == "Cube")
			SetType(GeometryType::Cube);
		else
			SetType(GeometryType::None);

	}

	void BaseGeometryComponent::SetType(GeometryType type)
	{
		Type = type;
		switch (Type)
		{
		case GeometryType::None:
			m_StaticMesh.reset();
			break;
		case GeometryType::Cube:
			generateCube();
			break;
		default:
			HZ_CORE_ERROR("No Support GeometryType");
			break;
		}
	}

	void BaseGeometryComponent::SetTexturePath(std::filesystem::path path)
	{
		if (TexturePath!=path)
		{
			TexturePath = path;
			auto Texture = Texture2D::Create(TexturePath.string(), true, false);
			Texture->SetShaderUniformName("diffuse");
			if (m_StaticMesh)
			{
				m_StaticMesh->textures[0] = Texture;
			}
		}
	}


	void BaseGeometryComponent::DrawMesh(Ref<Shader>& shader)
	{
		if (!m_StaticMesh)
			return;
		m_StaticMesh->DrawStaticMesh(shader);
	}

	void BaseGeometryComponent::generateCube()
	{
		auto CubeData = Math::GetCubeData((uint32_t)Owner);
		std::vector<StaticMeshVertex> vertices = CubeData.vertices;
		std::vector<uint32_t> indices = CubeData.indices;
		std::vector<Ref<Texture2D>>textures;

		if (TexturePath.empty())
		{
			auto defaultTexture = Texture2D::Create("Resources/Icons/SceneHierarchyPanel/TextureIcon.png", true, false);
			defaultTexture->SetShaderUniformName("diffuse");
			textures.push_back(std::move(defaultTexture));
		}
		else
		{
			auto Texture = Texture2D::Create(TexturePath.string(), true, false);
			Texture->SetShaderUniformName("diffuse");
			textures.push_back(std::move(Texture));
		}

		m_StaticMesh = CreateRef<StaticMesh>(std::move(vertices), std::move(indices), std::move(textures));
		SetTexturePath(TexturePath);
	}
	void BaseGeometryComponent::generateSphere()
	{
	}
	void BaseGeometryComponent::generateCylinder()
	{
	}
}
