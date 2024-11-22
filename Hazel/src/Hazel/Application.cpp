#include"hzpch.h"
#include "Application.h"
#include"Events/ApplicationEvent.h"


namespace Hazel {
#define BIND_EVENT_FN(x) std::bind(&x,this,std::placeholders::_1)

	 Application::Application()
	{
		 m_Window = std::unique_ptr<Window>(Window::Create());
		 m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
	}
	 void Application::OnEvent(Event& e)
	 {
		 EventDispatcher dispatcher(e);
		 dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		 HZ_CORE_TRACE("{0}",e);
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
			m_Window->OnUpdate();
		}
	}

}