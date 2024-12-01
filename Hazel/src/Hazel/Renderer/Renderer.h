#pragma once
#include"RenderCommand.h"

namespace Hazel {
	class Camera;
	class Shader;
	class Renderer {
	public:
		static void BeginScene(Camera& camera);
		static void EndScene();
		static void Submit(std::shared_ptr<Shader>& shader, std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.f));
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