#pragma once
#include<string>

namespace	Hazel {

	class Shader
	{
	public:
		virtual ~Shader() = default;
		virtual void Bind()const = 0;
		virtual void UnBind()const = 0;

		static Ref<Shader> Create(const std::string& vertexSrc, const std::string& fragmentSrc);
		static Ref<Shader> Create(const std::string& filePath);

	};
}