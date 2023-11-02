//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/FileSystem/Path.hpp"

#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>
#endif

namespace oe::FileSystem
{
	class DynamicLibrary
	{
	public:
		bool Load(const std::string& path);

		void* GetFunction(const std::string& name);

		void Close();

	private:
		HMODULE m_Module{};
	};
} // namespace oe::FileSystem