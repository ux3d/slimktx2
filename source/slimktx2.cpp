// Copyright (c) 2020 UX3D GmbH. All rights reserved.

#include "slimktx2.h"
#include <string.h>

using namespace ux3d::slimktx2;

const uint8_t Header::Magic[12] = { 0xAB, 0x4B, 0x54, 0x58, 0x20, 0x32, 0x30, 0xBB, 0x0D, 0x0A, 0x1A, 0x0A };

template <class T>
T max(T x, T y) { return x > y ? x : y; }

template <class T>
T max(T x, T y, T z) { return max(max(x,y),z); }

template <class T>
T min(T x, T y) { return x < y ? x : y; }

template <class T>
T min(T x, T y, T z) { return min(min(x, y), z); }

SlimKTX2::SlimKTX2(const Callbacks& _callbacks) : m_callbacks(_callbacks)
{
}

SlimKTX2::~SlimKTX2()
{
	clear();
}

void ux3d::slimktx2::SlimKTX2::setCallbacks(const Callbacks& _callbacks)
{
	m_callbacks = _callbacks;
}

void SlimKTX2::clear()
{
	// level index
	if (m_pLevels != nullptr)
	{
		free(m_pLevels);
	}
	m_pLevels = nullptr;

	destroyDFD();

	destroyKVD();

	destoryMipLevelArray();
}

uint64_t SlimKTX2::getFaceImageOffset(uint32_t _level, uint32_t _face, uint32_t _layer) const
{
	uint64_t offset = 0u;

	if (m_pLevels == nullptr)
	{
		return 0u;
	}

	const uint32_t pixelSize = getPixelSize(m_header.vkFormat);

	// add largest level
	const uint64_t faceSize = getFaceSize(pixelSize, _level, m_header.pixelWidth, m_header.pixelHeight, m_header.pixelDepth);

	// number of previous layers with either 1 or 6 faces
	uint64_t prevFaces = faceSize * _layer * m_header.faceCount;

	// add the faces within this cube map
	prevFaces += faceSize * _face;

	// we are now at the start of the current level, add faces an layers:
	offset += prevFaces;

	return offset;
}

Result SlimKTX2::parse(IOHandle _file)
{
	clear();

	if (read(_file, &m_header) == false)
	{
		return Result::IOReadFail;
	}

	if (memcmp(m_header.identifier, Header::Magic, sizeof(m_header.identifier)) != 0)
	{
		return Result::InvalidIdentifier;
	}

	if (read(_file, &m_sections) == false)
	{
		return Result::IOReadFail;
	}

	const uint32_t levelCount = getLevelCount();

	m_pLevels = allocateArray<LevelIndex>(levelCount);

	if (read(_file, m_pLevels, levelCount) == false)
	{
		return Result::IOReadFail;
	}

	// dfd is mandatory
	if (m_sections.dfdByteLength < sizeof(uint32_t) || seek(_file, m_sections.dfdByteOffset) == false)
	{
		return Result::IOReadFail;
	}

	if (readDFD(_file) == false)
	{
		return Result::IOReadFail;
	}

	// kvd is mandatory
	if (m_sections.kvdByteLength < (sizeof(uint32_t) + 2u) || seek(_file, m_sections.kvdByteOffset) == false)
	{
		return Result::IOReadFail;
	}

	if (readKVD(_file) == false)
	{
		return Result::IOReadFail;
	}

	// TODO: sgd	

	Result res = allocateMipLevelArray();

	if (res != Result::Success)
	{
		return res;
	}

	for (uint32_t level = levelCount - 1u; level <= levelCount; --level)
	{
		const LevelIndex& lvl = m_pLevels[level];

		// skip to first level
		if (seek(_file, lvl.byteOffset) == false)
		{
			return Result::IOReadFail;
		}

		if (read(_file, m_pMipLevelArray[level], lvl.byteLength) == false)
		{
			return Result::IOReadFail;
		}
	}

	// TODO: copy from _file to m_pContainer using m_pLevel offsets instead of copying the whole container to memory

	return Result::Success;
}

