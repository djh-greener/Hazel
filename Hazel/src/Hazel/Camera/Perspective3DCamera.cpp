#include "hzpch.h"
#include "Perspective3DCamera.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
namespace Hazel {
	Perspective3DCamera::Perspective3DCamera(glm::vec3 position) :
		m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_Up(glm::vec3(0, 1, 0)), m_WorldUp(glm::vec3(0, 1, 0)),
		m_MovementSpeed(10.f), m_MouseSensitivity(0.1f),
		m_Fov(45.f), m_Aspect(1),m_Near(0.1f), m_Far(50.0f)
	{
		m_Position = position;
		m_Rotation = glm::vec3(0);
		RecalculateViewMatrix();
		
		m_Projection = glm::perspective(glm::radians(m_Fov), m_Aspect, m_Near, m_Far);
	}
	void Perspective3DCamera::RecalculateViewMatrix()
	{
		//rotation
		glm::quat quaternion_x = glm::angleAxis(glm::clamp(m_Rotation.x, -90.f, 90.f), glm::vec3(1,0,0));
		glm::quat quaternion_y = glm::angleAxis(m_Rotation.y, glm::vec3(0, 1, 0));
		m_Front = quaternion_y * quaternion_x * glm::vec3(0, 0, -1);
		m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
		m_Up = glm::normalize(glm::cross(m_Right, m_Front));
		
		m_View = glm::lookAt(m_Position, m_Position+m_Front, m_Up);
	}
}
