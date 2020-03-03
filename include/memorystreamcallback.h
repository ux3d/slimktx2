// Copyright (c) 2019 UX3D GmbH. All rights reserved.

#pragma once

#include "slimktx2.h"

namespace ux3d
{
	namespace slimktx2
	{

		class MemoryStreamCallback
		{
		public:

			MemoryStreamCallback(const uint8_t* _data, const size_t _size);
			~MemoryStreamCallback();

			slimktx2::Callbacks getCallback();

			const uint8_t* getData() const { return m_data; };
			size_t getSize() const { return m_size; };

			size_t getOffset() const { return m_offset; };
			void setOffset(size_t _offset) { m_offset = _offset; };

		private:

			const uint8_t* m_data = nullptr;
			const size_t m_size = 0;

			size_t m_offset = 0;

			static size_t read(void* _pUserData, IOHandle _iohandle, void* _pData, size_t _size);
			static size_t tell(void* _pUserData, IOHandle _iohandle);
			static bool seek(void* _pUserData, IOHandle _iohandle, size_t _offset);

			static void* allocate(void* _pUserData, size_t _size);
			static void deallocate(void* _pUserData, void* _pData);

		};

	}// !slimktx2
} // ux3d
