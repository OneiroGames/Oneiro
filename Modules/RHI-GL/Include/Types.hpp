//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Fwog/Buffer.h"
#include "Fwog/Context.h"
#include "Fwog/Pipeline.h"
#include "Fwog/Rendering.h"
#include "Fwog/Shader.h"
#include "Fwog/Texture.h"

namespace oe
{
	namespace fwog = Fwog;

	fwog::Format GetFormat(RHI::Format format)
	{
		switch (format)
		{
			case RHI::Format::UNDEFINED: return fwog::Format::UNDEFINED;
			case RHI::Format::R8_UNORM: return fwog::Format::R8_UNORM;
			case RHI::Format::R8_SNORM: return fwog::Format::R8_SNORM;
			case RHI::Format::R16_UNORM: return fwog::Format::R16_UNORM;
			case RHI::Format::R16_SNORM: return fwog::Format::R16_SNORM;
			case RHI::Format::R8G8_UNORM: return fwog::Format::R8G8_UNORM;
			case RHI::Format::R8G8_SNORM: return fwog::Format::R8G8_SNORM;
			case RHI::Format::R16G16_UNORM: return fwog::Format::R16G16_UNORM;
			case RHI::Format::R16G16_SNORM: return fwog::Format::R16G16_SNORM;
			case RHI::Format::R3G3B2_UNORM: return fwog::Format::R3G3B2_UNORM;
			case RHI::Format::R4G4B4_UNORM: return fwog::Format::R4G4B4_UNORM;
			case RHI::Format::R5G5B5_UNORM: return fwog::Format::R5G5B5_UNORM;
			case RHI::Format::R8G8B8_UNORM: return fwog::Format::R8G8B8_UNORM;
			case RHI::Format::R8G8B8_SNORM: return fwog::Format::R8G8B8_SNORM;
			case RHI::Format::R10G10B10_UNORM: return fwog::Format::R10G10B10_UNORM;
			case RHI::Format::R12G12B12_UNORM: return fwog::Format::R12G12B12_UNORM;
			case RHI::Format::R16G16B16_SNORM: return fwog::Format::R16G16B16_SNORM;
			case RHI::Format::R2G2B2A2_UNORM: return fwog::Format::R2G2B2A2_UNORM;
			case RHI::Format::R4G4B4A4_UNORM: return fwog::Format::R4G4B4A4_UNORM;
			case RHI::Format::R5G5B5A1_UNORM: return fwog::Format::R5G5B5A1_UNORM;
			case RHI::Format::R8G8B8A8_UNORM: return fwog::Format::R8G8B8A8_UNORM;
			case RHI::Format::R8G8B8A8_SNORM: return fwog::Format::R8G8B8A8_SNORM;
			case RHI::Format::R10G10B10A2_UNORM: return fwog::Format::R10G10B10A2_UNORM;
			case RHI::Format::R10G10B10A2_UINT: return fwog::Format::R10G10B10A2_UINT;
			case RHI::Format::R12G12B12A12_UNORM: return fwog::Format::R12G12B12A12_UNORM;
			case RHI::Format::R16G16B16A16_UNORM: return fwog::Format::R16G16B16A16_UNORM;
			case RHI::Format::R16G16B16A16_SNORM: return fwog::Format::R16G16B16A16_SNORM;
			case RHI::Format::R8G8B8_SRGB: return fwog::Format::R8G8B8_SRGB;
			case RHI::Format::R8G8B8A8_SRGB: return fwog::Format::R8G8B8A8_SRGB;
			case RHI::Format::R16_FLOAT: return fwog::Format::R16_FLOAT;
			case RHI::Format::R16G16_FLOAT: return fwog::Format::R16G16_FLOAT;
			case RHI::Format::R16G16B16_FLOAT: return fwog::Format::R16G16B16_FLOAT;
			case RHI::Format::R16G16B16A16_FLOAT: return fwog::Format::R16G16B16A16_FLOAT;
			case RHI::Format::R32_FLOAT: return fwog::Format::R32_FLOAT;
			case RHI::Format::R32G32_FLOAT: return fwog::Format::R32G32_FLOAT;
			case RHI::Format::R32G32B32_FLOAT: return fwog::Format::R32G32B32_FLOAT;
			case RHI::Format::R32G32B32A32_FLOAT: return fwog::Format::R32G32B32A32_FLOAT;
			case RHI::Format::R11G11B10_FLOAT: return fwog::Format::R11G11B10_FLOAT;
			case RHI::Format::R9G9B9_E5: return fwog::Format::R9G9B9_E5;
			case RHI::Format::R8_SINT: return fwog::Format::R8_SINT;
			case RHI::Format::R8_UINT: return fwog::Format::UNDEFINED;
			case RHI::Format::R16_SINT: return fwog::Format::R8_UINT;
			case RHI::Format::R16_UINT: return fwog::Format::R16_UINT;
			case RHI::Format::R32_SINT: return fwog::Format::R32_SINT;
			case RHI::Format::R32_UINT: return fwog::Format::R32_UINT;
			case RHI::Format::R8G8_SINT: return fwog::Format::R8G8_SINT;
			case RHI::Format::R8G8_UINT: return fwog::Format::R8G8_UINT;
			case RHI::Format::R16G16_SINT: return fwog::Format::R16G16_SINT;
			case RHI::Format::R16G16_UINT: return fwog::Format::R16G16_UINT;
			case RHI::Format::R32G32_SINT: return fwog::Format::R32G32_SINT;
			case RHI::Format::R32G32_UINT: return fwog::Format::R32G32_UINT;
			case RHI::Format::R8G8B8_SINT: return fwog::Format::R8G8B8_SINT;
			case RHI::Format::R8G8B8_UINT: return fwog::Format::R8G8B8_UINT;
			case RHI::Format::R16G16B16_SINT: return fwog::Format::R16G16B16_SINT;
			case RHI::Format::R16G16B16_UINT: return fwog::Format::R16G16B16_UINT;
			case RHI::Format::R32G32B32_SINT: return fwog::Format::R32G32B32_SINT;
			case RHI::Format::R32G32B32_UINT: return fwog::Format::R32G32B32_UINT;
			case RHI::Format::R8G8B8A8_SINT: return fwog::Format::R8G8B8A8_SINT;
			case RHI::Format::R8G8B8A8_UINT: return fwog::Format::R8G8B8A8_UINT;
			case RHI::Format::R16G16B16A16_SINT: return fwog::Format::R16G16B16A16_SINT;
			case RHI::Format::R16G16B16A16_UINT: return fwog::Format::R16G16B16A16_UINT;
			case RHI::Format::R32G32B32A32_SINT: return fwog::Format::R32G32B32A32_SINT;
			case RHI::Format::R32G32B32A32_UINT: return fwog::Format::R32G32B32A32_UINT;
			case RHI::Format::D32_FLOAT: return fwog::Format::D32_FLOAT;
			case RHI::Format::D32_UNORM: return fwog::Format::D32_UNORM;
			case RHI::Format::D24_UNORM: return fwog::Format::D24_UNORM;
			case RHI::Format::D16_UNORM: return fwog::Format::D16_UNORM;
			case RHI::Format::D32_FLOAT_S8_UINT: return fwog::Format::D32_FLOAT_S8_UINT;
			case RHI::Format::D24_UNORM_S8_UINT: return fwog::Format::D24_UNORM_S8_UINT;
			case RHI::Format::BC1_RGB_UNORM: return fwog::Format::BC1_RGB_UNORM;
			case RHI::Format::BC1_RGB_SRGB: return fwog::Format::BC1_RGB_SRGB;
			case RHI::Format::BC1_RGBA_UNORM: return fwog::Format::BC1_RGBA_UNORM;
			case RHI::Format::BC1_RGBA_SRGB: return fwog::Format::BC1_RGBA_SRGB;
			case RHI::Format::BC2_RGBA_UNORM: return fwog::Format::BC2_RGBA_UNORM;
			case RHI::Format::BC2_RGBA_SRGB: return fwog::Format::BC2_RGBA_SRGB;
			case RHI::Format::BC3_RGBA_UNORM: return fwog::Format::BC3_RGBA_UNORM;
			case RHI::Format::BC3_RGBA_SRGB: return fwog::Format::BC3_RGBA_SRGB;
			case RHI::Format::BC4_R_UNORM: return fwog::Format::BC4_R_UNORM;
			case RHI::Format::BC4_R_SNORM: return fwog::Format::BC4_R_SNORM;
			case RHI::Format::BC5_RG_UNORM: return fwog::Format::BC5_RG_UNORM;
			case RHI::Format::BC5_RG_SNORM: return fwog::Format::BC5_RG_SNORM;
			case RHI::Format::BC6H_RGB_UFLOAT: return fwog::Format::BC6H_RGB_UFLOAT;
			case RHI::Format::BC6H_RGB_SFLOAT: return fwog::Format::BC6H_RGB_SFLOAT;
			case RHI::Format::BC7_RGBA_UNORM: return fwog::Format::BC7_RGBA_UNORM;
			case RHI::Format::BC7_RGBA_SRGB: return fwog::Format::BC7_RGBA_SRGB;
		}
		return {};
	}

