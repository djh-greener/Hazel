#include"hzpch.h"
#include "Application.h"
#include"Hazel/Events/ApplicationEvent.h"
#include"Input.h"
#include"Hazel/Camera/OrthographicCamera.h"
#include"Timestep.h"
#include"Hazel/Renderer/Renderer.h"
#include<GLFW/glfw3.h>
#include<glad/glad.h>
namespace Hazel {


	Application* Application::s_Instance = nullptr;
	 Application::Application()
	{
		 HZ_CORE_ASSERT(!s_Instance,"Application alReady Exists!")
		 s_Instance = this;

		 m_Window = std::unique_ptr<Window>(Window::Create());
		 m_Window->SetEventCallback(HZ_BIND_EVENT_FN(Application::OnEvent));
		 m_Window->SetVSync(true);
		 Renderer::Init();

		 m_ImGuiLayer = new ImGuiLayer;
		 PushOverlay(m_ImGuiLayer);
	 }
	 void Application::OnEvent(Event& e)
	 {
		 EventDispatcher dispatcher(e);
		 dispatcher.Dispatch<WindowCloseEvent>(HZ_BIND_EVENT_FN(Application::OnWindowClose));
		 dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(Application::OnWindowResize));
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

	 bool Application::OnWindowResize(WindowResizeEvent& e)
	 {
		 if (e.GetWidth() == 0 || e.GetHeight() == 0)
		 {
			 m_Minimized = true;
			 return false;
		 }

		 m_Minimized = false;
		 Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		 return false;
	 }
	 
	void Application::Run()
	{
		while (m_Running) {
			float time = (float)glfwGetTime();
			float ts = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized) {
				for (auto it : m_LayerStack)
					it->OnUpdate(ts);
			}

			m_ImGuiLayer->Begin();
			for (auto it : m_LayerStack)
				it->OnImGuiRender();
			m_ImGuiLayer->End();


			m_Window->OnUpdate();
		}
	}

}