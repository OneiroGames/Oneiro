//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Common/FileSystem/Path.hpp"

#include "physfs.h"

oe::FileSystem::Path::Path(std::filesystem::path path)
{
	path.swap(*this);
}

oe::FileSystem::Path oe::FileSystem::Path::GetLocal() const
{
	if (IsLocal())
		return *this;
	return {this->string().erase(0, CurrentPath().string().size() + 1)};
}

oe::FileSystem::Path oe::FileSystem::Path::GetGlobal() const
{
	if (IsGlobal())
		return *this;
	return {CurrentPath() / *this};
}

bool oe::FileSystem::Path::IsValid() const noexcept
{
	return !this->empty() && IsExists(*this);
}

bool oe::FileSystem::Path::IsLocal() const noexcept
{
	return !IsGlobal();
}

bool oe::FileSystem::Path::IsGlobal() const noexcept
{
	return this->string().find(CurrentPath().string()) != std::string::npos;
}

oe::FileSystem::Path oe::FileSystem::CurrentPath() noexcept
{
	return {std::filesystem::current_path()};
}

void oe::FileSystem::CreatePath(const oe::FileSystem::Path& path) noexcept
{
	std::filesystem::create_directory(path);
}

bool oe::FileSystem::IsPath(const oe::FileSystem::Path& path) noexcept
{
	PHYSFS_Stat stat{};
	PHYSFS_stat(path.string().c_str(), &stat);
	return stat.filetype == PHYSFS_FILETYPE_DIRECTORY;
}

bool oe::FileSystem::IsFile(const oe::FileSystem::Path& path) noexcept
{
	PHYSFS_Stat stat{};
	PHYSFS_stat(path.string().c_str(), &stat);
	return stat.filetype == PHYSFS_FILETYPE_REGULAR;
}

bool oe::FileSystem::IsExists(const oe::FileSystem::Path& path) noexcept
{
	PHYSFS_Stat stat{};
	PHYSFS_stat(path.string().c_str(), &stat);
	return stat.filesize != -1;
}
