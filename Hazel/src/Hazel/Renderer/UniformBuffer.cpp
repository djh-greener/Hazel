#include "hzpch.h"
#include "UniformBuffer.h"

#include "Hazel/Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"

namespace Hazel {

	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:    HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLUniformBuffer>(size, binding);
		}

		HZ_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
