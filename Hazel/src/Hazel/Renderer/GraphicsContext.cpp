#include "hzpch.h"
#include "Hazel/Renderer/GraphicsContext.h"

#include "Hazel/Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Hazel {

	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:    HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		HZ_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
