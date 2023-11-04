//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/EngineApi.hpp"
#include "Oneiro/Common/FileSystem/ConfigFile.hpp"
#include "Oneiro/Common/FileSystem/DynamicLibrary.hpp"
#include "Oneiro/Common/IApplication.hpp"
#include "Oneiro/Common/IModule.hpp"

namespace oe
{
	class Engine
	{
	public:
		void Init(IApplication* application);
		void Run();
		void Shutdown();

		static float GetDeltaTime() noexcept;
		static bool IsRuntime() noexcept;

	private:
		Ref<EngineApi> m_EngineApi{};
		Ref<FileSystem::ConfigFile> m_ConfigFile{};

		IModule* m_WMModule{};
		IModule* m_RendererModule{};

		inline static float mDeltaTime{};
		inline static bool mIsRuntime{};

		bool mIsWorldStateControl{};
	};
} // namespace oe
