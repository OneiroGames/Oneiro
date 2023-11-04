//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/IModule.hpp"
#include "Oneiro/Common/IRenderer.hpp"

#include "glad/gl.h"

#include "Fwog/Context.h"
#include "Fwog/Pipeline.h"
#include "Fwog/Rendering.h"
#include "Fwog/Shader.h"
#include "imgui_impl_opengl3.h"

namespace oe::GL
{
	namespace fwog = Fwog;
	class Renderer : public IRenderer
	{
	public:
		using IRenderer::IRenderer;

		bool PreInitialize() override
		{
			m_RendererBackendType = ERendererBackendType::GL;
			return true;
		}

		bool Initialize(IWindow* window) override
		{
			gladLoadGL(static_cast<GLADloadfunc>(window->GetWindowHandleData().procAddress));
			fwog::Initialize();
			return true;
		}

		bool Shutdown() override
		{
			fwog::Terminate();
			return true;
		}

		bool InitializeImGui(void* ctx) override
		{
			ImGui::SetCurrentContext(static_cast<ImGuiContext*>(ctx));
			ImGui_ImplOpenGL3_Init("#version 460");
			return true;
		}

		bool BeginImGuiFrame() override
		{
			ImGui_ImplOpenGL3_NewFrame();
			return true;
		}

		bool EndImGuiFrame() override
		{
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			return true;
		}

		bool ShutdownImGui() override
		{
			ImGui_ImplOpenGL3_Shutdown();
			return true;
		}
	};

	class RendererEngineModule : public IModule
	{
	public:
		using IModule::IModule;

		bool Initialize() override
		{
			m_Renderer = CreateRef<Renderer>();
			GetEngineApi()->rendererBackend = m_Renderer.get();
			return true;
		}

		bool Shutdown() override
		{
			GetEngineApi()->rendererBackend = nullptr;
			m_Renderer.reset();
			return true;
		}

	private:
		Ref<Renderer> m_Renderer{};
	};

	extern "C" OE_MODULE_API IModule* CreateModule(EngineApi* engineApi)
	{
		return new RendererEngineModule(engineApi);
	}

	extern "C" OE_MODULE_API void DestroyModule(IModule* engineModule)
	{
		delete engineModule;
	}
} // namespace oe::GL