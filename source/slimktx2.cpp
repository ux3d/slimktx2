// Copyright (c) 2019 UX3D GmbH. All rights reserved.

#include "slimktx2.h"
#include <stdarg.h>
#include <string.h>

using namespace ux3d::slimktx2;

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
	switch (_vkFormat)
	{
	case Format::UNDEFINED:
		return 1u;
	case Format::R16G16B16A16_SFLOAT:
		return 2u;
	default:
		return 0u; // invalid
	}
}

uint32_t SlimKTX2::getPixelSize(Format _vkFormat)
{
	switch (_vkFormat)
	{
	case Format::R16G16B16A16_SFLOAT:
		return 8u;
	default:
		return 0u; // invalid
	}
}

uint64_t SlimKTX2::getPaddedImageSize(uint32_t _pixelByteSize, uint32_t _level, uint32_t _width, uint32_t _height, uint32_t _depth, uint32_t _faceCount, uint32_t _layerCount)
{
	const uint32_t resolution = getPixelCount(_level, _width, _height, _depth);
	const uint64_t levelSize = resolution * _pixelByteSize * _faceCount * _layerCount;
	const uint32_t padding = (8u - (levelSize % 8u)) % 8u;

	return levelSize + padding;
}

uint64_t SlimKTX2::getLevelContainerImageOffset(uint32_t _pixelByteSize, uint32_t _levelCount, uint32_t _level, uint32_t _width, uint32_t _height, uint32_t _depth, uint32_t _faceCount, uint32_t _layerCount)
{
	uint64_t offset = 0u;

	// Mip level data is ordered from the level with the smallest size images, levelp to that with the largest size images, levelbase where p=levelCount−1 and base=0. levelp must not be greater than the maximum possible
	// -> we go in reverse order from small mip levels to big
	for (uint32_t level = _levelCount - 1u; level >= 0u; --level)
	{
		// image contains all faces (1-6) and all layers
		const uint64_t levelSize = getPaddedImageSize(_pixelByteSize, level, _width, _height, _depth, _faceCount, _layerCount);
		offset += levelSize;
	}

	return offset;
}

uint64_t SlimKTX2::getLevelContainerSize(const Header& _header)
{
	return getLevelContainerImageOffset(getPixelSize(_header.vkFormat), _header.levelCount, _header.levelCount, _header.pixelWidth, _header.pixelHeight, _header.pixelDepth, _header.faceCount, _header.layerCount);
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

	return Result::Success;
}

Result SlimKTX2::serialize(IOHandle _file)
{
	write(_file, &m_header, sizeof(Header));
	write(_file, &m_sections, sizeof(SectionIndex));
	write(_file, m_pLevels, sizeof(LevelIndex) * m_header.levelCount);

	// TODO: write dfd, kvd and sgd

	write(_file, m_pContainer, getLevelContainerSize(m_header));

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
	m_header.levelCount = _levelCount;
	m_header.supercompressionScheme = 0u;

	// compression and sections index are not used

	const uint32_t levelCount = getLevelCount();
	const size_t levelIndexSize = sizeof(LevelIndex) * levelCount;

	m_pLevels = static_cast<LevelIndex*>(allocate(levelIndexSize));
	const uint32_t pixelSize = getPixelSize(m_header.vkFormat);

	const uint64_t offset = sizeof(Header) +
		sizeof(SectionIndex) + levelIndexSize +
		m_sections.dfdByteLength +
		m_sections.kvdByteLength + // what about align(8) ?
		m_sections.sgdByteLength;

	for (uint32_t level = levelCount - 1u; level >= 0u; --level)
	{
		const uint64_t levelSize = getPaddedImageSize(pixelSize, level, m_header.pixelWidth, m_header.pixelHeight, m_header.pixelDepth, m_header.faceCount, m_header.layerCount);

		// absolute offset within the file
		m_pLevels[level].byteOffset = offset + levelSize;
		m_pLevels[level].byteLength = levelSize;
		m_pLevels[level].uncompressedByteLength = levelSize; // uncompressedByteLength % (faceCount * max(1, layerCount)) == 0
	}

	return Result::Success;
}

Result SlimKTX2::allocateLevelContainer()
{
	if (m_pContainer != nullptr)
	{
		free(m_pContainer);
	}

	const uint64_t size = getLevelContainerSize(m_header);

	m_pContainer = static_cast<uint8_t*>(allocate(size));
	
	return Result::Success;
}

uint8_t* SlimKTX2::getLevelContainerPointer()
{
	return m_pContainer;
}

Result SlimKTX2::setImage(const void* _pData, size_t _byteSize, uint32_t _level, uint32_t _face, uint32_t _layer)
{
	Result res = Result::Success;

	const uint32_t pixelSize = getPixelSize(m_header.vkFormat);
	const uint32_t maxLevel = getLevelCount();

	const uint64_t imageSize = m_pLevels[_level].byteLength / m_header.faceCount / m_header.layerCount;

	// for debugging: size of one mip level image
	const uint64_t paddedImageSize = getPaddedImageSize(pixelSize, _level, m_header.pixelWidth, m_header.pixelHeight, m_header.pixelDepth, 1u, 1u);

	if (_byteSize != imageSize || _byteSize != paddedImageSize)
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

Result SlimKTX2::getImage(uint8_t*& _outImageData, uint32_t _level, uint32_t _face, uint32_t _layer, uint64_t _imageSize)
{
	if (_level >= m_header.levelCount)
	{
		return Result::InvalidLevelIndex;
	}
	if (_face >= m_header.faceCount)
	{
		return Result::InvalidFaceIndex;
	}
	if (_layer >= m_header.layerCount)
	{
		return Result::InvalidLayerIndex;
	}

	const uint32_t pixelSize = getPixelSize(m_header.vkFormat);
	const uint32_t levelCount = getLevelCount();
	const uint64_t offset = getLevelContainerImageOffset(pixelSize, levelCount, _level, m_header.pixelWidth, m_header.pixelHeight, m_header.pixelDepth, m_header.faceCount, m_header.layerCount);

	if (_imageSize != 0u) 
	{
		// for debugging
		const uint64_t containerSize = getLevelContainerSize(m_header);

		// offset / image out of container bounds
		if (offset + _imageSize >= containerSize)
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
