#pragma once

#include"Hazel/Core/Core.h"

#include"Hazel/Renderer/Framebuffer.h"

#include"entt.hpp"
#include<glm/glm.hpp>
namespace Hazel {
	class Shader;
	class CameraComponent;
	class Renderer3D
	{
	public:
		static void Init();

		static void RendererScene(entt::registry& SceneRegistry);
		static void OnViewportResize(uint32_t ViewportWidth, uint32_t ViewportHeight);
		static Framebuffer& GetLastFramebuffer();
		//static void RenderSceneMeshes(entt::registry& SceneRegistry, Ref<Shader>& CurrentShader);
	};
	void RenderSceneMeshes(entt::registry& SceneRegistry, Ref<Shader>& CurrentShader);
	std::vector<float> CalcProjectionRange(CameraComponent& CameraComp, const glm::mat4& lightViewMatrix);
}
