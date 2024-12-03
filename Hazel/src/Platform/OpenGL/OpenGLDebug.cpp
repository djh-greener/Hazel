#include"hzpch.h"
#include<glad/glad.h>

#include"OpenGLDebug.h"
namespace Hazel {
	void GLClearError()
	{
		while (glGetError() != GL_NO_ERROR);
	}

	std::string Enum2String(unsigned int ErrorEnum)
	{
		std::string error;
		switch (ErrorEnum)
		{
		case GL_INVALID_ENUM:										error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:										error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:							error = "INVALID_OPERATION"; break;
		case GL_OUT_OF_MEMORY:									error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:	error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		return error;
	}
	bool GLLogCall(const char* function, const char* file, int line)
	{
		while (GLenum error = glGetError()) {
			std::cout << "OpenGL Error :"
				<< Enum2String(error) << " "
				<< function << " "
				<< line << std::endl;
			return false;
		}
		return true;
	}
}