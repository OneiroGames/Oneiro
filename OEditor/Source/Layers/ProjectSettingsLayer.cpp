//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "ProjectSettingsLayer.hpp"

#include "imgui.h"
#include "imgui_stdlib.h"

#include "../OEditorManager.hpp"

void OEditor::ProjectSettingsLayer::OnCreate() {}

void OEditor::ProjectSettingsLayer::OnDestroy() {}

void OEditor::ProjectSettingsLayer::OnBegin() {}

void OEditor::ProjectSettingsLayer::OnUpdate(float deltaTime)
{
	const auto& activeProject = Manager::Get()->GetActiveProject();
	ImGui::Begin("Project Settings");
	if (activeProject)
	{
		{
			auto name = activeProject->GetName();
			if (ImGui::InputText("Name", &name, ImGuiInputTextFlags_EnterReturnsTrue))
				activeProject->SetName(name);
		}
		{
			auto assetsPath = activeProject->GetAssetsPath().string();
			if (ImGui::InputText("Assets Path", &assetsPath, ImGuiInputTextFlags_EnterReturnsTrue))
				activeProject->SetAssetsPath(assetsPath);
		}
		{
			auto shadersPath = activeProject->GetShadersPath().string();
			if (ImGui::InputText("Shaders Path", &shadersPath, ImGuiInputTextFlags_EnterReturnsTrue))
				activeProject->SetShadersPath(shadersPath);
		}
		{
			auto worldsPath = activeProject->GetWorldsPath().string();
			if (ImGui::InputText("Worlds Path", &worldsPath, ImGuiInputTextFlags_EnterReturnsTrue))
				activeProject->SetWorldsPath(worldsPath);
		}
		{
			auto startWorldPath = activeProject->GetStartWorldPath().string();
			if (ImGui::InputText("Start World Path", &startWorldPath, ImGuiInputTextFlags_EnterReturnsTrue))
				activeProject->SetStartWorldPath(startWorldPath);
		}
	}
	else
	{
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Project not opened!\nPlease open .oeproject file!");
	}
	ImGui::End();
}

void OEditor::ProjectSettingsLayer::OnEvent(const oe::Event::Base& baseEvent) {}

void OEditor::ProjectSettingsLayer::OnEnd() {}
