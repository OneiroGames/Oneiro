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

		virtual bool Initialize(EngineApi* api) = 0;
		virtual bool Shutdown() = 0;

		const std::string& GetName() const
		{
			return m_Name;
		}

	protected:
		friend class ModuleManager;
		std::string m_Name{};
	};

	extern "C" OE_MODULE_API IModule* CreateModule();
	extern "C" OE_MODULE_API void DestroyModule(IModule* engineModule);

	using CreateModuleFunc = IModule* (*)();
	using DestroyModuleFunc = void (*)(IModule*);
} // namespace oe