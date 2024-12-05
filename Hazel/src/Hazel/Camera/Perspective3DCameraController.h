#pragma once
#include "Hazel/Camera/Perspective3DCamera.h"
#include "Hazel/Core/Timestep.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"

namespace Hazel {

	class Perspective3DCameraController
	{
	public:
		Perspective3DCameraController();

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		Perspective3DCamera& GetCamera() { return m_Camera; }
		const Perspective3DCamera& GetCamera() const { return m_Camera; }
	private:
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		Perspective3DCamera m_Camera;
		float m_LastX, m_LastY;
	};

}