//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/RHI/RHI_Defines.hpp"

namespace oe::RHI
{
	class ICommandBuffer
	{
	public:
		virtual ~ICommandBuffer() = default;
		/// @brief Binds a graphics pipeline to be used for future draw operations
		/// @param pipeline The pipeline to bind
		///
		/// Valid in rendering scopes.
		virtual void BindGraphicsPipeline(const Ref<IGraphicsPipeline>& pipeline) = 0;

		/// @brief Binds a compute pipeline to be used for future dispatch operations
		/// @param pipeline The pipeline to bind
		///
		/// Valid in compute scopes.
		virtual void BindComputePipeline(const Ref<IComputePipeline>& pipeline) = 0;

		/// @brief Dynamically sets the viewport
		/// @param viewport The new viewport
		///
		/// Similar to glViewport. Valid in rendering scopes.
		virtual void SetViewport(const Viewport& viewport) = 0;

		/// @brief Dynamically sets the scissor rect
		/// @param scissor The new scissor rect
		///
		/// Similar to glScissor. Valid in rendering scopes.
		virtual void SetScissor(const Rect2D& scissor) = 0;

		/// @brief Equivalent to glDrawArraysInstancedBaseInstance or vkCmdDraw
		/// @param vertexCount The number of vertices to draw
		/// @param instanceCount The number of instances to draw
		/// @param firstVertex The index of the first vertex to draw
		/// @param firstInstance The instance ID of the first instance to draw
		///
		/// Valid in rendering scopes.
		virtual void Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) = 0;

