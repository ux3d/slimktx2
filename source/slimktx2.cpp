// Copyright (c) 2019 UX3D GmbH. All rights reserved.

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

	// DFD
	destroyDFD();

	// mip map array
	destoryMipLevelArray();
}

uint32_t SlimKTX2::getTypeSize(Format _vkFormat)
{
	if (_vkFormat == Format::UNDEFINED || 
		(_vkFormat >= Format::R8_UNORM && _vkFormat <= Format::B8G8R8A8_SRGB))
	{
		return 1u;
	}
	else if (_vkFormat >= Format::R16_UNORM && _vkFormat <= Format::R16G16B16A16_SFLOAT)
	{
		return 2u;
	}
	else if (_vkFormat >= Format::R32_UINT && _vkFormat <= Format::R32G32B32A32_SFLOAT)
	{
		return 4u;
	}

	return 0u; // invalid
}

uint32_t SlimKTX2::getPixelSize(Format _vkFormat)
{
	switch (_vkFormat)
	{
		// 1 BYTE
	case Format::R8_UNORM:
	case Format::R8_SNORM:
	case Format::R8_UINT:
	case Format::R8_SINT:
	case Format::R8_SRGB:
		return 1u;

		// 2 BYTE
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
		return 16u;
	default:
		return 0u; // invalid
	}
}

uint32_t SlimKTX2::getChannelCount(Format _vkFormat)
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
		return 1u;

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
		return 2u;

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
		return 3u;

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
		return 4u;

	default:
		return 0u; // invalid
	}
}

uint32_t SlimKTX2::getChannelSize(Format _vkFormat, uint32_t channel)
{
	if (channel >= 4u)
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
	}

	return 0u;
}

bool SlimKTX2::isFloat(Format _vkFormat)
{

	switch (_vkFormat)
	{
	case Format::R16_SFLOAT:

	case Format::R16G16_SFLOAT:

	case Format::R16G16B16_SFLOAT:

	case Format::R16G16B16A16_SFLOAT:

	case Format::R32_SFLOAT:

	case Format::R32G32_SFLOAT:

	case Format::R32G32B32_SFLOAT:

	case Format::R32G32B32A32_SFLOAT:
		return true;
	}

	return false;
}

bool SlimKTX2::isSigned(Format _vkFormat)
{
	switch (_vkFormat)
	{
		// 8 bits; does not depend on channel.
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
		return true;
	}

	return false;
}

bool SlimKTX2::isNormalized(Format _vkFormat)
{

	switch (_vkFormat)
	{
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
	}

	return false;
}

uint64_t SlimKTX2::getFaceSize(uint32_t _pixelByteSize, uint32_t _level, uint32_t _width, uint32_t _height, uint32_t _depth)
{
	const uint64_t resolution = getPixelCount(_level, _width, _height, _depth);
	return resolution * _pixelByteSize;
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
	const uint64_t faceSize = getFaceSize(getPixelSize(m_header.vkFormat), _level, m_header.pixelWidth, m_header.pixelHeight, m_header.pixelDepth);

	// number of previous layers with either 1 or 6 faces
	uint64_t prevFaces = faceSize * _layer * m_header.faceCount;

	// add the faces within this cube map
	prevFaces += faceSize * _face;

	// we are now at the start of the current level, add faces an layers:
	offset += prevFaces;

	return offset;
}

uint32_t SlimKTX2::getPixelCount(uint32_t _level, uint32_t _width, uint32_t _height, uint32_t _depth)
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

uint32_t SlimKTX2::padding(uint64_t _value, uint32_t _alginment)
{
	return (_alginment - (_value % _alginment)) % _alginment;
}

uint32_t SlimKTX2::lcm(uint32_t _x, uint32_t _y)
{
	uint32_t max = _x > _y ? _x : _y;

	while (!((max % _x == 0u) && (max % _y == 0u)))
	{
		++max;
	}

	return max;
}

