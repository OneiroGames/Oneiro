//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "MainMenuBarLayer.hpp"

#include "../OEditorManager.hpp"
#include "ContentBrowserLayer.hpp"
#include "ImGui/ImGuiUtils.hpp"
#include "Oneiro/Common/JobSystem.hpp"
#include "Oneiro/Common/LayerManager.hpp"
#include "Oneiro/Common/Loggger.hpp"
#include "Oneiro/Common/Signals/Events.hpp"
#include "Oneiro/Core/Assets/AssetsManager.hpp"
#include "Oneiro/Core/WM/Input.hpp"
#include "Oneiro/World/WorldManager.hpp"
#include "ProfilerLayer.hpp"
#include "Project/Project.hpp"
#include "imgui.h"

void OEditor::MainMenuBarLayer::OnCreate() {}

void OEditor::MainMenuBarLayer::OnDestroy() {}

void OEditor::MainMenuBarLayer::OnBegin() {}

void OEditor::MainMenuBarLayer::OnUpdate(float /*deltaTime*/)
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (Manager::Get()->GetActiveProject())
			{
				if (ImGui::BeginMenu("Project"))
				{
					if (ImGui::MenuItem("Open")) {}

					ImGui::BeginDisabled(!Manager::Get()->GetActiveProject());
					if (ImGui::MenuItem("Save"))
					{
						Manager::Get()->GetActiveProject()->Save(Manager::Get()->GetActiveProject()->GetPath());
					}

					if (ImGui::MenuItem("Close"))
					{
						Manager::Get()->GetActiveProject()->Save(Manager::Get()->GetActiveProject()->GetPath());
						Manager::Get()->SetActiveProject(nullptr);
					}
					ImGui::EndDisabled();

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("World"))
				{
					if (ImGui::MenuItem("Open")) {}

					ImGui::BeginDisabled(!oe::WorldManager::Get()->GetWorld()->IsLoaded());
					if (ImGui::MenuItem("Save"))
					{
						oe::WorldManager::Get()->SaveWorld();
					}

					if (ImGui::MenuItem("Close"))
					{
						oe::WorldManager::Get()->SaveWorld();
						oe::WorldManager::Get()->UnLoadWorld();
					}
					ImGui::EndDisabled();

					ImGui::EndMenu();
				}
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::BeginMenu("Windows"))
			{
				if (ImGui::MenuItem("Profiler"))
				{
					const auto& layerManager = oe::LayerManager::Get();
					if (!layerManager->HasLayer("ProfilerLayer"))
						layerManager->PushLayer<ProfilerLayer>("ProfilerLayer");
					else
						layerManager->PopLayer("ProfilerLayer");
				}

				if (ImGui::MenuItem("ImGuiMain Demo"))
					mShowDemoWindow = !mShowDemoWindow;

				if (ImGui::MenuItem("ImGuiMain Style Editor"))
					mShowStyleEditor = !mShowStyleEditor;

				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	if (mShowDemoWindow)
		ImGui::ShowDemoWindow();
	if (mShowStyleEditor)
		ImGui::ShowStyleEditor(&ImGui::GetStyle());
}

void OEditor::MainMenuBarLayer::OnEvent(const oe::Event::Base& baseEvent)
{
	if (!ImGui::GetIO().WantTextInput && IsSame<oe::Event::Keyboard::Input>(baseEvent))
	{
		const auto& keyInputEvent = Cast<oe::Event::Keyboard::Input>(baseEvent);
		if (keyInputEvent.repeat == 0 && keyInputEvent.action == oe::Input::Action::PRESS && oe::Input::IsKeyPressed(oe::Input::Key::LCTRL))
		{
			switch (keyInputEvent.key)
			{
				using enum oe::Input::Key;
				case S: {
					if (oe::WorldManager::Get()->GetWorld()->IsLoaded())
						oe::WorldManager::Get()->SaveWorld();
					if (Manager::Get()->GetActiveProject())
						Manager::Get()->GetActiveProject()->Save(Manager::Get()->GetActiveProject()->GetPath());
					break;
				}
				default: break;
			}
		}
	}
}

void OEditor::MainMenuBarLayer::OnEnd() {}
