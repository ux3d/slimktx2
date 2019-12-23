// Copyright (c) 2019 UX3D GmbH. All rights reserved.

#pragma once

#include <stddef.h>
#include <stdint.h>

namespace ux3d
{
	namespace slimktx2
	{
		using AllocationFunc = void*(*)(void* _pUserData, size_t _size);
		using FreeFunc = void(*)(void* _pUserData, void* _pData);

		// IO - stream or file
		using IOHandle = void*;

		using ReadFunc = size_t(*)(void* _pUserData, IOHandle _file, void* _pData, size_t _size);
		using WriteFunc = void(*)(void* _pUserData, IOHandle _file, const void* _pData, size_t _size);
		using TellFunc = size_t(*)(void* _pUserData, const IOHandle _file);
		using SeekFunc = bool(*)(void* _pUserData, IOHandle _file, size_t _offset);

		using LogFunc = void(*)(void* _pUserData, const char* _pFormat, ...);

		struct Callbacks
		{
			void* userData = nullptr; // holds allocator, file/stream handle

			AllocationFunc allocate = nullptr;
			FreeFunc free = nullptr;

			ReadFunc read = nullptr;
			WriteFunc write = nullptr;

			TellFunc tell = nullptr;
			SeekFunc seek = nullptr;

			// optional
			LogFunc log = nullptr;
		};

		enum class Format : uint32_t
		{
			UNDEFINED = 0u,
			R16G16B16A16_SFLOAT = 97u, //VK_FORMAT_R16G16B16A16_SFLOAT
		};

		struct Header
		{
			static constexpr uint8_t Magic[12] = { 0xAB, 0x4B, 0x54, 0x58, 0x20, 0x32, 0x30, 0xBB, 0x0D, 0x0A, 0x1A, 0x0A };

			uint8_t identifier[12];
			Format vkFormat;
			uint32_t typeSize;
			uint32_t pixelWidth;
			uint32_t pixelHeight;
			uint32_t pixelDepth;
			uint32_t layerCount;
			uint32_t faceCount;
			uint32_t levelCount;
			uint32_t supercompressionScheme;
		};

		struct SectionIndex
		{
			uint32_t dfdByteOffset;
			uint32_t dfdByteLength;
			uint32_t kvdByteOffset;
			uint32_t kvdByteLength;
			uint64_t sgdByteOffset;
			uint64_t sgdByteLength;
		};

		struct LevelIndex
		{
			uint64_t byteOffset;
			uint64_t byteLength;
			uint64_t uncompressedByteLength;
		};

		enum class Result : uint32_t
		{
			Success = 0u,
			NotImplemented,
			IOReadFail,
			InvalidIdentifier,
			InvalidImageSize, // setImage _byteSize does not match information of specifyFormat
			InvalidLevelIndex,
			InvalidFaceIndex,
			InvalidLayerIndex,
			LevelIndexNotAllocated,
			ContainerNotAllocated
		};

		// Serialization API:

		// SlimKTX2 ktx(callbacks);
		// ktx.specifyFormat(Format::R16G16B16A16_SFLOAT, 1024, 1024);
		// ktx.allocateContainer();
		// ktx.setImage();
		// ktx.serialize(_file);

		// ktx internally stores mip levels in order from small to total (pixelWidth * pixelHeight) size
		// SlimKTX2 hides this and exposes mip levels like in most GPU APIs where level 0 contains the full image of pixelWidth * pixelHeight, and level 1 half-size image and so on

		class SlimKTX2
		{
		public:
			SlimKTX2() = default;
			SlimKTX2(const Callbacks& _callbacks);
			~SlimKTX2();

			void setCallbacks(const Callbacks& _callbacks);

			Result parse(IOHandle _file);

			Result serialize(IOHandle _file);

			uint32_t getLevelCount() const;
			uint32_t getLayerCount() const;
			uint32_t getFaceCount() const;

			const Header& getHeader() const;

			// fills header and locks format/data-layout for addImage
			Result specifyFormat(Format _vkFormat, uint32_t _width, uint32_t _height, uint32_t _levelCount = 1u, uint32_t _faceCount = 1u, uint32_t _depth = 0u, uint32_t _layerCount = 0u);

			// allocates all image memory required for setImage
			Result allocateContainer();

			uint64_t getContainerSize() const;

			//returns pointer to container
			uint8_t* getContainerPointer();

			// copy image data to container (that was allocated by allocateContainer)
			Result setImage(const void* _pData, size_t _byteSize, uint32_t _level, uint32_t _face, uint32_t _layer);

			// _imageSize is used for validation if _imageSize != 0u
			Result getImage(uint8_t*& _outImageData, uint32_t _level, uint32_t _face, uint32_t _layer, uint64_t _imageSize = 0u);

			// free allocated memory, clear members
			void clear();

			// as defined by KTX (channel size)
			static uint32_t getTypeSize(Format _vkFormat);

			// as defined by vulkan (pixel size)
			static uint32_t getPixelSize(Format _vkFormat);

			static uint64_t getFaceSize(uint32_t _pixelByteSize, uint32_t _level, uint32_t _width, uint32_t _height, uint32_t _depth = 0u);

			// compute byte offset withing m_pContainer for the specified level, face and layer indices, requres m_pLevels to be initialized
			uint64_t getContainerImageOffset(uint32_t _level, uint32_t _face, uint32_t _layer) const;

			// computes the pixel count (resolution) of an image of the given level
			static uint32_t getPixelCount(uint32_t _level, uint32_t _width, uint32_t _height, uint32_t _depth);

			static uint64_t padding(uint64_t _value, uint32_t _alginment);

		private:

			void* allocate(size_t _size);
			void free(void* _pData);

			size_t read(IOHandle _file, void* _pData, size_t _size);
			void write(IOHandle _file, const void* _pData, size_t _size);
			size_t tell(const IOHandle _file);
			bool seek(IOHandle _file, size_t _offset);

			void log(const char* _pFormat, ...);

			uint32_t getKtxLevel(uint32_t _level) const;

		private:
			Callbacks m_callbacks{};

			Header m_header{};
			SectionIndex m_sections{};

			// in KTX mip level 0 is the smallest, so m_pLevels[0] contains the image with (resolution >> levelCount) pixels
			LevelIndex* m_pLevels = nullptr;

			uint8_t* m_pContainer = nullptr;
		};
	}// !slimktx2
} // ux3d