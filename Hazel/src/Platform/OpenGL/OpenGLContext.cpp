#include"hzpch.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include<GLFW/glfw3.h>
#include<glad/glad.h>
namespace Hazel {
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle):
		m_windowHandle(windowHandle)
	{
#ifdef HZ_ENABLE_ASSERTS
		HZ_CORE_ASSERT(m_windowHandle, "Window Handle is null!");
#endif
	}
	void OpenGLContext::Init()
	{
		HZ_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
#ifdef HZ_ENABLE_ASSERTS
		HZ_CORE_ASSERT(status, "GLFW Initialize Failed!");
#endif
		HZ_CORE_INFO("OpenGL Device Info: ");
		HZ_CORE_INFO(" Renderer: {0} ", glGetString(GL_RENDERER));
		HZ_CORE_INFO(" Version: {0} ", glGetString(GL_VERSION));

#ifdef HZ_ENABLE_ASSERTS
		int versionMajor;
		int versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

		HZ_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Hazel requires at least OpenGL version 4.5!");
#endif
	}
	void OpenGLContext::SwapBuffers()
	{
		HZ_PROFILE_FUNCTION();

		glfwSwapBuffers(m_windowHandle);

	}
}