	fwog::PolygonMode GetPolygonModeTo(RHI::PolygonMode mode)
	{
		switch (mode)
		{
			case RHI::PolygonMode::FILL: return fwog::PolygonMode::FILL;
			case RHI::PolygonMode::LINE: return fwog::PolygonMode::LINE;
			case RHI::PolygonMode::POINT: return fwog::PolygonMode::POINT;
		}
		return {};
	}

	fwog::CullMode GetCullModeTo(RHI::CullMode mode)
	{
		switch (mode)
		{
			case RHI::CullMode::NONE: return fwog::CullMode::NONE;
			case RHI::CullMode::FRONT: return fwog::CullMode::FRONT;
			case RHI::CullMode::BACK: return fwog::CullMode::BACK;
			case RHI::CullMode::FRONT_AND_BACK: return fwog::CullMode::FRONT_AND_BACK;
		}
		return {};
	}

	fwog::FrontFace GetFrontFaceTo(RHI::FrontFace mode)
	{
		switch (mode)
		{
			case RHI::FrontFace::CLOCKWISE: return fwog::FrontFace::CLOCKWISE;
			case RHI::FrontFace::COUNTERCLOCKWISE: return fwog::FrontFace::COUNTERCLOCKWISE;
		}
		return {};
	}

	fwog::CompareOp GetCompareOpTo(RHI::CompareOp mode)
	{
		switch (mode)
		{

			case RHI::CompareOp::NEVER: return fwog::CompareOp::NEVER;
			case RHI::CompareOp::LESS: return fwog::CompareOp::LESS;
			case RHI::CompareOp::EQUAL: return fwog::CompareOp::EQUAL;
			case RHI::CompareOp::LESS_OR_EQUAL: return fwog::CompareOp::LESS_OR_EQUAL;
			case RHI::CompareOp::GREATER: return fwog::CompareOp::GREATER;
			case RHI::CompareOp::NOT_EQUAL: return fwog::CompareOp::NOT_EQUAL;
			case RHI::CompareOp::GREATER_OR_EQUAL: return fwog::CompareOp::GREATER_OR_EQUAL;
			case RHI::CompareOp::ALWAYS: return fwog::CompareOp::ALWAYS;
		}
		return {};
	}

