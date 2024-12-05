#include<Hazel.h>
#include <Hazel/Core/EntryPoint.h>
#include"glm/gtc/matrix_transform.hpp"
#include"glm/gtc/type_ptr.hpp"
#include"Platform/OpenGL/OpenGLShader.h"

#include "Sandbox2D/Sandbox2D.h"
using namespace Hazel;

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer()
		: Layer("Example"),
		m_CameraController(std::make_shared<OrthographicCameraController>(1280.0f / 720.0f,true) )
	{
		
		//Square
		m_VASquare = VertexArray::Create();
		m_VASquare->Bind();
		{
			float squareVertices[4 * 5] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
			};
			Ref<VertexBuffer>vertexBuffer;
			vertexBuffer = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
			BufferLayout layout =
			{
				 {ShaderDataType::Float3,"aPosition"},
				 {ShaderDataType::Float2,"aTexCoord"},

			};
			vertexBuffer->SetLayout(layout);
			m_VASquare->AddVertexBuffer(vertexBuffer);
			
			unsigned int indices[6] = { 0,1,2 ,2,3,0 };
			Ref<IndexBuffer>indexBuffer;
			indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
			m_VASquare->SetIndexBuffer(indexBuffer);

			m_Texture = Texture2D::Create("assets/textures/Checkerboard.png");
			m_LogoTexture = Texture2D::Create("assets/textures/ChernoLogo.png");
		}
		{
			
			auto ShaderTexture = m_ShaderLibrary.Load("assets/shaders/testTexture/texture.glsl");
			std::dynamic_pointer_cast<OpenGLShader>(ShaderTexture)->Bind();
			std::dynamic_pointer_cast<OpenGLShader>(ShaderTexture)->UploadUniformInt("u_Texture", 0);

		}
	}
	void OnEvent(Hazel::Event& e) override
	{
		m_CameraController->OnEvent(e);
	}
	virtual void OnImGuiRender()override
	{
		//ImGui::Begin("Test");
		//ImGui::End();
	}
	void OnUpdate(Timestep ts) override
	{
		
		m_CameraController->OnUpdate(ts);

		RenderCommand::ClearColor({ 0.2f, 0.3f, 0.3f, 1.0f });
		RenderCommand::Clear();
		Renderer::BeginScene(m_CameraController->GetCamera());
		

		m_Texture->Bind();
		Renderer::Submit(m_ShaderLibrary.Get("texture"), m_VASquare, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_LogoTexture->Bind();
		Renderer::Submit(m_ShaderLibrary.Get("texture"), m_VASquare, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		Renderer::EndScene();
	}

private:
	ShaderLibrary m_ShaderLibrary;
	Ref<VertexArray>m_VASquare;
	Ref<Texture2D>m_Texture, m_LogoTexture;

	Ref<OrthographicCameraController> m_CameraController;
	float m_LastX, m_LastY;
};

class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		PushLayer(new Sandbox2D);
	}

	~Sandbox()
	{

	}

};
namespace Hazel {
	Application* CreateApplication()
	{
		return new Sandbox;
	}
}
