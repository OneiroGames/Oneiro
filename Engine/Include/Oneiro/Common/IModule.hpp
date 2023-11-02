//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/Common.hpp"
#include "Oneiro/Common/EngineApi.hpp"

#define OE_MODULE_API __declspec(dllexport)

namespace oe
{
	class IModule
	{
	public:
		virtual ~IModule() = default;

		IModule() = delete;

		IModule(EngineApi* engineApi)
		{
			m_EngineApi = engineApi;
			m_EngineApiInstance = CreateRef<EngineApi>();
			m_EngineApiInstance->instance = engineApi;
		}

		virtual bool Initialize() = 0;
		virtual bool Shutdown() = 0;

	protected:
		EngineApi* GetEngineApi()
		{
			return m_EngineApi;
		}

	private:
		EngineApi* m_EngineApi{};
		Ref<EngineApi> m_EngineApiInstance{};
	};

	extern "C" OE_MODULE_API IModule* CreateModule(EngineApi* engineApi);
	extern "C" OE_MODULE_API void DestroyModule(IModule* engineModule);

	using CreateModuleFunc = IModule* (*)(EngineApi*);
	using DestroyModuleFunc = void (*)(IModule*);
} // namespace oe