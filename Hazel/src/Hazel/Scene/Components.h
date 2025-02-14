#pragma once
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include<glm/gtx/quaternion.hpp>

#include "Hazel/Camera/SceneCamera.h"
#include"Hazel/Scene/Entity.h"

#include"Hazel/Core/Input.h"
#include"Hazel/Math/Math.h"
namespace Hazel {

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
	};

	struct TransformComponent 
	{
		glm::vec3 Position = { 0.0f,0.0f,0.0f };
		glm::vec3 Rotation= { 0.0f,0.0f,0.0f };//store in rad, show in degree
		glm::vec3 Scale = { 1.0f,1.0f,1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& Position)
			: Position(Position) {}

		glm::mat4 GetTransform()const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
			return glm::translate(glm::mat4(1.f), Position)
				* rotation
				* glm::scale(glm::mat4(1), Scale);
		}
	};

	struct SpriteRendererComponent 
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}
	};

	struct CameraComponent 
	{
		Entity Owner;
		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	public:
		glm::mat4 GetViewMatrix()
		{
			auto& transform = Owner.GetComponent<TransformComponent>();
			glm::vec3 Front, Right, Up;
			Math::EulerToDirectionVectors(transform.Rotation, Front, Right, Up);
			return glm::lookAt(transform.Position, transform.Position + Front, Up);
		}
		glm::mat4 GetProjMatrix()
		{
			return Camera.GetProjection();
		};
		void OnUpdate(Timestep ts)
		{
			if (!Primary)
				return;
			if (Owner)
			{
				auto& transform = Owner.GetComponent<TransformComponent>();
				glm::vec3 Front, Right, Up;
				Math::EulerToDirectionVectors(transform.Rotation, Front, Right, Up);
				Move(ts, transform.Position,Front, Right);
				Rotate(transform.Rotation);
			}
		}
		void Move(Timestep ts, glm::vec3& Position, const glm::vec3& Front, const glm::vec3& Right)
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

		void Rotate(glm::vec3& Rotation)
		{
			auto MousePos = Input::GetMousePosition();
			if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
			{
				float xoffset = MousePos.x - LastMousePos.x;
				float yoffset = LastMousePos.y - MousePos.y;
				xoffset *= RotateSpeed;
				yoffset *= RotateSpeed;
				Rotation.y += xoffset*0.01;//Yaw
				Rotation.x += yoffset*0.01;//Pitch

				// make sure that when pitch is out of bounds, screen doesn't get flipped
				float AngleLimit = glm::radians(89.0f);
				if (Rotation.x > AngleLimit)
					Rotation.x = AngleLimit;
				if (Rotation.x < -AngleLimit)
					Rotation.x = -AngleLimit;
			}
			LastMousePos = MousePos;
		}



		SceneCamera Camera;
		float MoveSpeed = 10.f;
		float RotateSpeed = 0.1f;

		glm::vec2 LastMousePos;

		bool Primary = true;
		bool FixedAspectRatio = false;

	};
	//struct NativeScriptComponent :public BaseComponent
	//{
	//	ScriptableEntity* Instance = nullptr;
	//	std::function<ScriptableEntity* ()> InstantiateScript;
	//	std::function<void(NativeScriptComponent*)> DestroyScript;
	//
	//	template<typename T>
	//	void Bind()
	//	{
	//		InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
	//		DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
	//	}
	//};

}
