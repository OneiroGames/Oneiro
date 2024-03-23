//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

module;

#include "Oneiro/Common/StdAfx.hpp"

module Oneiro.Common.IApplication;

import <utility>;

namespace oe
{
	IApplication::IApplication(ApplicationProperties applicationProperties) : mProps(std::move(applicationProperties)) {}

	IApplication::~IApplication() = default;

	bool IApplication::OnPreInitialize()
	{
		return true;
	}

	bool IApplication::OnInitialize()
	{
		return true;
	}

	bool IApplication::OnLogicUpdate(float /*deltaTime*/)
	{
		return true;
	}

	bool IApplication::OnRender(float deltaTime)
	{
		return true;
	}

	void IApplication::OnShutdown() {}

	const ApplicationProperties& IApplication::GetProperties() const noexcept
	{
		return mProps;
	}

} // namespace oe