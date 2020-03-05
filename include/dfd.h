// Copyright (c) 2019 UX3D GmbH. All rights reserved.

#pragma once

#include <stdint.h>

namespace ux3d
{
	namespace slimktx2
	{
		enum class VendorId
		{
			Khronos = 0U,
			Max = 0x1FFFFU
		};

		enum class DescriptorType
		{
			BasicFormat = 0U,
			AdditionalPlanes = 0x6001U,
			AdditionalDimensions = 0x6002U,
			NeededForWriteBit = 0x2000U,
			NeededForReadBit = 0x4000U,
			Max = 0x7FFFU
		};

		enum class VersionNumber
		{
			Version_1_0 = 0U,
			Version_1_1 = 0U,
			Version_1_2 = 1U,
			Version_1_3 = 2U,
			Version_Lastest = Version_1_3,
			Version_Max = 0xFFFFU
		};

		enum class ColorModel
		{
            UNSPECIFIED = 0U,

			RGBSDA = 1U,
			YUVSDA = 2U,
			YIQSDA = 3U,
			LABSDA = 4U,
			CMYKA = 5U,
			XYZW = 6U,
			HSVA_ANG = 7U,
			HSLA_ANG = 8U,
			HSVA_HEX = 9U,
			HSLA_HEX = 10U,
			YCGCOA = 11U,
			YCCBCCRC = 12U,
			ICTCP = 13U,
			CIEXYZ = 14U,
			CIEXYY = 15U,

			DXT1A = 128U,
			BC1A = 128U,
			DXT2 = 129U,
			DXT3 = 129U,
			BC2 = 129U,
			DXT4 = 130U,
			DXT5 = 130U,
			BC3 = 130U,
			BC4 = 131U,
			BC5 = 132U,
			BC6H = 133U,
			BC7 = 134U,

			ETC1 = 160U,

			ETC2 = 161U,
			ASTC = 162U,
			ETC1S = 163U,
			PVRTC = 164U,
			PVRTC2 = 165U,
			MAX = 0xFFU
		};

        enum class ColorChannels
        {
            UNSPECIFIED_0 = 0U,
            UNSPECIFIED_1 = 1U,
            UNSPECIFIED_2 = 2U,
            UNSPECIFIED_3 = 3U,
            UNSPECIFIED_4 = 4U,
            UNSPECIFIED_5 = 5U,
            UNSPECIFIED_6 = 6U,
            UNSPECIFIED_7 = 7U,
            UNSPECIFIED_8 = 8U,
            UNSPECIFIED_9 = 9U,
            UNSPECIFIED_10 = 10U,
            UNSPECIFIED_11 = 11U,
            UNSPECIFIED_12 = 12U,
            UNSPECIFIED_13 = 13U,
            UNSPECIFIED_14 = 14U,
            UNSPECIFIED_15 = 15U,

            RGBSDA_RED = 0U,
            RGBSDA_R = 0U,
            RGBSDA_GREEN = 1U,
            RGBSDA_G = 1U,
            RGBSDA_BLUE = 2U,
            RGBSDA_B = 2U,
            RGBSDA_STENCIL = 13U,
            RGBSDA_S = 13U,
            RGBSDA_DEPTH = 14U,
            RGBSDA_D = 14U,
            RGBSDA_ALPHA = 15U,
            RGBSDA_A = 15U,

            YUVSDA_Y = 0U,
            YUVSDA_CB = 1U,
            YUVSDA_U = 1U,
            YUVSDA_CR = 2U,
            YUVSDA_V = 2U,
            YUVSDA_STENCIL = 13U,
            YUVSDA_S = 13U,
            YUVSDA_DEPTH = 14U,
            YUVSDA_D = 14U,
            YUVSDA_ALPHA = 15U,
            YUVSDA_A = 15U,

            YIQSDA_Y = 0U,
            YIQSDA_I = 1U,
            YIQSDA_Q = 2U,
            YIQSDA_STENCIL = 13U,
            YIQSDA_S = 13U,
            YIQSDA_DEPTH = 14U,
            YIQSDA_D = 14U,
            YIQSDA_ALPHA = 15U,
            YIQSDA_A = 15U,

            LABSDA_L = 0U,
            LABSDA_A = 1U,
            LABSDA_B = 2U,
            LABSDA_STENCIL = 13U,
            LABSDA_S = 13U,
            LABSDA_DEPTH = 14U,
            LABSDA_D = 14U,
            LABSDA_ALPHA = 15U,

