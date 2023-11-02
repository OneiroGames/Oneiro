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
		profiler = &::tracy::GetProfiler();
		moduleManager = CreateRef<ModuleManager>();
	}
} // namespace oe