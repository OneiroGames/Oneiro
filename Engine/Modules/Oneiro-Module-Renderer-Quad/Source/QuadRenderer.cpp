//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

module;

#include "Oneiro/Common/StdAfx.hpp"

#include "glm/glm.hpp"

module Oneiro.Module.Renderer.Quad;

import Oneiro.Common.RHI.ShaderCompiler;
import Oneiro.Common.EngineApi;
import Oneiro.Common.ECS.World;
import Oneiro.Common.IModule;
import Oneiro.Common.RHI.Base;
import Oneiro.Common.ECS.Components.Transform;

namespace oe
{
	struct Vertex
	{
		glm::vec3 vertPos{};
	};
	
	struct Data
	{
		Ref<RHI::IRenderGraph> renderGraph{};
		Ref<RHI::IGraphicsPipeline> graphicsPipeline{};
		const std::vector<Vertex> vertices = {{{-0.5f, -0.5f, 0.0f}}, {{0.5f, -0.5f, 0.0f}}, {{0.0f, 0.5f, 0.0f}}};
		Ref<RHI::IBuffer> vertexBuffer;

		flecs::system system{};
	};
	Ref<Data> data{};
	
    bool QuadRendererEngineModule::Initialize()
	{
    	data = CreateRef<Data>();

    	Ref<RHI::IShader> vertexShader{};
    	Ref<RHI::IShader> fragmentShader{};

    	auto shaderCompileRes = RHI::ShaderCompiler::Compile({"Modules/Oneiro-Module-Renderer-Quad/Shaders/quad", {{RHI::EShaderStage::VERTEX, "vertexMain"}, {RHI::EShaderStage::FRAGMENT,
    	 "fragmentMain"}}});

    	vertexShader = EngineApi::GetRHI()->CreateShader(RHI::EShaderStage::VERTEX, shaderCompileRes[RHI::EShaderStage::VERTEX]);
    	fragmentShader = EngineApi::GetRHI()->CreateShader(RHI::EShaderStage::FRAGMENT, shaderCompileRes[RHI::EShaderStage::FRAGMENT]);
    	
    	data->graphicsPipeline = EngineApi::GetRHI()->CreateGraphicsPipeline({.vertexShader = vertexShader.get(),
																				  .fragmentShader = fragmentShader.get(),
																				  .vertexInputState = {{RHI::VertexInputBindingDescription{
																					  .location = 0,
																					  .binding = 0,
																					  .format = RHI::Format::R32G32_FLOAT,
																					  .offset = static_cast<uint32_t>(offsetof(Vertex, vertPos)),
																				  }}}});
    	data->vertexBuffer = EngineApi::GetRHI()->CreateBuffer(data->vertices.data(), data->vertices.size() * sizeof(Vertex));
    	data->renderGraph = EngineApi::GetRHI()->CreateRenderGraph();
    	
        auto system = EngineApi::GetWorldManager()->RegisterSystem<TransformComponent>("test");
    	data->system = system.each([&](flecs::entity entity, TransformComponent& component) {
    		const auto& windowSize = EngineApi::GetWindowManager()->GetPlatformWindow(0)->GetSize();
			data->renderGraph->Begin(
				{
					.viewport = {.drawRect{.offset = {0, 0}, .extent = {static_cast<uint32_t>(windowSize.x), static_cast<uint32_t>(windowSize.y)}}},
					.colorLoadOp = RHI::AttachmentLoadOp::CLEAR,
					.clearColorValue = {.2f, .0f, .2f, 1.0f},
				},
				[&](RHI::ICommandBuffer* commandBuffer) {
					commandBuffer->BindGraphicsPipeline(data->graphicsPipeline);
					commandBuffer->BindVertexBuffer(0, data->vertexBuffer, 0, sizeof(Vertex));
					commandBuffer->Draw(3, 1, 0, 0);
				});
			data->renderGraph->End();
    	});

		return true;
	}

	bool QuadRendererEngineModule::Shutdown()
	{
    	EngineApi::GetWorldManager()->UnRegisterSystem(data->system);
    	data.reset();
		return true;
	}
} // namespace oe

extern "C" __declspec(dllexport) oe::IModule* CreateModule()
{
	return new oe::QuadRendererEngineModule();
}

extern "C" __declspec(dllexport) void DestroyModule(oe::IModule* engineModule)
{
    delete engineModule;
}