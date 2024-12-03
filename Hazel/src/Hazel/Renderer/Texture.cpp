#pragma once
#include"hzpch.h"
#include"Texture.h"
#include<string>
#include"Renderer.h"
#include"Platform/OpenGL/OpenGLTexture.h"
namespace Hazel {
	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
			switch (Renderer::GetAPI())
			{
			case RendererAPI::API::None:			HZ_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:		return std::make_shared< OpenGLTexture2D>(path);
			}
			HZ_CORE_ASSERT(false, "RendererAPI not find!");

			return nullptr;
		
	}
}