// Copyright (c) 2020 UX3D GmbH. All rights reserved.

#include "dfd.h"

using namespace ux3d::slimktx2;

DataFormatDesc::BlockHeader::BlockHeader() :
	vendorId(VendorId_KHRONOS),
	type(DescriptorType_BASICFORMAT),
	versionNumber(VersionNumber_1_3),
	blockSize(blockHeaderSize), // 2 => no samples
	colorModel(ColorModel_UNSPECIFIED), 
	colorPrimaries(ColorPrimaries_UNSPECIFIED), // unspecified
	transferFunction(TransferFunction_LINEAR), // linear
	flags(AlphaFlags_STRAIGHT), // alpha straight
	texelBlockDimension0(0u),
	texelBlockDimension1(0u),
	texelBlockDimension2(0u),
	texelBlockDimension3(0u),
	bytesPlane0(0u),
	bytesPlane1(0u),
	bytesPlane2(0u),
	bytesPlane3(0u),
	bytesPlane4(0u),
	bytesPlane5(0u),
	bytesPlane6(0u),
	bytesPlane7(0u)
{
}

uint32_t DataFormatDesc::computeSize() const
{
	size_t size = sizeof(uint32_t); // totalSize member

	const Block* pBlock = pBlocks;
	while (pBlock != nullptr)
	{
		size += blockHeaderSize;

		if (pBlock->pSamples != nullptr)
		{
			size += static_cast<uint64_t>(pBlock->getSampleCount())* sampleSize;
		}

		pBlock = pBlock->pNext;
	};

	return static_cast<uint32_t>(size);
}

ux3d::slimktx2::DataFormatDesc::Block* ux3d::slimktx2::DataFormatDesc::getLastBlock() const
{
	Block* pBlock = pBlocks;
	while (pBlock != nullptr)
	{
		if (pBlock->pNext == nullptr)
		{
			break;
		}
		pBlock = pBlock->pNext;
	}

	return pBlock;
}