	fwog::StencilOp GetStencilOp(RHI::StencilOp mode)
	{
		switch (mode)
		{

			case RHI::StencilOp::KEEP: return fwog::StencilOp::KEEP;
			case RHI::StencilOp::ZERO: return fwog::StencilOp::ZERO;
			case RHI::StencilOp::REPLACE: return fwog::StencilOp::REPLACE;
			case RHI::StencilOp::INCREMENT_AND_CLAMP: return fwog::StencilOp::INCREMENT_AND_CLAMP;
			case RHI::StencilOp::DECREMENT_AND_CLAMP: return fwog::StencilOp::DECREMENT_AND_CLAMP;
			case RHI::StencilOp::INVERT: return fwog::StencilOp::INVERT;
			case RHI::StencilOp::INCREMENT_AND_WRAP: return fwog::StencilOp::INCREMENT_AND_WRAP;
			case RHI::StencilOp::DECREMENT_AND_WRAP: return fwog::StencilOp::DECREMENT_AND_WRAP;
		}
		return {};
	}

	fwog::ClipDepthRange GetClipDepthRange(RHI::ClipDepthRange mode)
	{
		switch (mode)
		{

			case RHI::ClipDepthRange::NEGATIVE_ONE_TO_ONE: return fwog::ClipDepthRange::NEGATIVE_ONE_TO_ONE;
			case RHI::ClipDepthRange::ZERO_TO_ONE: return fwog::ClipDepthRange::ZERO_TO_ONE;
		}
		return {};
	}

