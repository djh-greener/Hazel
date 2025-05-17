#include "hzpch.h"
#include "SceneHierarchyPanel.h"

#include "Hazel/Scene/Components.h"
#include"Hazel/Camera/CameraComponent.h"
#include"Hazel/Renderer/Material/Material.h"
#include"Hazel/Renderer/Mesh/StaticMeshComponent.h"
#include"Hazel/Renderer/Light/PointLightComponent.h"
#include"Hazel/Renderer/Light/DirLightComponent.h"

#include"Hazel/Core/GlobalData.h"
#include"UI.h"

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
		//GlobalData::GetGlobalData().SelectedEntity = {};
		m_DefaultTextureIcon = Texture2D::Create("Resources/Icons/SceneHierarchyPanel/TextureIcon.png");

	}

    Entity SceneHierarchyPanel::GetSelectedEntity() const
    {
		 return GlobalData::GetGlobalData().SelectedEntity; 
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
			GlobalData::GetGlobalData().SelectedEntity = {};
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
		if (GlobalData::GetGlobalData().SelectedEntity)
		{
			DrawComponents(GlobalData::GetGlobalData().SelectedEntity);
		}
		ImGui::End();

	}


	void SceneHierarchyPanel::SetSelectedEntity(Entity entity)
	{
		GlobalData::GetGlobalData().SelectedEntity =entity;
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		if (!m_Scene->IsEntityExist(entity))
			return;
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((GlobalData::GetGlobalData().SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			GlobalData::GetGlobalData().SelectedEntity = entity;
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
			bool opened = ImGui::TreeNodeEx((void*)9817239, flags, "Null");//tag.c_str()
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}
		if (EntityDeleted) {
			if (GlobalData::GetGlobalData().SelectedEntity == entity)
				GlobalData::GetGlobalData().SelectedEntity = {};
			m_Scene->DestroyEntity(entity);

		}

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
			DisplayAddComponentEntry<StaticMeshComponent>("StaticMesh");
			DisplayAddComponentEntry<PointLightComponent>("PointLight");
			DisplayAddComponentEntry<DirLightComponent>("DirLight");


			ImGui::EndPopup();
		}
		ImGui::PopItemWidth();


		DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
			{
				UI::DrawVec3Control("Position", component.Position);

				glm::vec3 rotation = glm::degrees(component.Rotation);
				UI::DrawVec3Control("Rotation", rotation);
				component.Rotation = glm::radians(rotation);

				UI::DrawVec3Control("Scale", component.Scale, 1.f);
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

		DrawComponent<StaticMeshComponent>("StaticMesh", entity, [&](StaticMeshComponent& component)
			{
				if(!component.name.empty())
					ImGui::Text(component.name.string().c_str(), ImVec2(100.0f, 100.0f));
				else
					ImGui::Text("Empty", ImVec2(100.0f, 100.0f));
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path modelPath = std::filesystem::path(g_AssetPath) / path;
						std::string fileExtension = modelPath.extension().string();
						HZ_ASSERT(fileExtension == ".obj"|| fileExtension == ".OBJ"|| fileExtension == ".fbx"|| fileExtension == ".FBX", "HAZEL now only support .obj .fbx ");
						std::unordered_map<std::string, Ref<Material>> overrideMaterialPerMesh;
						component.meshes.clear();
						component.loadStaticMesh(modelPath, overrideMaterialPerMesh);
					}
					ImGui::EndDragDropTarget();
				}
				{
					static float left_panel_width = 80.0f; // 调整为原来1/3宽
					static float image_size = 80.0f;
					static float item_spacing = 10.0f;
					static ImVec4 separator_color = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
					static float texture_item_height = image_size + item_spacing * 2;
					static ImVec4 splitter_color = ImVec4(0.3f, 0.3f, 0.3f, 1.0f); // 分隔线颜色


					for (size_t i = 0; i < component.meshes.size(); i++)
					{
						auto& mesh = component.meshes[i];
						auto& material = mesh->m_Material;
						const char* materialName = mesh->name.c_str();
						int texture_count = material->textures.size();

						// 计算当前行需要的高度
						float row_height = texture_count * texture_item_height;

						// 开始一行（材质行）
						ImGui::BeginChild(("##MaterialRow" + std::to_string(i)).c_str(),
							ImVec2(0, row_height),
							false,
							ImGuiWindowFlags_NoScrollbar);

						// 左侧：材质名（垂直居中）
						ImGui::BeginChild(("##LeftPanel" + std::to_string(i)).c_str(),
							ImVec2(left_panel_width, row_height),
							false);
						{
							ImVec2 text_size = ImGui::CalcTextSize(materialName);
							float y_pos = (row_height - text_size.y) * 0.5f;

							ImGui::SetCursorPosY(y_pos);
							ImGui::TextColored(ImVec4(1, 1, 1, 1), "%s", materialName);
						}
						ImGui::EndChild();

						// 可拖拽的分隔条
						ImGui::SameLine();
						ImGui::InvisibleButton(("##Splitter" + std::to_string(i)).c_str(),
							ImVec2(2.0f, std::max(row_height,0.1f)));
						if (ImGui::IsItemHovered() || ImGui::IsItemActive())
							splitter_color = ImVec4(0.5f, 0.5f, 0.8f, 1.0f); // 悬停/活动时高亮
						else
							splitter_color = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);

						ImGui::GetWindowDrawList()->AddRectFilled(
							ImGui::GetItemRectMin(),
							ImGui::GetItemRectMax(),
							ImGui::GetColorU32(splitter_color),
							0.0f);
						if (ImGui::IsItemActive())
						{
							left_panel_width += ImGui::GetIO().MouseDelta.x;
							left_panel_width = ImClamp(left_panel_width, 60.0f, 150.0f);
						}
						ImGui::SameLine();

						// 右侧：纹理列表（图片+右侧名称）
						ImGui::BeginChild(("##RightPanel" + std::to_string(i)).c_str(),
							ImVec2(0, row_height),
							false);
						{
							for (size_t tex_idx = 0; tex_idx < material->textures.size(); tex_idx++)
							{
								auto& texture = material->textures[tex_idx];

								// 计算当前纹理项的起始Y位置
								float item_y = tex_idx * texture_item_height + item_spacing;
								ImGui::SetCursorPosY(item_y);

								// 纹理项组
								ImGui::BeginGroup();
								{
									// 图片（固定高度）
									ImGui::Image((ImTextureID)(uint64_t)texture->GetRendererID(),
										ImVec2(image_size, image_size));

									if (ImGui::BeginDragDropTarget())
									{
										if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
										{
											const wchar_t* path = (const wchar_t*)payload->Data;
											std::filesystem::path texturePath = std::filesystem::path(g_AssetPath) / path;
											//TODO:Check if texturePath is picture
											std::string fileExtension = texturePath.extension().string();
											HZ_ASSERT(fileExtension == ".png" || fileExtension == ".jpg", "HAZEL now only support .png or .jpg as texture");

											auto uniformName = texture->GetShaderUniformName();
											texture = Texture2D::Create(texturePath.string(), uniformName =="diffuse");
											texture->SetShaderUniformName(uniformName);
										}
										ImGui::EndDragDropTarget();
									}
									// 图片右侧的名称
									ImGui::SameLine();
									ImGui::SetCursorPosY(item_y + (image_size - ImGui::GetTextLineHeight()) * 0.5f);
									ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "%s",
										texture->GetShaderUniformName().c_str());
								}
								ImGui::EndGroup();

								// 纹理间的水平分隔线
								if (tex_idx < material->textures.size())
								{
									ImGui::SetCursorPosY((tex_idx + 1) * texture_item_height - 1);
									ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);
									ImGui::GetWindowDrawList()->AddLine(
										ImGui::GetItemRectMin(),
										ImGui::GetItemRectMax(),
										ImGui::GetColorU32(separator_color),
										1.0f);
								}
							}
						}
						ImGui::EndChild();

						ImGui::EndChild(); // 结束材质行

						// 材质间的分隔线
						if (i < component.meshes.size())
						{
							ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);
							ImGui::GetWindowDrawList()->AddLine(
								ImGui::GetItemRectMin(),
								ImGui::GetItemRectMax(),
								ImGui::GetColorU32(separator_color),
								1.0f);
						}
					}

				}
			
			});

		DrawComponent<PointLightComponent>("PointLight", entity, [&](auto& component)
			{
				ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
				ImGui::DragFloat("Linear", &component.Linear, 0.1f, 0.35f, 1.4f);
				ImGui::DragFloat("Quadratic", &component.Quadratic, 0.1f, 0.9f, 3.6f);
				ImGui::DragFloat("Intensity", &component.Intensity, 0.1f, 0.1f, 100.f);
			});
		
		DrawComponent<DirLightComponent>("DirLight", entity, [&](auto& component)
			{
				ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
				ImGui::DragFloat("Intensity", &component.Intensity, 0.1f, 0.1f, 100.f);
			});
		}
	template<typename T>
	void SceneHierarchyPanel::DisplayAddComponentEntry(const std::string& entryName) {
		if (!GlobalData::GetGlobalData().SelectedEntity.HasComponent<T>())
		{
			if (ImGui::MenuItem(entryName.c_str()))
			{
				GlobalData::GetGlobalData().SelectedEntity.AddComponent<T>();
				ImGui::CloseCurrentPopup();
			}
		}
	}
}
