#pragma once
#include"Core.h"
#include"Window.h"
#include"LayerStack.h"
#include"Hazel/Events/Event.h"
#include"Hazel/ImGui/ImGuiLayer.h"
#include<memory>


namespace Hazel {

	class WindowCloseEvent;
	class WindowResizeEvent;
	class HAZEL_API  Application
	{
	public:
		Application();
		virtual ~Application() = default;
		void Run();
		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }
	private:
		std::unique_ptr<Window>m_Window;
		ImGuiLayer* m_ImGuiLayer;
		LayerStack m_LayerStack;

		bool m_Running = true;
		float m_LastFrameTime = 0.0f;

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		static Application* s_Instance;//single instance
		bool m_Minimized = false;
	};
	Application* CreateApplication();
}