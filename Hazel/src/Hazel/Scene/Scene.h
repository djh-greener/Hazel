#pragma once
#include"entt.hpp"
#include"Hazel/Core/Timestep.h"
#include"Hazel/Camera/EditorCamera.h"
namespace Hazel {
	class Entity;

	class Scene {
	public:
		Scene(uint32_t ViewportWidth, uint32_t ViewportHeight);
		~Scene();
		//run scripts,only render meshes
		void OnUpdateRuntime(Timestep ts);
		//show gizmos, floor grid ,outlines. And for scene in editor, the EditorCamera should be unvisiable, must pass in
		void OnUpdateEditor(Timestep ts, EditorCamera& camera);

		void OnViewportResize(uint32_t width, uint32_t height);
		
		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);
		Entity GetPrimaryCameraEntity();

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
