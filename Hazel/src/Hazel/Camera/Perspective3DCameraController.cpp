#include"hzpch.h"
#include "Perspective3DCameraController.h"

#include "Hazel/Core/Input.h"
#include "Hazel/Core/KeyCodes.h"
#include "Hazel/Core/MouseButtonCodes.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Hazel {
	 Perspective3DCameraController::Perspective3DCameraController()
		 :m_Camera()
	{

	}

	void  Perspective3DCameraController::OnUpdate(Timestep ts)
	{
		auto position = m_Camera.GetPosition();
		
		if (Input::IsKeyPressed(HZ_KEY_A))
			position -= m_Camera.m_Right * (m_Camera.m_MovementSpeed * ts);
		else if (Input::IsKeyPressed(HZ_KEY_D))
			position += m_Camera.m_Right * (m_Camera.m_MovementSpeed * ts);

		if (Input::IsKeyPressed(HZ_KEY_W))
			position += m_Camera.m_Front * (m_Camera.m_MovementSpeed * ts);
		else if (Input::IsKeyPressed(HZ_KEY_S))
			position -= m_Camera.m_Front * (m_Camera.m_MovementSpeed * ts);
		m_Camera.SetPosition(position);


		if (Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_RIGHT))
		{
			float velocity = m_Camera.m_MouseSensitivity * ts;
			auto rotation = m_Camera.GetRotation();
			float deltaX = Input::GetMouseX() - m_LastX;
			float deltaY = m_LastY - Input::GetMouseY();
			rotation.x += deltaY * velocity;
			rotation.y += -deltaX * velocity;
			m_Camera.SetRotation(rotation);
		}
		m_LastX = Input::GetMouseX();
		m_LastY = Input::GetMouseY();
	}

	void  Perspective3DCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(Perspective3DCameraController::OnWindowResized));
	}
	bool  Perspective3DCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		float AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.m_Projection = glm::perspective(glm::radians(45.f), AspectRatio, 0.1f, 50.f);

		return false;
	}
}