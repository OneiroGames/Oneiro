//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "SandBoxApp.hpp"

#include "Oneiro/Core/EntryPoint.hpp"

#include "Oneiro/Common/Assets/WorldAsset.hpp"

bool SandBox::SandBoxApp::OnInitialize()
{
	oe::EngineApi::GetAssetsManager()->RegisterAssetsProvider<oe::WorldAssetsProvider>(typeid(oe::World).hash_code());

	world = oe::EngineApi::GetAssetsManager()->CreateAssetAndLoad<oe::World>("WORLD", false, oe::FileSystem::Path("world.oeworld"))->Get<oe::World>();
	world->GetOrCreateEntity("test");
	return true;
}

bool SandBox::SandBoxApp::OnLogicUpdate(float deltaTime)
{
	return true;
}

void SandBox::SandBoxApp::OnShutdown() {}

std::unique_ptr<oe::IApplication> CreateApplication()
{
	// clang-format off
	const oe::ApplicationProperties applicationProperties{
		.windowProperties = {
			.title = "SandBox",
			.width = 1600,
			.height = 900
		},
		.projectFilePath = "/SandBox.oeproject"
	};
	// clang-format on
	return std::make_unique<SandBox::SandBoxApp>(applicationProperties);
}

OE_MAIN(CreateApplication)