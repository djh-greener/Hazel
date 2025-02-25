#pragma once

#include"Hazel/Core/Core.h"

#include"Hazel/Renderer/Framebuffer.h"

#include"entt.hpp"
namespace Hazel {

	class Renderer3D
	{
	public:
		static void Init();

		static void RendererScene(entt::registry& SceneRegistry);
		static void OnViewportResize(uint32_t ViewportWidth, uint32_t ViewportHeight);
		static Framebuffer& GetLastFramebuffer();
	};

}
