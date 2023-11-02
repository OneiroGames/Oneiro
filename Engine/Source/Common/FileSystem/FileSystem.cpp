//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Common/FileSystem/FileSystem.hpp"

#include "Oneiro/Common/FileSystem/Path.hpp"

#include "physfs.h"

void oe::FileSystem::Init()
{
	PHYSFS_init(nullptr);
}

void oe::FileSystem::Shutdown()
{
	if (IsInitialized())
		PHYSFS_deinit();
}

void oe::FileSystem::Mount(const Path& path, const std::string& mountPoint)
{
	PHYSFS_mount(path.string().c_str(), mountPoint.c_str(), 1);
}

void oe::FileSystem::UnMount(const Path& path)
{
	PHYSFS_unmount(path.string().c_str());
}

std::string oe::FileSystem::Read(const Path& path)
{
	if (!FileSystem::IsExists(path))
	{
		return {};
	}
	const auto& file = PHYSFS_openRead(path.string().c_str());
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

void oe::FileSystem::Write(const Path& path, const uint8_t* data, size_t size)
{

	PHYSFS_setWriteDir(PHYSFS_getBaseDir());
	const auto& file = PHYSFS_openWrite(path.string().c_str());
	if (file)
	{
		PHYSFS_writeBytes(file, data, size);
		PHYSFS_close(file);
	}
}

bool oe::FileSystem::IsInitialized() noexcept
{
	return PHYSFS_isInit();
}
