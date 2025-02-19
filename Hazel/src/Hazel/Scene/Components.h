#pragma once
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include<glm/gtx/quaternion.hpp>

#include "Hazel/Camera/SceneCamera.h"
#include"Hazel/Scene/Entity.h"

#include"Hazel/Core/Input.h"
#include"Hazel/Math/Math.h"
#include"Hazel/Renderer/Texture.h"
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
		//TODO: Serialize
		Ref<Texture2D> Texture;
		float TilingFactor = 1.0f;
		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}

	};


}