Result SlimKTX2::serialize(IOHandle _file)
{
	if (m_pLevels == nullptr)
	{
		log("LevelIndex not specified\n");
		return Result::LevelIndexNotAllocated;
	}

	if (m_pMipLevelArray == nullptr)
	{
		log("MipLevelArray not specified\n");
		return Result::MipLevelArryNotAllocated;
	}

	if (m_dfd.pBlocks == nullptr)
	{
		log("DFD not specified\n");
		return Result::DataFormatDescNotAllocated;
	}

	if (m_kvd.pKeyValues == nullptr)
	{
		log("KVD not specified\n");
		return Result::KeyValueDataNotAllocated;
	}

	const size_t streamStart = tell(_file);
	auto filePos = [&](IOHandle file) -> size_t { return tell(file) - streamStart; };

	const uint32_t pixelSize = getPixelSize(m_header.vkFormat);
	const uint32_t levelCount = getLevelCount();

	const uint32_t dfdByteLength = m_dfd.computeSize();
	const uint32_t dfdByteOffset = sizeof(Header) + sizeof(SectionIndex) + sizeof(LevelIndex) * m_header.levelCount;
	const uint32_t kvdByteLength = m_kvd.computeSize();
	const uint32_t kvdByteOffset = dfdByteOffset + dfdByteLength;
	const uint64_t sgdByteLength = 0u;
	uint64_t sgdByteOffset = kvdByteOffset + kvdByteLength;

	const uint32_t sdgPadding = getPadding(sgdByteOffset, 8u);

	if (sgdByteLength > 0u)
	{
		sgdByteOffset += sdgPadding;
	}

	uint64_t levelOffset = sgdByteOffset + sgdByteLength;

	for (uint32_t level = levelCount - 1u; level <= levelCount; --level)
	{
		const uint32_t mipPad = getMipPadding(levelOffset, m_header.vkFormat, m_header.supercompressionScheme != 0u);
		levelOffset += mipPad;

		// start with the small level, fill them in reverse
		uint64_t levelSize = getFaceSize(pixelSize, level, m_header.pixelWidth, m_header.pixelHeight, m_header.pixelDepth);
		levelSize *= getFaceCount();
		levelSize *= getLayerCount();

		// absolute levelOffset within the file
		m_pLevels[level].byteOffset = levelOffset;
		m_pLevels[level].byteLength = levelSize;
		m_pLevels[level].uncompressedByteLength = levelSize; // uncompressedByteLength % (faceCount * max(1, layerCount)) == 0

		levelOffset += levelSize;
	}

	write(_file, &m_header);

	m_sections.dfdByteLength = dfdByteLength;
	m_sections.dfdByteOffset = dfdByteOffset;

	m_sections.kvdByteLength = kvdByteLength;
	m_sections.kvdByteOffset = kvdByteOffset;

	//sdg is optional
	m_sections.sgdByteLength = sgdByteLength;
	m_sections.sgdByteOffset = sgdByteLength != 0u ? sgdByteOffset : 0u;

	size_t curPos = filePos(_file);
	log("SectionIndex offset %llu size %llu\n", curPos, sizeof(SectionIndex));
	write(_file, &m_sections);

	curPos = filePos(_file);
	log("LevelIndex offset %llu size %llu\n", curPos, sizeof(LevelIndex) * m_header.levelCount);
	write(_file, m_pLevels, m_header.levelCount);

	curPos = filePos(_file);
	log("DFD offset %llu size %u\n", curPos, dfdByteLength);

	if (dfdByteLength != 0u)
	{
		if (curPos != m_sections.dfdByteOffset)
		{
			return Result::IOWriteFail;
		}

		writeDFD(_file);	
	}

	curPos = filePos(_file);
	log("KVD offset %llu size %u\n", curPos, kvdByteLength);

	if (kvdByteLength != 0u)
	{
		if (curPos != m_sections.kvdByteOffset)
		{
			return Result::IOWriteFail;
		}

		writeKVD(_file);
	}

	// TODO: sgd
	// dfd and kvd are required fields and the validator will emit warnings
	// some parser implementation still work as they also ignore those fields.

	// after kvd, before sdg
	if (m_sections.sgdByteLength > 0u)
	{
		writePadding(_file, sdgPadding);
	}

	for (uint32_t level = levelCount - 1u; level <= levelCount; --level)
	{
		const LevelIndex& lvl = m_pLevels[level];

		curPos = filePos(_file);

		const uint32_t mipPad = getMipPadding(curPos, m_header.vkFormat, m_header.supercompressionScheme != 0u);

		writePadding(_file, mipPad);

		curPos = filePos(_file);

		log("level %u offset %llu length %llu padding %u\n", level, lvl.byteOffset, lvl.byteLength, mipPad);

		// skip to first level
		if (lvl.byteOffset != curPos)
		{
			return Result::IOWriteFail;
		}

		write(_file, m_pMipLevelArray[level], lvl.byteLength);
	}

	curPos = filePos(_file);
	log("Total file size %llu\n", curPos);

	if (levelOffset != curPos)
	{
		return Result::IOWriteFail;
	}

	return Result::Success;
}

