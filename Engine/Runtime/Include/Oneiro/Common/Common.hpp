//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/Config.hpp"
#include "Oneiro/Common/Loggger.hpp"
#include "Oneiro/Common/Utils.hpp"

#include <cstdint>
#include <string>

#define NOMINMAX

// Begin debugbreak detection
#if OE_DEBUG
#if defined(_WIN64)
#define OE_DEBUGBREAK() __debugbreak()
#elif defined(__linux__)
#include <csignal>
#define OE_DEBUGBREAK() raise(SIGTRAP)
#else
#error "Platform doesn't support debugbreak yet!"
#endif
#endif
// End debugbreak detection

// Begin asserts
#if OE_DEBUG && OE_ASSERTS
#define OE_CORE_ASSERT(x, ...)         \
	if (!(x))                          \
	{                                  \
		OE_CORE_CRITICAL(__VA_ARGS__); \
		OE_DEBUGBREAK();               \
	}
#define OE_ASSERT(x, ...)         \
	if (!(x))                     \
	{                             \
		OE_CRITICAL(__VA_ARGS__); \
		OE_DEBUGBREAK();          \
	}
#else
#define OE_CORE_ASSERT(x, ...)
#define OE_ASSERT(x, ...)
#endif