//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

module;

#include "Oneiro/Common/StdAfx.hpp"

#include "slang.h"
#include "slang-com-ptr.h"

module Oneiro.Common.RHI.ShaderCompiler;

import Oneiro.Common.EngineApi;
import Oneiro.Common.FileSystem.Path;
import <vector>;
import <initializer_list>;
import <unordered_map>;

namespace oe::RHI
{
    Slang::ComPtr<slang::ISession> ShaderCompiler::session = {};

	bool ShaderCompiler::Initialize()
	{
		slang::SessionDesc sessionDesc{};
		slang::TargetDesc targetDesc;
		targetDesc.format = SLANG_GLSL;
		targetDesc.profile = EngineApi::GetSlangGlobalSession()->findProfile("glsl_450");
		sessionDesc.targets = &targetDesc;
		sessionDesc.targetCount = 1;
		EngineApi::GetSlangGlobalSession()->createSession(sessionDesc, session.writeRef());
		return true;
	}

	bool ShaderCompiler::Shutdown()
	{
		session->Release();
		session.~ComPtr();
		return true;
	}

	std::unordered_map<EShaderStage, std::string> ShaderCompiler::Compile(const SShaderCompileDescription& description)
	{
		Slang::ComPtr<slang::IBlob> diagnosticsBlob;

		std::unordered_map<EShaderStage, std::string> results{};

		std::vector<slang::IComponentType*> componentTypes{};

		auto* module = session->loadModule(description.fileName.c_str(), diagnosticsBlob.writeRef());
		if(!module)
			OE_CORE_WARN("Failed to load shader module '{}'. Error: {}", description.fileName, static_cast<const char*>(diagnosticsBlob->getBufferPointer()));
		else
			componentTypes.push_back(module);

		for (const auto& componentDesc : description.descriptions)
		{
			Slang::ComPtr<slang::IEntryPoint> entryPoint{};
			if (module->findEntryPointByName(componentDesc.entryPoint.c_str(), entryPoint.writeRef()) != SLANG_OK)
				OE_CORE_WARN("Failed to find {} entry point in shader module '{}'", componentDesc.entryPoint, description.fileName);
			else
				componentTypes.push_back(entryPoint);
		}

		Slang::ComPtr<slang::IComponentType> program{};
		if (session->createCompositeComponentType(componentTypes.data(), componentTypes.size(), program.writeRef(), diagnosticsBlob.writeRef()) != SLANG_OK)
			OE_CORE_WARN("Failed to createComponentType for shader module '{}'", description.fileName);
		else
		{
			int i{};
			for (const auto& componentDesc : description.descriptions)
			{
				Slang::ComPtr<slang::IBlob> kernelBlob;
				program->getEntryPointCode(i, 0, kernelBlob.writeRef(), diagnosticsBlob.writeRef());
				results[componentDesc.shaderStage] = static_cast<const char*>(kernelBlob->getBufferPointer());
				i++;
			}
		}

		return results;
	}
}