	fwog::AttachmentLoadOp GetAttachmentLoadOp(RHI::AttachmentLoadOp mode)
	{
		switch (mode)
		{
			case RHI::AttachmentLoadOp::LOAD: return fwog::AttachmentLoadOp::LOAD;
			case RHI::AttachmentLoadOp::CLEAR: return fwog::AttachmentLoadOp::CLEAR;
			case RHI::AttachmentLoadOp::DONT_CARE: return fwog::AttachmentLoadOp::DONT_CARE;
		}
		return {};
	}

	fwog::ImageType GetImageType(RHI::ImageType mode)
	{
		switch (mode)
		{
			case RHI::ImageType::TEX_1D: return fwog::ImageType::TEX_1D;
			case RHI::ImageType::TEX_2D: return fwog::ImageType::TEX_2D;
			case RHI::ImageType::TEX_3D: return fwog::ImageType::TEX_3D;
			case RHI::ImageType::TEX_1D_ARRAY: return fwog::ImageType::TEX_1D_ARRAY;
			case RHI::ImageType::TEX_2D_ARRAY: return fwog::ImageType::TEX_2D_ARRAY;
			case RHI::ImageType::TEX_CUBEMAP: return fwog::ImageType::TEX_CUBEMAP;
			case RHI::ImageType::TEX_CUBEMAP_ARRAY: return fwog::ImageType::TEX_CUBEMAP_ARRAY;
			case RHI::ImageType::TEX_2D_MULTISAMPLE: return fwog::ImageType::TEX_2D_MULTISAMPLE;
			case RHI::ImageType::TEX_2D_MULTISAMPLE_ARRAY: return fwog::ImageType::TEX_2D_MULTISAMPLE_ARRAY;
		}
		return {};
	}

	fwog::SampleCount GetSampleCount(RHI::SampleCount mode)
	{
		switch (mode)
		{
			case RHI::SampleCount::SAMPLES_1: return fwog::SampleCount::SAMPLES_1;
			case RHI::SampleCount::SAMPLES_2: return fwog::SampleCount::SAMPLES_2;
			case RHI::SampleCount::SAMPLES_4: return fwog::SampleCount::SAMPLES_4;
			case RHI::SampleCount::SAMPLES_8: return fwog::SampleCount::SAMPLES_8;
			case RHI::SampleCount::SAMPLES_16: return fwog::SampleCount::SAMPLES_16;
			case RHI::SampleCount::SAMPLES_32: return fwog::SampleCount::SAMPLES_32;
		}
		return {};
	}

