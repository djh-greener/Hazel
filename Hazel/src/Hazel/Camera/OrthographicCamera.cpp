#include "hzpch.h"
#include "Hazel/Camera/OrthographicCamera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Hazel {
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top) 
	{
		HZ_PROFILE_FUNCTION();
		Init(left, right, bottom, top);
	}

	void OrthographicCamera::RecalculateProjectionViewMatrix()
	{
		HZ_PROFILE_FUNCTION();
		glm::mat4 transform = glm::translate(glm::mat4(1), m_Position)*
			glm::rotate(glm::mat4(1), glm::radians(m_Rotation.z), glm::vec3(0, 0, 1));
		 
		m_View = glm::inverse(transform);

	}
	void OrthographicCamera::Init(float left, float right, float bottom, float top)
	{
		HZ_PROFILE_FUNCTION();
		m_Projection = glm::ortho(left, right, bottom, top, -1.f, 1.f);
		m_View = glm::mat4(1);
		m_Position = glm::vec3(0);
		m_Rotation = glm::vec3(0);
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		HZ_PROFILE_FUNCTION();
		m_Projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
	}

}
