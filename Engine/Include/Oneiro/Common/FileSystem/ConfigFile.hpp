//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/FileSystem/FileSystem.hpp"

#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

#include <array>
#include <utility>

namespace oe::FileSystem
{
	class ConfigFile
	{
	public:
		ConfigFile() = default;

		ConfigFile(Path path);

		~ConfigFile();

		bool LoadFromData(const std::string& data);

		bool Open(const Path& path);

		bool Save();

		bool Save(const Path& path);

		auto& operator[](const std::string& key)
		{
			return m_File[key.c_str()];
		}

		bool Has(const std::string& key)
		{
			return m_File.HasMember(key.c_str());
		}

	private:
		rapidjson::Document m_File{};
		Path m_Path{};
	};
} // namespace oe::FileSystem