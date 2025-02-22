#pragma once
#include"hzpch.h"
#include"CameraComponent.h"
#include"Hazel/Scene/Components.h"
#include"Hazel/Core/Input.h"
#include"Hazel/Math/Math.h"


namespace Hazel {
	uint32_t CameraComponent::PrimaryID = (uint32_t)entt::null;

	glm::mat4 CameraComponent::GetViewMatrix()
	{
		auto& transform = Owner.GetComponent<TransformComponent>();
		glm::vec3 Front, Right, Up;
		Math::EulerToDirectionVectors(transform.Rotation, Front, Right, Up);
		return glm::lookAt(transform.Position, transform.Position + Front, Up);
	}

	glm::mat4 CameraComponent::GetProjMatrix()
	{
		return Camera.GetProjection();
	};

	void CameraComponent::OnInput(Timestep ts)
	{
		if (IsPrimary())
		{
			auto& transform = Owner.GetComponent<TransformComponent>();
			glm::vec3 Front, Right, Up;
			Math::EulerToDirectionVectors(transform.Rotation, Front, Right, Up);
			Move(ts, transform.Position, Front, Right);
			Rotate(transform.Rotation);
		}
	}

	void CameraComponent::Move(Timestep ts, glm::vec3& Position, const glm::vec3& Front, const glm::vec3& Right)
	{
		float velocity = MoveSpeed * ts;

		if (Input::IsKeyPressed(Key::W))
			Position += Front * velocity;
		else if (Input::IsKeyPressed(Key::S))
			Position -= Front * velocity;
		if (Input::IsKeyPressed(Key::A))
			Position -= Right * velocity;
		else if (Input::IsKeyPressed(Key::D))
			Position += Right * velocity;
	}

	void CameraComponent::Rotate(glm::vec3& Rotation)
	{
		auto MousePos = Input::GetMousePosition();
		if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
		{
			float xoffset = MousePos.x - LastMousePos.x;

			float yoffset = LastMousePos.y - MousePos.y;
			xoffset *= RotateSpeed;
			yoffset *= RotateSpeed;
			Rotation.y += xoffset * (float)0.01;//Yaw
			Rotation.x += yoffset * (float)0.01;//Pitch
			
			float AngleLimit = glm::radians(89.0f);
			if (Rotation.x > AngleLimit)
				Rotation.x = AngleLimit;
			if (Rotation.x < -AngleLimit)
				Rotation.x = -AngleLimit;
		}
		LastMousePos = MousePos;
	}
	bool CameraComponent::IsPrimary() const
	{
		return Owner && (uint32_t)Owner == PrimaryID;
	}
	void CameraComponent::SetPrimary()
	{
		PrimaryID = (uint32_t)Owner;
	}
	void CameraComponent::SetNoPrimary()
	{
		PrimaryID = (uint32_t)entt::null;
	}
}

