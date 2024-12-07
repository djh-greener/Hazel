#include"hzpch.h"
#include "Application.h"
#include"Hazel/Events/ApplicationEvent.h"
#include"Hazel/Core/Input.h"
#include"Hazel/Camera/OrthographicCamera.h"
#include"Hazel/Core/Timestep.h"
#include"Hazel/Renderer/Renderer.h"
#include<GLFW/glfw3.h>
#include<glad/glad.h>
namespace Hazel {


	Application* Application::s_Instance = nullptr;
	 Application::Application()
	{
		 HZ_PROFILE_FUNCTION();
		 HZ_CORE_ASSERT(!s_Instance,"Application alReady Exists!")
		 s_Instance = this;

		 m_Window = Window::Create();
		 m_Window->SetEventCallback(HZ_BIND_EVENT_FN(Application::OnEvent));
		 m_Window->SetVSync(true);
		 Renderer::Init();

		 m_ImGuiLayer = new ImGuiLayer;
		 PushOverlay(m_ImGuiLayer);
	 }
	 Application::~Application()
	 {
		 HZ_PROFILE_FUNCTION();
		 Renderer::Shutdown();
	 }
	 void Application::OnEvent(Event& e)
	 {
		 HZ_PROFILE_FUNCTION();
		 EventDispatcher dispatcher(e);
		 dispatcher.Dispatch<WindowCloseEvent>(HZ_BIND_EVENT_FN(Application::OnWindowClose));
		 dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(Application::OnWindowResize));
		 //pass to all layers
		 for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend();++it)
		 {
			 (*it)->OnEvent(e);
			 if (e.Handled)
				 break;
		 }
	 }
	 void Application::PushLayer(Layer* layer)
	 {
		 HZ_PROFILE_FUNCTION();
		 m_LayerStack.PushLayer(layer);
		 layer->OnAttach();
	 }
	 void Application::PushOverlay(Layer* layer)
	 {
		 HZ_PROFILE_FUNCTION();
		 m_LayerStack.PushOverlay(layer);
		 layer->OnAttach();
	 }
	 bool Application::OnWindowClose(WindowCloseEvent& e)
	 {
		 m_Running = false;
		 return true;
	 }

	 bool Application::OnWindowResize(WindowResizeEvent& e)
	 {
		 HZ_PROFILE_FUNCTION();
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
		HZ_PROFILE_FUNCTION();
		while (m_Running) {
			HZ_PROFILE_SCOPE("RunLoop");
			float time = (float)glfwGetTime();
			float ts = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				{
					HZ_PROFILE_SCOPE("LayerStack OnUpdate");
					for (auto it : m_LayerStack)
						it->OnUpdate(ts);
				}
				{
					HZ_PROFILE_SCOPE("LayerStack OnImGuiRender");

					m_ImGuiLayer->Begin();
					for (auto it : m_LayerStack)
						it->OnImGuiRender();
					m_ImGuiLayer->End();

				}
				m_Window->OnUpdate();
			}
		}
	}

}