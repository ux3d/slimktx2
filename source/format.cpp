// Copyright (c) 2019 UX3D GmbH. All rights reserved.

#include "format.h"
#include <cmath>

uint32_t ux3d::slimktx2::getTypeSize(ux3d::slimktx2::Format _vkFormat)
{
	if ((_vkFormat == Format::UNDEFINED) || (_vkFormat == Format::R4G4_UNORM_PACK8) ||
		(_vkFormat >= Format::R8_UNORM && _vkFormat <= Format::B8G8R8A8_SRGB))
	{
		return 1u;
	}
	else if ((_vkFormat >= Format::R16_UNORM && _vkFormat <= Format::R16G16B16A16_SFLOAT) ||
		(_vkFormat >= Format::R4G4B4A4_UNORM_PACK16 && _vkFormat <= Format::A1R5G5B5_UNORM_PACK16))
	{
		return 2u;
	}
	else if ((_vkFormat >= Format::R32_UINT && _vkFormat <= Format::R32G32B32A32_SFLOAT) || 
		(_vkFormat >= Format::A2R10G10B10_UNORM_PACK32 && _vkFormat <= Format::B10G11R11_UFLOAT_PACK32))
	{
		return 4u;
	}
	else if (_vkFormat >= Format::R64_UINT && _vkFormat <= Format::R64G64B64A64_SFLOAT)
	{
		return 8u;
	}
	else if (_vkFormat >= Format::BC1_RGB_UNORM_BLOCK && _vkFormat <= Format::PVRTC2_4BPP_SRGB_BLOCK_IMG)
	{
		return 1u; // "For formats whose Vulkan names have the suffix _BLOCK it must equal 1"
	}

	// TODO: Implement depth/stencil formats

	return 0u; // invalid
}

uint32_t ux3d::slimktx2::getFormatSize(ux3d::slimktx2::Format _vkFormat)
{
	switch (_vkFormat)
	{
		// 1 BYTE
	case Format::R4G4_UNORM_PACK8:

	case Format::R8_UNORM:
	case Format::R8_SNORM:
	case Format::R8_UINT:
	case Format::R8_SINT:
	case Format::R8_SRGB:
		return 1u;

		// 2 BYTE
	case Format::R4G4B4A4_UNORM_PACK16:
	case Format::B4G4R4A4_UNORM_PACK16:
	case Format::R5G6B5_UNORM_PACK16:
	case Format::B5G6R5_UNORM_PACK16:
	case Format::R5G5B5A1_UNORM_PACK16:
	case Format::B5G5R5A1_UNORM_PACK16:
	case Format::A1R5G5B5_UNORM_PACK16:

	case Format::R8G8_UNORM:
	case Format::R8G8_SNORM:
	case Format::R8G8_UINT:
	case Format::R8G8_SINT:
	case Format::R8G8_SRGB:

	case Format::R16_UNORM:
	case Format::R16_SNORM:
	case Format::R16_UINT:
	case Format::R16_SINT:
	case Format::R16_SFLOAT:
		return 2u;

		// 3 BYTE
	case Format::R8G8B8_UNORM:
	case Format::R8G8B8_SNORM:
	case Format::R8G8B8_UINT:
	case Format::R8G8B8_SINT:
	case Format::R8G8B8_SRGB:

	case Format::B8G8R8_UNORM:
	case Format::B8G8R8_SNORM:
	case Format::B8G8R8_UINT:
	case Format::B8G8R8_SINT:
	case Format::B8G8R8_SRGB:
		return 3u;

		// 4 BYTE
	case Format::A2R10G10B10_UNORM_PACK32:
	case Format::A2R10G10B10_SNORM_PACK32:

	case Format::A2R10G10B10_UINT_PACK32:
	case Format::A2R10G10B10_SINT_PACK32:
	case Format::A2B10G10R10_UNORM_PACK32:
	case Format::A2B10G10R10_SNORM_PACK32:

	case Format::A2B10G10R10_UINT_PACK32:
	case Format::A2B10G10R10_SINT_PACK32:

	case Format::B10G11R11_UFLOAT_PACK32:

	case Format::R8G8B8A8_UNORM:
	case Format::R8G8B8A8_SNORM:
	case Format::R8G8B8A8_UINT:
	case Format::R8G8B8A8_SINT:
	case Format::R8G8B8A8_SRGB:

	case Format::B8G8R8A8_UNORM:
	case Format::B8G8R8A8_SNORM:
	case Format::B8G8R8A8_UINT:
	case Format::B8G8R8A8_SINT:
	case Format::B8G8R8A8_SRGB:

	case Format::R16G16_UNORM:
	case Format::R16G16_SNORM:
	case Format::R16G16_UINT:
	case Format::R16G16_SINT:
	case Format::R16G16_SFLOAT:

	case Format::R32_UINT:
	case Format::R32_SINT:
	case Format::R32_SFLOAT:
		return 4u;

		// 6 BYTE
	case Format::R16G16B16_UNORM:
	case Format::R16G16B16_SNORM:
	case Format::R16G16B16_UINT:
	case Format::R16G16B16_SINT:
	case Format::R16G16B16_SFLOAT:
		return 6u;

		// 8 BYTE
	case Format::R16G16B16A16_UNORM:
	case Format::R16G16B16A16_SNORM:
	case Format::R16G16B16A16_UINT:
	case Format::R16G16B16A16_SINT:
	case Format::R16G16B16A16_SFLOAT:

	case Format::R32G32_UINT:
	case Format::R32G32_SINT:
	case Format::R32G32_SFLOAT:

	case Format::R64_UINT:
	case Format::R64_SINT:
	case Format::R64_SFLOAT:
		return 8u;

		// 12 BYTE
	case Format::R32G32B32_UINT:
	case Format::R32G32B32_SINT:
	case Format::R32G32B32_SFLOAT:
		return 12u;

		// 16 BYTE
	case Format::R32G32B32A32_UINT:
	case Format::R32G32B32A32_SINT:
	case Format::R32G32B32A32_SFLOAT:

	case Format::R64G64_UINT:
	case Format::R64G64_SINT:
	case Format::R64G64_SFLOAT:
		return 16u;

		// 24 BYTE
	case Format::R64G64B64_UINT:
	case Format::R64G64B64_SINT:
	case Format::R64G64B64_SFLOAT:
		return 24u;

		// 32 BYTE
	case Format::R64G64B64A64_UINT:
	case Format::R64G64B64A64_SINT:
	case Format::R64G64B64A64_SFLOAT:
		return 32u;

		// TODO: BLOCK FORMATS!

	default:
		return 0u; // invalid
	}
}

