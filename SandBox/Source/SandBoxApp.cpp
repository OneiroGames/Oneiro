//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Common/StdAfx.hpp"

import SandBox.Application;

import Oneiro.Common.AssetsProviders.WorldAsset;
import Oneiro.Common.EngineApi;
import Oneiro.Core.Engine;
import Oneiro.Common.AssetsManager;
import Oneiro.Common.FileSystem.Path;
import Oneiro.Common.ECS.World;
import Oneiro.Common.Logger;

bool SandBox::SandBoxApp::OnInitialize()
{
	oe::EngineApi::GetAssetsManager()->RegisterAssetsProvider<oe::WorldAssetsProvider>(typeid(oe::World).hash_code());

	world = oe::EngineApi::GetAssetsManager()->CreateAssetAndLoad<oe::World>("WORLD", false, oe::FileSystem::Path("world.oeworld"))->Get<oe::World>();
	world->CreateEntity("test");
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

int main(int /*argc*/, char** /*argv*/)                  
{
	auto application = CreateApplication();              
	auto engine = std::make_unique<oe::Engine>();
	try
	{
		engine->PreInit(application.get());
		engine->Init();
		engine->Run();          
	}
	catch (const std::exception& exception)
	{
		// ::oe::EngineApi::GetLogger()->GetEngineLogger()->critical("[ENGINE] " + fmt::format("ASDASD"));
		engine->Shutdown();
		return 1;
	}
	engine->Shutdown();
	application.reset();
	engine.reset();
	return 0;
}                                                            
