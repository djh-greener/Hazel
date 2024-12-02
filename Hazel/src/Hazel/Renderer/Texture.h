#pragma once


namespace Hazel {

	class Texture
	{
	public:
		virtual uint32_t GetWidth()const = 0;
		virtual uint32_t GetHeight()const = 0;
		virtual void Bind(uint32_t slot = 0) = 0;
		virtual ~Texture()=default;
	};

	class Texture2D :public Texture
	{
	public:
		static Ref<Texture2D> Create(const std::string& path);
		virtual ~Texture2D()=default;
	};
}