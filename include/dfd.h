// Copyright (c) 2019 UX3D GmbH. All rights reserved.

#pragma once

#include <stdint.h>

namespace ux3d
{
	namespace slimktx2
	{
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