bool ux3d::slimktx2::getBlockSize(Format _vkFormat, uint32_t& _outWdith, uint32_t& _outHeight)
{
	_outWdith = _outHeight = 1u;
	switch (_vkFormat)
	{
	case Format::ETC2_R8G8B8_SRGB_BLOCK:
	case Format::ETC2_R8G8B8_UNORM_BLOCK:
	case Format::ETC2_R8G8B8A8_SRGB_BLOCK:
	case Format::ETC2_R8G8B8A8_UNORM_BLOCK:
	case Format::BC1_RGB_SRGB_BLOCK:
	case Format::BC1_RGB_UNORM_BLOCK:
	case Format::BC3_SRGB_BLOCK:
	case Format::BC3_UNORM_BLOCK:
	case Format::BC4_UNORM_BLOCK:
	case Format::BC5_UNORM_BLOCK:
	case Format::BC7_SRGB_BLOCK:
	case Format::BC7_UNORM_BLOCK:
	case Format::PVRTC1_4BPP_SRGB_BLOCK_IMG:
	case Format::PVRTC1_4BPP_UNORM_BLOCK_IMG:
	case Format::ASTC_4x4_SRGB_BLOCK:
	case Format::ASTC_4x4_UNORM_BLOCK:
	case Format::PVRTC2_4BPP_SRGB_BLOCK_IMG:
	case Format::PVRTC2_4BPP_UNORM_BLOCK_IMG:
	case Format::EAC_R11_UNORM_BLOCK:
	case Format::EAC_R11G11_UNORM_BLOCK:
		_outWdith = 4u;
		_outHeight = 4u;
		break;
		
	default:
		return false; // not block compressed
	}

	return true;
}

