//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/EngineApi.hpp"

#include <CompilerGlsl/compileGlsl.hpp>
#include <ShaderAST/Var/Variable.hpp>
#include <ShaderAST/Visitors/SelectEntryPoint.hpp>
#include <ShaderWriter/BaseTypes/Sampler.hpp>
#include <ShaderWriter/BaseTypes/StorageImage.hpp>
#include <ShaderWriter/CompositeTypes/IOStructHelper.hpp>
#include <ShaderWriter/CompositeTypes/IOStructInstanceHelper.hpp>
#include <ShaderWriter/CompositeTypes/MixedStructInstanceHelper.hpp>
#include <ShaderWriter/CompositeTypes/PushConstantBuffer.hpp>
#include <ShaderWriter/Intrinsics/Intrinsics.hpp>
#include <ShaderWriter/MeshWriterNV.hpp>
#include <ShaderWriter/Source.hpp>
#include <ShaderWriter/TaskWriterNV.hpp>
#include <ShaderWriter/Writer.hpp>
#include <ShaderWriter/WriterDebug.hpp>

namespace oe
{
	struct Vertex
	{
		glm::vec2 vertPos{};
	};

	template <sdw::var::Flag FlagT>
	using VertexStructT = sdw::MixedStructInstanceHelperT<FlagT, "VertexStruct", sdw::type::MemoryLayout::eStd430, sdw::IOVec3Field<"position", 0u>>;

	template <sdw::var::Flag FlagT>
	struct VertexStruct : public VertexStructT<FlagT>
	{
		VertexStruct(sdw::ShaderWriter& writer, sdw::expr::ExprPtr expr, bool enabled = true) : VertexStructT<FlagT>{writer, std::move(expr), enabled}
		{
		}

		[[nodiscard]] auto position() const
		{
			return this->template getMember<"position">();
		}
	};

	template <sdw::var::Flag FlagT>
	using ColorStructT =
		sdw::MixedStructInstanceHelperT<FlagT, "ColorStruct", sdw::type::MemoryLayout::eStd430, sdw::IOStructFieldT<sdw::Vec4, "color", 0u>>;

	template <sdw::var::Flag FlagT>
	struct ColorStruct : public ColorStructT<FlagT>
	{
		ColorStruct(sdw::ShaderWriter& writer, sdw::expr::ExprPtr expr, bool enabled = true) : ColorStructT<FlagT>{writer, std::move(expr), enabled}
		{
		}

		[[nodiscard]] auto color() const
		{
			return this->template getMember<"color">();
		}
	};

	class Renderer2D
	{
	public:
		static void Initialize()
		{
			data = CreateRef<Data>();

			Ref<RHI::IShader> vertexShader{};
			Ref<RHI::IShader> fragmentShader{};

			{
				sdw::VertexWriter vertexShaderWriter;
				vertexShaderWriter.implementMainT<VertexStruct, sdw::VoidT>([&](const sdw::VertexInT<VertexStruct>& in, sdw::VertexOut out) {
					out.vtx.position = vec4(in.position(), 1.0_f);
				});

				glsl::GlslConfig vertexConfig{.shaderStage = sdw::ShaderStage::eVertex};
				auto src = glsl::compileGlsl(vertexShaderWriter.getShader(), sdw::SpecialisationInfo{}, vertexConfig);
				vertexShader = EngineApi::GetRHI()->CreateShader(RHI::EShaderStage::VERTEX, src);
			}

			{
				sdw::FragmentWriter fragmentShaderWriter;
				fragmentShaderWriter.implementMainT<sdw::VoidT, ColorStruct>([&](const sdw::FragmentIn& in, sdw::FragmentOutT<ColorStruct> out) {
					out.color() = vec4(1.0_f, 1.0_f, 1.0_f, 1.0_f);
				});

				glsl::GlslConfig fragmentConfig{.shaderStage = sdw::ShaderStage::eFragment};
				auto src = glsl::compileGlsl(fragmentShaderWriter.getShader(), sdw::SpecialisationInfo{}, fragmentConfig);
				fragmentShader = EngineApi::GetRHI()->CreateShader(RHI::EShaderStage::FRAGMENT, src);
			}

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