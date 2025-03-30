#pragma once
#include"Hazel/Core/Core.h"
#include"Hazel/Scene/Entity.h"
#include<glm/glm.hpp>

namespace Hazel {
	class StaticMesh;
	class Shader;
	class PointLightComponent 
	{
	public:
		PointLightComponent() { 
			Color = glm::vec3(1.f); 
			Linear = 0.7f;
			Quadratic = 1.8f;
			Intensity = 1.f;
		};
		
		void DrawMesh(Ref<Shader>& shader);
	private:
		void generateMesh();
	public:
		Entity Owner;
		Ref<StaticMesh> m_StaticMesh;
		glm::vec3 Color;
		float Linear;
		float Quadratic;
		float Intensity;
	};
}