uint32_t ux3d::slimktx2::getChannelCount(ux3d::slimktx2::Format _vkFormat)
{
	switch (_vkFormat)
	{
	case Format::R8_UNORM:
	case Format::R8_SNORM:
	case Format::R8_UINT:
	case Format::R8_SINT:
	case Format::R8_SRGB:

	case Format::R16_UNORM:
	case Format::R16_SNORM:
	case Format::R16_UINT:
	case Format::R16_SINT:
	case Format::R16_SFLOAT:

	case Format::R32_UINT:
	case Format::R32_SINT:
	case Format::R32_SFLOAT:

	case Format::R64_UINT:
	case Format::R64_SINT:
	case Format::R64_SFLOAT:
		return 1u;

	case Format::R4G4_UNORM_PACK8:

	case Format::R8G8_UNORM:
	case Format::R8G8_SNORM:
	case Format::R8G8_UINT:
	case Format::R8G8_SINT:
	case Format::R8G8_SRGB:

	case Format::R16G16_UNORM:
	case Format::R16G16_SNORM:
	case Format::R16G16_UINT:
	case Format::R16G16_SINT:
	case Format::R16G16_SFLOAT:

	case Format::R32G32_UINT:
	case Format::R32G32_SINT:
	case Format::R32G32_SFLOAT:

	case Format::R64G64_UINT:
	case Format::R64G64_SINT:
	case Format::R64G64_SFLOAT:
		return 2u;

	case Format::R5G6B5_UNORM_PACK16:
	case Format::B5G6R5_UNORM_PACK16:
	case Format::B10G11R11_UFLOAT_PACK32:

	case Format::R8G8B8_UNORM:
	case Format::R8G8B8_SNORM:
	case Format::R8G8B8_UINT:
	case Format::R8G8B8_SINT:
	case Format::R8G8B8_SRGB:
	case Format::B8G8R8_UNORM:
	case Format::B8G8R8_SNORM:
	case Format::B8G8R8_UINT:
	case Format::B8G8R8_SINT:
	case Format::B8G8R8_SRGB:

	case Format::R16G16B16_UNORM:
	case Format::R16G16B16_SNORM:
	case Format::R16G16B16_UINT:
	case Format::R16G16B16_SINT:
	case Format::R16G16B16_SFLOAT:

	case Format::R32G32B32_UINT:
	case Format::R32G32B32_SINT:
	case Format::R32G32B32_SFLOAT:

	case Format::R64G64B64_UINT:
	case Format::R64G64B64_SINT:
	case Format::R64G64B64_SFLOAT:
		return 3u;

	case Format::R4G4B4A4_UNORM_PACK16:
	case Format::B4G4R4A4_UNORM_PACK16:
	case Format::R5G5B5A1_UNORM_PACK16:
	case Format::B5G5R5A1_UNORM_PACK16:
	case Format::A1R5G5B5_UNORM_PACK16:

	case Format::A2R10G10B10_UNORM_PACK32:
	case Format::A2R10G10B10_SNORM_PACK32:

	case Format::A2R10G10B10_UINT_PACK32:
	case Format::A2R10G10B10_SINT_PACK32:
	case Format::A2B10G10R10_UNORM_PACK32:
	case Format::A2B10G10R10_SNORM_PACK32:

	case Format::A2B10G10R10_UINT_PACK32:
	case Format::A2B10G10R10_SINT_PACK32:

	case Format::R8G8B8A8_UNORM:
	case Format::R8G8B8A8_SNORM:
	case Format::R8G8B8A8_UINT:
	case Format::R8G8B8A8_SINT:
	case Format::R8G8B8A8_SRGB:
	case Format::B8G8R8A8_UNORM:
	case Format::B8G8R8A8_SNORM:
	case Format::B8G8R8A8_UINT:
	case Format::B8G8R8A8_SINT:
	case Format::B8G8R8A8_SRGB:

	case Format::R16G16B16A16_UNORM:
	case Format::R16G16B16A16_SNORM:
	case Format::R16G16B16A16_UINT:
	case Format::R16G16B16A16_SINT:
	case Format::R16G16B16A16_SFLOAT:

	case Format::R32G32B32A32_UINT:
	case Format::R32G32B32A32_SINT:
	case Format::R32G32B32A32_SFLOAT:

	case Format::R64G64B64A64_UINT:
	case Format::R64G64B64A64_SINT:
	case Format::R64G64B64A64_SFLOAT:
		return 4u;

	default:
		return 0u; // invalid
	}
}

