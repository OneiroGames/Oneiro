//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Common/FileSystem/Path.hpp"

#include "physfs.h"

namespace oe::FileSystem
{
	Path::Path(std::filesystem::path path)
	{
		path.swap(*this);
	}

	Path Path::GetLocal() const
	{
		if (IsLocal())
			return *this;
		return {this->string().erase(0, CurrentPath().string().size() + 1)};
	}

	Path Path::GetGlobal() const
	{
		if (IsGlobal())
			return *this;
		return {CurrentPath() / *this};
	}

	bool Path::IsValid() const noexcept
	{
		return !this->empty() && IsExists(*this);
	}

	bool Path::IsLocal() const noexcept
	{
		return !IsGlobal();
	}

	bool Path::IsGlobal() const noexcept
	{
		return this->string().find(CurrentPath().string()) != std::string::npos;
	}

	oe::FileSystem::Path CurrentPath() noexcept
	{
		return {std::filesystem::current_path()};
	}

	void CreatePath(const oe::FileSystem::Path& path) noexcept
	{
		std::filesystem::create_directory(path);
	}

	bool IsPath(const oe::FileSystem::Path& path) noexcept
	{
		PHYSFS_Stat stat{};
		PHYSFS_stat(path.string().c_str(), &stat);
		return stat.filetype == PHYSFS_FILETYPE_DIRECTORY;
	}

	bool IsFile(const oe::FileSystem::Path& path) noexcept
	{
		PHYSFS_Stat stat{};
		PHYSFS_stat(path.string().c_str(), &stat);
		return stat.filetype == PHYSFS_FILETYPE_REGULAR;
	}

	bool IsExists(const oe::FileSystem::Path& path) noexcept
	{
		PHYSFS_Stat stat{};
		PHYSFS_stat(path.string().c_str(), &stat);
		return stat.filesize != -1;
	}
} // namespace oe::FileSystem