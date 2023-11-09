//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/FileSystem/Path.hpp"

#ifdef _WIN32
#include <Windows.h>
#endif

namespace oe::FileSystem
{
	class DynamicLibrary
	{
	public:
		bool Load(const std::string& path);

		template <class T>
		T GetFunction(const std::string& name)
		{
			return reinterpret_cast<T>(GetProcAddress(m_Module, name.c_str()));
		}

		void Close();

	private:
		HMODULE m_Module{};
	};
} // namespace oe::FileSystem