uint32_t SlimKTX2::getLevelCount() const
{
	return m_header.levelCount != 0u ? m_header.levelCount : 1u;
}

uint32_t SlimKTX2::getLayerCount() const
{
	return m_header.layerCount != 0u ? m_header.layerCount : 1u;
}

uint32_t SlimKTX2::getFaceCount() const
{
	return m_header.faceCount != 0u ? m_header.faceCount : 1u;
}

const Header& SlimKTX2::getHeader() const
{
	return m_header;
}

const DataFormatDesc& SlimKTX2::getDFD() const
{
	return m_dfd;
}

const KeyValueData& SlimKTX2::getKVD() const
{
	return m_kvd;
}

Result SlimKTX2::specifyFormat(Format _vkFormat, uint32_t _width, uint32_t _height, uint32_t _levelCount, uint32_t _faceCount, uint32_t _depth, uint32_t _layerCount)
{
	if (m_pLevels != nullptr)
	{
		free(m_pLevels);
	}

	memcpy(m_header.identifier, Header::Magic, sizeof(m_header.identifier));
	m_header.vkFormat = _vkFormat;
	m_header.typeSize = getTypeSize(_vkFormat);
	m_header.pixelWidth = _width;
	m_header.pixelHeight = _height == 1u ? 0u : _height;
	m_header.pixelDepth = _depth == 1u ? 0u : _depth;
	m_header.layerCount = _layerCount;
	m_header.faceCount = _faceCount;

	uint32_t maxDim = max(_width, _height, _depth);
	uint32_t maxLevel = 0u;

	// compute maximum number of mip levels for the specified resolution
	while (maxDim > 0u)
	{
		maxDim >>= 1u;
		++maxLevel;
	}

	m_header.levelCount = min(maxLevel, _levelCount);
	m_header.supercompressionScheme = 0u;

	const uint32_t levelCount = getLevelCount();
	m_pLevels = allocateArray<LevelIndex>(levelCount);

	addKeyValue(KeyValueData::KTXwriterKey, KeyValueData::KTXwriterKeyLength, KeyValueData::KTXwriterValue, KeyValueData::KTXwriterValueLength);

	return Result::Success;
}

void ux3d::slimktx2::SlimKTX2::addDFDBlock(const DataFormatDesc::BlockHeader& _header, const DataFormatDesc::Sample* _pSamples, uint32_t _numSamples)
{
	auto pBlock = m_dfd.getLastBlock();

	if (pBlock == nullptr) // first block
	{
		pBlock = allocateArray<DataFormatDesc::Block>();
		m_dfd.pBlocks = pBlock;
	}
	else
	{
		pBlock->pNext = allocateArray<DataFormatDesc::Block>();
		pBlock = pBlock->pNext;
	}

	pBlock->header = _header;
	pBlock->setSampleCount(_numSamples);

	if (_pSamples != nullptr && _numSamples != 0u)
	{
		pBlock->pSamples = allocateArray<DataFormatDesc::Sample>(_numSamples);
		memcpy(pBlock->pSamples, _pSamples, _numSamples * sizeof(DataFormatDesc::Sample));
	}
}

void SlimKTX2::addKeyValue(const void* _key, uint32_t _keyLength, const void* _value, uint32_t _valueLength)
{
	auto pEntry = m_kvd.getLastEntry();

	if (pEntry == nullptr) // first entry
	{
		pEntry = allocateArray<KeyValueData::Entry>();
		m_kvd.pKeyValues = pEntry;
	}
	else
	{
		pEntry->pNext = allocateArray<KeyValueData::Entry>();
		pEntry = pEntry->pNext;
	}

	pEntry->keyAndValueByteLength = _keyLength + _valueLength;
	pEntry->pKeyValue = allocateArray<uint8_t>(pEntry->keyAndValueByteLength);

	memcpy(pEntry->pKeyValue, _key, _keyLength);
	memcpy(pEntry->pKeyValue + _keyLength, _value, _valueLength);
}

