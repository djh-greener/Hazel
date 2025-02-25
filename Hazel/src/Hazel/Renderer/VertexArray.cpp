#include "hzpch.h"
#include "Hazel/Renderer/VertexArray.h"
#include "Hazel/Renderer/RendererAPI.h"
#include"Platform/OpenGL/OpenGLVertexArray.h"
namespace Hazel {
	Ref<VertexArray> VertexArray::Create()
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:			HZ_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:		return CreateRef< OpenGLVertexArray>();
		}
		HZ_CORE_ASSERT(false, "RendererAPI not find!");

		return nullptr;
	}
}
