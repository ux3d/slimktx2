﻿// Copyright (c) 2019 UX3D GmbH. All rights reserved.

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
	if (m_pLevels != nullptr)
	{
		free(m_pLevels);
		m_pLevels = nullptr;
	}

	if (m_pContainer != nullptr)
	{
		free(m_pContainer);
		m_pContainer = nullptr;
	}
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
		return 6u;
	default:
		return 0u; // invalid
	}
}

uint64_t SlimKTX2::getFaceSize(uint32_t _pixelByteSize, uint32_t _level, uint32_t _width, uint32_t _height, uint32_t _depth)
{
	const uint64_t resolution = getPixelCount(_level, _width, _height, _depth);
	return resolution * _pixelByteSize;
}

uint64_t SlimKTX2::getContainerImageOffset(uint32_t _level, uint32_t _face, uint32_t _layer) const
{
	uint64_t offset = 0u;

	if (m_pLevels == nullptr)
	{
		return 0u;
	}

	const uint32_t pixelSize = getPixelSize(m_header.vkFormat);

	// small to large levels
	for (uint32_t l = getLevelCount() - 1u; l > _level; --l)
	{
		//const uint64_t levelSize = m_pLevels[l].byteLength;
		const uint64_t levelSize = getFaceSize(pixelSize, l, m_header.pixelWidth, m_header.pixelHeight, m_header.pixelDepth) * getFaceCount() * getLayerCount();
		offset += levelSize;
		offset += padding(levelSize, 8u);
	}

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

uint64_t SlimKTX2::padding(uint64_t _value, uint32_t _alginment)
{
	return (_alginment - (_value % _alginment)) % _alginment;
}

Result SlimKTX2::parse(IOHandle _file)
{
	clear();

	if (sizeof(Header) != read(_file, &m_header, sizeof(Header)))
	{
		return Result::IOReadFail;
	}

	if (memcmp(m_header.identifier, Header::Magic, sizeof(m_header.identifier)) != 0)
	{
		return Result::InvalidIdentifier;
	}

	if (sizeof(SectionIndex) != read(_file, &m_sections, sizeof(SectionIndex)))
	{
		return Result::IOReadFail;
	}

	const uint32_t levelCount = getLevelCount();
	const size_t levelIndexSize = sizeof(LevelIndex) * levelCount;

	m_pLevels = static_cast<LevelIndex*>(allocate(levelIndexSize));

	if (levelIndexSize != read(_file, m_pLevels, levelIndexSize))
	{
		return Result::IOReadFail;
	}

	const uint64_t offset = sizeof(Header) +
		sizeof(SectionIndex) + levelIndexSize +
		m_sections.dfdByteLength +
		m_sections.kvdByteLength +
		m_sections.sgdByteLength;

	const size_t pos = tell(_file);

	if (pos != offset)
	{
		return Result::IOReadFail;
	}

	const uint64_t containerSize = getContainerSize();

	m_pContainer = static_cast<uint8_t*>(allocate(containerSize));

	if (containerSize != read(_file, m_pContainer, containerSize))
	{
		return Result::IOReadFail;
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

	if (m_pContainer == nullptr)
	{
		return Result::ContainerNotAllocated;
	}

	write(_file, &m_header, sizeof(Header));
	write(_file, &m_sections, sizeof(SectionIndex));
	write(_file, m_pLevels, sizeof(LevelIndex) * m_header.levelCount);

	// TODO: write dfd, kvd and sgd

	const uint64_t containerSize = getContainerSize();

	write(_file, m_pContainer, containerSize);

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

	// compression and sections index are not used

	const uint32_t levelCount = getLevelCount();
	const size_t levelIndexSize = sizeof(LevelIndex) * levelCount;

	m_pLevels = static_cast<LevelIndex*>(allocate(levelIndexSize));
	const uint32_t pixelSize = getPixelSize(m_header.vkFormat);

	uint64_t offset = sizeof(Header) +
		sizeof(SectionIndex) + levelIndexSize +
		m_sections.dfdByteLength +
		m_sections.kvdByteLength + // what about align(8) ?
		m_sections.sgdByteLength;

	for (uint32_t level = levelCount - 1u; level <= levelCount; --level)
	{
		// start with the small level, fill them in reverse
		uint64_t levelSize = getFaceSize(pixelSize, level, m_header.pixelWidth, m_header.pixelHeight, m_header.pixelDepth);
		levelSize *= m_header.faceCount;
		levelSize *= getLayerCount();

		// absolute offset within the file
		m_pLevels[level].byteOffset = offset;
		m_pLevels[level].byteLength = levelSize;
		m_pLevels[level].uncompressedByteLength = levelSize; // uncompressedByteLength % (faceCount * max(1, layerCount)) == 0

		log("level %u offset %llu length %llu\n", level, offset, levelSize);

		offset += levelSize + padding(levelSize, 8u);
	}

	return Result::Success;
}

Result SlimKTX2::allocateContainer()
{
	if (m_pContainer != nullptr)
	{
		free(m_pContainer);
	}

	const uint64_t size = getContainerSize();

	m_pContainer = static_cast<uint8_t*>(allocate(size));
	memset(m_pContainer, 0, size); //set memory to 0 incase of level padding
	
	return Result::Success;
}

uint64_t SlimKTX2::getContainerSize() const
{
	if (m_pLevels == nullptr)
	{
		return 0u;
	}

	uint64_t size = 0u;
	for (uint32_t level = 0u; level < getLevelCount(); ++level)
	{
		size += m_pLevels[level].byteLength;
		size += padding(m_pLevels[level].byteLength, 8u);
	}

	return size;
}

uint8_t* SlimKTX2::getContainerPointer() const
{
	return m_pContainer;
}

Result SlimKTX2::setImage(const void* _pData, size_t _byteSize, uint32_t _level, uint32_t _face, uint32_t _layer)
{
	Result res = Result::Success;

	if (_level >= m_header.levelCount)
	{
		return Result::InvalidLevelIndex;
	}

	const uint32_t pixelSize = getPixelSize(m_header.vkFormat);
	const uint64_t imageSize = m_pLevels[_level].byteLength / m_header.faceCount / getLayerCount();

	// for debugging: size of one mip level image
	const uint64_t dbgSize = getFaceSize(pixelSize, _level,  m_header.pixelWidth, m_header.pixelHeight, m_header.pixelDepth);

	if (_byteSize != imageSize || _byteSize != dbgSize)
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
	if (m_pContainer == nullptr)
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

	const uint64_t offset = getContainerImageOffset(_level, _face, _layer);

	if (_imageSize != 0u) 
	{
		// for debugging
		const uint64_t containerSize = getContainerSize();

		// offset / image out of container bounds
		if (offset + _imageSize > containerSize)
		{
			return Result::InvalidImageSize;
		}
	}

	_outImageData = m_pContainer + offset;

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

size_t SlimKTX2::read(IOHandle _file, void* _pData, size_t _size)
{
	return m_callbacks.read(m_callbacks.userData, _file, _pData, _size);
}

void SlimKTX2::write(IOHandle _file, const void* _pData, size_t _size)
{
	m_callbacks.write(m_callbacks.userData, _file, _pData, _size);
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
