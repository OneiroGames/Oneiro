//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "SandBoxApp.hpp"

#include "Oneiro/Core/EntryPoint.hpp"

bool SandBox::SandBoxApp::OnInit()
{
	return true;
}

bool SandBox::SandBoxApp::OnUpdate(float deltaTime)
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

OE_MAIN(CreateApplication);