uint32_t ux3d::slimktx2::getChannelSize(ux3d::slimktx2::Format _vkFormat, uint32_t _channelIndex)
{
	if (_channelIndex >= 4u)
	{
		return 0u;
	}

	switch (_vkFormat)
	{
		// 8 bits; does not depend on channel.
	case Format::R8_UNORM:
	case Format::R8_SNORM:
	case Format::R8_UINT:
	case Format::R8_SINT:
	case Format::R8_SRGB:

	case Format::R8G8_UNORM:
	case Format::R8G8_SNORM:
	case Format::R8G8_UINT:
	case Format::R8G8_SINT:
	case Format::R8G8_SRGB:

	case Format::R8G8B8_UNORM:
	case Format::R8G8B8_SNORM:
	case Format::R8G8B8_UINT:
	case Format::R8G8B8_SINT:
	case Format::R8G8B8_SRGB:

	case Format::B8G8R8_UNORM:
	case Format::B8G8R8_SNORM:
	case Format::B8G8R8_UINT:
	case Format::B8G8R8_SINT:
	case Format::B8G8R8_SRGB:

	case Format::R8G8B8A8_UNORM:
	case Format::R8G8B8A8_SNORM:
	case Format::R8G8B8A8_UINT:
	case Format::R8G8B8A8_SINT:
	case Format::R8G8B8A8_SRGB:

	case Format::B8G8R8A8_UNORM:
	case Format::B8G8R8A8_SNORM:
	case Format::B8G8R8A8_UINT:
	case Format::B8G8R8A8_SINT:
	case Format::B8G8R8A8_SRGB:
		return 8u;

		// 16 bits; does not depend on channel.
	case Format::R16_UNORM:
	case Format::R16_SNORM:
	case Format::R16_UINT:
	case Format::R16_SINT:
	case Format::R16_SFLOAT:

	case Format::R16G16_UNORM:
	case Format::R16G16_SNORM:
	case Format::R16G16_UINT:
	case Format::R16G16_SINT:
	case Format::R16G16_SFLOAT:

	case Format::R16G16B16_UNORM:
	case Format::R16G16B16_SNORM:
	case Format::R16G16B16_UINT:
	case Format::R16G16B16_SINT:
	case Format::R16G16B16_SFLOAT:

	case Format::R16G16B16A16_UNORM:
	case Format::R16G16B16A16_SNORM:
	case Format::R16G16B16A16_UINT:
	case Format::R16G16B16A16_SINT:
	case Format::R16G16B16A16_SFLOAT:
		return 16u;

		// 32 bits; does not depend on channel.
	case Format::R32_UINT:
	case Format::R32_SINT:
	case Format::R32_SFLOAT:

	case Format::R32G32_UINT:
	case Format::R32G32_SINT:
	case Format::R32G32_SFLOAT:

	case Format::R32G32B32_UINT:
	case Format::R32G32B32_SINT:
	case Format::R32G32B32_SFLOAT:

	case Format::R32G32B32A32_UINT:
	case Format::R32G32B32A32_SINT:
	case Format::R32G32B32A32_SFLOAT:
		return 32u;

		// 64 bits; does not depend on channel.
	case Format::R64_UINT:
	case Format::R64_SINT:
	case Format::R64_SFLOAT:

	case Format::R64G64_UINT:
	case Format::R64G64_SINT:
	case Format::R64G64_SFLOAT:

	case Format::R64G64B64_UINT:
	case Format::R64G64B64_SINT:
	case Format::R64G64B64_SFLOAT:

	case Format::R64G64B64A64_UINT:
	case Format::R64G64B64A64_SINT:
	case Format::R64G64B64A64_SFLOAT:
		return 64u;

	case Format::R4G4_UNORM_PACK8:
		if (_channelIndex < 2)
		{
			return 4u;
		}
		else
		{
			return 0u;
		}

	case Format::R5G6B5_UNORM_PACK16:
	case Format::B5G6R5_UNORM_PACK16:
		if (_channelIndex < 3)
		{
			if (_channelIndex == 1)
			{
				return 6u;
			}
			else
			{
				return 5u;
			}
		}
		else
		{
			return 0u;
		}

	case Format::B10G11R11_UFLOAT_PACK32:
		if (_channelIndex < 3)
		{
			if (_channelIndex == 0)
			{
				return 10u;
			}
			else
			{
				return 11u;
			}
		}
		else
		{
			return 0u;
		}

	case Format::R4G4B4A4_UNORM_PACK16:
	case Format::B4G4R4A4_UNORM_PACK16:
		return 4u;

	case Format::R5G5B5A1_UNORM_PACK16:
	case Format::B5G5R5A1_UNORM_PACK16:
		if (_channelIndex < 3)
		{
			return 5u;
		}
		else
		{
			return 1u;
		}

	case Format::A1R5G5B5_UNORM_PACK16:
		if (_channelIndex > 0)
		{
			return 5u;
		}
		else
		{
			return 1u;
		}

	case Format::A2R10G10B10_UNORM_PACK32:
	case Format::A2R10G10B10_SNORM_PACK32:

	case Format::A2R10G10B10_UINT_PACK32:
	case Format::A2R10G10B10_SINT_PACK32:
	case Format::A2B10G10R10_UNORM_PACK32:
	case Format::A2B10G10R10_SNORM_PACK32:

	case Format::A2B10G10R10_UINT_PACK32:
	case Format::A2B10G10R10_SINT_PACK32:
		if (_channelIndex > 0)
		{
			return 10u;
		}
		else
		{
			return 2u;
		}
	default:
		return 0u;
	}
}

