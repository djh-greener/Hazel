#include "EditorLayer.h"
#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include"Hazel/Scene/SceneSerializer.h"
#include"Hazel/Utils/PlatformUtils.h"
#include"ImGuizmo.h"
#include"Hazel/Math/Math.h"
#include"Hazel/Camera/CameraController.h"
#include <glad/glad.h>

namespace Hazel {

	EditorLayer::EditorLayer()
		: Layer("EditorLayer")
	{
	}

	void EditorLayer::OnAttach()
	{
		HZ_PROFILE_FUNCTION();

		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		fbSpec.Samples = 1;
		m_Framebuffers["Default"] = Framebuffer::Create(fbSpec);
		


		m_ActiveScene = CreateRef<Scene>((uint32_t)fbSpec.Width, (uint32_t)fbSpec.Height);

		m_CameraEntity = m_ActiveScene->CreateEntity("Camera");
		m_CameraEntity.AddComponent<CameraComponent>();
		//m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();

		m_SceneHierarchyPanel.SetScene(m_ActiveScene);

	}

	void EditorLayer::OnDetach()
	{
		HZ_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		HZ_PROFILE_FUNCTION();
		
		// Update ImGui Viewport & Camera
		glm::vec2 FrameBufferSize = { m_Framebuffers["Default"]->GetSpecification().Width, m_Framebuffers["Default"]->GetSpecification().Height};
		if (m_ViewportSize != FrameBufferSize&& m_ViewportSize.x>0.0f&& m_ViewportSize.y>0.0f)
		{
			for(auto&fbo:m_Framebuffers)
				fbo.second->Resize(m_ViewportSize.x, m_ViewportSize.y);
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		// Render
		Renderer2D::ResetStats();
		m_Framebuffers["Default"]->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();
		m_Framebuffers["Default"]->ClearAttachment(1, -1);		// Clear our entity ID attachment to -1
		//m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);
		m_ActiveScene->OnUpdateRuntime(ts);

		//Mouse Picking
		if (Input::IsMouseButtonPressed(Mouse::ButtonLeft))
		{
			auto [mx, my] = ImGui::GetMousePos();
			mx -= m_ViewportBounds[0].x;
			my -= m_ViewportBounds[0].y;
			glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
			my = viewportSize.y - my;
			if (m_ViewportHovered)//(mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
			{
				int pixelData = m_Framebuffers["Default"]->ReadPixel(1, (int)mx, (int)my);
				//HZ_CORE_WARN("Pixel data = {0}", pixelData);
				m_HoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_ActiveScene.get());

				if (!ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::LeftAlt))
					m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
			}
		}
		m_Framebuffers["Default"]->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		HZ_PROFILE_FUNCTION();
		static bool* dockspaceOpen = (bool*)true;

