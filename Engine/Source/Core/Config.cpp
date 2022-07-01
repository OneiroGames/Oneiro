//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Core/Config.hpp"
#include "Oneiro/Core/Logger.hpp"

#include <filesystem>
#include <fstream>

namespace
{
	std::unordered_map<std::string, std::unique_ptr<oe::Core::Config>> configsMap;
}

namespace oe::Core
{
	/*Config::Config(const std::string& file)
	{
		try
		{
			if (!std::filesystem::is_directory("Configs"))
				std::filesystem::create_directory("Configs");

			mFileState.OpenLibraries(sol::lib::base);
			if (!mFileState.LoadFile("Configs/" + file + ".cfg").valid() ||
				!mFileState.GetFile().is_open())
				log::get("log")->warn("Failed to open config " + file + ".cfg" + " file!");
		}
		catch (const std::exception& ex)
		{
			throw std::runtime_error(ex.what());
		}
	}

	void Config::WriteData(const std::string& cfg, const std::string& value)
	{
		(*mFileState.GetState())[cfg] = value;
		mFileState.GetFile() << cfg << " = " << value;
	}

	Config* Config::Get(const std::string& name)
	{
		if (const auto& it = configsMap.find(name); it != configsMap.end())
			return it->second.get();
		return nullptr;
	}

	Config* Config::Add(const std::string& name)
	{
		return (configsMap[name] = std::make_unique<Config>(name)).get();
	}*/
}