Result SlimKTX2::allocateMipLevelArray()
{
	destoryMipLevelArray();

	const uint32_t pixelSize = getPixelSize(m_header.vkFormat);

	const auto levelCount = getLevelCount();

	m_pMipLevelArray = allocateArray<uint8_t*>(levelCount);

	if (m_pMipLevelArray == nullptr)
	{
		return Result::MipLevelArryNotAllocated;
	}

	for (uint32_t l = 0u; l < getLevelCount(); ++l)
	{
		uint64_t levelSize = getFaceSize(pixelSize, l, m_header.pixelWidth, m_header.pixelHeight, m_header.pixelDepth);
		levelSize *= getFaceCount();
		levelSize *= getLayerCount();

		m_pMipLevelArray[l] = allocateArray<uint8_t>(levelSize);
		if (m_pMipLevelArray[l] == nullptr)
		{
			return Result::MipLevelArryNotAllocated;
		}
	}

	return Result::Success;
}

Result SlimKTX2::setImage(const void* _pData, size_t _byteSize, uint32_t _level, uint32_t _face, uint32_t _layer)
{
	Result res = Result::Success;

	if (_level >= m_header.levelCount)
	{
		return Result::InvalidLevelIndex;
	}

	const uint32_t pixelSize = getPixelSize(m_header.vkFormat);
	const uint64_t imageSize = getFaceSize(pixelSize, _level,  m_header.pixelWidth, m_header.pixelHeight, m_header.pixelDepth);

	if (_byteSize != imageSize)
	{
		return Result::InvalidImageSize;
	}

	uint8_t* pDst = nullptr;
	if ((res = getImage(pDst, _level, _face, _layer, _byteSize)) != Result::Success)
	{
		return res;
	}

	memcpy(pDst, _pData, _byteSize);

	return res;
}

Result SlimKTX2::getImage(uint8_t*& _outImageData, uint32_t _level, uint32_t _face, uint32_t _layer, uint64_t _imageSize) const
{
	if (m_pMipLevelArray == nullptr)
	{
		return Result::MipLevelArryNotAllocated;
	}
	if (_level >= m_header.levelCount)
	{
		return Result::InvalidLevelIndex;
	}
	if (_face >= m_header.faceCount)
	{
		return Result::InvalidFaceIndex;
	}
	if (_layer >= getLayerCount())
	{
		return Result::InvalidLayerIndex;
	}

	const uint64_t offset = getFaceImageOffset(_level, _face, _layer);

	if (_imageSize != 0u) 
	{
		// for debugging
		uint64_t levelSize = getFaceSize(getPixelSize(m_header.vkFormat), _level, m_header.pixelWidth, m_header.pixelHeight, m_header.pixelDepth);
		levelSize *= getFaceCount();
		levelSize *= getLayerCount();

		// levelOffset / image out of container bounds
		if (offset + _imageSize > levelSize)
		{
			return Result::InvalidImageSize;
		}
	}

	_outImageData = m_pMipLevelArray[_level] + offset;

	return Result::Success;
}

void* SlimKTX2::allocate(size_t _size)
{
	return m_callbacks.allocate(m_callbacks.userData, _size);
}

void SlimKTX2::free(void* _pData)
{
	m_callbacks.free(m_callbacks.userData, _pData);
}

void ux3d::slimktx2::SlimKTX2::writePadding(IOHandle _file, size_t _byteSize) const
{
	const uint8_t pad = 0u;
	for (uint32_t i = 0; i < _byteSize; ++i)
	{
		write(_file, &pad);
	}
}

size_t SlimKTX2::tell(const IOHandle _file)
{
	return m_callbacks.tell(m_callbacks.userData, _file);
}

bool SlimKTX2::seek(IOHandle _file, size_t _offset)
{
	return m_callbacks.seek(m_callbacks.userData, _file, _offset);
}

void SlimKTX2::log(const char* _pFormat, ...)
{
	if (m_callbacks.log != nullptr)
	{
		va_list args;
		va_start(args, _pFormat);
		m_callbacks.log(m_callbacks.userData, _pFormat, args);
		va_end(args);
	}
}

uint32_t SlimKTX2::getKtxLevel(uint32_t _level) const
{
	return getLevelCount() - _level - 1u;
}

void SlimKTX2::destroyDFD()
{
	auto* pBlock = m_dfd.pBlocks;
	while (pBlock != nullptr)
	{
		auto* pNext = pBlock->pNext;
		pBlock->pNext = nullptr;

		if (pBlock->pSamples != nullptr)
		{
			free(pBlock->pSamples);
			pBlock->pSamples = nullptr;
		}

		free(pBlock);
		pBlock = pNext;
	};
	m_dfd.pBlocks = nullptr;
}