uint32_t SlimKTX2::mipPadding(uint64_t _value, Format _vkFormat, bool _superCompression)
{
	if (_superCompression)
	{
		return 0; // 1u byte alignment
	}
	else if (_vkFormat == Format::UNDEFINED)
	{
		return SlimKTX2::padding(_value, 16u); // 16 byte alignment
	}

	const uint32_t texelBlockSize = getPixelSize(_vkFormat);
	const uint32_t lcm = SlimKTX2::lcm(texelBlockSize, 4u);
	const uint32_t padding = SlimKTX2::padding(_value, lcm);
	return padding;
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

	// TODO: kvd and sgd	

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
		return Result::LevelIndexNotAllocated;
	}

	if (m_pMipLevelArray == nullptr)
	{
		return Result::ContainerNotAllocated;
	}

	if (m_dfd.pBlocks == nullptr)
	{
		return Result::DataFormatDescNotAllocated;
	}

	const size_t streamStart = tell(_file);
	auto filePos = [&](IOHandle file) -> size_t { return tell(file) - streamStart; };

	const uint32_t pixelSize = getPixelSize(m_header.vkFormat);
	const uint32_t levelCount = getLevelCount();

	const uint32_t dfdByteLength = m_dfd.totalSize + sizeof(uint32_t); // size of totalSize field
	const uint32_t dfdByteOffset = sizeof(Header) + sizeof(SectionIndex) + sizeof(LevelIndex) * m_header.levelCount;
	const uint32_t kvdByteLength = 0u; // TODO compute
	const uint32_t kvdByteOffset = dfdByteOffset + dfdByteLength;
	const uint64_t sgdByteLength = 0u;
	/*const*/ uint64_t sgdByteOffset = kvdByteOffset + kvdByteLength;

	const uint32_t sdgPadding = padding(sgdByteOffset, 8u);

	if (sgdByteLength > 0u)
	{
		sgdByteOffset += sdgPadding;
	}

	uint64_t levelOffset = sgdByteOffset + sgdByteLength;

	for (uint32_t level = levelCount - 1u; level <= levelCount; --level)
	{
		const uint32_t mipPad = mipPadding(levelOffset, m_header.vkFormat, m_header.supercompressionScheme != 0u);
		levelOffset += mipPad;

		// start with the small level, fill them in reverse
		uint64_t levelSize = getFaceSize(pixelSize, level, m_header.pixelWidth, m_header.pixelHeight, m_header.pixelDepth);
		levelSize *= m_header.faceCount;
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
	m_sections.kvdByteOffset = kvdByteLength != 0u ? kvdByteOffset : 0u;

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

	// TODO: write kvd and sgd
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

		const uint32_t mipPad = mipPadding(curPos, m_header.vkFormat, m_header.supercompressionScheme != 0u);

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

Result SlimKTX2::specifyFormat(Format _vkFormat, uint32_t _width, uint32_t _height, uint32_t _levelCount, uint32_t _faceCount, uint32_t _depth, uint32_t _layerCount)
{
	if (m_pLevels != nullptr)
	{
		free(m_pLevels);
	}

	destroyDFD();

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
	const uint32_t levelIndexSize = static_cast<uint32_t>(sizeof(LevelIndex)) * levelCount;

	m_pLevels = allocateArray<LevelIndex>(levelCount);
	const uint32_t pixelSize = getPixelSize(m_header.vkFormat);

	// TODO: fill with meaningful data
	m_dfd.pBlocks = allocateArray<DataFormatDesc::Block>();
	m_dfd.totalSize = m_dfd.computeSize();

	return Result::Success;
}

Result SlimKTX2::allocateMipLevelArray()
{
	destoryMipLevelArray();

	const uint32_t pixelSize = getPixelSize(m_header.vkFormat);

	const auto levelCount = getLevelCount();

	m_pMipLevelArray = allocateArray<uint8_t*>(levelCount);

	if (m_pMipLevelArray == nullptr)
	{
		return Result::ContainerNotAllocated;
	}

	for (uint32_t l = 0u; l < getLevelCount(); ++l)
	{
		uint64_t levelSize = getFaceSize(pixelSize, l, m_header.pixelWidth, m_header.pixelHeight, m_header.pixelDepth);
		levelSize *= getFaceCount();
		levelSize *= getLayerCount();

		m_pMipLevelArray[l] = allocateArray<uint8_t>(levelSize);
		//memset(m_pMipLevelArray[l], 0, levelSize);

		if (m_pMipLevelArray[l] == nullptr)
		{
			return Result::ContainerNotAllocated;
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
		return Result::ContainerNotAllocated;
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
	// TODO: validate total size
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

void ux3d::slimktx2::SlimKTX2::destoryMipLevelArray()
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

DataFormatDesc::BlockHeader::BlockHeader() :
	vendorId(0u), // KHR
	type(0u), // basicformat
	versionNumber(2u), // v1.3
	blockSize(blockHeaderSize),
	colorModel(0u), // unspecified
	colorPrimaries(0u), // unspecified
	transferFunction(1u), // linear
	flags(0u), // alpha straight
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
	size_t size = 0u; // totalSize member ignored

	const Block* pBlock = pBlocks;
	while (pBlock != nullptr)
	{
		size += blockHeaderSize;

		if (pBlock->pSamples != nullptr)
		{
			size += static_cast<uint64_t>(pBlock->getSampleCount()) * sampleSize;
		}

		pBlock = pBlock->pNext;
	};

	return static_cast<uint32_t>(size);
}
