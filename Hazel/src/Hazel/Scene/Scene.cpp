#include"hzpch.h"
#include "Scene.h"
#include<glm/glm.hpp>
#include"Hazel/Scene/Components.h"
#include "Hazel/Scene/Entity.h"

#include"Hazel/Renderer/Renderer2D.h"

namespace Hazel {

	Scene::Scene()
	{

	}


	Scene::~Scene()
	{
	}
	void Scene::OnUpdate(Timestep ts)
	{
		//Render
		auto group = m_Registry.group<TransformComponent, SpriteRendererComponent>();
		for (auto entity : group)
		{
			auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::DrawQuad(transform, sprite.Color);
		}

	}
	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}
}