int32_t ux3d::slimktx2::getChannelIndex(ux3d::slimktx2::Format _vkFormat, ux3d::slimktx2::Channel _channel)
{
	switch (_vkFormat)
	{
	case Format::R8_UNORM:
	case Format::R8_SNORM:
	case Format::R8_UINT:
	case Format::R8_SINT:
	case Format::R8_SRGB:

	case Format::R16_UNORM:
	case Format::R16_SNORM:
	case Format::R16_UINT:
	case Format::R16_SINT:
	case Format::R16_SFLOAT:

	case Format::R32_UINT:
	case Format::R32_SINT:
	case Format::R32_SFLOAT:

	case Format::R64_UINT:
	case Format::R64_SINT:
	case Format::R64_SFLOAT:
		if (_channel <= Channel_Red)
		{
			return _channel;
		}
		else
		{
			return -1;
		}

	case Format::R4G4_UNORM_PACK8:

	case Format::R8G8_UNORM:
	case Format::R8G8_SNORM:
	case Format::R8G8_UINT:
	case Format::R8G8_SINT:
	case Format::R8G8_SRGB:

	case Format::R16G16_UNORM:
	case Format::R16G16_SNORM:
	case Format::R16G16_UINT:
	case Format::R16G16_SINT:
	case Format::R16G16_SFLOAT:

	case Format::R32G32_UINT:
	case Format::R32G32_SINT:
	case Format::R32G32_SFLOAT:

	case Format::R64G64_UINT:
	case Format::R64G64_SINT:
	case Format::R64G64_SFLOAT:
		if (_channel <= Channel_Green)
		{
			return _channel;
		}
		else
		{
			return -1;
		}

	case Format::R5G6B5_UNORM_PACK16:

	case Format::R8G8B8_UNORM:
	case Format::R8G8B8_SNORM:
	case Format::R8G8B8_UINT:
	case Format::R8G8B8_SINT:
	case Format::R8G8B8_SRGB:

	case Format::R16G16B16_UNORM:
	case Format::R16G16B16_SNORM:
	case Format::R16G16B16_UINT:
	case Format::R16G16B16_SINT:
	case Format::R16G16B16_SFLOAT:

	case Format::R32G32B32_UINT:
	case Format::R32G32B32_SINT:
	case Format::R32G32B32_SFLOAT:

	case Format::R64G64B64_UINT:
	case Format::R64G64B64_SINT:
	case Format::R64G64B64_SFLOAT:
		if (_channel <= Channel_Blue)
		{
			return _channel;
		}
		else
		{
			return -1;
		}

	case Format::R4G4B4A4_UNORM_PACK16:
	case Format::R5G5B5A1_UNORM_PACK16:

	case Format::R8G8B8A8_UNORM:
	case Format::R8G8B8A8_SNORM:
	case Format::R8G8B8A8_UINT:
	case Format::R8G8B8A8_SINT:
	case Format::R8G8B8A8_SRGB:

	case Format::R16G16B16A16_UNORM:
	case Format::R16G16B16A16_SNORM:
	case Format::R16G16B16A16_UINT:
	case Format::R16G16B16A16_SINT:
	case Format::R16G16B16A16_SFLOAT:

	case Format::R32G32B32A32_UINT:
	case Format::R32G32B32A32_SINT:
	case Format::R32G32B32A32_SFLOAT:

	case Format::R64G64B64A64_UINT:
	case Format::R64G64B64A64_SINT:
	case Format::R64G64B64A64_SFLOAT:
		return _channel;

	case Format::B5G6R5_UNORM_PACK16:
	case Format::B10G11R11_UFLOAT_PACK32:

	case Format::B8G8R8_UNORM:
	case Format::B8G8R8_SNORM:
	case Format::B8G8R8_UINT:
	case Format::B8G8R8_SINT:
	case Format::B8G8R8_SRGB:
		if (_channel <= Channel_Blue)
		{
			if (_channel == Channel_Red)
			{
				return 2;
			}
			if (_channel == Channel_Green)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return -1;
		}

	case Format::B4G4R4A4_UNORM_PACK16:
	case Format::B5G5R5A1_UNORM_PACK16:

	case Format::B8G8R8A8_UNORM:
	case Format::B8G8R8A8_SNORM:
	case Format::B8G8R8A8_UINT:
	case Format::B8G8R8A8_SINT:
	case Format::B8G8R8A8_SRGB:
		if (_channel == Channel_Red)
		{
			return 2;
		}
		if (_channel == Channel_Green)
		{
			return 1;
		}
		if (_channel == Channel_Blue)
		{
			return 0;
		}
		else
		{
			return 3;
		}

	case Format::A1R5G5B5_UNORM_PACK16:

	case Format::A2R10G10B10_UNORM_PACK32:
	case Format::A2R10G10B10_SNORM_PACK32:

	case Format::A2R10G10B10_UINT_PACK32:
	case Format::A2R10G10B10_SINT_PACK32:
		if (_channel == Channel_Red)
		{
			return 1;
		}
		if (_channel == Channel_Green)
		{
			return 2;
		}
		if (_channel == Channel_Blue)
		{
			return 3;
		}
		else
		{
			return 0;
		}

	case Format::A2B10G10R10_UNORM_PACK32:
	case Format::A2B10G10R10_SNORM_PACK32:

	case Format::A2B10G10R10_UINT_PACK32:
	case Format::A2B10G10R10_SINT_PACK32:
		if (_channel == Channel_Red)
		{
			return 3;
		}
		if (_channel == Channel_Green)
		{
			return 2;
		}
		if (_channel == Channel_Blue)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	default:
		return -1;
	}
}

