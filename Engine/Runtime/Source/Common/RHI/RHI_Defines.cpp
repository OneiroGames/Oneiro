//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

module;

#include "Oneiro/Common/StdAfx.hpp"

module Oneiro.Common.RHI.Defines;

namespace oe::RHI
{
	Extent2D Extent2D::operator+(const Extent2D& other) const
	{
		return {width + other.width, height + other.height};
	}

	Extent2D Extent2D::operator-(const Extent2D& other) const
	{
		return {width - other.width, height - other.height};
	}

	Extent2D Extent2D::operator*(const Extent2D& other) const
	{
		return {width * other.width, height * other.height};
	}

	Extent2D Extent2D::operator/(const Extent2D& other) const
	{
		return {width / other.width, height / other.height};
	}

	Extent2D Extent2D::operator>>(const Extent2D& other) const
	{
		return {width >> other.width, height >> other.height};
	}

	Extent2D Extent2D::operator<<(const Extent2D& other) const
	{
		return {width << other.width, height << other.height};
	}

	Extent2D Extent2D::operator+(uint32_t val) const
	{
		return *this + Extent2D{val, val};
	}

	Extent2D Extent2D::operator-(uint32_t val) const
	{
		return *this - Extent2D{val, val};
	}

	Extent2D Extent2D::operator*(uint32_t val) const
	{
		return *this * Extent2D{val, val};
	}

	Extent2D Extent2D::operator/(uint32_t val) const
	{
		return *this / Extent2D{val, val};
	}

	Extent2D Extent2D::operator>>(uint32_t val) const
	{
		return *this >> Extent2D{val, val};
	}

	Extent2D Extent2D::operator<<(uint32_t val) const
	{
		return *this << Extent2D{val, val};
	}

	Extent2D operator+(uint32_t val, Extent2D ext)
	{
		return ext + val;
	}

	Extent2D operator-(uint32_t val, Extent2D ext)
	{
		return ext - val;
	}

	Extent2D operator*(uint32_t val, Extent2D ext)
	{
		return ext * val;
	}

	Extent2D operator/(uint32_t val, Extent2D ext)
	{
		return ext / val;
	}

	Extent2D operator>>(uint32_t val, Extent2D ext)
	{
		return ext >> val;
	}

	Extent2D operator<<(uint32_t val, Extent2D ext)
	{
		return ext << val;
	}

	Extent3D::operator Extent2D() const
	{
		return {width, height};
	}

	Extent3D Extent3D::operator+(const Extent3D& other) const
	{
		return {width + other.width, height + other.height, depth + other.depth};
	}

	Extent3D Extent3D::operator-(const Extent3D& other) const
	{
		return {width - other.width, height - other.height, depth - other.depth};
	}

	Extent3D Extent3D::operator*(const Extent3D& other) const
	{
		return {width * other.width, height * other.height, depth * other.depth};
	}

	Extent3D Extent3D::operator/(const Extent3D& other) const
	{
		return {width / other.width, height / other.height, depth / other.depth};
	}

	Extent3D Extent3D::operator>>(const Extent3D& other) const
	{
		return {width >> other.width, height >> other.height, depth >> other.depth};
	}

	Extent3D Extent3D::operator<<(const Extent3D& other) const
	{
		return {width << other.width, height << other.height, depth << other.depth};
	}

	Extent3D Extent3D::operator+(uint32_t val) const
	{
		return *this + Extent3D{val, val, val};
	}

	Extent3D Extent3D::operator-(uint32_t val) const
	{
		return *this - Extent3D{val, val, val};
	}

	Extent3D Extent3D::operator*(uint32_t val) const
	{
		return *this * Extent3D{val, val, val};
	}

	Extent3D Extent3D::operator/(uint32_t val) const
	{
		return *this / Extent3D{val, val, val};
	}

	Extent3D Extent3D::operator>>(uint32_t val) const
	{
		return *this >> Extent3D{val, val, val};
	}

	Extent3D Extent3D::operator<<(uint32_t val) const
	{
		return *this << Extent3D{val, val, val};
	}

	Extent3D operator+(uint32_t val, Extent3D ext)
	{
		return ext + val;
	}

	Extent3D operator-(uint32_t val, Extent3D ext)
	{
		return ext - val;
	}

	Extent3D operator*(uint32_t val, Extent3D ext)
	{
		return ext * val;
	}

	Extent3D operator/(uint32_t val, Extent3D ext)
	{
		return ext / val;
	}

	Extent3D operator>>(uint32_t val, Extent3D ext)
	{
		return ext >> val;
	}

	Extent3D operator<<(uint32_t val, Extent3D ext)
	{
		return ext << val;
	}
	Offset2D Offset2D::operator+(const Offset2D& other) const
	{
		return {x + other.x, y + other.y};
	}
	Offset2D Offset2D::operator-(const Offset2D& other) const
	{
		return {x - other.x, y - other.y};
	}
	Offset2D Offset2D::operator*(const Offset2D& other) const
	{
		return {x * other.x, y * other.y};
	}
	Offset2D Offset2D::operator/(const Offset2D& other) const
	{
		return {x / other.x, y / other.y};
	}
	Offset2D Offset2D::operator>>(const Offset2D& other) const
	{
		return {x >> other.x, y >> other.y};
	}
	Offset2D Offset2D::operator<<(const Offset2D& other) const
	{
		return {x << other.x, y << other.y};
	}
	Offset2D Offset2D::operator+(uint32_t val) const
	{
		return *this + Offset2D{val, val};
	}
	Offset2D Offset2D::operator-(uint32_t val) const
	{
		return *this - Offset2D{val, val};
	}
	Offset2D Offset2D::operator*(uint32_t val) const
	{
		return *this * Offset2D{val, val};
	}
	Offset2D Offset2D::operator/(uint32_t val) const
	{
		return *this / Offset2D{val, val};
	}
	Offset2D Offset2D::operator>>(uint32_t val) const
	{
		return *this >> Offset2D{val, val};
	}
	Offset2D Offset2D::operator<<(uint32_t val) const
	{
		return *this << Offset2D{val, val};
	}
} // namespace oe::RHI