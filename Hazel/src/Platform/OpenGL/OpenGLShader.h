#pragma once
#include"Hazel/Renderer/Shader.h"
#include"glm/glm.hpp"
namespace	Hazel {
	using GLenum = uint32_t;
	class OpenGLShader:public Shader
	{
	public: 
		OpenGLShader(const std::string& filePath);

		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		~OpenGLShader(); 

		void Bind()const override;
		void UnBind()const override;
	
		virtual const std::string& GetName()const override { return m_Name; }
		void UploadUniformInt(const std::string& name, int value);
		void UploadUniformFloat (const std::string& name, const float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& values);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& values);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& values);
		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
	private:
		int GetLocation(const std::string& name);
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);

	private:
		uint32_t m_RendererID;
		std::unordered_map<std::string, int> m_UniformCach;
		std::string m_Name;
	};
}