#include"hzpch.h"
#include "Application.h"
#include"Events/ApplicationEvent.h"
#include"Hazel/Input.h"
 
#include"Renderer/Buffer.h"
#include"Renderer/Shader.h"
#include"Renderer/VertexArray.h"
#include"Renderer/Renderer.h"

namespace Hazel {


	Application* Application::s_Instance = nullptr;
	 Application::Application()
	{
		 HZ_CORE_ASSERT(!s_Instance,"Application alReady Exists!")
		 s_Instance = this;
		 m_Window = std::unique_ptr<Window>(Window::Create());
		 m_Window->SetEventCallback(HZ_BIND_EVENT_FN(Application::OnEvent));

		 m_ImGuiLayer = new ImGuiLayer;
		 PushOverlay(m_ImGuiLayer);

		 //Triangle
		 m_VATriangle.reset(VertexArray::Create());
		 m_VATriangle->Bind();
		 {
			 float vertices[3 * 7] = {
					-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
					 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
					 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
			 };
			 std::shared_ptr<VertexBuffer>vertexBuffer;
			 vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
			 BufferLayout layout =
			 {
				  {ShaderDataType::Float3,"aPosition"},
				  {ShaderDataType::Float4,"aColor"},
			 };
			 vertexBuffer->SetLayout(layout);
			 m_VATriangle->AddVertexBuffer(vertexBuffer);

			 unsigned int indices[3] = { 0,1,2 };
			 std::shared_ptr<IndexBuffer>indexBuffer;
			 indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
			 m_VATriangle->SetIndexBuffer(indexBuffer);
		 }

		 //Square
		 m_VASquare.reset(VertexArray::Create());
		 m_VASquare->Bind();
		 {
			 float squareVertices[3 * 4] = {
				 -0.75f, -0.75f, 0.0f,
				  0.75f, -0.75f, 0.0f,
				  0.75f,  0.75f, 0.0f,
				 -0.75f,  0.75f, 0.0f
			 };
			 std::shared_ptr<VertexBuffer>vertexBuffer;
			 vertexBuffer.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
			 BufferLayout layout =
			 {
				  {ShaderDataType::Float3,"aPosition"},
			 };
			 vertexBuffer->SetLayout(layout);
			 m_VASquare->AddVertexBuffer(vertexBuffer);

			 unsigned int indices[6] = { 0,1,2 ,2,3,0};
			 std::shared_ptr<IndexBuffer>indexBuffer;
			 indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
			 m_VASquare->SetIndexBuffer(indexBuffer);
		 }
		 std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);	
			}
		)";

		 std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		 m_ShaderTriangle.reset(new Shader(vertexSrc, fragmentSrc));

		 std::string blueVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			void main()
			{
				gl_Position = vec4(a_Position, 1.0);	
			}
		)";

		 std::string blueFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			
			void main()
			{
				color = vec4(0.2f,0.3f,0.8f,1.0f);
			}
		)";

		 m_ShaderSquare.reset(new Shader(blueVertexSrc,blueFragmentSrc));
	 }
	 void Application::OnEvent(Event& e)
	 {
		 EventDispatcher dispatcher(e);
		 dispatcher.Dispatch<WindowCloseEvent>(HZ_BIND_EVENT_FN(Application::OnWindowClose));

		 //pass to all layers
		 for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		 {
			 (*--it)->OnEvent(e);
			 if (e.Handled)
				 break;
		 }
	 }
	 void Application::PushLayer(Layer* layer)
	 {
		 m_LayerStack.PushLayer(layer);
	 }
	 void Application::PushOverlay(Layer* layer)
	 {
		 m_LayerStack.PushOverlay(layer);
	 }
	 bool Application::OnWindowClose(WindowCloseEvent& e)
	 {
		 m_Running = false;
		 return true;
	 }
	void Application::Run()
	{
		while (m_Running) {
			RenderCommand::ClearColor({ 0.2f, 0.3f, 0.3f, 1.0f });
			RenderCommand::Clear();

			Renderer::BeginScene();
					m_ShaderSquare->Bind();
					Renderer::Submit(m_VASquare);

					m_ShaderTriangle->Bind();
					Renderer::Submit(m_VATriangle);
			Renderer::EndScene();



			for (auto it : m_LayerStack) 
				it->OnUpdate();
			

			m_ImGuiLayer->Begin();
			for (auto it : m_LayerStack)
				it->OnImGuiRender();
			m_ImGuiLayer->End();


			m_Window->OnUpdate();
		}
	}

}