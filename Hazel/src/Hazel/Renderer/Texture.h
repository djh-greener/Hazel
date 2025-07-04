#pragma once

#include "Hazel/Core/Core.h"

#include <string>
namespace Hazel {

	class Texture
	{
	public:
		virtual uint32_t GetWidth()const = 0;
		virtual uint32_t GetHeight()const = 0;
		virtual uint32_t GetRendererID()const = 0;
		virtual const std::string& GetPath() const = 0;
		virtual const std::string GetShaderUniformName() = 0;
		virtual void SetShaderUniformName(const std::string&) = 0;

		virtual void Bind(uint32_t slot = 0) = 0;
		virtual void UnBind(uint32_t slot = 0) = 0;
		virtual void SetData(void* data, uint32_t size) = 0;

		virtual bool IsLoaded() const = 0;
		virtual ~Texture()=default;
		virtual bool operator==(const Texture& other) const = 0;

	};

	class Texture2D :public Texture
	{
	public:
		static Ref<Texture2D> Create(uint32_t width,uint32_t height);

		static Ref<Texture2D> Create(const std::string& path, bool gamma = false, bool flip = true);
		virtual ~Texture2D()=default;
	};
}
