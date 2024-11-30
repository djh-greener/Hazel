#include"hzpch.h"
#include"Renderer.h"
#include"Shader.h"
#include"Hazel/Camera/Camera.h"
namespace Hazel {
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;
    void Renderer::BeginScene(Camera& camera)
    {
		m_SceneData->Projection = camera.GetPorjectionMatrix(); 
		m_SceneData->View = camera.GetViewMatrix();
    }
    void Renderer::EndScene()
    {
    }
    void Renderer::Submit(const std::shared_ptr<Shader>& shader, std::shared_ptr<VertexArray>& vertexArray)
    {
		shader->Bind();
		shader->UploadUniformMat4("u_View", m_SceneData->View);
		shader->UploadUniformMat4("u_Projection", m_SceneData->Projection);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
    }
}