		/// @brief Equivalent to glDrawElementsInstancedBaseVertexBaseInstance or vkCmdDrawIndexed
		/// @param indexCount The number of vertices to draw
		/// @param instanceCount The number of instances to draw
		/// @param firstIndex The base index within the index buffer
		/// @param vertexOffset The value added to the vertex index before indexing into the vertex buffer
		/// @param firstInstance The instance ID of the first instance to draw
		///
		/// Valid in rendering scopes.
		virtual void DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) = 0;

		/// @brief Equivalent to glMultiDrawArraysIndirect or vkCmdDrawDrawIndirect
		/// @param commandBuffer The buffer containing draw parameters
		/// @param commandBufferOffset The byte offset into commandBuffer where parameters begin
		/// @param drawCount The number of draws to execute
		/// @param stride The byte stride between successive sets of draw parameters
		///
		/// Valid in rendering scopes.
		virtual void DrawIndirect(const Ref<IBuffer>& commandBuffer, uint64_t commandBufferOffset, uint32_t drawCount, uint32_t stride) = 0;

		/// @brief Equivalent to glMultiDrawArraysIndirectCount or vkCmdDrawIndirectCount
		/// @param commandBuffer The buffer containing draw parameters
		/// @param commandBufferOffset The byte offset into commandBuffer where parameters begin
		/// @param countBuffer The buffer containing the draw count
		/// @param countBufferOffset The byte offset into countBuffer where the draw count begins
		/// @param maxDrawCount The maximum number of draws that will be executed
		/// @param stride The byte stride between successive sets of draw parameters
		///
		/// Valid in rendering scopes.
		virtual void DrawIndirectCount(const Ref<IBuffer>& commandBuffer, uint64_t commandBufferOffset, const Ref<IBuffer>& countBuffer,
									   uint64_t countBufferOffset, uint32_t maxDrawCount, uint32_t stride) = 0;

		/// @brief Equivalent to glMultiDrawElementsIndirect or vkCmdDrawIndexedIndirect
		/// @param commandBuffer The buffer containing draw parameters
		/// @param commandBufferOffset The byte offset into commandBuffer where parameters begin
		/// @param drawCount The number of draws to execute
		/// @param stride The byte stride between successive sets of draw parameters
		///
		/// Valid in rendering scopes.
		virtual void DrawIndexedIndirect(const Ref<IBuffer>& commandBuffer, uint64_t commandBufferOffset, uint32_t drawCount, uint32_t stride) = 0;

		/// @brief Equivalent to glMultiDrawElementsIndirectCount or vkCmdDrawIndexedIndirectCount
		/// @param commandBuffer The buffer containing draw parameters
		/// @param commandBufferOffset The byte offset into commandBuffer where parameters begin
		/// @param countBuffer The buffer containing the draw count
		/// @param countBufferOffset The byte offset into countBuffer where the draw count begins
		/// @param maxDrawCount The maximum number of draws that will be executed
		/// @param stride The byte stride between successive sets of draw parameters
		///
		/// Valid in rendering scopes.
		virtual void DrawIndexedIndirectCount(const Ref<IBuffer>& commandBuffer, uint64_t commandBufferOffset, const Ref<IBuffer>& countBuffer,
											  uint64_t countBufferOffset, uint32_t maxDrawCount, uint32_t stride) = 0;

		/// @brief Binds a buffer to a vertex buffer binding point
		///
		/// Similar to glVertexArrayVertexBuffer. Valid in rendering scopes.
		virtual void BindVertexBuffer(uint32_t bindingIndex, const Ref<IBuffer>& buffer, uint64_t offset, uint64_t stride) = 0;

		/// @brief Binds an index buffer
		///
		/// Similar to glVertexArrayElementBuffer. Valid in rendering scopes.
		virtual void BindIndexBuffer(const Ref<IBuffer>& buffer, IndexType indexType) = 0;

		/// @brief Binds a range within a buffer as a uniform buffer
		///
		/// Similar to glBindBufferRange(GL_UNIFORM_BUFFER, ...)
		virtual void BindUniformBuffer(uint32_t index, const Ref<IBuffer>& buffer, uint64_t offset = 0, uint64_t size = WHOLE_BUFFER) = 0;

		/// @brief Binds a range within a buffer as a uniform buffer
		/// @param block The name of the uniform block whose index to bind to
		/// @note Must be called after a pipeline is bound in order to get reflected program info
		virtual void BindUniformBuffer(std::string_view block, const Ref<IBuffer>& buffer, uint64_t offset = 0, uint64_t size = WHOLE_BUFFER) = 0;

		/// @brief Binds a range within a buffer as a storage buffer
		///
		/// Similar to glBindBufferRange(GL_SHADER_STORAGE_BUFFER, ...)
		virtual void BindStorageBuffer(uint32_t index, const Ref<IBuffer>& buffer, uint64_t offset = 0, uint64_t size = WHOLE_BUFFER) = 0;

		/// @brief Binds a range within a buffer as a storage buffer
		/// @param block The name of the storage block whose index to bind to
		/// @note Must be called after a pipeline is bound in order to get reflected program info
		virtual void BindStorageBuffer(std::string_view block, const Ref<IBuffer>& buffer, uint64_t offset = 0, uint64_t size = WHOLE_BUFFER) = 0;

		//		/// @brief Binds a texture and a sampler to a texture unit
		//		///
		//		/// Similar to glBindTextureUnit + glBindSampler
		//		void BindSampledImage(uint32_t index, const Ref<ITexture>& texture, const Sampler& sampler) = 0;
		//
		//		/// @brief Binds a texture and a sampler to a texture unit
		//		/// @param uniform The name of the uniform whose index to bind to
		//		/// @note Must be called after a pipeline is bound in order to get reflected program info
		//		void BindSampledImage(std::string_view uniform, const Ref<ITexture>& texture, const Sampler& sampler) = 0;

		/// @brief Binds a texture to an image unit
		///
		/// Similar to glBindImageTexture{s}
		virtual void BindImage(uint32_t index, const Ref<ITexture>& texture, uint32_t level) = 0;

		/// @brief Binds a texture to an image unit
		/// @param uniform The name of the uniform whose index to bind to
		/// @note Must be called after a pipeline is bound in order to get reflected program info
		virtual void BindImage(std::string_view uniform, const Ref<ITexture>& texture, uint32_t level) = 0;

		/// @brief Invokes a compute shader
		/// @param groupCountX The number of local workgroups to dispatch in the X dimension
		/// @param groupCountY The number of local workgroups to dispatch in the Y dimension
		/// @param groupCountZ The number of local workgroups to dispatch in the Z dimension
		///
		/// Valid in compute scopes.
		virtual void Dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) = 0;

		/// @brief Invokes a compute shader
		/// @param groupCount The number of local workgroups to dispatch
		///
		/// Valid in compute scopes.
		virtual void Dispatch(Extent3D groupCount) = 0;

		/// @brief Invokes a compute shader a specified number of times
		/// @param invocationCountX The minimum number of invocations in the X dimension
		/// @param invocationCountY The minimum number of invocations in the Y dimension
		/// @param invocationCountZ The minimum number of invocations in the Z dimension
		///
		/// Automatically computes the number of workgroups to invoke based on the formula
		/// groupCount = (invocationCount + workgroupSize - 1) / workgroupSize.
		/// Valid in compute scopes.
		virtual void DispatchInvocations(uint32_t invocationCountX, uint32_t invocationCountY, uint32_t invocationCountZ) = 0;

		/// @brief Invokes a compute shader a specified number of times
		/// @param invocationCount The minimum number of invocations
		///
		/// Automatically computes the number of workgroups to invoke based on the formula
		/// groupCount = (invocationCount + workgroupSize - 1) / workgroupSize.
		/// Valid in compute scopes.
		virtual void DispatchInvocations(Extent3D invocationCount) = 0;

		/// @brief Invokes a compute shader with the group count provided by a buffer
		/// @param commandBuffer The buffer containing dispatch parameters
		/// @param commandBufferOffset The byte offset into commandBuffer where the parameters begin
		///
		/// Valid in compute scopes.
		virtual void DispatchIndirect(const Ref<IBuffer>& commandBuffer, uint64_t commandBufferOffset) = 0;
	};
} // namespace oe::RHI