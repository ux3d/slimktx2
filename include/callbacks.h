// Copyright (c) 2020 UX3D GmbH. All rights reserved.

#pragma once

#include <stdarg.h>
#include <stddef.h>

namespace ux3d
{
	namespace slimktx2
	{
		using AllocationFunc = void* (*)(void* _pUserData, size_t _size);
		using DeallocationFunc = void(*)(void* _pUserData, void* _pData);

		// IO - stream or file
		using IOHandle = void*;

		using ReadFunc = size_t(*)(void* _pUserData, IOHandle _file, void* _pData, size_t _size);
		using WriteFunc = void(*)(void* _pUserData, IOHandle _file, const void* _pData, size_t _size);
		using TellFunc = size_t(*)(void* _pUserData, const IOHandle _file);
		using SeekFunc = bool(*)(void* _pUserData, IOHandle _file, size_t _offset);

		using LogFunc = void(*)(void* _pUserData, const char* _pFormat, va_list args);

		struct Callbacks
		{
			void* userData = nullptr; // holds allocator, user implementations

			AllocationFunc allocate = nullptr;
			DeallocationFunc deallocate = nullptr;

			ReadFunc read = nullptr;
			WriteFunc write = nullptr;

			TellFunc tell = nullptr;
			SeekFunc seek = nullptr;

			// optional
			LogFunc log = nullptr;
		};
	}// !slimktx2
} // ux3d
