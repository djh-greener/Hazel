#pragma once

#include "Hazel.h"
#include "Panels/SceneHierarchyPanel.h"
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
		void SaveSceneAs();
	private:
		Hazel::OrthographicCameraController m_CameraController;

		Ref<Framebuffer> m_Framebuffer;

		Ref<Scene>m_ActiveScene;

		Entity m_CameraEntity;
		Entity m_SecondCamera;
		EditorCamera m_EditorCamera;
		bool m_PrimaryCamera = true;

		glm::vec2 m_ViewportSize;
		bool m_ViewportFocused = false, m_ViewportHovered = false;

		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;

		//Gizmo
		int m_GizmoType = -1;
	};

}
