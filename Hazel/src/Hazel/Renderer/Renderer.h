#pragma once
#include"RenderCommand.h"

namespace Hazel {
	class Camera;
	class Shader;
	class Renderer {
	public:
		static void Init();
		static void BeginScene(Camera& camera);
		static void EndScene();
		static void Submit(Ref<Shader>& shader, Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.f));
		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 Projection;
			glm::mat4 View;
		};
		static SceneData* m_SceneData;
	};
}