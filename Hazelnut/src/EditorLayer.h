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
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		Hazel::OrthographicCameraController m_CameraController;

		// Temp
		Ref<VertexArray> m_SquareVA;
		Ref<Shader> m_FlatColorShader;
		Ref<Framebuffer> m_Framebuffer;

		Ref<Texture2D> m_CheckerboardTexture;

		Ref<Scene>m_ActiveScene;

		Entity m_CameraEntity;
		Entity m_SecondCamera;

		bool m_PrimaryCamera = true;

		glm::vec2 m_ViewportSize;
		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };


		bool m_ViewportFocused = false, m_ViewportHovered = false;

		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
	};

}