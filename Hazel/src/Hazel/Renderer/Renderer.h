#pragma once
#include"Hazel/Renderer/RenderCommand.h"
namespace Hazel {
	class OrthographicCamera;
	class Shader;
	class Renderer {
	public:
		static void Init();		
		static void Shutdown();

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();
		static void OnWindowResize(uint32_t width, uint32_t height);
		static void Submit(Ref<Shader>& shader, Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.f));
		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 Projection;
			glm::mat4 View;
		};
		static Scope<SceneData> m_SceneData;
	};
}