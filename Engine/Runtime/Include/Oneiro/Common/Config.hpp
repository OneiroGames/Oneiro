//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <string_view>

// clang-format off
#define OE_VERSION_MAJOR 1
#define OE_VERSION_MINOR 0
#define OE_VERSION_ALTER 0
#define OE_DEBUG 1
#define OE_ASSERTS 1

#define SDL_MAIN_HANDLED

static constexpr std::string_view OE_VERSION = "1.0";
static constexpr std::string_view OE_VERSION_BUILD = "202305301948";
static constexpr std::string_view OE_PLATFORM = "LINUX";
static constexpr std::string_view OE_ARCH = "X86_64";
static constexpr std::string_view OE_OPERATING_SYSTEM = "LINUX";
static constexpr std::string_view OE_BUILD_MODE = "DEBUG";

static constexpr std::string_view OE_GIT_COMMIT_HASH = "4c22b48aa57f3ea6a2526aaf0ca8c26d56b0fb58";
static constexpr std::string_view OE_GIT_COMMIT_DATE = "20230530123222";
// clang-format on
