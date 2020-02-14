// Copyright (c) 2019 UX3D GmbH. All rights reserved.

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <new.h>

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

		using LogFunc = void(*)(void* _pUserData, const char* _pFormat, va_list args);

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

			///
			/// 8 BIT PER CHANNEL
			///

			// R8
			R8_UNORM = 9,
			R8_SNORM = 10,
			R8_UINT = 13,
			R8_SINT = 14,
			R8_SRGB = 15,

			// RG8
			R8G8_UNORM = 16,
			R8G8_SNORM = 17,
			R8G8_UINT = 20,
			R8G8_SINT = 21,
			R8G8_SRGB = 22,

			// RGB8
			R8G8B8_UNORM = 23,
			R8G8B8_SNORM = 24,
			R8G8B8_UINT = 27,
			R8G8B8_SINT = 28,
			R8G8B8_SRGB = 29,

			// BGR8
			B8G8R8_UNORM = 30,
			B8G8R8_SNORM = 31,
			B8G8R8_UINT = 34,
			B8G8R8_SINT = 35,
			B8G8R8_SRGB = 36,

			// RGBA8
			R8G8B8A8_UNORM = 37,
			R8G8B8A8_SNORM = 38,
			R8G8B8A8_UINT = 41,
			R8G8B8A8_SINT = 42,
			R8G8B8A8_SRGB = 43,

			// BGRA8
			B8G8R8A8_UNORM = 44,
			B8G8R8A8_SNORM = 45,
			B8G8R8A8_UINT = 48,
			B8G8R8A8_SINT = 49,
			B8G8R8A8_SRGB = 50,

			///
			/// 16 BIT PER CHANNEL
			///

			// R16
			R16_UNORM = 70,
			R16_SNORM = 71,
			R16_UINT = 74,
			R16_SINT = 75,
			R16_SFLOAT = 76,

			// RG16
			R16G16_UNORM = 77,
			R16G16_SNORM = 78,
			R16G16_UINT = 81,
			R16G16_SINT = 82,
			R16G16_SFLOAT = 83,

			// RGB16
			R16G16B16_UNORM = 84,
			R16G16B16_SNORM = 85,
			R16G16B16_UINT = 88,
			R16G16B16_SINT = 89,
			R16G16B16_SFLOAT = 90,

			// RGBA16
			R16G16B16A16_UNORM = 91,
			R16G16B16A16_SNORM = 92,
			R16G16B16A16_UINT = 95,
			R16G16B16A16_SINT = 96,
			R16G16B16A16_SFLOAT = 97,

			///
			/// 32 BIT PER CHANNEL
			///

			// R32
			R32_UINT = 98,
			R32_SINT = 99,
			R32_SFLOAT = 100,
			
			// RG32
			R32G32_UINT = 101,
			R32G32_SINT = 102,
			R32G32_SFLOAT = 103,

			// RGB32
			R32G32B32_UINT = 104,
			R32G32B32_SINT = 105,
			R32G32B32_SFLOAT = 106,

			// RGBA32
			R32G32B32A32_UINT = 107,
			R32G32B32A32_SINT = 108,
			R32G32B32A32_SFLOAT = 109,
		};

		// dont use enum class to allow converssion to uint32_t
		enum CubeMapFace : uint32_t
		{
			Right = 0,
			Left,
			Top,
			Bottom,
			Front,
			Back,

			Xpos = Right,
			Xneg = Left,
			Ypos = Top,
			Yneg = Bottom,
			Zpos = Front,
			Zneg = Back
		};

		struct Header
		{
			static const uint8_t Magic[12];

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

		struct DataFormatDesc
		{
			uint32_t totalSize{};

			//https://www.khronos.org/registry/DataFormat/specs/1.3/dataformat.1.3.html#DescriptorPrefix
			struct BlockHeader 
			{
				uint32_t vendorId: 17;
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

		struct KeyValueData
		{
			uint32_t keyAndValueByteLength = 0u;
			uint8_t* pKeyAndValue = nullptr; // uint8_t [keyAndValueByteLength]
			// align(4)
		};

		struct SuperCompressionGlobalData
		{
			//uint8_t[sgdByteLength];
		};

		enum class Result : uint32_t
		{
			Success = 0u,
			NotImplemented,
			IOReadFail,
			IOWriteFail,
			InvalidIdentifier,
			InvalidImageSize, // setImage _byteSize does not match information of specifyFormat
			InvalidLevelIndex,
			InvalidFaceIndex,
			InvalidLayerIndex,
			LevelIndexNotAllocated,
			ContainerNotAllocated,
			DataFormatDescNotAllocated,
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
			uint8_t* getContainerPointer() const;

			// compute byte offset withing m_pContainer for the specified level, face and layer indices, requres m_pLevels to be initialized
			uint64_t getContainerImageOffset(uint32_t _level, uint32_t _face, uint32_t _layer) const;

			// copy image data to container (that was allocated by allocateContainer)
			Result setImage(const void* _pData, size_t _byteSize, uint32_t _level, uint32_t _face, uint32_t _layer);

			// returns pointer to face at _level, _layer, _face index, _imageSize is used for validation if _imageSize != 0u
			Result getImage(uint8_t*& _outImageData, uint32_t _level, uint32_t _face, uint32_t _layer, uint64_t _imageSize = 0u) const;

			// free allocated memory, clear members
			void clear();

			// as defined by KTX (channel size)
			static uint32_t getTypeSize(Format _vkFormat);

			// as defined by vulkan (pixel size)
			static uint32_t getPixelSize(Format _vkFormat);

			static uint64_t getFaceSize(uint32_t _pixelByteSize, uint32_t _level, uint32_t _width, uint32_t _height, uint32_t _depth = 0u);

			// computes the pixel count (resolution) of an image of the given level
			static uint32_t getPixelCount(uint32_t _level, uint32_t _width, uint32_t _height, uint32_t _depth);

			static uint32_t padding(uint64_t _value, uint32_t _alginment);

			// greedy least common multiple
			static uint32_t lcm(uint32_t _x, uint32_t _y);

			static uint32_t mipPadding(uint64_t _value, uint32_t _pixelByteSize);

		private:

			void* allocate(size_t _size);
			void free(void* _pData);

			template<class T>
			T* allocateArray(size_t _count = 1u) { return new(reinterpret_cast<T*>(allocate(sizeof(T) * _count))) T{}; }

			template<class T>
			bool read(IOHandle _file, T* _pData, size_t _count = 1u) { return sizeof(T) * _count == m_callbacks.read(m_callbacks.userData, _file, _pData, sizeof(T) * _count); }

			template<class T>
			void write(IOHandle _file, const T* _pData, size_t _count = 1u) const { m_callbacks.write(m_callbacks.userData, _file, _pData, sizeof(T)* _count); }

			void writePadding(IOHandle _file, size_t _byteSize) const;

			size_t tell(const IOHandle _file);
			bool seek(IOHandle _file, size_t _offset);

			void log(const char* _pFormat, ...);

			uint32_t getKtxLevel(uint32_t _level) const;

			void destroyDFD();
			bool readDFD(IOHandle _file);
			void writeDFD(IOHandle _file) const;

		private:
			Callbacks m_callbacks{};

			Header m_header{};
			SectionIndex m_sections{};

			// in KTX mip level 0 is the smallest, so m_pLevels[0] contains the image with (resolution >> levelCount) pixels
			LevelIndex* m_pLevels = nullptr;

			DataFormatDesc m_dfd{};

			// mipLevel array
			uint8_t* m_pContainer = nullptr;
		};
	}// !slimktx2
} // ux3d