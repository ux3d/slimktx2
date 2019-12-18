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

		struct Header
		{
			static constexpr char* Magic = "«KTX 20»\r\n\x1A\n";

			char identifier[12];
			uint32_t vkFormat;
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
			IOReadFail,
			InvalidIdentifier,
		};

		class SlimKTX2
		{
		public:
			SlimKTX2(const Callbacks& _callbacks);
			~SlimKTX2();

			Result parse(IOHandle _file);

			Result serialize(IOHandle _file);

			// free allocated memory, clear members
			void clear();

		private:

			void* allocate(size_t _size);
			void free(void* _pData);

			size_t read(IOHandle _file, void* _pData, size_t _size);
			void write(IOHandle _file, const void* _pData, size_t _size);
			size_t tell(IOHandle _file);
			bool seek(IOHandle _file, size_t _offset);

			void log(const char* _pFormat, ...);

		private:
			Callbacks m_callbacks{};

			Header m_header{};
			SectionIndex m_sections;
			LevelIndex* m_pLevels = nullptr;
		};
	}// !slimktx2
} // ux3d