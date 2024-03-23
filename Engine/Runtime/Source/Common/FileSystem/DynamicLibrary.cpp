//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

module;

#include "Oneiro/Common/StdAfx.hpp"

module Oneiro.Common.FileSystem.DynamicLibrary;

#ifdef _WIN32
import <Windows.h>;
#endif

import Oneiro.Common.FileSystem.Base;

namespace oe::FileSystem
{
	bool DynamicLibrary::Load(const std::string& path)
	{
		auto p = FixSeparator(Path{path});
#ifdef _WIN32
		m_Module = LoadLibrary((p.string() + ".dll").c_str());
		return m_Module && m_Module != INVALID_HANDLE_VALUE;
#endif
	}

	void DynamicLibrary::Close()
	{
#ifdef _WIN32
		FreeLibrary(m_Module);
#endif
	}
} // namespace oe::FileSystem