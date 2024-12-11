#pragma once
#include"Hazel/Core/Layer.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/MouseEvent.h"

namespace Hazel {
	class  ImGuiLayer :public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach()override;
		virtual void OnDetach()override;
		virtual void OnEvent(Event& e)override;
		bool OnWindowResize(WindowResizeEvent& e);
		void Begin();
		void End();
		void LetEventGo(bool flag) { m_LetEventGo = flag; }
	private:
		float m_Time=0.0f;
		bool m_LetEventGo = true;
	};
}