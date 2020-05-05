// Copyright (c) 2020 UX3D GmbH. All rights reserved.

#include "DefaultFileIOCallback.h"

#include <cstdio>

using namespace ux3d::slimktx2;

Callbacks DefaultFileIOCallback::getCallback() const
{
	Callbacks callback{};

	// userdata not required
	callback.read = read;
	callback.write = write;
	callback.seek = seek;
	callback.tell = tell;

	return callback;
}

DefaultFileIOCallback::operator Callbacks() const
{
	return getCallback();
}

size_t DefaultFileIOCallback::read(void* _pUserData, IOHandle _file, void* _pData, size_t _size)
{
	FILE* pFile = static_cast<FILE*>(_file);
	return fread(_pData, 1u, _size, pFile);
}

void DefaultFileIOCallback::write(void* _pUserData, IOHandle _file, const void* _pData, size_t _size)
{
	FILE* pFile = static_cast<FILE*>(_file);
	fwrite(_pData, 1u, _size, pFile);
}

size_t DefaultFileIOCallback::tell(void* _pUserData, IOHandle _file)
{
	FILE* pFile = static_cast<FILE*>(_file);
	return ftell(pFile);
}

bool DefaultFileIOCallback::seek(void* _pUserData, IOHandle _file, size_t _offset)
{
	FILE* pFile = static_cast<FILE*>(_file);
	return fseek(pFile, _offset, SEEK_SET) == 0;
}
