#pragma once
#include"Core.h"
#include"Window.h"
#include"LayerStack.h"
#include"Events/Event.h"

namespace Hazel {
	class WindowCloseEvent;
	class HAZEL_API  Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
	private:
		std::unique_ptr<Window>m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
		bool OnWindowClose(WindowCloseEvent& e);
	};
	Application* CreateApplication();
}