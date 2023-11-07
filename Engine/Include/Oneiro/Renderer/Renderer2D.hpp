//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/EngineApi.hpp"

namespace oe
{
	class Renderer2D
	{
	public:
		static void Initialize()
		{
			const char* gVertexSource = R"(
				#version 460 core

				layout(location = 0) in vec2 aPos;

				void main()
				{
				  gl_Position = vec4(aPos, 0.0, 1.0);
				}
			)";

			const char* gFragmentSource = R"(
					#version 460 core

					layout(location = 0) out vec4 o_color;

					void main()
					{
					  o_color = vec4(1.0, 1.0, 1.0, 1.0);
					}
			)";
			auto descPos = Renderer::VertexInputBindingDescription{
				.location = 0,
				.binding = 0,
				.format = Renderer::Format::R32G32_FLOAT,
				.offset = 0,
			};

			data = CreateRef<Data>();

			auto inputDescs = {descPos};
			auto vert = EngineApi::GetRendererBackend()->CreateShader(Renderer::EShaderStage::VERTEX, gVertexSource);
			auto frag = EngineApi::GetRendererBackend()->CreateShader(Renderer::EShaderStage::FRAGMENT, gFragmentSource);
			data->graphicsPipeline = EngineApi::GetRendererBackend()->CreateGraphicsPipeline(
				{.vertexShader = vert.get(), .fragmentShader = frag.get(), .vertexInputState = {inputDescs}});

			data->vertexPosBuffer =
				EngineApi::GetRendererBackend()->CreateBuffer(data->triPositions.data(), data->triPositions.size() * sizeof(float));
			data->renderPass = EngineApi::GetRendererBackend()->CreateRenderPass();
		}

		static void Shutdown()
		{
			data.reset();
		}

		static void Draw()
		{
			const auto& windowSize = EngineApi::GetWindowManager()->GetPlatformWindow(0)->GetSize();
			data->renderPass->Begin(
				{
					.viewport = {.drawRect{.offset = {0, 0}, .extent = {static_cast<uint32_t>(windowSize.x), static_cast<uint32_t>(windowSize.y)}}},
					.colorLoadOp = Renderer::AttachmentLoadOp::CLEAR,
					.clearColorValue = {.2f, .0f, .2f, 1.0f},
				},
				[&](Renderer::ICommandBuffer* commandBuffer) {
					commandBuffer->BindGraphicsPipeline(data->graphicsPipeline);
					commandBuffer->BindVertexBuffer(0, data->vertexPosBuffer, 0, 2 * sizeof(float));
					commandBuffer->Draw(3, 1, 0, 0);
				});
			data->renderPass->End();
		}

	private:
		struct Data
		{
			Ref<Renderer::IRenderPass> renderPass{};
			Ref<Renderer::IGraphicsPipeline> graphicsPipeline{};

			static constexpr std::array<float, 6> triPositions = {-0.5f, -0.5f, 0.5f, -0.5f, 0.0f, 0.5f};
			Ref<Renderer::IBuffer> vertexPosBuffer;
		};
		inline static Ref<Data> data{};
	};
} // namespace oe