bool ux3d::slimktx2::isFloat(ux3d::slimktx2::Format _vkFormat)
{
	switch (_vkFormat)
	{
	case Format::B10G11R11_UFLOAT_PACK32:
	case Format::R16_SFLOAT:
	case Format::R16G16_SFLOAT:
	case Format::R16G16B16_SFLOAT:
	case Format::R16G16B16A16_SFLOAT:
	case Format::R32_SFLOAT:
	case Format::R32G32_SFLOAT:
	case Format::R32G32B32_SFLOAT:
	case Format::R32G32B32A32_SFLOAT:
	case Format::R64_SFLOAT:
	case Format::R64G64_SFLOAT:
	case Format::R64G64B64_SFLOAT:
	case Format::R64G64B64A64_SFLOAT:
		return true;
	default:
		return false;
	}	
}

bool ux3d::slimktx2::isSigned(ux3d::slimktx2::Format _vkFormat)
{
	switch (_vkFormat)
	{
	case Format::A2R10G10B10_SNORM_PACK32:

	case Format::A2R10G10B10_SINT_PACK32:
	case Format::A2B10G10R10_SNORM_PACK32:

	case Format::A2B10G10R10_UINT_PACK32:
	case Format::A2B10G10R10_SINT_PACK32:

	case Format::R8_SNORM:
	case Format::R8_SINT:
	case Format::R8_SRGB:

	case Format::R8G8_SNORM:
	case Format::R8G8_SINT:
	case Format::R8G8_SRGB:

	case Format::R8G8B8_SNORM:
	case Format::R8G8B8_SINT:
	case Format::R8G8B8_SRGB:

	case Format::B8G8R8_SNORM:
	case Format::B8G8R8_SINT:
	case Format::B8G8R8_SRGB:

	case Format::R8G8B8A8_SNORM:
	case Format::R8G8B8A8_SINT:
	case Format::R8G8B8A8_SRGB:

	case Format::B8G8R8A8_SNORM:
	case Format::B8G8R8A8_SINT:
	case Format::B8G8R8A8_SRGB:

	case Format::R16_SNORM:
	case Format::R16_SINT:
	case Format::R16_SFLOAT:

	case Format::R16G16_SNORM:
	case Format::R16G16_SINT:
	case Format::R16G16_SFLOAT:

	case Format::R16G16B16_SNORM:
	case Format::R16G16B16_SINT:
	case Format::R16G16B16_SFLOAT:

	case Format::R16G16B16A16_SNORM:
	case Format::R16G16B16A16_SINT:
	case Format::R16G16B16A16_SFLOAT:

	case Format::R32_SINT:
	case Format::R32_SFLOAT:

	case Format::R32G32_SINT:
	case Format::R32G32_SFLOAT:

	case Format::R32G32B32_SINT:
	case Format::R32G32B32_SFLOAT:

	case Format::R32G32B32A32_SINT:
	case Format::R32G32B32A32_SFLOAT:

	case Format::R64_SINT:
	case Format::R64_SFLOAT:

	case Format::R64G64_SINT:
	case Format::R64G64_SFLOAT:

	case Format::R64G64B64_SINT:
	case Format::R64G64B64_SFLOAT:

	case Format::R64G64B64A64_SINT:
	case Format::R64G64B64A64_SFLOAT:
		return true;
	default:
		return false;
	}	
}

