#pragma once
#include "Camera.h"
namespace Hazel {
	class Perspective3DCameraController;

	class Perspective3DCamera :public Camera
	{
		friend Perspective3DCameraController;
	private:
		Perspective3DCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f));
		virtual void RecalculateProjectionViewMatrix()override;
		
	private:
		glm::vec3 m_Front;
		glm::vec3 m_Up;
		glm::vec3 m_Right;
		glm::vec3 m_WorldUp;

		float m_MovementSpeed;
		float m_MouseSensitivity;

		float m_Near, m_Far;
		float m_Fov,m_Aspect;
	};
}
