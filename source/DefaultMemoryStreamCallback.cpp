// Copyright (c) 2020 UX3D GmbH. All rights reserved.

#include "DefaultMemoryStreamCallback.h"

#include <cstring>

using namespace ux3d::slimktx2;

Callbacks DefaultMemoryStreamCallback::getCallback() const
{
	Callbacks callback{};

	callback.read = read;
	callback.write = write;
	callback.seek = seek;
	callback.tell = tell;

	return callback;
}

DefaultMemoryStreamCallback::operator Callbacks() const
{
	return getCallback();
}

size_t DefaultMemoryStreamCallback::read(void* _pUserData, IOHandle _iohandle, void* _pData, size_t _size)
{
	DefaultMemoryStream* stream = static_cast<DefaultMemoryStream*>(_iohandle);

	const size_t size = stream->getSize();
	size_t offset = stream->getOffset();

	if (offset + _size >= size)
	{
		_size -= (offset + _size) - size;
	}

	const uint8_t* pData = stream->getDataConst() + offset;

	memcpy(_pData, pData, _size);

	offset += _size;
	stream->setOffset(offset);

	return _size;
}

void DefaultMemoryStreamCallback::write(void* _pUserData, IOHandle _iohandle, const void* _pData, size_t _size)
{
	DefaultMemoryStream* stream = static_cast<DefaultMemoryStream*>(_iohandle);

	const size_t size = stream->getSize();
	size_t offset = stream->getOffset();

	if (offset + _size >= size)
	{
		_size -= (offset + _size) - size;
	}

	if (_size != 0u)
	{
		uint8_t* pData = stream->getData() + offset;
		memcpy(pData, _pData, _size);
	}
}

size_t DefaultMemoryStreamCallback::tell(void* _pUserData, IOHandle _iohandle)
{
	return static_cast<const DefaultMemoryStream*>(_iohandle)->getOffset();
}

bool DefaultMemoryStreamCallback::seek(void* _pUserData, IOHandle _iohandle, size_t _offset)
{
	DefaultMemoryStream* stream = static_cast<DefaultMemoryStream*>(_iohandle);

	size_t size = stream->getSize();
	if (_offset >= size)
	{
		return false;
	}

	stream->setOffset(_offset);

	return true;
}