bool SlimKTX2::readDFD(IOHandle _file)
{
	if (read(_file, &m_dfd.totalSize) == false)
	{
		return false;
	}

	destroyDFD();

	uint32_t remainingSize = m_dfd.totalSize;

	auto* pBlock = m_dfd.pBlocks;
	// we still have data to read
	while (remainingSize >= DataFormatDesc::blockHeaderSize)
	{
		auto* pNew = allocateArray<DataFormatDesc::Block>();

		if (pBlock != nullptr)
		{
			pBlock->pNext = pNew;
		}
		else
		{
			m_dfd.pBlocks = pNew;
		}

		if (read(_file, &pNew->header) == false)
		{
			destroyDFD();
			return false;
		}

		const uint32_t numSamples = pNew->getSampleCount();
		const size_t sampleSize = numSamples * sizeof(DataFormatDesc::Sample);

		remainingSize -= sizeof(DataFormatDesc::BlockHeader);

		if (numSamples > 0 && remainingSize >= sampleSize)
		{
			pNew->pSamples = allocateArray<DataFormatDesc::Sample>(numSamples);

			if (read(_file, pNew->pSamples, numSamples) == false)
			{
				destroyDFD();
				return false;
			}

			remainingSize -= static_cast<uint32_t>(sampleSize);
		}

		pBlock = pNew;
	}

	return true;
}

void SlimKTX2::writeDFD(IOHandle _file) const
{
	write(_file, &m_dfd.totalSize);
	auto* pBlock = m_dfd.pBlocks;
	while (pBlock != nullptr)
	{
		write(_file, &pBlock->header);

		if (pBlock->pSamples != nullptr)
		{
			const uint32_t numSamples = pBlock->getSampleCount();
			write(_file, pBlock->pSamples, numSamples);
		}

		pBlock = pBlock->pNext;
	};
}

void SlimKTX2::destroyKVD()
{
	auto* pEntry = m_kvd.pKeyValues;
	while (pEntry != nullptr)
	{
		auto* pNext = pEntry->pNext;
		pEntry->pNext = nullptr;

		if (pEntry->pKeyValue != nullptr)
		{
			free(pEntry->pKeyValue);
			pEntry->pKeyValue = nullptr;
		}

		free(pEntry);
		pEntry = pNext;
	};
	m_kvd.pKeyValues = nullptr;
}

bool SlimKTX2::readKVD(IOHandle _file)
{
	destroyKVD();

	uint32_t remainingSize = m_sections.kvdByteLength;

	auto* pEntry = m_kvd.pKeyValues;
	while (remainingSize >= sizeof(uint32_t) + 2u) // minimum entry size 
	{
		auto* pNew = allocateArray<KeyValueData::Entry>();

		if (pEntry != nullptr)
		{
			pEntry->pNext = pNew;
		}
		else
		{
			m_kvd.pKeyValues = pNew;
		}

		if (read(_file, &pNew->keyAndValueByteLength) == false)
		{
			return false;
		}

		remainingSize -= sizeof(uint32_t);
		remainingSize -= pNew->keyAndValueByteLength;

		pNew->pKeyValue = allocateArray<uint8_t>(pNew->keyAndValueByteLength);
		if (read(_file, pNew->pKeyValue, pNew->keyAndValueByteLength) == false)
		{
			return false;
		}

		const uint32_t padding = getPadding(pNew->keyAndValueByteLength, 4u);
		if (padding != 0u)
		{
			auto pos = tell(_file);
			seek(_file, pos + padding);
			remainingSize -= padding;		
		}
	}

	return remainingSize == 0u;
}

void SlimKTX2::writeKVD(IOHandle _file) const
{
	auto* pEntry = m_kvd.pKeyValues;
	while (pEntry != nullptr)
	{
		write(_file, &pEntry->keyAndValueByteLength);
		if (pEntry->pKeyValue != nullptr && pEntry->keyAndValueByteLength > 0u)
		{
			write(_file, pEntry->pKeyValue, pEntry->keyAndValueByteLength);
			const uint32_t padding = getPadding(pEntry->keyAndValueByteLength, 4u);
			writePadding(_file, padding);
		}
		pEntry = pEntry->pNext;
	};
}

void SlimKTX2::destoryMipLevelArray()
{
	if (m_pMipLevelArray != nullptr)
	{
		for (uint32_t i = 0u; i < getLevelCount(); ++i)
		{
			free(m_pMipLevelArray[i]);
		}

		free(m_pMipLevelArray);
		m_pMipLevelArray = nullptr;
	}
}