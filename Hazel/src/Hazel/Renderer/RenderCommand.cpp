#include"hzpch.h"
#include"RenderCommand.h"
#include"Platform/OpenGL/OpenGLRendererAPI.h"
namespace Hazel
{
	//TMP
	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}
