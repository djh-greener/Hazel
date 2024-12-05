#pragma once
#include"glm/glm.hpp"
namespace Hazel
{
	class Camera
	{
	public:
		Camera() = default;

		const glm::vec3& GetPosition()const { return m_Position; }
		const glm::vec3& GetRotation()const { return m_Rotation; }
		void SetPosition(const glm::vec3& postion) {
			m_Position = postion;
			RecalculateProjectionViewMatrix();
		}
		void SetRotation(const glm::vec3& rotation) {
			m_Rotation = rotation;
			RecalculateProjectionViewMatrix();
		}

		const glm::mat4 GetProjectionMatrix()const  { return m_Projection; };
		const glm::mat4 GetViewMatrix()const  { return m_View; }

	protected:
		virtual void RecalculateProjectionViewMatrix() = 0;
	protected:

		glm::mat4 m_Projection;
		glm::mat4 m_View;
		glm::vec3 m_Position;
		glm::vec3 m_Rotation;//pitch,yaw,row<--->rotation.xyz  (degree)
	};
}