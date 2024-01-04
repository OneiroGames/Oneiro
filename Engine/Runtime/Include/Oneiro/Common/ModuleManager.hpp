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
		~ModuleManager()
		{
			UnLoadModules();
		}

		IModule* LoadModule(const FileSystem::Path& path, bool initialize = true)
		{
			auto& pair = m_Modules.emplace_back(std::pair<FileSystem::DynamicLibrary, IModule*>{{}, {}});
			pair.first.Load(path.string());
			pair.second = pair.first.GetFunction<CreateModuleFunc>("CreateModule")();
			if (initialize)
				pair.second->Initialize(EngineApi::GetInstance());
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
					break;
				}
			}
		}

		void UnLoadModules()
		{
			for (auto& module : m_Modules)
			{
				module.second->Shutdown();
				module.first.GetFunction<DestroyModuleFunc>("DestroyModule")(module.second);
			}
		}

		void LoadModulesFromPath(const FileSystem::Path& path)
		{
			const FileSystem::Path::Iterator iterator{path};
			for (const auto& modulePath : iterator)
			{
				if (modulePath.is_directory())
				{
					FileSystem::ConfigFile configFile{modulePath / FileSystem::Path("module.json")};
					//					const auto& deps = configFile["Dependencies"].GetArray();
					//					for (const auto& dep : deps)
					//					{
					//						if (!HasModule(dep.GetString()) && !FindAndLoadDepModule(iterator, dep.GetString()))
					//							continue;
					//					}
					const auto& module = LoadModule(modulePath / FileSystem::Path(configFile["EntryPoint"].GetString()));
					module->m_Name = configFile["Name"].GetString();
				}
			}
		}

		bool HasModule(const std::string& name)
		{
			for (const auto& item : m_Modules)
				if (item.second->GetName() == name)
					return true;
			return false;
		}

	private:
		bool FindAndLoadDepModule(const FileSystem::Path::Iterator& iterator, const std::string& name)
		{
			for (const auto& modulePath : iterator)
			{
				if (modulePath.is_directory())
				{
					FileSystem::ConfigFile configFile{modulePath / FileSystem::Path("module.json")};
					if (configFile["Name"].GetString() == name)
					{
						const auto& deps = configFile["Dependencies"].GetArray();
						for (const auto& dep : deps)
						{
							if (!HasModule(dep.GetString()) && !FindAndLoadDepModule(iterator, dep.GetString()))
								return false;
						}
						const auto& module = LoadModule(modulePath / FileSystem::Path(configFile["EntryPoint"].GetString()));
						module->m_Name = configFile["Name"].GetString();
						return true;
					}
				}
			}
			return false;
		}

		std::vector<std::pair<FileSystem::DynamicLibrary, IModule*>> m_Modules{};
	};
} // namespace oe