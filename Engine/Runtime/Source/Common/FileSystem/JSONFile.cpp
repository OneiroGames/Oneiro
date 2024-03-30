//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

module;

#include "Oneiro/Common/StdAfx.hpp"

module Oneiro.Common.FileSystem.JSONFile;

import Oneiro.Common.FileSystem.Base;

import <rapidjson/document.h>;
import <rapidjson/rapidjson.h>;
import <rapidjson/stringbuffer.h>;
import <rapidjson/writer.h>;

namespace oe::FileSystem
{
	JSONFile::JSONFile(Path path) : m_Path(std::move(path))
	{
		Open(m_Path);
	}

	JSONFile::~JSONFile()
	{
		Save();
	}

	bool JSONFile::LoadFromData(const std::string& data)
	{
		m_File.Parse(data.c_str());
		return true;
	}

	bool JSONFile::Open(const Path& path)
	{
		m_Path = FixSeparator(path);
		auto data = Read(m_Path);
		return LoadFromData(data);
	}

	bool JSONFile::Save()
	{
		rapidjson::StringBuffer writerBuffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(writerBuffer);
		m_File.Accept(writer);

		const std::string buffer{writerBuffer.GetString()};
		if (!buffer.empty() && !m_Path.empty())
			Write(m_Path, reinterpret_cast<const uint8_t*>(buffer.c_str()), buffer.size());
		return true;
	}

	bool JSONFile::Save(const Path& path)
	{
		auto p = FixSeparator(path);

		rapidjson::StringBuffer writerBuffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(writerBuffer);
		m_File.Accept(writer);

		const std::string buffer{writerBuffer.GetString()};
		if (!buffer.empty() && !p.string().empty())
			Write(p, reinterpret_cast<const uint8_t*>(buffer.c_str()), buffer.size());
		return true;
	}
} // namespace oe::FileSystem