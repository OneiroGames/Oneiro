//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/Common.hpp"
#include "Oneiro/Common/FileSystem/DynamicLibrary.hpp"
#include "Oneiro/Common/FileSystem/Path.hpp"

namespace oe::FileSystem
{
	void Init();
	void Shutdown();

	void Mount(const Path& path, const std::string& mountPoint = "");
	void UnMount(const Path& path);

	std::string Read(const Path& path);
	void Write(const Path& path, const uint8_t* data, size_t size);

	[[nodiscard]] bool IsInitialized() noexcept;
} // namespace oe::FileSystem
