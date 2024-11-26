#include"hzpch.h"
#include "Application.h"
#include"Events/ApplicationEvent.h"
#include<glad/glad.h>
#include"Hazel/Input.h"
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

		 glGenVertexArrays(1, &VAO);
		 glBindVertexArray(VAO);
		 glGenBuffers(1, &VBO);
		 glBindBuffer(GL_ARRAY_BUFFER, VBO);

		 float vertices[3 * 3] = {
			-0.5f, -0.5f,	0.0f,
			 0.5f,  -0.5f,  0.0f,
			 0.0f,   0.5f,  0.0f,
		 };
		 glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		 glEnableVertexAttribArray(0);
		 glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
		 
		 glGenBuffers(1, &EBO);
		 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		 unsigned int indices[3] = { 0,1,2 };
		 glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		 std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);	
			}
		)";

		 std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
			}
		)";

		 m_Shader = new Shader(vertexSrc,fragmentSrc);
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
		 layer->OnAttach();

	 }
	 void Application::PushOverlay(Layer* layer)
	 {
		 m_LayerStack.PushOverlay(layer);
		 layer->OnAttach();

	 }
	 bool Application::OnWindowClose(WindowCloseEvent& e)
	 {
		 m_Running = false;
		 return true;
	 }
	Application::~Application()
	{
	}
	void Application::Run()
	{
		while (m_Running) {
			glClear(GL_COLOR_BUFFER_BIT);
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

			m_Shader->Bind();
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT,nullptr);






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