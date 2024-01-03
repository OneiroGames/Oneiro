//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "OEditorApp.hpp"

#include "Layers/ContentBrowserLayer.hpp"
#include "Layers/DockSpaceLayer.hpp"
#include "Layers/EditorSettingsLayer.hpp"
#include "Layers/EntityEditorLayer.hpp"
#include "Layers/MainMenuBarLayer.hpp"
#include "Layers/ProjectSettingsLayer.hpp"
#include "Layers/ViewportLayer.hpp"
#include "Layers/WorldViewLayer.hpp"
#include "OEditorManager.hpp"
#include "Oneiro/Common/LayerManager.hpp"
#include "Oneiro/Core/Assets/AssetsManager.hpp"
#include "Oneiro/Core/EntryPoint.hpp"
#include "Project/Project.hpp"

bool OEditor::OEditorApp::OnInitialize()
{
	const auto& layerManager = oe::LayerManager::Get();
	layerManager->PushLayer<MainMenuBarLayer>("MainMenuBarLayer");
	layerManager->PushLayer<DockSpaceLayer>("DockSpaceLayer");
	layerManager->PushLayer<WorldViewLayer>("WorldViewLayer");
	layerManager->PushLayer<EntityEditorLayer>("EntityEditorLayer");
	layerManager->PushLayer<ViewportLayer>("ViewportLayer");
	layerManager->PushLayer<ContentBrowserLayer>("ContentBrowserLayer");
	layerManager->PushLayer<ProjectSettingsLayer>("ProjectSettingsLayer");
	layerManager->PushLayer<EditorSettingsLayer>("EditorSettingsLayer");

	return true;
}

bool OEditor::OEditorApp::OnLogicUpdate(float /*deltaTime*/)
{
	return true;
}

bool OEditor::OEditorApp::OnEvent(const oe::Event::Base& /*baseEvent*/)
{
	return true;
}

void OEditor::OEditorApp::OnShutdown()
{
	if (Manager::Get()->GetActiveProject())
		Manager::Get()->GetActiveProject()->Save(Manager::Get()->GetActiveProject()->GetPath());
	oe::Project::GetActive()->Save(oe::Project::GetActive()->GetPath());
	oe::WorldManager::Get()->UnLoadWorld();
}

std::unique_ptr<oe::IApplication> oe::CreateApplication()
{
	// clang-format off
	const ApplicationProperties applicationProperties{
		.windowProperties = {
			.title = "Oneiro Editor",
			.width = 1600,
			.height = 900
		},
		.engine = {
			.controlWorldState = false
		},
		.projectFilePath = "/OEditor.oeproject"
	};
	// clang-format on
	return std::make_unique<OEditor::OEditorApp>(applicationProperties);
}
