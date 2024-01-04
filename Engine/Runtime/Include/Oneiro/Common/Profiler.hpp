//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/EngineApi.hpp"

#define OE_PROFILE_FRAME() ::oe::EngineApi::GetProfiler()->SendFrameMark(nullptr)
#define OE_PROFILE_FRAME_NAMED(name) ::oe::EngineApi::GetProfiler()->SendFrameMark(name)
#define OE_PROFILE_FRAME_START(name) ::oe::EngineApi::GetProfiler()->SendFrameMark(name, tracy::QueueType::FrameMarkMsgStart)
#define OE_PROFILE_FRAME_END(name) ::oe::EngineApi::GetProfiler()->SendFrameMark(name, tracy::QueueType::FrameMarkMsgEnd)
#define OE_PROFILE_SCOPED_ZONE(name)                                     \
	std::source_location sourceLocation{};                               \
	tracy::ScopedZone scopedZone{sourceLocation.line(),                  \
								 sourceLocation.file_name(),             \
								 strlen(sourceLocation.file_name()),     \
								 sourceLocation.function_name(),         \
								 strlen(sourceLocation.function_name()), \
								 name,                                   \
								 strlen(name)};