            CMYKSDA_CYAN = 0U,
            CMYKSDA_C = 0U,
            CMYKSDA_MAGENTA = 1U,
            CMYKSDA_M = 1U,
            CMYKSDA_YELLOW = 2U,
            CMYKSDA_Y = 2U,
            CMYKSDA_KEY = 3U,
            CMYKSDA_BLACK = 3U,
            CMYKSDA_K = 3U,
            CMYKSDA_ALPHA = 15U,
            CMYKSDA_A = 15U,

            XYZW_X = 0U,
            XYZW_Y = 1U,
            XYZW_Z = 2U,
            XYZW_W = 3U,

            HSVA_ANG_VALUE = 0U,
            HSVA_ANG_V = 0U,
            HSVA_ANG_SATURATION = 1U,
            HSVA_ANG_S = 1U,
            HSVA_ANG_HUE = 2U,
            HSVA_ANG_H = 2U,
            HSVA_ANG_ALPHA = 15U,
            HSVA_ANG_A = 15U,

            HSLA_ANG_LIGHTNESS = 0U,
            HSLA_ANG_L = 0U,
            HSLA_ANG_SATURATION = 1U,
            HSLA_ANG_S = 1U,
            HSLA_ANG_HUE = 2U,
            HSLA_ANG_H = 2U,
            HSLA_ANG_ALPHA = 15U,
            HSLA_ANG_A = 15U,

            HSVA_HEX_VALUE = 0U,
            HSVA_HEX_V = 0U,
            HSVA_HEX_SATURATION = 1U,
            HSVA_HEX_S = 1U,
            HSVA_HEX_HUE = 2U,
            HSVA_HEX_H = 2U,
            HSVA_HEX_ALPHA = 15U,
            HSVA_HEX_A = 15U,

            HSLA_HEX_LIGHTNESS = 0U,
            HSLA_HEX_L = 0U,
            HSLA_HEX_SATURATION = 1U,
            HSLA_HEX_S = 1U,
            HSLA_HEX_HUE = 2U,
            HSLA_HEX_H = 2U,
            HSLA_HEX_ALPHA = 15U,
            HSLA_HEX_A = 15U,

            YCGCOA_Y = 0U,
            YCGCOA_CG = 1U,
            YCGCOA_CO = 2U,
            YCGCOA_ALPHA = 15U,
            YCGCOA_A = 15U,

            CIEXYZ_X = 0U,
            CIEXYZ_Y = 1U,
            CIEXYZ_Z = 2U,

            CIEXYY_X = 0U,
            CIEXYY_YCHROMA = 1U,
            CIEXYY_YLUMA = 2U,

            DXT1A_COLOR = 0U,
            BC1A_COLOR = 0U,
            DXT1A_ALPHAPRESENT = 1U,
            DXT1A_ALPHA = 1U,
            BC1A_ALPHAPRESENT = 1U,
            BC1A_ALPHA = 1U,

            DXT2_COLOR = 0U,
            DXT3_COLOR = 0U,
            BC2_COLOR = 0U,
            DXT2_ALPHA = 15U,
            DXT3_ALPHA = 15U,
            BC2_ALPHA = 15U,

            DXT4_COLOR = 0U,
            DXT5_COLOR = 0U,
            BC3_COLOR = 0U,
            DXT4_ALPHA = 15U,
            DXT5_ALPHA = 15U,
            BC3_ALPHA = 15U,

            BC4_DATA = 0U,

            BC5_RED = 0U,
            BC5_R = 0U,
            BC5_GREEN = 1U,
            BC5_G = 1U,

            BC6H_COLOR = 0U,
            BC6H_DATA = 0U,

            BC7_DATA = 0U,
            BC7_COLOR = 0U,

            ETC1_DATA = 0U,
            ETC1_COLOR = 0U,

            ETC2_RED = 0U,
            ETC2_R = 0U,
            ETC2_GREEN = 1U,
            ETC2_G = 1U,
            ETC2_COLOR = 2U,
            ETC2_ALPHA = 15U,
            ETC2_A = 15U,

            ASTC_DATA = 0U,

            ETC1S_DATA = 0U,
            ETC1S_COLOR = 0U,

            PVRTC_DATA = 0U,
            PVRTC_COLOR = 0U,

