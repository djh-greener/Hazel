#include"hzpch.h"
#include"Renderer.h"
#include "Hazel/Renderer/Renderer2D.h"

#include"Hazel/Renderer/Shader.h"
#include"Hazel/Camera/Camera.h"
#include"Platform/OpenGL/OpenGLShader.h"
namespace Hazel {
	Scope<Renderer::SceneData> Renderer::m_SceneData = CreateScope<Renderer::SceneData>();
	void Renderer::Init()
	{
		HZ_PROFILE_FUNCTION();

		RenderCommand::Init();
		Renderer2D::Init();
	}
	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
	}
	void Renderer::BeginScene(Camera& camera)
    {
		m_SceneData->Projection = camera.GetProjectionMatrix(); 
		m_SceneData->View = camera.GetViewMatrix();
    }
    void Renderer::EndScene()
    {
    }
	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}
    void Renderer::Submit(Ref<Shader>& shader, Ref<VertexArray>& vertexArray, const glm::mat4& transform)
    {
		shader->Bind();
		
		shader->SetMat4("u_View", m_SceneData->View);
		shader->SetMat4("u_Projection", m_SceneData->Projection);
		shader->SetMat4("u_Model", transform);
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
    }
}