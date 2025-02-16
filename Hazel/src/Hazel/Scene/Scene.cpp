#include"hzpch.h"
#include "Scene.h"
#include<glm/glm.hpp>
#include"Hazel/Scene/Components.h"
#include "Hazel/Scene/Entity.h"

#include"Hazel/Renderer/Renderer2D.h"

namespace Hazel {

	Scene::Scene(uint32_t ViewportWidth, uint32_t ViewportHeight):
		m_ViewportWidth(ViewportWidth),
		m_ViewportHeight(ViewportHeight)
	{

	}


	Scene::~Scene()
	{
	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
	{
		Renderer2D::BeginScene(camera);

		auto group = m_Registry.group<TransformComponent, SpriteRendererComponent>();
		for (auto entity : group)
		{
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);

		}

		Renderer2D::EndScene();
	}

	void Scene::OnUpdateRuntime(Timestep ts)
	{
		// -----------------------------Update Components-----------------------------------------------//


		//-------------------------------------Render--------------------------------------//
		//Find MainCamera
		//Camera* mainCamera = nullptr;
		CameraComponent *MainCameraComp = nullptr;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					//Input
					camera.OnUpdate(ts);

					MainCameraComp = &camera;
					break;
				}
			}
		}
		//if Find MainCamera, then Render Scene
		if (MainCameraComp)
		{
			Renderer2D::BeginScene(*MainCameraComp);

			auto group = m_Registry.group<TransformComponent, SpriteRendererComponent>();
			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		// Resize our non-FixedAspectRatio cameras
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.SetViewportSize(width, height);
		}

	}
	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<TagComponent>(name.empty() ? "Entity" : name);
		return entity;
	}
	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto& entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.Primary)
			{
				return Entity{ entity,this };
			}
		}
		return Entity{};
	}


	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.Owner = entity;
		if (m_ViewportWidth > 0 && m_ViewportHeight > 0)
			component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{
	}

	//template<>
	//void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	//{
	//}
}
