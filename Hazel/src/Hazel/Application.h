#pragma once
#include"Core.h"
#include"Window.h"
#include"LayerStack.h"
#include"Events/Event.h"
#include"ImGui/ImGuiLayer.h"
#include"Hazel/Renderer/Shader.h"

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

		inline Window& GetWindow() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }
	private:
		std::unique_ptr<Window>m_Window;
		ImGuiLayer* m_ImGuiLayer;

		bool m_Running = true;
		LayerStack m_LayerStack;

		unsigned int VAO, VBO, EBO;
		Shader* m_Shader;
	private:
		bool OnWindowClose(WindowCloseEvent& e);
	private:
		static Application* s_Instance;//single instance
	};
	Application* CreateApplication();
}