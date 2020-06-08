// Copyright (c) 2020 UX3D GmbH. All rights reserved.

#pragma once

#include <cstdint>

namespace ux3d
{
	namespace slimktx2
	{
		// http://github.khronos.org/KTX-Specification/#_supercompression_global_data
		// supercompression global data
		struct BasisLZ
		{
			//  References to the offset of this data in a .basis file
			struct Header
			{
				uint16_t endpointCount;
				uint16_t selectorCount;
				uint32_t endpointsByteLength;
				uint32_t selectorsByteLength;
				uint32_t tablesByteLength;
				uint32_t extendedByteLength; // Must be 0 if the data format desciptor colorModel is KHR_DF_MODEL_ETC1S (= 163).
			} header{};

			struct ImageDesc
			{
				enum ImageFlag : uint32_t
				{
					None = 0,
					isIFrame = 0x2
				};

				uint32_t imageFlags;
				uint32_t rgbSliceByteOffset;
				uint32_t rgbSliceByteLength;
				uint32_t alphaSliceByteOffset;
				uint32_t alphaSliceByteLength;
			};

			// for each image in the mip level array
			ImageDesc* pImageDescs = nullptr;

			uint8_t* pEndpoints = nullptr;
			uint8_t* pSelectors = nullptr;
			uint8_t* pTables = nullptr;
			uint8_t* pExtendedData = nullptr;
		};
	}
}