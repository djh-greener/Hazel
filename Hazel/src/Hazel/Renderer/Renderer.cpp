#include"hzpch.h"
#include"Renderer.h"
#include"Shader.h"
#include"Hazel/Camera/Camera.h"
#include"Platform/OpenGL/OpenGLShader.h"
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
    void Renderer::Submit(Ref<Shader>& shader, Ref<VertexArray>& vertexArray, const glm::mat4& transform)
    {
		shader->Bind();
		
		auto openglShader = std::dynamic_pointer_cast<OpenGLShader>(shader);
		openglShader->UploadUniformMat4("u_View", m_SceneData->View);
		openglShader->UploadUniformMat4("u_Projection", m_SceneData->Projection);
		openglShader->UploadUniformMat4("u_Model", transform);
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
    }
}