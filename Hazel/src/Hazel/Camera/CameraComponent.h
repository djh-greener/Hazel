#pragma once
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include<glm/gtx/quaternion.hpp>

#include"Hazel/Camera/SceneCamera.h"
#include"Hazel/Scene/Entity.h"



namespace Hazel {
	struct TransformComponent;
	class Timestep;

	//SceneCamera is derived class can't forword declare in .h 
	//Entity is template class can't forword declare in .h
	struct CameraComponent
	{
		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;

		glm::mat4 GetViewMatrix();
		glm::mat4 GetProjMatrix();

		void OnInput(Timestep ts);
		void Move(Timestep ts, glm::vec3& Position, const glm::vec3& Front, const glm::vec3& Right);
		void Rotate(glm::vec3& Rotation);

		bool IsPrimary()const;
		void SetPrimary();
		void SetNoPrimary();
		Entity Owner;
		SceneCamera Camera;
		float MoveSpeed = 10.f;
		float RotateSpeed = 0.1f;
		glm::vec2 LastMousePos;
		
		static uint32_t PrimaryID;
		bool FixedAspectRatio = false;

	};
}
