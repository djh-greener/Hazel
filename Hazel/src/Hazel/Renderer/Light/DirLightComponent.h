#pragma once
#include"Hazel/Core/Core.h"
#include"Hazel/Scene/Entity.h"
#include<glm/glm.hpp>

namespace Hazel {
	class StaticMesh;
	class Shader;
	class DirLightComponent
	{
	public:
		DirLightComponent() {
			Color = glm::vec3(1.f);
			Intensity = 1.f;
		};

		void DrawMesh(Ref<Shader>& shader);
	private:
		void generateMesh();
	public:
		Entity Owner;
		Ref<StaticMesh> m_StaticMesh;
		glm::vec3 Color;
		float Intensity;
	};
}
