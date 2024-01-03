//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/EngineApi.hpp"

namespace oe
{
	struct Vertex
	{
		glm::vec2 vertPos{};
	};

	class Renderer2D
	{
	public:
		static void Initialize()
		{
			const char* gVertexSource = R"(
                #version 460 core
                layout(location = 0) in vec2 aPos;
                layout(location = 0) out vec2 Pos;
                void main()
                {
                    Pos = aPos;
                      gl_Position = vec4(aPos, 0.0, 1.0);
                }
            )";
			const char* gFragmentSource = R"(
                #version 460 core
                layout(location = 0) out vec4 oColor;
                layout(location = 0) in vec2 Pos;
                void main()
                {
                    oColor = vec4(Pos, 0.0, 1.0);
                }
            )";
			data = CreateRef<Data>();
			auto vert = EngineApi::GetRHI()->CreateShader(RHI::EShaderStage::VERTEX, gVertexSource);
			auto frag = EngineApi::GetRHI()->CreateShader(RHI::EShaderStage::FRAGMENT, gFragmentSource);
			data->graphicsPipeline = EngineApi::GetRHI()->CreateGraphicsPipeline({.vertexShader = vert.get(),
																				  .fragmentShader = frag.get(),
																				  .vertexInputState = {{RHI::VertexInputBindingDescription{
																					  .location = 0,
																					  .binding = 0,
																					  .format = RHI::Format::R32G32_FLOAT,
																					  .offset = static_cast<uint32_t>(offsetof(Vertex, vertPos)),
																				  }}}});
			data->vertexBuffer = EngineApi::GetRHI()->CreateBuffer(data->vertices.data(), data->vertices.size() * sizeof(Vertex));
			data->renderGraph = EngineApi::GetRHI()->CreateRenderGraph();
		}

		static void Shutdown()
		{
			data.reset();
		}

		static void Draw()
		{
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
		}

	private:
		struct Data
		{
			Ref<RHI::IRenderGraph> renderGraph{};
			Ref<RHI::IGraphicsPipeline> graphicsPipeline{};
			const std::vector<Vertex> vertices = {{{-0.5f, -0.5f}}, {{0.5f, -0.5f}}, {{0.0f, 0.5f}}};
			Ref<RHI::IBuffer> vertexBuffer;
		};
		inline static Ref<Data> data{};
	};
} // namespace oe