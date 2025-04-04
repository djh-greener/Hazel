#pragma once
#include"hzpch.h"
#include "Hazel/Renderer/Texture.h"
#include "Hazel/Renderer/RendererAPI.h"
#include"Platform/OpenGL/OpenGLTexture.h"
namespace Hazel {
    Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
    {
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:			HZ_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:		return CreateRef< OpenGLTexture2D>(width,height);
		}
		HZ_CORE_ASSERT(false, "RendererAPI not find!");

		return nullptr;
    }
    Ref<Texture2D> Texture2D::Create(const std::string& path, bool gamma, bool flip)
	{
			switch (RendererAPI::GetAPI())
			{
			case RendererAPI::API::None:			HZ_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:		return CreateRef< OpenGLTexture2D>(path, gamma,flip);
			}
			HZ_CORE_ASSERT(false, "RendererAPI not find!");

			return nullptr;
		
	}
}