	fwog::UploadFormat GetUploadFormat(RHI::UploadFormat mode)
	{
		switch (mode)
		{
			case RHI::UploadFormat::UNDEFINED: return fwog::UploadFormat::UNDEFINED;
			case RHI::UploadFormat::R: return fwog::UploadFormat::R;
			case RHI::UploadFormat::RG: return fwog::UploadFormat::RG;
			case RHI::UploadFormat::RGB: return fwog::UploadFormat::RGB;
			case RHI::UploadFormat::BGR: return fwog::UploadFormat::BGR;
			case RHI::UploadFormat::RGBA: return fwog::UploadFormat::RGBA;
			case RHI::UploadFormat::BGRA: return fwog::UploadFormat::BGRA;
			case RHI::UploadFormat::R_INTEGER: return fwog::UploadFormat::R_INTEGER;
			case RHI::UploadFormat::RG_INTEGER: return fwog::UploadFormat::RG_INTEGER;
			case RHI::UploadFormat::RGB_INTEGER: return fwog::UploadFormat::RGB_INTEGER;
			case RHI::UploadFormat::BGR_INTEGER: return fwog::UploadFormat::BGR_INTEGER;
			case RHI::UploadFormat::RGBA_INTEGER: return fwog::UploadFormat::RGBA_INTEGER;
			case RHI::UploadFormat::BGRA_INTEGER: return fwog::UploadFormat::BGRA_INTEGER;
			case RHI::UploadFormat::DEPTH_COMPONENT: return fwog::UploadFormat::DEPTH_COMPONENT;
			case RHI::UploadFormat::STENCIL_INDEX: return fwog::UploadFormat::STENCIL_INDEX;
			case RHI::UploadFormat::DEPTH_STENCIL: return fwog::UploadFormat::DEPTH_STENCIL;
			case RHI::UploadFormat::INFER_FORMAT: return fwog::UploadFormat::INFER_FORMAT;
		}
		return {};
	}

	fwog::UploadType GetUploadType(RHI::UploadType mode)
	{
		switch (mode)
		{
			case RHI::UploadType::UNDEFINED: return fwog::UploadType::UNDEFINED;
			case RHI::UploadType::UBYTE: return fwog::UploadType::UBYTE;
			case RHI::UploadType::SBYTE: return fwog::UploadType::SBYTE;
			case RHI::UploadType::USHORT: return fwog::UploadType::USHORT;
			case RHI::UploadType::SSHORT: return fwog::UploadType::SSHORT;
			case RHI::UploadType::UINT: return fwog::UploadType::UINT;
			case RHI::UploadType::SINT: return fwog::UploadType::SINT;
			case RHI::UploadType::FLOAT: return fwog::UploadType::FLOAT;
			case RHI::UploadType::UBYTE_3_3_2: return fwog::UploadType::UBYTE_3_3_2;
			case RHI::UploadType::UBYTE_2_3_3_REV: return fwog::UploadType::UBYTE_2_3_3_REV;
			case RHI::UploadType::USHORT_5_6_5: return fwog::UploadType::USHORT_5_6_5;
			case RHI::UploadType::USHORT_5_6_5_REV: return fwog::UploadType::USHORT_5_6_5_REV;
			case RHI::UploadType::USHORT_4_4_4_4: return fwog::UploadType::USHORT_4_4_4_4;
			case RHI::UploadType::USHORT_4_4_4_4_REV: return fwog::UploadType::USHORT_4_4_4_4_REV;
			case RHI::UploadType::USHORT_5_5_5_1: return fwog::UploadType::USHORT_5_5_5_1;
			case RHI::UploadType::USHORT_1_5_5_5_REV: return fwog::UploadType::USHORT_1_5_5_5_REV;
			case RHI::UploadType::UINT_8_8_8_8: return fwog::UploadType::UINT_8_8_8_8;
			case RHI::UploadType::UINT_8_8_8_8_REV: return fwog::UploadType::UINT_8_8_8_8_REV;
			case RHI::UploadType::UINT_10_10_10_2: return fwog::UploadType::UINT_10_10_10_2;
			case RHI::UploadType::UINT_2_10_10_10_REV: return fwog::UploadType::UINT_2_10_10_10_REV;
			case RHI::UploadType::INFER_TYPE: return fwog::UploadType::INFER_TYPE;
		}
		return {};
	}
} // namespace oe