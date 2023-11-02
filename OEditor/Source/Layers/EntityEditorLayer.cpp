//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "EntityEditorLayer.hpp"
#include "Oneiro/Common/FileSystem/FileSystem.hpp"
#include "Oneiro/Common/Loggger.hpp"

#include "../OEditorManager.hpp"
#include "ImGui/ImGuiUtils.hpp"
#include "Project/Project.hpp"

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_stdlib.h"

void OEditor::EntityEditorLayer::OnCreate() {}

void OEditor::EntityEditorLayer::OnDestroy() {}

void OEditor::EntityEditorLayer::OnBegin() {}

void OEditor::EntityEditorLayer::OnUpdate(float /*deltaTime*/)
{
	auto selectedEntity = WorldViewLayer::GetSelectedEntity();

	ImGui::Begin("Entity Editor");
	if (selectedEntity.IsValid())
	{
		auto name = selectedEntity.GetName();
		if (ImGui::InputText("##Tag", &name))
		{
			oe::ImGuiUtils::BeginInvalid(!oe::WorldManager::Get()->GetWorld()->HasEntity(name));
			selectedEntity.SetName(name);
			oe::ImGuiUtils::EndInvalid();
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add ..."))
			ImGui::OpenPopup("AddSmthPopup");

		if (ImGui::BeginPopup("AddSmthPopup"))
		{
			DisplayAddComponentEntry<oe::World::Components::Quad>(selectedEntity, "Quad Component");
			DisplayAddComponentEntry<oe::World::Components::Point>(selectedEntity, "Point Component");
			DisplayAddComponentEntry<oe::World::Components::Line>(selectedEntity, "Line Component");
			DisplayAddComponentEntry<oe::World::Components::Sprite2D>(selectedEntity, "Sprite2D Component");
			DisplayAddComponentEntry<oe::World::Components::AudioSource>(selectedEntity, "AudioSource Component");

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

		DrawComponent<oe::World::Components::Transform>(
			"Transform", selectedEntity,
			[this](const auto& component) {
				DrawVec3Control("Position", component->position);
				glm::vec3 rotation = glm::degrees(component->rotation);
				DrawVec3Control("Rotation", rotation);
				component->rotation = glm::radians(rotation);
				DrawVec3Control("Scale", component->scale, 1.0f);
			},
			false, false);
		DrawComponent<oe::World::Components::Quad>("Quad", selectedEntity, [](const auto& component) {
			ImGui::ColorEdit4("Color", component->color, ImGuiColorEditFlags_::ImGuiColorEditFlags_Float);
		});
		DrawComponent<oe::World::Components::Point>("Point", selectedEntity, [](const auto& component) {
			ImGui::ColorEdit4("Color", component->color, ImGuiColorEditFlags_::ImGuiColorEditFlags_Float);
			ImGui::DragFloat("Size", &component->size, 0.01f, 0.0f, 1000.0f);
		});
		DrawComponent<oe::World::Components::Line>("Line", selectedEntity, [](const auto& component) {
			ImGui::ColorEdit4("Color", component->color, ImGuiColorEditFlags_::ImGuiColorEditFlags_Float);
			ImGui::DragFloat2("From Position", component->fromPosition, 0.01f, -1000.0f, 1000.0f);
			ImGui::DragFloat2("To Position", component->toPosition, 0.01f, -1000.0f, 1000.0f);
		});
		DrawComponent<oe::World::Components::Sprite2D>("Sprite2D", selectedEntity, [this](const auto& component) {
			const auto& path = component->GetMetadata().path;
			const auto& path2File = oe::FileSystem::Path("/") / Manager::Get()->GetActiveProject()->GetAssetsPath().string() / path;
			oe::ImGuiUtils::BeginInvalid(!oe::FileSystem::IsFile(path2File.string()));
			std::string pathStr = path.string();
			if (ImGui::InputText("Path", &pathStr))
			{
				const auto assetsPathBackup = oe::Project::GetActive()->GetAssetsPath().string();
				oe::Project::GetActive()->SetAssetsPath(("/" / Manager::Get()->GetActiveProject()->GetAssetsPath()).string());
				component->Load(pathStr);
				oe::Project::GetActive()->SetAssetsPath(assetsPathBackup);
			}
			auto dragAndDrop = [this, component]() {
				if (ImGui::BeginDragDropTarget())
				{
					AcceptContentBrowserPayload("TextureItem", [=](const std::string& pathStr) {
						if (pathStr.starts_with(Manager::Get()->GetActiveProject()->GetAssetsPath().string()))
						{
							auto str = pathStr;
							str.erase(0, Manager::Get()->GetActiveProject()->GetAssetsPath().string().size());
							const auto assetsPathBackup = oe::Project::GetActive()->GetAssetsPath().string();
							oe::Project::GetActive()->SetAssetsPath(("/" / Manager::Get()->GetActiveProject()->GetAssetsPath()).string());
							component->Load(str);
							oe::Project::GetActive()->SetAssetsPath(assetsPathBackup);
						};
					});
					ImGui::EndDragDropTarget();
				}
			};
			oe::ImGuiUtils::EndInvalid();
			dragAndDrop();
			ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionAvail().x / 2 - 128, ImGui::GetCursorPosY()));
			ImGui::Image(reinterpret_cast<void*>(static_cast<size_t>(component->GetTexture()->GetId())), ImVec2(256, 256), ImVec2(0, 0), ImVec2(1, 1),
						 ImVec4(1, 1, 1, 1), ImVec4(0.5f, 0, 0, 1));
			dragAndDrop();
		});
		DrawComponent<oe::World::Components::AudioSource>("AudioSource", selectedEntity, [this](const auto& component) {
			const auto& path = component->GetMetadata().path;
			const auto& path2File = oe::FileSystem::Path("/") / Manager::Get()->GetActiveProject()->GetAssetsPath() / path;
			oe::ImGuiUtils::BeginInvalid(!oe::FileSystem::IsFile(path2File.string()));
			std::string pathStr = path.string();
			if (ImGui::InputText("Path", &pathStr))
			{
				const auto assetsPathBackup = oe::Project::GetActive()->GetAssetsPath().string();
				oe::Project::GetActive()->SetAssetsPath(("/" / Manager::Get()->GetActiveProject()->GetAssetsPath()).string());
				component->Load(pathStr, false);
				oe::Project::GetActive()->SetAssetsPath(assetsPathBackup);
			}
			oe::ImGuiUtils::EndInvalid();

			if (ImGui::BeginDragDropTarget())
			{
				AcceptContentBrowserPayload("AudioItem", [=](const std::string& pathStr) {
					if (pathStr.starts_with(Manager::Get()->GetActiveProject()->GetAssetsPath().string()))
					{
						auto str = pathStr;
						str.erase(0, Manager::Get()->GetActiveProject()->GetAssetsPath().string().size());
						const auto assetsPathBackup = oe::Project::GetActive()->GetAssetsPath().string();
						oe::Project::GetActive()->SetAssetsPath(("/" / Manager::Get()->GetActiveProject()->GetAssetsPath()).string());
						component->Load(str, false);
						oe::Project::GetActive()->SetAssetsPath(assetsPathBackup);
					};
				});
				ImGui::EndDragDropTarget();
			}

			if (!component->GetAudioSource()->IsPlaying())
			{
				if (ImGui::Button("Play"))
				{
					component->GetAudioSource()->Play();
				}
			}
			else
			{
				if (ImGui::Button("Stop"))
				{
					component->GetAudioSource()->Stop();
				}
			}
		});
	}
	ImGui::End();
}

void OEditor::EntityEditorLayer::OnEvent(const oe::Event::Base& baseEvent) {}

void OEditor::EntityEditorLayer::OnEnd() {}

void OEditor::EntityEditorLayer::DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue, float columnWidth)
{
	const auto& guiIo = ImGui::GetIO();
	const auto& boldFont = guiIo.Fonts->Fonts[0];
	const auto& lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

	const ImVec2 buttonSize{lineHeight + 3.0f, lineHeight};

	ImGui::PushID(label.c_str());

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text("%s", label.c_str());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
	ImGui::PushFont(boldFont);
	if (ImGui::Button("X", buttonSize))
		values.x = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Y", buttonSize))
		values.y = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.2f, 0.35f, 0.9f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Z", buttonSize))
		values.z = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();

	ImGui::Columns(1);

	ImGui::PopID();
}

void OEditor::EntityEditorLayer::AcceptContentBrowserPayload(const std::string& type, const std::function<void(const std::string&)>& func) const
{
	if (const auto& payload = ImGui::AcceptDragDropPayload(type.c_str()))
	{
		auto pathStr = std::string(static_cast<const char*>(payload->Data));
		func(pathStr);
	}
}
