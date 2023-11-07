//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/Renderer/IRendererBackend.hpp"

#include "Fwog/Buffer.h"
#include "Fwog/Context.h"
#include "Fwog/Pipeline.h"
#include "Fwog/Rendering.h"
#include "Fwog/Shader.h"
#include "Fwog/Texture.h"

namespace oe
{
	namespace fwog = Fwog;

	fwog::Format FormatToFwog(Renderer::Format format)
	{
		switch (format)
		{
			case Renderer::Format::UNDEFINED: return fwog::Format::UNDEFINED;
			case Renderer::Format::R8_UNORM: return fwog::Format::R8_UNORM;
			case Renderer::Format::R8_SNORM: return fwog::Format::R8_SNORM;
			case Renderer::Format::R16_UNORM: return fwog::Format::R16_UNORM;
			case Renderer::Format::R16_SNORM: return fwog::Format::R16_SNORM;
			case Renderer::Format::R8G8_UNORM: return fwog::Format::R8G8_UNORM;
			case Renderer::Format::R8G8_SNORM: return fwog::Format::R8G8_SNORM;
			case Renderer::Format::R16G16_UNORM: return fwog::Format::R16G16_UNORM;
			case Renderer::Format::R16G16_SNORM: return fwog::Format::R16G16_SNORM;
			case Renderer::Format::R3G3B2_UNORM: return fwog::Format::R3G3B2_UNORM;
			case Renderer::Format::R4G4B4_UNORM: return fwog::Format::R4G4B4_UNORM;
			case Renderer::Format::R5G5B5_UNORM: return fwog::Format::R5G5B5_UNORM;
			case Renderer::Format::R8G8B8_UNORM: return fwog::Format::R8G8B8_UNORM;
			case Renderer::Format::R8G8B8_SNORM: return fwog::Format::R8G8B8_SNORM;
			case Renderer::Format::R10G10B10_UNORM: return fwog::Format::R10G10B10_UNORM;
			case Renderer::Format::R12G12B12_UNORM: return fwog::Format::R12G12B12_UNORM;
			case Renderer::Format::R16G16B16_SNORM: return fwog::Format::R16G16B16_SNORM;
			case Renderer::Format::R2G2B2A2_UNORM: return fwog::Format::R2G2B2A2_UNORM;
			case Renderer::Format::R4G4B4A4_UNORM: return fwog::Format::R4G4B4A4_UNORM;
			case Renderer::Format::R5G5B5A1_UNORM: return fwog::Format::R5G5B5A1_UNORM;
			case Renderer::Format::R8G8B8A8_UNORM: return fwog::Format::R8G8B8A8_UNORM;
			case Renderer::Format::R8G8B8A8_SNORM: return fwog::Format::R8G8B8A8_SNORM;
			case Renderer::Format::R10G10B10A2_UNORM: return fwog::Format::R10G10B10A2_UNORM;
			case Renderer::Format::R10G10B10A2_UINT: return fwog::Format::R10G10B10A2_UINT;
			case Renderer::Format::R12G12B12A12_UNORM: return fwog::Format::R12G12B12A12_UNORM;
			case Renderer::Format::R16G16B16A16_UNORM: return fwog::Format::R16G16B16A16_UNORM;
			case Renderer::Format::R16G16B16A16_SNORM: return fwog::Format::R16G16B16A16_SNORM;
			case Renderer::Format::R8G8B8_SRGB: return fwog::Format::R8G8B8_SRGB;
			case Renderer::Format::R8G8B8A8_SRGB: return fwog::Format::R8G8B8A8_SRGB;
			case Renderer::Format::R16_FLOAT: return fwog::Format::R16_FLOAT;
			case Renderer::Format::R16G16_FLOAT: return fwog::Format::R16G16_FLOAT;
			case Renderer::Format::R16G16B16_FLOAT: return fwog::Format::R16G16B16_FLOAT;
			case Renderer::Format::R16G16B16A16_FLOAT: return fwog::Format::R16G16B16A16_FLOAT;
			case Renderer::Format::R32_FLOAT: return fwog::Format::R32_FLOAT;
			case Renderer::Format::R32G32_FLOAT: return fwog::Format::R32G32_FLOAT;
			case Renderer::Format::R32G32B32_FLOAT: return fwog::Format::R32G32B32_FLOAT;
			case Renderer::Format::R32G32B32A32_FLOAT: return fwog::Format::R32G32B32A32_FLOAT;
			case Renderer::Format::R11G11B10_FLOAT: return fwog::Format::R11G11B10_FLOAT;
			case Renderer::Format::R9G9B9_E5: return fwog::Format::R9G9B9_E5;
			case Renderer::Format::R8_SINT: return fwog::Format::R8_SINT;
			case Renderer::Format::R8_UINT: return fwog::Format::UNDEFINED;
			case Renderer::Format::R16_SINT: return fwog::Format::R8_UINT;
			case Renderer::Format::R16_UINT: return fwog::Format::R16_UINT;
			case Renderer::Format::R32_SINT: return fwog::Format::R32_SINT;
			case Renderer::Format::R32_UINT: return fwog::Format::R32_UINT;
			case Renderer::Format::R8G8_SINT: return fwog::Format::R8G8_SINT;
			case Renderer::Format::R8G8_UINT: return fwog::Format::R8G8_UINT;
			case Renderer::Format::R16G16_SINT: return fwog::Format::R16G16_SINT;
			case Renderer::Format::R16G16_UINT: return fwog::Format::R16G16_UINT;
			case Renderer::Format::R32G32_SINT: return fwog::Format::R32G32_SINT;
			case Renderer::Format::R32G32_UINT: return fwog::Format::R32G32_UINT;
			case Renderer::Format::R8G8B8_SINT: return fwog::Format::R8G8B8_SINT;
			case Renderer::Format::R8G8B8_UINT: return fwog::Format::R8G8B8_UINT;
			case Renderer::Format::R16G16B16_SINT: return fwog::Format::R16G16B16_SINT;
			case Renderer::Format::R16G16B16_UINT: return fwog::Format::R16G16B16_UINT;
			case Renderer::Format::R32G32B32_SINT: return fwog::Format::R32G32B32_SINT;
			case Renderer::Format::R32G32B32_UINT: return fwog::Format::R32G32B32_UINT;
			case Renderer::Format::R8G8B8A8_SINT: return fwog::Format::R8G8B8A8_SINT;
			case Renderer::Format::R8G8B8A8_UINT: return fwog::Format::R8G8B8A8_UINT;
			case Renderer::Format::R16G16B16A16_SINT: return fwog::Format::R16G16B16A16_SINT;
			case Renderer::Format::R16G16B16A16_UINT: return fwog::Format::R16G16B16A16_UINT;
			case Renderer::Format::R32G32B32A32_SINT: return fwog::Format::R32G32B32A32_SINT;
			case Renderer::Format::R32G32B32A32_UINT: return fwog::Format::R32G32B32A32_UINT;
			case Renderer::Format::D32_FLOAT: return fwog::Format::D32_FLOAT;
			case Renderer::Format::D32_UNORM: return fwog::Format::D32_UNORM;
			case Renderer::Format::D24_UNORM: return fwog::Format::D24_UNORM;
			case Renderer::Format::D16_UNORM: return fwog::Format::D16_UNORM;
			case Renderer::Format::D32_FLOAT_S8_UINT: return fwog::Format::D32_FLOAT_S8_UINT;
			case Renderer::Format::D24_UNORM_S8_UINT: return fwog::Format::D24_UNORM_S8_UINT;
			case Renderer::Format::BC1_RGB_UNORM: return fwog::Format::BC1_RGB_UNORM;
			case Renderer::Format::BC1_RGB_SRGB: return fwog::Format::BC1_RGB_SRGB;
			case Renderer::Format::BC1_RGBA_UNORM: return fwog::Format::BC1_RGBA_UNORM;
			case Renderer::Format::BC1_RGBA_SRGB: return fwog::Format::BC1_RGBA_SRGB;
			case Renderer::Format::BC2_RGBA_UNORM: return fwog::Format::BC2_RGBA_UNORM;
			case Renderer::Format::BC2_RGBA_SRGB: return fwog::Format::BC2_RGBA_SRGB;
			case Renderer::Format::BC3_RGBA_UNORM: return fwog::Format::BC3_RGBA_UNORM;
			case Renderer::Format::BC3_RGBA_SRGB: return fwog::Format::BC3_RGBA_SRGB;
			case Renderer::Format::BC4_R_UNORM: return fwog::Format::BC4_R_UNORM;
			case Renderer::Format::BC4_R_SNORM: return fwog::Format::BC4_R_SNORM;
			case Renderer::Format::BC5_RG_UNORM: return fwog::Format::BC5_RG_UNORM;
			case Renderer::Format::BC5_RG_SNORM: return fwog::Format::BC5_RG_SNORM;
			case Renderer::Format::BC6H_RGB_UFLOAT: return fwog::Format::BC6H_RGB_UFLOAT;
			case Renderer::Format::BC6H_RGB_SFLOAT: return fwog::Format::BC6H_RGB_SFLOAT;
			case Renderer::Format::BC7_RGBA_UNORM: return fwog::Format::BC7_RGBA_UNORM;
			case Renderer::Format::BC7_RGBA_SRGB: return fwog::Format::BC7_RGBA_SRGB;
		}
		return {};
	}

