#pragma once

#include "Hazel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"
//REMOVE
#include"Hazel/Renderer/StaticMeshComponent.h"
#include"Hazel/Renderer/Shader.h"
namespace Hazel {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e) override;

	private:
		bool OnKeyPressed(KeyPressedEvent& e);

		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);
		void SaveScene();
		void SaveSceneAs();
	private:

		Ref<Scene>m_ActiveScene;
		std::filesystem::path m_ActiveScenePath;

		//Entity m_CameraEntity;
		Entity m_HoveredEntity;

		glm::vec2 m_ViewportSize;
		glm::vec2 m_ViewportBounds[2];
		bool m_ViewportFocused = false, m_ViewportHovered = false;


		// Panels
		Ref<SceneHierarchyPanel> m_SceneHierarchyPanel;
		Ref<ContentBrowserPanel> m_ContentBrowserPanel;
		//Gizmo
		int m_GizmoType = -1;
	};

}
