//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Common/FileSystem/ConfigFile.hpp"

namespace oe::FileSystem
{
	ConfigFile::ConfigFile(Path path) : m_Path(std::move(path))
	{
		Open(m_Path);
	}

	ConfigFile::~ConfigFile()
	{
		Save();
	}

	bool ConfigFile::LoadFromData(const std::string& data)
	{
		m_File.Parse(data.c_str());
		return true;
	}

	bool ConfigFile::Open(const Path& path)
	{
		m_Path = path;
		auto data = Read(m_Path);
		return LoadFromData(data);
	}

	bool ConfigFile::Save()
	{
		rapidjson::StringBuffer writerBuffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(writerBuffer);
		m_File.Accept(writer);

		const std::string buffer{writerBuffer.GetString()};
		if (!buffer.empty() && !m_Path.empty())
			Write(m_Path, reinterpret_cast<const uint8_t*>(buffer.c_str()), buffer.size());
		return true;
	}

	bool ConfigFile::Save(const Path& path)
	{
		rapidjson::StringBuffer writerBuffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(writerBuffer);
		m_File.Accept(writer);

		const std::string buffer{writerBuffer.GetString()};
		if (!buffer.empty() && !path.empty())
			Write(path, reinterpret_cast<const uint8_t*>(buffer.c_str()), buffer.size());
		return true;
	}
} // namespace oe::FileSystem