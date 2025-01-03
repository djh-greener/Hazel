#pragma once
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include<glm/gtx/quaternion.hpp>

#include "Hazel/Scene/SceneCamera.h"
#include"Hazel/Scene/ScriptableEntity.h"
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
		glm::vec3 Rotation= { 0.0f,0.0f,0.0f };
		glm::vec3 Scale = { 1.0f,1.0f,1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& Position)
			: Position(Position) {}

		glm::mat4 GetTranform()const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(glm::radians(Rotation)));
				//glm::rotate(glm::mat4(1.f), glm::radians(Rotation.x), { 1,0,0 })
				//* glm::rotate(glm::mat4(1.f), glm::radians(Rotation.y), { 0,1,0 })
				//* glm::rotate(glm::mat4(1.f), glm::radians(Rotation.z), { 0,0,1 });
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
		SceneCamera Camera;
		bool Primary = true; // TODO: think about moving to Scene
		bool FixedAspectRatio = false;
		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};
	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;
		std::function<ScriptableEntity* ()> InstantiateScript;
		std::function<void(NativeScriptComponent*)> DestroyScript;


		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};

}
