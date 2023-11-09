//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/FileSystem/DynamicLibrary.hpp"
#include "Oneiro/Common/IModule.hpp"

namespace oe
{
	class ModuleManager
	{
	public:
		IModule* LoadModule(const FileSystem::Path& path, bool initialize = true)
		{
			auto& pair = m_Modules.emplace_back(std::pair<FileSystem::DynamicLibrary, IModule*>{{}, {}});
			pair.first.Load(path.string());
			pair.second = pair.first.GetFunction<CreateModuleFunc>("CreateModule")(EngineApi::instance);
			if (initialize)
				pair.second->Initialize();
			return pair.second;
		}

		void UnLoadModule(IModule* module, bool shutdown = true)
		{
			for (size_t i{}; i < m_Modules.size(); i++)
			{
				auto& mod = m_Modules[i];
				if (mod.second == module)
				{
					if (shutdown)
						module->Shutdown();
					mod.first.GetFunction<DestroyModuleFunc>("DestroyModule")(module);
					m_Modules.erase(m_Modules.begin() + i);
				}
			}
		}

	private:
		std::vector<std::pair<FileSystem::DynamicLibrary, IModule*>> m_Modules{};
	};
} // namespace oe