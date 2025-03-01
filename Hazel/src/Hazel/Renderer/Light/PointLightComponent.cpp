#include"hzpch.h"
#include"Hazel/Renderer/Mesh/StaticMesh.h"
#include"Hazel/Renderer/Texture.h"
#include"Hazel/Renderer/Shader.h"
#include"Hazel/Math/Math.h"
#include"PointLightComponent.h"

namespace Hazel {
    void PointLightComponent::generateMesh()
    {
		if (Owner) {
			auto [vertices, indices] = Math::GetCubeData((uint32_t)Owner);

			std::vector<Ref<Texture2D>>textures;
			m_StaticMesh = CreateRef<StaticMesh>(std::move(vertices), std::move(indices), std::move(textures));
		}
		else
			HZ_CORE_WARN("PointLightComponent Has No Owner");

    }
	void PointLightComponent::DrawMesh(Ref<Shader>& shader)
	{
		if (!m_StaticMesh)
			generateMesh();
		shader->Bind();
		shader->SetFloat3("Color", Color);
		m_StaticMesh->DrawStaticMesh(shader);
	}
}
