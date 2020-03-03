// Copyright (c) 2019 UX3D GmbH. All rights reserved.

#include "memorystreamcallback.h"

#include <stdlib.h>
#include <string.h>

using namespace ux3d::slimktx2;

MemoryStreamCallback::MemoryStreamCallback(const uint8_t* _data, const size_t _size) :
	m_data(_data), m_size(_size)
{
}

MemoryStreamCallback::~MemoryStreamCallback()
{
}

Callbacks MemoryStreamCallback::getCallback()
{
	ux3d::slimktx2::Callbacks callbacks{};

	callbacks.allocate = allocate;
	callbacks.free = deallocate;
	callbacks.read = read;
	callbacks.tell = tell;
	callbacks.seek = seek;

	return callbacks;
}

size_t MemoryStreamCallback::read(void* _pUserData, IOHandle _iohandle, void* _pData, size_t _size)
{
	MemoryStreamCallback* memoryStreamCallback = (MemoryStreamCallback*)_iohandle;

	size_t size = memoryStreamCallback->getSize();
	size_t offset = memoryStreamCallback->getOffset();

	if (offset + _size >= size)
	{
		_size -= (offset + _size) - size;
	}

	memcpy(_pData, &(memoryStreamCallback->getData()[offset]), _size);

	offset += _size;
	memoryStreamCallback->setOffset(offset);

	return _size;
}

size_t MemoryStreamCallback::tell(void* _pUserData, IOHandle _iohandle)
{
	MemoryStreamCallback* memoryStreamCallback = (MemoryStreamCallback*)_iohandle;

	return memoryStreamCallback->getOffset();
}

bool MemoryStreamCallback::seek(void* _pUserData, IOHandle _iohandle, size_t _offset)
{
	MemoryStreamCallback* memoryStreamCallback = (MemoryStreamCallback*)_iohandle;

	size_t size = memoryStreamCallback->getSize();
	if (_offset >= size)
	{
		return false;
	}

	memoryStreamCallback->setOffset(_offset);

	return true;
}

void* MemoryStreamCallback::allocate(void* _pUserData, size_t _size)
{
	return malloc(_size);
}

void MemoryStreamCallback::deallocate(void* _pUserData, void* _pData)
{
	free(_pData);
}
