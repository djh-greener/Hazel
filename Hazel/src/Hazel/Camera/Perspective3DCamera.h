#pragma once
#include "Camera.h"
namespace Hazel {
	class Perspective3DCameraController;

	class Perspective3DCamera 
	{
		friend Perspective3DCameraController;

		const glm::vec3& GetPosition()const { return m_Position; }
		const glm::vec3& GetRotation()const { return m_Rotation; }
		void SetPosition(const glm::vec3& postion) {
			m_Position = postion;
			RecalculateViewMatrix();
		}
		void SetRotation(const glm::vec3& rotation) {
			m_Rotation = rotation;
			RecalculateViewMatrix();
		}

		const glm::mat4 GetProjectionMatrix()const { return m_Projection; };
		const glm::mat4 GetViewMatrix()const { return m_View; }

	private:
		Perspective3DCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f));
		void RecalculateViewMatrix();
		
	private:
		glm::mat4 m_Projection;
		glm::mat4 m_View;
		glm::vec3 m_Position;
		glm::vec3 m_Rotation;

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
