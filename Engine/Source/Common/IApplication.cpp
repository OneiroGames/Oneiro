//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Common/IApplication.hpp"

oe::IApplication::IApplication(const ApplicationProperties& applicationProperties) : mProps(applicationProperties) {}

oe::IApplication::~IApplication() {}

bool oe::IApplication::OnPreInit()
{
	return true;
}

bool oe::IApplication::OnInit()
{
	return true;
}

bool oe::IApplication::OnUpdate(float /*deltaTime*/)
{
	return true;
}

void oe::IApplication::OnShutdown() {}

const oe::ApplicationProperties& oe::IApplication::GetProperties() const noexcept
{
	return mProps;
}
