//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <memory>

#define OE_DECLARE_FLAG_TYPE(FLAG_TYPE, FLAG_BITS, BASE_TYPE)                                 \
                                                                                              \
	struct FLAG_TYPE                                                                          \
	{                                                                                         \
		BASE_TYPE flags = static_cast<BASE_TYPE>(0);                                          \
                                                                                              \
		constexpr FLAG_TYPE() noexcept = default;                                             \
		constexpr explicit FLAG_TYPE(BASE_TYPE in) noexcept : flags(in) {}                    \
		constexpr FLAG_TYPE(FLAG_BITS in) noexcept : flags(static_cast<BASE_TYPE>(in)) {}     \
		constexpr bool operator==(FLAG_TYPE const& right) const                               \
		{                                                                                     \
			return flags == right.flags;                                                      \
		}                                                                                     \
		constexpr bool operator!=(FLAG_TYPE const& right) const                               \
		{                                                                                     \
			return flags != right.flags;                                                      \
		}                                                                                     \
		constexpr explicit operator BASE_TYPE() const                                         \
		{                                                                                     \
			return flags;                                                                     \
		}                                                                                     \
		constexpr explicit operator bool() const noexcept                                     \
		{                                                                                     \
			return flags != 0;                                                                \
		}                                                                                     \
	};                                                                                        \
	constexpr FLAG_TYPE operator|(FLAG_TYPE a, FLAG_TYPE b) noexcept                          \
	{                                                                                         \
		return static_cast<FLAG_TYPE>(a.flags | b.flags);                                     \
	}                                                                                         \
	constexpr FLAG_TYPE operator&(FLAG_TYPE a, FLAG_TYPE b) noexcept                          \
	{                                                                                         \
		return static_cast<FLAG_TYPE>(a.flags & b.flags);                                     \
	}                                                                                         \
	constexpr FLAG_TYPE operator^(FLAG_TYPE a, FLAG_TYPE b) noexcept                          \
	{                                                                                         \
		return static_cast<FLAG_TYPE>(a.flags ^ b.flags);                                     \
	}                                                                                         \
	constexpr FLAG_TYPE operator~(FLAG_TYPE a) noexcept                                       \
	{                                                                                         \
		return static_cast<FLAG_TYPE>(~a.flags);                                              \
	}                                                                                         \
	constexpr FLAG_TYPE& operator|=(FLAG_TYPE& a, FLAG_TYPE b) noexcept                       \
	{                                                                                         \
		a.flags = (a.flags | b.flags);                                                        \
		return a;                                                                             \
	}                                                                                         \
	constexpr FLAG_TYPE& operator&=(FLAG_TYPE& a, FLAG_TYPE b) noexcept                       \
	{                                                                                         \
		a.flags = (a.flags & b.flags);                                                        \
		return a;                                                                             \
	}                                                                                         \
	constexpr FLAG_TYPE operator^=(FLAG_TYPE& a, FLAG_TYPE b) noexcept                        \
	{                                                                                         \
		a.flags = (a.flags ^ b.flags);                                                        \
		return a;                                                                             \
	}                                                                                         \
	constexpr FLAG_TYPE operator|(FLAG_BITS a, FLAG_BITS b) noexcept                          \
	{                                                                                         \
		return static_cast<FLAG_TYPE>(static_cast<BASE_TYPE>(a) | static_cast<BASE_TYPE>(b)); \
	}                                                                                         \
	constexpr FLAG_TYPE operator&(FLAG_BITS a, FLAG_BITS b) noexcept                          \
	{                                                                                         \
		return static_cast<FLAG_TYPE>(static_cast<BASE_TYPE>(a) & static_cast<BASE_TYPE>(b)); \
	}                                                                                         \
	constexpr FLAG_TYPE operator~(FLAG_BITS key) noexcept                                     \
	{                                                                                         \
		return static_cast<FLAG_TYPE>(~static_cast<BASE_TYPE>(key));                          \
	}                                                                                         \
	constexpr FLAG_TYPE operator^(FLAG_BITS a, FLAG_BITS b) noexcept                          \
	{                                                                                         \
		return static_cast<FLAG_TYPE>(static_cast<BASE_TYPE>(a) ^ static_cast<BASE_TYPE>(b)); \
	}

namespace oe
{
	template <class T>
	using Ref = std::shared_ptr<T>;

	template <class T, class... Args>
	constexpr Ref<T> CreateRef(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
} // namespace oe
