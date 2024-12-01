#include"hzpch.h"
#include"OpenGLContext.h"
#include<GLFW/glfw3.h>
#include<glad/glad.h>
namespace Hazel {
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle):
		m_windowHandle(windowHandle)
	{
		HZ_CORE_ASSERT(m_windowHandle, "Window Handle is null!");
	}
	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		HZ_CORE_ASSERT(status, "GLFW Initialize Failed!");

		HZ_CORE_INFO("OpenGL Device Info: ");
		HZ_CORE_INFO("Renderer: {0} ", glGetString(GL_RENDERER));
		HZ_CORE_INFO("Version: {0} ", glGetString(GL_VERSION));
	}
	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_windowHandle);

	}
}