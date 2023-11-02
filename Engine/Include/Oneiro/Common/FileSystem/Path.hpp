//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <filesystem>

namespace oe::FileSystem
{
	class Path : public std::filesystem::path
	{
	public:
		using std::filesystem::path::path;
		Path(std::filesystem::path path);

		[[nodiscard]] Path GetLocal() const;

		[[nodiscard]] Path GetGlobal() const;

		[[nodiscard]] bool IsValid() const noexcept;

		[[nodiscard]] bool IsLocal() const noexcept;

		[[nodiscard]] bool IsGlobal() const noexcept;

		class Iterator : public std::filesystem::directory_iterator
		{
		public:
			using std::filesystem::directory_iterator::directory_iterator;
		};
	};

	void CreatePath(const Path& path) noexcept;
	Path CurrentPath() noexcept;

	bool IsPath(const Path& path) noexcept;
	bool IsFile(const Path& path) noexcept;
	bool IsExists(const Path& path) noexcept;
} // namespace oe::FileSystem
