//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Core/Engine.hpp"

#define OE_MAIN(applicationFunc)                                 \
	int main(int /*argc*/, char** /*argv*/)                      \
	{                                                            \
		auto application = applicationFunc();                    \
		auto engine = std::make_unique<oe::Engine>();            \
		try                                                      \
		{                                                        \
			engine->PreInit(application.get());                  \
			engine->Init();                                      \
			engine->Run();                                       \
		}                                                        \
		catch (const std::exception& exception)                  \
		{                                                        \
			OE_CORE_CRITICAL("Exception: {}", exception.what()); \
			engine->Shutdown();                                  \
			return 1;                                            \
		}                                                        \
		engine->Shutdown();                                      \
		application.reset();                                     \
		engine.reset();                                          \
		return 0;                                                \
	}                                                            \
	\