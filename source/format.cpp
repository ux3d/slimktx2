// Copyright (c) 2019 UX3D GmbH. All rights reserved.

#include "format.h"

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

	// TODO: Implement depth/stencil formats

	return 0u; // invalid
}

uint32_t ux3d::slimktx2::getPixelSize(ux3d::slimktx2::Format _vkFormat)
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

	default:
		return 0u; // invalid
	}
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
	return false;
}

uint64_t ux3d::slimktx2::getFaceSize(uint32_t _pixelByteSize, uint32_t _level, uint32_t _width, uint32_t _height, uint32_t _depth)
{
	const uint64_t resolution = getPixelCount(_level, _width, _height, _depth);
	return resolution * _pixelByteSize;
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

	const uint32_t texelBlockSize = getPixelSize(_vkFormat);
	const uint32_t lcm = getLCM(texelBlockSize, 4u);
	const uint32_t padding = getPadding(_value, lcm);
	return padding;
}