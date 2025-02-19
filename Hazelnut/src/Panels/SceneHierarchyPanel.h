#pragma once

#include "Hazel/Core/Core.h"
#include "Hazel/Scene/Scene.h"
#include "Hazel/Scene/Entity.h"
#include"Hazel/Renderer/Texture.h"
namespace Hazel {

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetScene(const Ref<Scene>& scene);
		Entity GetSelectedEntity() const{ return m_SelectionEntity; }
		void SetSelectedEntity(Entity entity);
		void OnImGuiRender();
	private:
		template<typename T>
		void DisplayAddComponentEntry(const std::string& entryName);

		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
	private:
		Ref<Scene> m_Scene;
		Entity m_SelectionEntity;
		Ref<Texture2D>m_DefaultTextureIcon;
	};

}