bool ux3d::slimktx2::isNormalized(ux3d::slimktx2::Format _vkFormat)
{
	switch (_vkFormat)
	{
	case Format::R4G4_UNORM_PACK8:
	case Format::R4G4B4A4_UNORM_PACK16:
	case Format::B4G4R4A4_UNORM_PACK16:
	case Format::R5G6B5_UNORM_PACK16:
	case Format::B5G6R5_UNORM_PACK16:
	case Format::R5G5B5A1_UNORM_PACK16:
	case Format::B5G5R5A1_UNORM_PACK16:
	case Format::A1R5G5B5_UNORM_PACK16:

	case Format::A2R10G10B10_UNORM_PACK32:
	case Format::A2R10G10B10_SNORM_PACK32:

	case Format::A2B10G10R10_UNORM_PACK32:
	case Format::A2B10G10R10_SNORM_PACK32:

	case Format::R8_UNORM:
	case Format::R8_SNORM:

	case Format::R8G8_UNORM:
	case Format::R8G8_SNORM:

	case Format::R8G8B8_UNORM:
	case Format::R8G8B8_SNORM:

	case Format::B8G8R8_UNORM:
	case Format::B8G8R8_SNORM:

	case Format::R8G8B8A8_UNORM:
	case Format::R8G8B8A8_SNORM:

	case Format::B8G8R8A8_UNORM:
	case Format::B8G8R8A8_SNORM:

	case Format::R16_UNORM:
	case Format::R16_SNORM:

	case Format::R16G16_UNORM:
	case Format::R16G16_SNORM:

	case Format::R16G16B16_UNORM:
	case Format::R16G16B16_SNORM:

	case Format::R16G16B16A16_UNORM:
	case Format::R16G16B16A16_SNORM:
		return true;
	default:
		return false;
	}
}

bool ux3d::slimktx2::isSrgb(ux3d::slimktx2::Format _vkFormat)
{
	switch (_vkFormat)
	{
	case Format::R8_SRGB:
	case Format::R8G8_SRGB:
	case Format::R8G8B8_SRGB:
	case Format::B8G8R8_SRGB:
	case Format::R8G8B8A8_SRGB:
	case Format::B8G8R8A8_SRGB:
		return true;
	default:
		return false;
	}
}

bool ux3d::slimktx2::isPacked(ux3d::slimktx2::Format _vkFormat)
{
	switch (_vkFormat)
	{
	case Format::R4G4_UNORM_PACK8:
	case Format::R4G4B4A4_UNORM_PACK16:
	case Format::B4G4R4A4_UNORM_PACK16:
	case Format::R5G6B5_UNORM_PACK16:
	case Format::B5G6R5_UNORM_PACK16:
	case Format::R5G5B5A1_UNORM_PACK16:
	case Format::B5G5R5A1_UNORM_PACK16:
	case Format::A1R5G5B5_UNORM_PACK16:

	case Format::A2R10G10B10_UNORM_PACK32:
	case Format::A2R10G10B10_SNORM_PACK32:

	case Format::A2R10G10B10_UINT_PACK32:
	case Format::A2R10G10B10_SINT_PACK32:
	case Format::A2B10G10R10_UNORM_PACK32:
	case Format::A2B10G10R10_SNORM_PACK32:

	case Format::A2B10G10R10_UINT_PACK32:
	case Format::A2B10G10R10_SINT_PACK32:

	case Format::B10G11R11_UFLOAT_PACK32:
		return true;
	default:
		return false;
	}
}

bool ux3d::slimktx2::isCompressed(ux3d::slimktx2::Format _vkFormat)
{
	return _vkFormat >= Format::BC1_RGB_UNORM_BLOCK && _vkFormat <= Format::PVRTC2_4BPP_SRGB_BLOCK_IMG;
}

uint64_t ux3d::slimktx2::getFaceSize(Format _vkFormat, uint32_t _level, uint32_t _width, uint32_t _height, uint32_t _depth)
{
	uint32_t blockWidth = 1u;
	uint32_t blockHeight = 1u;

	const uint32_t bytesPerBlock = getFormatSize(_vkFormat);

	if (getBlockSize(_vkFormat, blockWidth, blockHeight))
	{
		const uint32_t width = _width >> _level;
		const uint32_t height = _height >> _level;

		const uint32_t blockCountX = static_cast<uint32_t>(ceilf(width / static_cast<float>(blockWidth)));
		const uint32_t blockCountY = static_cast<uint32_t>(ceilf(height / static_cast<float>(blockHeight)));

		return bytesPerBlock * blockCountX * blockCountY;
	}
	else
	{
		const uint64_t resolution = getPixelCount(_level, _width, _height, _depth);

		return resolution * bytesPerBlock;
	}
}

