#pragma once
#include"Hazel/Scene/Components.h"
#include"Hazel/Scene/ScriptableEntity.h"
#include"Hazel/Core/Input.h"
namespace Hazel {
	class CameraController : public ScriptableEntity
	{
	public:
		virtual void OnCreate()override
		{
		}

		virtual void OnDestroy()override
		{
		}

		virtual void OnUpdate(Timestep ts)override
		{
			if (!GetComponent<CameraComponent>().Primary)
				return;

			auto& position = GetComponent<TransformComponent>().Position;
			float speed = 5.0f;

			if (Input::IsKeyPressed(Key::A))
				position.x -= speed * ts;
			if (Input::IsKeyPressed(Key::D))
				position.x += speed * ts;
			if (Input::IsKeyPressed(Key::W))
				position.y += speed * ts;
			if (Input::IsKeyPressed(Key::S))
				position.y -= speed * ts;
		}
	};
}
