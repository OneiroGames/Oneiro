//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

module;

#include "Oneiro/Common/StdAfx.hpp"

#include "physfs.h"

module Oneiro.Common.FileSystem.Base;

import Oneiro.Common.FileSystem.Path;

namespace oe::FileSystem
{
	void Init()
	{
		PHYSFS_init(nullptr);
	}

	void Shutdown()
	{
		if (IsInitialized())
			PHYSFS_deinit();
	}

	void Mount(const Path& path, const std::string& mountPoint)
	{
		PHYSFS_mount(path.string().c_str(), mountPoint.c_str(), 1);
	}

	void UnMount(const Path& path)
	{
		PHYSFS_unmount(path.string().c_str());
	}

	std::string Read(const Path& path)
	{
		auto p = FileSystem::FixSeparator(path);
		if (!FileSystem::IsExists(p))
		{
			return {};
		}
		const auto& file = PHYSFS_openRead(p.string().c_str());
		if (file)
		{
			std::string data{};
			auto lenght = PHYSFS_fileLength(file);
			data.resize(lenght);
			PHYSFS_readBytes(file, data.data(), lenght);
			PHYSFS_close(file);
			return data;
		}
		return {};
	}

	void Write(const Path& path, const uint8_t* data, size_t size)
	{
		auto p = FileSystem::FixSeparator(path);

		PHYSFS_setWriteDir(PHYSFS_getBaseDir());
		const auto& file = PHYSFS_openWrite(p.string().c_str());
		if (file)
		{
			PHYSFS_writeBytes(file, data, size);
			PHYSFS_close(file);
		}
	}

	bool IsInitialized() noexcept
	{
		return PHYSFS_isInit();
	}
} // namespace oe::FileSystem