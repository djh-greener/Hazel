#include "EditorLayer.h"

#include"Hazel/Scene/SceneSerializer.h"
#include"Hazel/Utils/PlatformUtils.h"
#include"Hazel/Math/Math.h"
#include"Hazel/Camera/CameraComponent.h"
#include"Hazel/Renderer/RenderCommand.h"
#include"Hazel/Renderer/Renderer3D.h"
#include <glad/glad.h>
#include <imgui/imgui.h>
#include"ImGuizmo.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Hazel {
	extern const std::filesystem::path g_AssetPath;
	EditorLayer::EditorLayer()
		: Layer("EditorLayer")
	{
	}

	void EditorLayer::OnAttach()
	{
		HZ_PROFILE_FUNCTION();

		m_ActiveScene = CreateRef<Scene>();

		auto CameraEntity = m_ActiveScene->CreateEntity("Main Camera");
		
		CameraEntity.AddComponent<CameraComponent>();
		CameraEntity.GetComponent<CameraComponent>().SetPrimary();

		m_SceneHierarchyPanel = CreateRef<SceneHierarchyPanel>();
		m_SceneHierarchyPanel->SetScene(m_ActiveScene);

		m_ContentBrowserPanel = CreateRef<ContentBrowserPanel>();
		RenderCommand::Init();
		Renderer3D::Init();
	}

	void EditorLayer::OnDetach()
	{
		HZ_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		HZ_PROFILE_FUNCTION();
		
		// Update ImGui Viewport & Camera
		Framebuffer& Framebuffer = Renderer3D::GetLastFramebuffer();
		glm::vec2 FrameBufferSize = { Framebuffer.GetSpecification().Width, Framebuffer.GetSpecification().Height};
		if (m_ViewportSize != FrameBufferSize&& m_ViewportSize.x>0.0f&& m_ViewportSize.y>0.0f)
		{
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		// Render
		//Renderer2D::ResetStats();
		Framebuffer.Bind();
		RenderCommand::SetClearColor({ 0.13f, 0.13f, 0.13f, 1 });
		RenderCommand::Clear();
		Framebuffer.ClearAttachment(1, -1);		// Clear our entity ID attachment to -1
		m_ActiveScene->OnUpdateRuntime(ts, m_ViewportHovered); // Render Scene

		//m_ModelShader->Bind();
		//m_ModelShader->SetMat4("u_Model", glm::mat4(1));
		//m_StaticMeshEntity.GetComponent<StaticMeshComponent>().DrawStaticMesh(m_ModelShader);
		//m_ModelShader->UnBind();

		//Mouse Picking
		if (Input::IsMouseButtonPressed(Mouse::ButtonLeft))
		{
			auto [mx, my] = ImGui::GetMousePos();
			mx -= m_ViewportBounds[0].x;
			my -= m_ViewportBounds[0].y;
			glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
			my = viewportSize.y - my;
			if (m_ViewportHovered)
			{
				int pixelData = Framebuffer.ReadPixel(1, (int)mx, (int)my);
				//HZ_CORE_WARN("Pixel data = {0}", pixelData);
				m_HoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_ActiveScene.get());

				if (!ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::LeftAlt))
					m_SceneHierarchyPanel->SetSelectedEntity(m_HoveredEntity);
			}
		}
		Framebuffer.Unbind();
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


		m_SceneHierarchyPanel->OnImGuiRender();
		m_ContentBrowserPanel->OnImGuiRender();

		ImGui::Begin("Stats");
			//auto stats = Renderer2D::GetStats();
			//ImGui::Text("Renderer2D Stats");
			ImGui::Text("fps: %.2f", ImGui::GetIO().Framerate);
			//ImGui::Text("Draw Calls: %d", stats.DrawCalls);
			//ImGui::Text("Quads: %d", stats.QuadCount);
			//ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
			//ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
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


			uint64_t textureID = Renderer3D::GetLastFramebuffer().GetColorAttachmentRendererID(0);
			ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					const wchar_t* path = (const wchar_t*)payload->Data;
					OpenScene(std::filesystem::path(g_AssetPath) / path);
				}
				ImGui::EndDragDropTarget();
			}

			//Gizmo
			Entity selectedEntity = m_SceneHierarchyPanel->GetSelectedEntity();
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
					Math::DeComposeTransform(transform, tc.Position, tc.Rotation, tc.Scale);
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


	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		// Shortcuts
		if (e.IsRepeat())
			return false;
		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
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
			case Key::S:
			{
				if (control)
				{
						SaveScene();
				}

				break;
			}
		}
		return false;
	}

	void EditorLayer::NewScene()
	{
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel->SetScene(m_ActiveScene);
	}

	void EditorLayer::OpenScene()
	{
		std::string filepath = FileDialogs::OpenFile("Hazel Scene (*.hazel)\0*.hazel\0");
		if (!filepath.empty())
		{
			OpenScene(filepath);
		}
	}
	void EditorLayer::OpenScene(const std::filesystem::path& path)
	{
		if (path.extension().string() != ".hazel")
		{
			HZ_WARN("Could not load {0} - not a scene file", path.filename().string());
			return;
		}
		Ref<Scene> newScene = CreateRef<Scene>();
		SceneSerializer serializer(newScene);
		if (serializer.DeSerialize(path.string()))
		{
			m_ActiveScene = newScene;
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_SceneHierarchyPanel->SetScene(m_ActiveScene);

			m_ActiveScenePath = path;
		}
	}
	void EditorLayer::SaveScene()
	{
		if (!m_ActiveScenePath.empty())
		{
			SceneSerializer serializer(m_ActiveScene);
			serializer.Serialize(m_ActiveScenePath.string());
		}
		else
			SaveSceneAs();
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
