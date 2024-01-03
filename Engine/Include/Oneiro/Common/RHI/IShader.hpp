//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/RHI/RHI_Defines.hpp"

namespace oe::RHI
{
	enum class EShaderStage
	{
		VERTEX,
		FRAGMENT
	};

	class IShader
	{
	public:
		virtual bool Load(EShaderStage stage, const std::string& source) = 0;
	};
} // namespace oe::RHI