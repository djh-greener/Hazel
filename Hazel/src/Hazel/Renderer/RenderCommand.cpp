#include"hzpch.h"
#include"RenderCommand.h"
#include"Platform/OpenGL/OpenGLRendererAPI.h"
namespace Hazel
{
	//TMP
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}
