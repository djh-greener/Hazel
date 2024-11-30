#pragma once
#include"RenderCommand.h"

namespace Hazel {
	class Renderer {
	public:
		static void BeginScene();
		static void EndScene();
		static void Submit(std::shared_ptr<VertexArray>& vertexArray);
		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
}