#include"hzpch.h"
#include "Scene.h"
#include<glm/glm.hpp>

#include "Hazel/Scene/Entity.h"

#include"Hazel/Renderer/Renderer3D.h"
#include"Hazel/Renderer/Light/PointLightComponent.h"
#include"Hazel/Renderer/RenderCommand.h"

#include"Hazel/Scene/Components.h"
#include"Hazel/Camera/CameraComponent.h"
#include"Hazel/Renderer/Mesh/StaticMeshComponent.h"
#include"Hazel/Renderer/Mesh/BaseGeometryComponent.h"
namespace Hazel {
	Scene::Scene() 
	{
	}

	Scene::~Scene()
	{
	}

	void Scene::OnUpdateRuntime(Timestep ts,bool ViewportHovered)
	{
		//-------------------------------------Render--------------------------------------//
		//Find MainCamera
		CameraComponent *MainCameraComp = nullptr;
		{
			auto view = m_Registry.view< CameraComponent>();
			for (auto entity : view)
			{
				auto &camera = view.get< CameraComponent>(entity);
				if (camera.IsPrimary())
				{
					if (ViewportHovered)
						camera.OnInput(ts);
					MainCameraComp = &camera;
					break;
				}
			}
		}


		//Render Scene Using Renderer2D API

		//if (MainCameraComp)
		//{
		//	Renderer2D::BeginScene(*MainCameraComp);
		//
		//	auto group = m_Registry.group<TransformComponent, SpriteRendererComponent>();
		//	for (auto entity : group)
		//	{
		//		auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
		//
		//		Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
		//	}
		//	Renderer2D::EndScene();
		//}
		//Render Scene Using Renderer3D API
		//Only Need To Call Renderer3D to Render Scene
		//beLike: Renderer3D::RendererScene(MainCameraComp,m_Registry);
		Renderer3D::RendererScene(m_Registry);
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;
		Renderer3D::OnViewportResize(m_ViewportWidth, m_ViewportHeight);

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
			if (camera.IsPrimary())
			{
				return Entity{ entity,this };
			}
		}
		return Entity{};
	}

	bool Scene::IsEntityExist(Entity entity) {
		return m_Registry.valid(entity);
	}
	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}


	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{
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
	void Scene::OnComponentAdded<StaticMeshComponent>(Entity entity, StaticMeshComponent& component)
	{
		component.Owner = entity;
	}
	template<>
	void Scene::OnComponentAdded<BaseGeometryComponent>(Entity entity, BaseGeometryComponent& component)
	{
		component.Owner = entity;
	}
	template<>
	void Scene::OnComponentAdded<PointLightComponent>(Entity entity, PointLightComponent& component)
	{
		component.Owner = entity;
	}
}