            PVRTC2_DATA = 0U,
            PVRTC2_COLOR = 0U,

            COMMON_LUMA = 0U,
            COMMON_L = 0U,
            COMMON_STENCIL = 13U,
            COMMON_S = 13U,
            COMMON_DEPTH = 14U,
            COMMON_D = 14U,
            COMMON_ALPHA = 15U,
            COMMON_A = 15U
        };

        enum class ColorPrimaries
        {
            UNSPECIFIED = 0U,
            BT709 = 1U,
            SRGB = 1U,
            BT601_EBU = 2U,
            BT601_SMPTE = 3U,
            BT2020 = 4U,
            CIEXYZ = 5U,
            ACES = 6U,
            ACESCC = 7U,
            NTSC1953 = 8U,
            PAL525 = 9U,
            DISPLAYP3 = 10U,
            ADOBERGB = 11U,
            MAX = 0xFFU
        };

        enum class TransferFunction
        {
            UNSPECIFIED = 0U,
            LINEAR = 1U,
            SRGB = 2U,
            ITU = 3U,
            SMTPE170M = 3U,
            NTSC = 4U,
            SLOG = 5U,
            SLOG2 = 6U,
            BT1886 = 7U,
            HLG_OETF = 8U,
            HLG_EOTF = 9U,
            PQ_EOTF = 10U,
            PQ_OETF = 11U,
            DCIP3 = 12U,
            PAL_OETF = 13U,
            PAL625_EOTF = 14U,
            ST240 = 15U,
            ACESCC = 16U,
            ACESCCT = 17U,
            ADOBERGB = 18U,
            MAX = 0xFFU
        };

        enum class AlphaFlags
        {
            STRAIGHT = 0U,
            PREMULTIPLIED = 1U
        };

        enum class SampleDataTypeQualifiers
        {
            LINEAR = 1U << 4U,
            EXPONENT = 1U << 5U,
            SIGNED = 1U << 6U,
            FLOAT = 1U << 7U
        };

		struct DataFormatDesc
		{
			uint32_t totalSize{};

			//https://www.khronos.org/registry/DataFormat/specs/1.3/dataformat.1.3.html#DescriptorPrefix
			struct BlockHeader
			{
				uint32_t vendorId : 17;
				uint32_t type : 15;

				uint32_t : 0; // start new word
				uint32_t versionNumber : 16;
				uint32_t blockSize : 16; // = 24 + 16 x #samples

				uint32_t : 0; // start new word
				uint32_t colorModel : 4;
				uint32_t colorPrimaries : 4;
				uint32_t transferFunction : 4;
				uint32_t flags : 4;

				uint32_t : 0; // start new word
				uint32_t texelBlockDimension0 : 4;
				uint32_t texelBlockDimension1 : 4;
				uint32_t texelBlockDimension2 : 4;
				uint32_t texelBlockDimension3 : 4;

				uint64_t : 0; // start new word
				uint64_t bytesPlane0 : 4;
				uint64_t bytesPlane1 : 4;
				uint64_t bytesPlane2 : 4;
				uint64_t bytesPlane3 : 4;
				uint64_t bytesPlane4 : 4;
				uint64_t bytesPlane5 : 4;
				uint64_t bytesPlane6 : 4;
				uint64_t bytesPlane7 : 4;

				BlockHeader();
			};

			struct Sample { uint32_t data[4]; }; // TODO: https://www.khronos.org/registry/DataFormat/specs/1.3/dataformat.1.3.html#_anchor_id_sample_xreflabel_sample_sample_information

			static constexpr uint32_t blockHeaderSize = sizeof(BlockHeader); // 24u
			static constexpr uint32_t sampleSize = sizeof(Sample); // 16u

			struct Block
			{
				uint32_t getSampleCount() const { return (header.blockSize - blockHeaderSize) / sampleSize; }
				void setSampleCount(uint32_t _sampleCount) { header.blockSize = blockHeaderSize + _sampleCount * sampleSize; }

				BlockHeader header{};
				// ...
				// Sample information for the first sample
				// Sample information for the second sample (optional), etc.
				Sample* pSamples = nullptr; // uint32_t[sampleCount]
				Block* pNext = nullptr;
			};

			Block* pBlocks = nullptr; // Linked list

			uint32_t computeSize() const;
		};
	} // !slimktx2
} // !ux3d