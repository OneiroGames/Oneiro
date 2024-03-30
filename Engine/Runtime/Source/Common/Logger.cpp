//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

module;

#include "Oneiro/Common/StdAfx.hpp"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#if defined(_WIN32)
#include "spdlog/sinks/msvc_sink.h"
#endif

module Oneiro.Common.Logger;

namespace oe
{
	Logger::Logger()
	{
		std::vector<spdlog::sink_ptr> logSinks;
		logSinks.emplace_back(CreateRef<spdlog::sinks::stdout_color_sink_mt>());
		logSinks.emplace_back(CreateRef<spdlog::sinks::basic_file_sink_mt>("log.txt", true));

#if defined(_WIN32)
		logSinks.emplace_back(CreateRef<spdlog::sinks::msvc_sink_mt>());
#endif

		logSinks[0]->set_pattern("%^[%T.%e] %v%$");
		logSinks[1]->set_pattern("[%T.%e] [%l] %v");

		mEngineLogger = CreateRef<spdlog::logger>("Engine", begin(logSinks), end(logSinks));
		spdlog::register_logger(mEngineLogger);
		mEngineLogger->set_level(spdlog::level::trace);
		mEngineLogger->flush_on(spdlog::level::trace);

		mClientLogger = CreateRef<spdlog::logger>("Client", begin(logSinks), end(logSinks));
		spdlog::register_logger(mClientLogger);
		mClientLogger->set_level(spdlog::level::trace);
		mClientLogger->flush_on(spdlog::level::trace);
	}

	const Ref<spdlog::logger>& Logger::GetEngineLogger() const noexcept
	{
		return mEngineLogger;
	}

	const Ref<spdlog::logger>& Logger::GetClientLogger() const noexcept
	{
		return mClientLogger;
	}
} // namespace oe
