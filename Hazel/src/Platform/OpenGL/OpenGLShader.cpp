#include "hzpch.h"
#include "OpenGLShader.h"
#include<glad/glad.h>
#include<glm/gtc/type_ptr.hpp>
#include <fstream>
#include"OpenGLDebug.h"
namespace Hazel {

	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;
		if (type == "geometry" )
			return GL_GEOMETRY_SHADER;
		HZ_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		auto shaderSources = ParseFile(filepath);
		Compile(shaderSources);
	}

	OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(sources);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		GLCall(glUseProgram(m_RendererID));
	}

	void OpenGLShader::UnBind() const
	{
		glUseProgram(0);
	}
    void OpenGLShader::UploadUniformInt(const std::string& name, int value)
    {
		GLCall(glUniform1i(GetLocation(name), value));
    }
    void OpenGLShader::UploadUniformFloat(const std::string& name, const float value)
    {
		GLCall(glUniform1f(GetLocation(name), value));
    }
    void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& values)
    {
		GLCall(glUniform2f(GetLocation(name), values.x, values.y));
    }
    void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& values)
    {
		GLCall(glUniform3f(GetLocation(name), values.x, values.y, values.z));
    }

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& values)
	{
		GLCall(glUniform4f(GetLocation(name), values.x, values.y, values.z, values.w));
	}

    void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
    {
		GLCall(glUniformMatrix3fv(GetLocation(name), 1, GL_FALSE, glm::value_ptr(matrix)));
    }

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLCall(glUniformMatrix4fv(GetLocation(name), 1, GL_FALSE, glm::value_ptr(matrix)));
	}

	int OpenGLShader::GetLocation(const std::string& name)
	{
		auto it = m_UniformCach.find(name);
		if (it == m_UniformCach.end())
		{
			GLCall(GLint location = glGetUniformLocation(m_RendererID, name.c_str()));
			if (location == -1)
				HZ_CORE_WARN("Uniform: {0} doesn't exist!", name);
			m_UniformCach[name] = location;
		}
		return m_UniformCach[name];
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::ParseFile(const std::string& filepath)
	{
		std::unordered_map<GLenum, std::string> shaderSources;
		std::unordered_map<GLenum, std::stringstream> shaderSourcesStream;

		std::fstream fstream(filepath);
		std::string line;
		GLenum type = 0;
		while (getline(fstream, line))
		{
			if (line.find("#type") != std::string::npos)
			{
				if (line.find("vertex") != std::string::npos)
					type = ShaderTypeFromString("vertex");
				else if (line.find("fragment") != std::string::npos)
					type = ShaderTypeFromString("fragment");
				else if (line.find("geometry") != std::string::npos)
					type = ShaderTypeFromString("geometry");
			}
			else
				shaderSourcesStream[type] << line << "\n";
		}
		for (auto& it: shaderSourcesStream)
		{
			shaderSources[it.first] = it.second.str();
		}

		return shaderSources;
	}
	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		GLuint program = glCreateProgram();
		std::vector<GLenum> glShaderIDs(shaderSources.size());
		for (auto& kv : shaderSources)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;

			GLuint shader = glCreateShader(type);

			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				HZ_CORE_ERROR("{0}", infoLog.data());
				HZ_CORE_ASSERT(false, "Shader compilation failure!");
				break;
			}

			glAttachShader(program, shader);
			glShaderIDs.push_back(shader);
		}

		m_RendererID = program;

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);

			for (auto id : glShaderIDs)
				glDeleteShader(id);

			HZ_CORE_ERROR("{0}", infoLog.data());
			HZ_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		for (auto id : glShaderIDs)
			glDetachShader(program, id);

	}
}