	fwog::PolygonMode PolygonModeToFwog(Renderer::PolygonMode mode)
	{
		switch (mode)
		{
			case Renderer::PolygonMode::FILL: return fwog::PolygonMode::FILL;
			case Renderer::PolygonMode::LINE: return fwog::PolygonMode::LINE;
			case Renderer::PolygonMode::POINT: return fwog::PolygonMode::POINT;
		}
		return {};
	}

	fwog::CullMode CullModeToFwog(Renderer::CullMode mode)
	{
		switch (mode)
		{
			case Renderer::CullMode::NONE: return fwog::CullMode::NONE;
			case Renderer::CullMode::FRONT: return fwog::CullMode::FRONT;
			case Renderer::CullMode::BACK: return fwog::CullMode::BACK;
			case Renderer::CullMode::FRONT_AND_BACK: return fwog::CullMode::FRONT_AND_BACK;
		}
		return {};
	}

	fwog::FrontFace FrontFaceToFwog(Renderer::FrontFace mode)
	{
		switch (mode)
		{
			case Renderer::FrontFace::CLOCKWISE: return fwog::FrontFace::CLOCKWISE;
			case Renderer::FrontFace::COUNTERCLOCKWISE: return fwog::FrontFace::COUNTERCLOCKWISE;
		}
		return {};
	}

