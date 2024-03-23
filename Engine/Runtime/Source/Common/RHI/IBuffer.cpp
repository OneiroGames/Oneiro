//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

module;

#include "Oneiro/Common/StdAfx.hpp"

module Oneiro.Common.RHI.IBuffer;

namespace oe::RHI
{
	void* IBuffer::GetMappedPointer() noexcept
	{
		return m_MappedMemory;
	}

	const void* IBuffer::GetMappedPointer() const noexcept
	{
		return m_MappedMemory;
	}

	size_t IBuffer::Size() const noexcept
	{
		return m_Size;
	}

	bool IBuffer::IsMapped() const noexcept
	{
		return m_MappedMemory != nullptr;
	}
} // namespace oe::RHI