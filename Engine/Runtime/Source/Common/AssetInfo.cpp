//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

module;

#include "Oneiro/Common/StdAfx.hpp"

module Oneiro.Common.AssetInfo;

size_t oe::AssetInfo::GetHash() const noexcept
{
	return m_Hash;
}

bool oe::AssetInfo::operator==(size_t hash) const noexcept
{
	return m_Hash == hash;
}

bool oe::AssetInfo::operator!=(size_t hash) const noexcept
{
	return !operator==(hash);
}

bool oe::AssetInfo::operator==(oe::AssetInfo& other) const noexcept
{
	return m_Hash == other.m_Hash;
}

bool oe::AssetInfo::operator!=(oe::AssetInfo& other) const noexcept
{
	return !operator==(other);
}
