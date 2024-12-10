#pragma once
#include"Hazel/Core/Core.h"
#include"Hazel/Core/Window.h"
#include"Hazel/Core/LayerStack.h"
#include"Hazel/Events/Event.h"
#include"Hazel/ImGui/ImGuiLayer.h"
#include<memory>


int main(int argc, char** argv);
namespace Hazel {

	class WindowCloseEvent;
	class WindowResizeEvent;
	class  Application
	{
	public:
		Application(const std::string& name = "Hazel App");
		virtual ~Application();
		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		void Close();
		inline Window& GetWindow() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }
	private:
		std::unique_ptr<Window>m_Window;
		ImGuiLayer* m_ImGuiLayer;
		LayerStack m_LayerStack;

		bool m_Running = true;
		float m_LastFrameTime = 0.0f;

	private:
		void Run();
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		friend int ::main(int argc, char** argv);
	private:
		static Application* s_Instance;//single instance
		bool m_Minimized = false;
	};
	Application* CreateApplication();
}