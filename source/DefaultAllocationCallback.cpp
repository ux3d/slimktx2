// Copyright (c) 2020 UX3D GmbH. All rights reserved.

#include "DefaultAllocationCallback.h"

#include <stdlib.h>

using namespace ux3d::slimktx2;

inline Callbacks DefaultAllocationCallback::getCallback() const
{
	Callbacks callback{};

	// userdata not required
	callback.allocate = allocate;
	callback.deallocate = deallocate;

	return callback;
}

DefaultAllocationCallback::operator Callbacks() const
{
	return getCallback();
}

void* DefaultAllocationCallback::allocate(void* _pUserData, size_t _size)
{
	return malloc(_size);
}

void DefaultAllocationCallback::deallocate(void* _pUserData, void* _pData)
{
	free(_pData);
}