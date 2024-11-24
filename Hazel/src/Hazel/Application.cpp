#include"hzpch.h"
#include "Application.h"
#include"Events/ApplicationEvent.h"
#include<glad/glad.h>
#include"GLFW/glfw3.h"

namespace Hazel {
#define BIND_EVENT_FN(x) std::bind(&x,this,std::placeholders::_1)
	Application* Application::s_Instance = nullptr;
	 Application::Application()
	{
		 HZ_CORE_ASSERT(!s_Instance,"Application alReady Exists!")
		 s_Instance = this;
		 m_Window = std::unique_ptr<Window>(Window::Create());
		 m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
	}
	 void Application::OnEvent(Event& e)
	 {
		 EventDispatcher dispatcher(e);
		 dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

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
			glClearColor(0.2, 0.3, 0.3, 1.0);
			for (auto it : m_LayerStack) {
				it->OnUpdate();
			}
			m_Window->OnUpdate();
		}
	}

}