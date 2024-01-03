//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/RHI/RHI_Defines.hpp"

namespace oe::RHI
{
	/// @brief Encapsulates an immutable OpenGL texture
	class ITexture
	{
	public:
		/// @brief Constructs the texture
		/// @param createInfo Parameters to construct the texture
		/// @param name An optional name for viewing the resource in a graphics debugger
		virtual void Create(const TextureCreateInfo& createInfo, std::string_view name = "") = 0;
		ITexture(ITexture&& old) noexcept = default;
		ITexture& operator=(ITexture&& old) noexcept = default;
		ITexture(const ITexture&) = delete;
		ITexture& operator=(const ITexture&) = delete;
		virtual ~ITexture() = default;

		/// @brief Updates a subresource of the image
		/// @param info The subresource and data to upload
		/// @note info.format must not be a compressed image format
		virtual void UpdateImage(const TextureUpdateInfo& info) = 0;

		/// @brief Updates a subresource of the image
		/// @param info The subresource and data to upload
		/// @note Image must be in a compressed image format
		/// @note info.data must be in a compatible compressed image format
		virtual void UpdateCompressedImage(const CompressedTextureUpdateInfo& info) = 0;

		/// @brief Clears a subresource of the image to a specified value
		/// @param info The subresource and value to clear it with
		virtual void ClearImage(const TextureClearInfo& info) = 0;

		/// @brief Automatically generates LoDs of the image. All mip levels beyond 0 are filled with the generated LoDs
		virtual void GenMipmaps() = 0;

		/// @brief Creates a view of a single mip level of the image
		[[nodiscard]] virtual Ref<ITextureView> CreateSingleMipView(uint32_t level) = 0;

		/// @brief Creates a view of a single array layer of the image
		[[nodiscard]] virtual Ref<ITextureView> CreateSingleLayerView(uint32_t layer) = 0;

		/// @brief Reinterpret the data of this texture
		/// @param newFormat The format to reinterpret the data as
		/// @return A new texture view
		[[nodiscard]] virtual Ref<ITextureView> CreateFormatView(Format newFormat) = 0;

		/// @brief Creates a view of the texture with a new component mapping
		/// @param components The swizzled components
		/// @return A new texture view
		[[nodiscard]] virtual Ref<ITextureView> CreateSwizzleView(ComponentMapping components) = 0;

		[[nodiscard]] const TextureCreateInfo& GetCreateInfo() const noexcept
		{
			return createInfo_;
		}

		[[nodiscard]] Extent3D Extent() const noexcept
		{
			return createInfo_.extent;
		}

	protected:
		ITexture() = default;
		TextureCreateInfo createInfo_{};
	};

	/// @brief Encapsulates an OpenGL texture view
	class ITextureView : public ITexture
	{
	public:
		/// @brief Constructs the texture view with explicit parameters
		/// @param viewInfo Parameters to construct the texture
		/// @param texture A texture of which to construct a view
		/// @param name An optional name for viewing the resource in a graphics debugger
		virtual void Create(const TextureViewCreateInfo& viewInfo, ITexture& texture, std::string_view name = "") = 0;
		virtual void Create(const TextureViewCreateInfo& viewInfo, ITextureView& textureView, std::string_view name = "") = 0;
		virtual void Create(ITexture& texture, std::string_view name = "") = 0;

		ITextureView(ITextureView&& old) noexcept = default;
		ITextureView& operator=(ITextureView&& old) noexcept = default;
		ITextureView(const ITextureView& other) = delete;
		ITextureView& operator=(const ITextureView& other) = delete;
		virtual ~ITextureView() = 0;

		[[nodiscard]] TextureViewCreateInfo GetViewInfo() const noexcept
		{
			return viewInfo_;
		}

	private:
		TextureViewCreateInfo viewInfo_{};
	};

	/// @brief Encapsulates an OpenGL sampler
	class ITextureSampler
	{
	public:
		virtual void Create(const SamplerState& samplerState) = 0;

	private:
		ITextureSampler() = default;
	};
} // namespace oe::RHI