#include"UI.h"
#include<imgui/imgui.h>
#include<imgui/imgui_internal.h>
namespace Hazel::UI
{


	void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue, float columnWidth)
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
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 ButtonSize = { lineHeight + 3.f,lineHeight };


		ImVec4 RedColor = { 0.8f,0.1f,0.15f,1.f };
		ImVec4 GreenColor = { 0.2f,0.7f,0.3f,1.f };
		ImVec4 BlueColor = { 0.1f,0.25f,0.8f,1.f };
		auto ImVec4Multi = [](ImVec4& vec, float value) {return ImVec4{ vec.x * value,vec.y * value, vec.z * value, vec.w }; };

		ImGui::PushStyleColor(ImGuiCol_Button, RedColor);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4Multi(RedColor, 1.3f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, RedColor);
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", ButtonSize))
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
	

}
