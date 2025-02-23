#include "hzpch.h"
#include "SceneHierarchyPanel.h"
#include "Hazel/Scene/Components.h"
#include"Hazel/Camera/CameraComponent.h"

#include <imgui/imgui.h>
#include<imgui/imgui_internal.h>
#include<glm/gtc/type_ptr.hpp>

namespace Hazel {
	extern const std::filesystem::path g_AssetPath;

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
	{
		SetScene(scene);
	}

	void SceneHierarchyPanel::SetScene(const Ref<Scene>& scene)
	{
		m_Scene = scene;
		m_SelectionEntity = {};
		m_DefaultTextureIcon = Texture2D::Create("Resources/Icons/SceneHierarchyPanel/TextureIcon.png");

	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");
		//entt's view is reverse, so reverse to show
		auto view = m_Scene->m_Registry.view<entt::entity>();
		for (auto it = view.rbegin(), last = view.rend(); it != last; ++it) {
			Entity entity = { *it, m_Scene.get() };
			DrawEntityNode(entity);
		}
		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectionEntity = {};
		//Right Click on Blank Space
		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
			{
				m_Scene->CreateEntity("Empty Enity");
			}
			ImGui::EndPopup();
		}
		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectionEntity)
		{
			DrawComponents(m_SelectionEntity);
		}
		ImGui::End();
	}

	void SceneHierarchyPanel::SetSelectedEntity(Entity entity)
	{
		m_SelectionEntity = entity;
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((m_SelectionEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionEntity = entity;
		}
		 
		bool EntityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
			{
				EntityDeleted = true;
			}
			ImGui::EndPopup();

		}
		if (opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow  ;
			bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}
		if (EntityDeleted) {
			m_Scene->DestroyEntity(entity);
			if (m_SelectionEntity == entity)
				m_SelectionEntity = {};
		}

	}
	//TODO:Move To UI.h
	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100)
	{
		ImGui::PushID(label.c_str());
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2({ 0,0 }));

		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y*2.0f;
		ImVec2 ButtonSize = { lineHeight + 3.f,lineHeight };


		ImVec4 RedColor = { 0.8f,0.1f,0.15f,1.f };
		ImVec4 GreenColor = { 0.2f,0.7f,0.3f,1.f };
		ImVec4 BlueColor = { 0.1f,0.25f,0.8f,1.f };
		auto ImVec4Multi = [](ImVec4& vec, float value) {return ImVec4{ vec.x * value,vec.y * value, vec.z * value, vec.w  }; };

		ImGui::PushStyleColor(ImGuiCol_Button, RedColor);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4Multi(RedColor,1.3f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, RedColor);
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X",ButtonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PopStyleColor(3);

		ImGui::PushStyleColor(ImGuiCol_Button, GreenColor);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4Multi(GreenColor, 1.3f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, GreenColor);
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", ButtonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PopStyleColor(3);

		ImGui::PushStyleColor(ImGuiCol_Button, BlueColor);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4Multi(BlueColor, 1.3f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, BlueColor);
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", ButtonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::PopStyleColor(3);

		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();
	}
	
	template<typename T,typename UIFunction>
	static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | 
			ImGuiTreeNodeFlags_AllowItemOverlap| ImGuiTreeNodeFlags_SpanAvailWidth;

		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();
			auto lineWidth = ImGui::GetContentRegionAvail().x;

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();

			ImGui::SameLine(lineWidth - lineHeight * 0.5f);
			bool ComponentDeleted = false;
			if (ImGui::Button("+", ImVec2({ lineHeight,lineHeight })))
			{
				ImGui::OpenPopup("Component Setting");
			}

			if (ImGui::BeginPopup("Component Setting"))
			{
				if (ImGui::MenuItem("Delete Component"))
					ComponentDeleted = true;
				ImGui::EndPopup();
			}
			if (open)
			{
				uiFunction(component);
				ImGui::TreePop();
			}
			if (ComponentDeleted)
			{
				entity.RemoveComponent<T>();
			}
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}
		ImGui::SameLine();
		ImGui::PushItemWidth(-1);
		if (ImGui::Button("Add Component")) {
			ImGui::OpenPopup("AddComponent");
		}

		if (ImGui::BeginPopup("AddComponent"))
		{
			DisplayAddComponentEntry<CameraComponent>("Camera");
			DisplayAddComponentEntry<SpriteRendererComponent>("Sprite Renderer");
			ImGui::EndPopup();
		}
		ImGui::PopItemWidth();


		DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
			{
				DrawVec3Control("Position", component.Position);

				glm::vec3 rotation = glm::degrees(component.Rotation);
				DrawVec3Control("Rotation", rotation);
				component.Rotation = glm::radians(rotation);

				DrawVec3Control("Scale", component.Scale, 1.f);
			});


		DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
			{
				auto& camera = component.Camera;
				bool Primary = component.IsPrimary();
				if (ImGui::Checkbox("Primary", &Primary))
				{
					if (Primary)
						component.SetPrimary();
					else
						component.SetNoPrimary();
				}

				float verticalFov = glm::degrees(camera.GetPerspectiveVerticalFOV());
				if (ImGui::DragFloat("Vertical FOV", &verticalFov,1.f,1.f,90.f))
					camera.SetPerspectiveVerticalFOV(glm::radians(verticalFov));

				float orthoNear = camera.GetPerspectiveNearClip();
				if (ImGui::DragFloat("Near", &orthoNear,0.01f,0.01f,1.f))
					camera.SetPerspectiveNearClip(orthoNear);

				float orthoFar = camera.GetPerspectiveFarClip();
				if (ImGui::DragFloat("Far", &orthoFar,10.f,50.f,1000.f))
					camera.SetPerspectiveFarClip(orthoFar);

				
				ImGui::DragFloat("MoveSpeed", &component.MoveSpeed,1.f,5.f,20.f);
				ImGui::DragFloat("RotateSpeed", &component.RotateSpeed,0.01f,0.05f,0.2f);

			});

		DrawComponent<SpriteRendererComponent>("SpriteRenderer", entity, [&](auto& component)
			{
				ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
				ImGui::Text("Texture", ImVec2(100.0f, 0.0f));
				if (component.Texture)
				{
					ImGui::Image((ImTextureID)(uint64_t)component.Texture->GetRendererID(), ImVec2(128, 128));
				}
				else
				{
					ImGui::Image((ImTextureID)(uint64_t)(m_DefaultTextureIcon->GetRendererID()), ImVec2(128, 128));
				}

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path texturePath = std::filesystem::path(g_AssetPath) / path;
						//TODO:Check if texturePath is picture
						std::string fileExtension = texturePath.extension().string();
						HZ_ASSERT(fileExtension == ".png" || fileExtension == ".jpg", "HAZEL now only support .png or .jpg as texture")

						component.Texture = Texture2D::Create(texturePath.string());
					}
					ImGui::EndDragDropTarget();
				}
				ImGui::DragFloat("Tiling Factor", &component.TilingFactor, 0.1f, 0.0f, 100.0f);

			});
		
	}

	template<typename T>
	void SceneHierarchyPanel::DisplayAddComponentEntry(const std::string& entryName) {
		if (!m_SelectionEntity.HasComponent<T>())
		{
			if (ImGui::MenuItem(entryName.c_str()))
			{
				m_SelectionEntity.AddComponent<T>();
				ImGui::CloseCurrentPopup();
			}
		}
	}
}
