//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Common/EngineApi.hpp"

#include "Oneiro/Common/ModuleManager.hpp"

namespace oe
{
	EngineApi::EngineApi()
	{
		instance = this;
	}

	bool EngineApi::Initialize(IApplication* application)
	{
		this->application = application;
		profiler = &::tracy::GetProfiler();
		moduleManager = CreateRef<ModuleManager>();
		cVars = CreateRef<CVars>();
		return true;
	}

	bool EngineApi::Shutdown()
	{
		cVars.reset();
		moduleManager.reset();
		return true;
	}
} // namespace oe