uint32_t ux3d::slimktx2::getPixelCount(uint32_t _level, uint32_t _width, uint32_t _height, uint32_t _depth)
{
	uint32_t result = _width >> _level;

	if (_height != 0u)
	{
		result *= _height >> _level;
	}
	if (_depth != 0u)
	{
		result *= _depth >> _level;
	}

	return result;
}

uint32_t ux3d::slimktx2::getPadding(uint64_t _value, uint32_t _alginment)
{
	return (_alginment - (_value % _alginment)) % _alginment;
}

uint32_t ux3d::slimktx2::getLCM(uint32_t _x, uint32_t _y)
{
	uint32_t max = _x > _y ? _x : _y;

	while (!((max % _x == 0u) && (max % _y == 0u)))
	{
		++max;
	}

	return max;
}

uint32_t ux3d::slimktx2::getMipPadding(uint64_t _value, ux3d::slimktx2::Format _vkFormat, bool _superCompression)
{
	if (_superCompression)
	{
		return 0; // 1u byte alignment
	}
	else if (_vkFormat == Format::UNDEFINED)
	{
		return getPadding(_value, 16u); // 16 byte alignment
	}

	const uint32_t texelBlockSize = getFormatSize(_vkFormat);
	const uint32_t lcm = getLCM(texelBlockSize, 4u);
	const uint32_t padding = getPadding(_value, lcm);
	return padding;
}

ux3d::slimktx2::Format ux3d::slimktx2::transcodeToVkFormat(TranscodeFormat _format, bool _sRGB)
{
	switch (_format)
	{
	case TranscodeFormat::ETC1_RGB:
		return _sRGB ? Format::ETC2_R8G8B8_SRGB_BLOCK : Format::ETC2_R8G8B8_UNORM_BLOCK;
	case TranscodeFormat::ETC2_RGBA:
		return _sRGB ? Format::ETC2_R8G8B8A8_SRGB_BLOCK : Format::ETC2_R8G8B8A8_UNORM_BLOCK;
	case TranscodeFormat::BC1_RGB:
		return _sRGB ? Format::BC1_RGB_SRGB_BLOCK : Format::BC1_RGB_UNORM_BLOCK;
	case TranscodeFormat::BC3_RGBA:
		return _sRGB ? Format::BC3_SRGB_BLOCK : Format::BC3_UNORM_BLOCK;
	case TranscodeFormat::BC4_R:
		return Format::BC4_UNORM_BLOCK;
	case TranscodeFormat::BC5_RG:
		return Format::BC5_UNORM_BLOCK;
	case TranscodeFormat::BC7_RGBA:
		return _sRGB ? Format::BC7_SRGB_BLOCK : Format::BC7_UNORM_BLOCK;
	case TranscodeFormat::PVRTC1_4_RGB:
	case TranscodeFormat::PVRTC1_4_RGBA:
		return _sRGB ? Format::PVRTC1_4BPP_SRGB_BLOCK_IMG : Format::PVRTC1_4BPP_UNORM_BLOCK_IMG;
	case TranscodeFormat::ASTC_4x4_RGBA:
		return _sRGB ? Format::ASTC_4x4_SRGB_BLOCK : Format::ASTC_4x4_UNORM_BLOCK;
	case TranscodeFormat::PVRTC2_4_RGB:
	case TranscodeFormat::PVRTC2_4_RGBA:
		return _sRGB ? Format::PVRTC2_4BPP_SRGB_BLOCK_IMG : Format::PVRTC2_4BPP_UNORM_BLOCK_IMG;
	case TranscodeFormat::ETC2_EAC_R11:
		return Format::EAC_R11_UNORM_BLOCK;
	case TranscodeFormat::ETC2_EAC_RG11:
		return Format::EAC_R11G11_UNORM_BLOCK;
	case TranscodeFormat::RGBA32:
		return _sRGB ? Format::R8G8B8A8_SRGB : Format::R8G8B8A8_UNORM;
	case TranscodeFormat::RGB565:
		return Format::R5G6B5_UNORM_PACK16;
	case TranscodeFormat::BGR565:
		return Format::B5G6R5_UNORM_PACK16;
	case TranscodeFormat::RGBA4444:
		return Format::R4G4B4A4_UNORM_PACK16;
	//case TranscodeFormat::ETC:
	//	break;
	//case TranscodeFormat::BC1_OR_3:
	//	break;
	default:
		return Format::UNDEFINED;
	}
}