	fwog::CompareOp CompareOpToFwog(Renderer::CompareOp mode)
	{
		switch (mode)
		{

			case Renderer::CompareOp::NEVER: return fwog::CompareOp::NEVER;
			case Renderer::CompareOp::LESS: return fwog::CompareOp::LESS;
			case Renderer::CompareOp::EQUAL: return fwog::CompareOp::EQUAL;
			case Renderer::CompareOp::LESS_OR_EQUAL: return fwog::CompareOp::LESS_OR_EQUAL;
			case Renderer::CompareOp::GREATER: return fwog::CompareOp::GREATER;
			case Renderer::CompareOp::NOT_EQUAL: return fwog::CompareOp::NOT_EQUAL;
			case Renderer::CompareOp::GREATER_OR_EQUAL: return fwog::CompareOp::GREATER_OR_EQUAL;
			case Renderer::CompareOp::ALWAYS: return fwog::CompareOp::ALWAYS;
		}
		return {};
	}

	fwog::StencilOp StencilOpToFwog(Renderer::StencilOp mode)
	{
		switch (mode)
		{

			case Renderer::StencilOp::KEEP: return fwog::StencilOp::KEEP;
			case Renderer::StencilOp::ZERO: return fwog::StencilOp::ZERO;
			case Renderer::StencilOp::REPLACE: return fwog::StencilOp::REPLACE;
			case Renderer::StencilOp::INCREMENT_AND_CLAMP: return fwog::StencilOp::INCREMENT_AND_CLAMP;
			case Renderer::StencilOp::DECREMENT_AND_CLAMP: return fwog::StencilOp::DECREMENT_AND_CLAMP;
			case Renderer::StencilOp::INVERT: return fwog::StencilOp::INVERT;
			case Renderer::StencilOp::INCREMENT_AND_WRAP: return fwog::StencilOp::INCREMENT_AND_WRAP;
			case Renderer::StencilOp::DECREMENT_AND_WRAP: return fwog::StencilOp::DECREMENT_AND_WRAP;
		}
		return {};
	}

	fwog::ClipDepthRange GetClipDepthRange(Renderer::ClipDepthRange mode)
	{
		switch (mode)
		{

			case Renderer::ClipDepthRange::NEGATIVE_ONE_TO_ONE: return fwog::ClipDepthRange::NEGATIVE_ONE_TO_ONE;
			case Renderer::ClipDepthRange::ZERO_TO_ONE: return fwog::ClipDepthRange::ZERO_TO_ONE;
		}
		return {};
	}

	fwog::AttachmentLoadOp GetAttachmentLoadOp(Renderer::AttachmentLoadOp mode)
	{
		switch (mode)
		{
			case Renderer::AttachmentLoadOp::LOAD: return fwog::AttachmentLoadOp::LOAD;
			case Renderer::AttachmentLoadOp::CLEAR: return fwog::AttachmentLoadOp::CLEAR;
			case Renderer::AttachmentLoadOp::DONT_CARE: return fwog::AttachmentLoadOp::DONT_CARE;
		}
		return {};
	}
} // namespace oe