		static bool opt_fullscreen = true;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;


		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", dockspaceOpen, window_flags);
		ImGui::PopStyleVar();
		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float MinWindowSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		style.WindowMinSize.x = MinWindowSizeX;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New..."))
				{
					NewScene();
				}
				if (ImGui::MenuItem("Open..."))
				{
					OpenScene();
				}
				if (ImGui::MenuItem("Save As..."))
				{
					SaveSceneAs();
				}
				if (ImGui::MenuItem("Exit")) Application::Get().Close();
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}


		m_SceneHierarchyPanel.OnImGuiRender();
		m_ContentBrowserPanel.OnImGuiRender();

		ImGui::Begin("Stats");
			auto stats = Renderer2D::GetStats();
			ImGui::Text("Renderer2D Stats");
			ImGui::Text("fps: %.2f", ImGui::GetIO().Framerate);
			ImGui::Text("Draw Calls: %d", stats.DrawCalls);
			ImGui::Text("Quads: %d", stats.QuadCount);
			ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
			ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");
			auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
			auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
			auto viewportOffset = ImGui::GetWindowPos();
			m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
			m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

			m_ViewportFocused = ImGui::IsWindowFocused();
			m_ViewportHovered = ImGui::IsWindowHovered();
			Application::Get().GetImGuiLayer()->LetEventGo(m_ViewportFocused || m_ViewportHovered);
			m_ViewportSize = *(glm::vec2*)&ImGui::GetContentRegionAvail();

			uint64_t textureID = m_Framebuffers["Default"]->GetColorAttachmentRendererID(0);
			ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });


			//Gizmo
			Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
			if (selectedEntity&&m_GizmoType!=-1)
			{
				ImGuizmo::SetOrthographic(false);
				ImGuizmo::SetDrawlist();
				ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

				
				// Runtime camera from entity
				auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
				CameraComponent& cameraComp = cameraEntity.GetComponent<CameraComponent>();
				const glm::mat4& cameraProjection = cameraComp.GetProjMatrix();
				glm::mat4 cameraView = cameraComp.GetViewMatrix();
				
				//EditorCamera only in EditorLayer, no register in Scene
				//const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
				//glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

				//Entity Transform
				auto& tc = selectedEntity.GetComponent<TransformComponent>();
				glm::mat4 transform = tc.GetTransform();
				// Snapping
				bool snap = Input::IsKeyPressed(Key::LeftControl);
				float snapValue = 0.5f; // Snap to 0.5m for translation/scale
				// Snap to 45 degrees for rotation
				if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
					snapValue = 45.0f;

				float snapValues[3] = { snapValue, snapValue, snapValue };
				ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), (ImGuizmo::OPERATION)m_GizmoType,
					ImGuizmo::LOCAL, glm::value_ptr(transform),nullptr, snap ? snapValues : nullptr);
				
				if (ImGuizmo::IsUsing())
				{
					//glm::vec3 position, rotation, scale;
					Math::DeComposeTransform(transform, tc.Position, tc.Rotation, tc.Scale);
					//glm::vec3 deltaRotation = rotation - tc.Rotation;
					//tc.Position = position;
					//tc.Rotation = rotation;
					//tc.Scale = scale;
				}
			}
		ImGui::End();
		ImGui::PopStyleVar();


		ImGui::End();
		
	}

	void EditorLayer::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(HZ_BIND_EVENT_FN(EditorLayer::OnKeyPressed));

		//m_CameraController.OnEvent(e);
		//m_EditorCamera.OnEvent(e);
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		// Shortcuts
		if (e.GetKeyRepeatCount() > 0)
			return false;

		//bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		//bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
		switch (e.GetKeyCode())
		{
			case Key::Q:
			{
				if (!ImGuizmo::IsUsing())
					m_GizmoType = -1;
				break;
			}
			case Key::W:
			{
				if (!ImGuizmo::IsUsing())
					m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			}
			case Key::E:
			{
				if (!ImGuizmo::IsUsing())
					m_GizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			}
			case Key::R:
			{
				if (!ImGuizmo::IsUsing())
					m_GizmoType = ImGuizmo::OPERATION::SCALE;
				break;
			}
		}
		return false;
	}

	void EditorLayer::NewScene()
	{
		FramebufferSpecification spec = m_Framebuffers["Default"]->GetSpecification();
		m_ActiveScene = CreateRef<Scene>((uint32_t)spec.Width, (uint32_t)spec.Height);
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetScene(m_ActiveScene);
	}

	void EditorLayer::OpenScene()
	{
		std::string filepath = FileDialogs::OpenFile("Hazel Scene (*.hazel)\0*.hazel\0");
		if (!filepath.empty())
		{
			FramebufferSpecification spec = m_Framebuffers["Default"]->GetSpecification();

			m_ActiveScene = CreateRef<Scene>((uint32_t)spec.Width, (uint32_t)spec.Height);
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_SceneHierarchyPanel.SetScene(m_ActiveScene);

			SceneSerializer serializer(m_ActiveScene);
			serializer.DeSerialize(filepath);
		}
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialogs::SaveFile("Hazel Scene (*.hazel)\0*.hazel\0");
		if (!filepath.empty())
		{
			SceneSerializer serializer(m_ActiveScene);
			serializer.Serialize(filepath);
		}
	}


}
