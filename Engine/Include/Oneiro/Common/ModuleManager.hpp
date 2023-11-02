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
			auto dynamicLibrary = CreateRef<FileSystem::DynamicLibrary>();
			dynamicLibrary->Load(path.string());
			auto* module = reinterpret_cast<CreateModuleFunc>(dynamicLibrary->GetFunction("CreateModule"))(EngineApi::instance);
			m_Modules.push_back({dynamicLibrary, module});
			if (initialize)
				module->Initialize();
			return module;
		}

		void UnLoadModule(IModule* module, bool shutdown = true)
		{
			for (size_t i{}; i < m_Modules.size(); i++)
			{
				const auto& mod = m_Modules[i];
				if (mod.second == module)
				{
					if (shutdown)
						module->Shutdown();
					reinterpret_cast<DestroyModuleFunc>(mod.first->GetFunction("DestroyModule"))(module);
					m_Modules.erase(m_Modules.begin() + i);
				}
			}
		}

	private:
		std::vector<std::pair<Ref<FileSystem::DynamicLibrary>, IModule*>> m_Modules{};
	};
} // namespace oe