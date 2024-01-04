//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/IApplication.hpp"
#include "Oneiro/Common/IModule.hpp"
#include "Oneiro/Common/JobManager.hpp"

namespace oe
{
	class Engine
	{
	public:
		void PreInit(IApplication* application);
		void Init();
		void Run();
		void Shutdown();

		static float GetDeltaTime() noexcept;
		static bool IsRuntime() noexcept;

		EngineApi* GetApi()
		{
			return m_EngineApi;
		}

	private:
		inline static float m_DeltaTime{};
		inline static bool m_IsRuntime{};

		IModule* m_WMModule{};
		IModule* m_RendererModule{};
		EngineApi* m_EngineApi{};
	};
} // namespace oe
