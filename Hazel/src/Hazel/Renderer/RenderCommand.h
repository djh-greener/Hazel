#pragma once
#include"RendererAPI.h"
 
namespace Hazel {
	
	class RenderCommand {
	public:
		static void Init() {
			s_RendererAPI->Init();
		}
		static void Clear() {
			s_RendererAPI->Clear();
		}
		static void ClearColor(const glm::vec4& color) {
			s_RendererAPI->SetClearColor(color);
		}
		static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawIndexed(vertexArray);
		}
	private:
		static RendererAPI* s_RendererAPI;
	};
}