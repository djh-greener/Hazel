#pragma once
#include"entt.hpp"
#include"Hazel/Core/Timestep.h"
namespace Hazel {
	class Entity;

	class Scene {
	public:
		Scene();
		~Scene();
		void OnUpdate(Timestep ts);
		void OnViewportResize(uint32_t width, uint32_t height);
		
		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		entt::registry m_Registry;
		
		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;

	};
}