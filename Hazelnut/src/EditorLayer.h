#pragma once

#include "Hazel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"
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
		void SaveSceneAs();
	private:
		std::unordered_map<std::string,Ref<Framebuffer>> m_Framebuffers;

		Ref<Scene>m_ActiveScene;

		Entity m_CameraEntity;
		//EditorCamera m_EditorCamera;
		bool m_PrimaryCamera = true;

		glm::vec2 m_ViewportSize;
		glm::vec2 m_ViewportBounds[2];
		bool m_ViewportFocused = false, m_ViewportHovered = false;


		Entity m_HoveredEntity;

		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
		ContentBrowserPanel m_ContentBrowserPanel;
		//Gizmo
		int m_GizmoType = -1;
	};

}
