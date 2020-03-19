// Copyright (c) 2020 UX3D GmbH. All rights reserved.

#include "DefaultConsoleLogCallback.h"
#include <stdio.h>

using namespace ux3d::slimktx2;

Callbacks DefaultConsoleLogCallback::getCallback() const
{
	Callbacks callback{};

	// userdata not required
	callback.log = log;

	return callback;
}

DefaultConsoleLogCallback::operator Callbacks() const
{
	return getCallback();
}

void DefaultConsoleLogCallback::log(void* _pUserData, const char* _pFormat, va_list _args)
{
	char buffer[512];
	vsnprintf(buffer, sizeof(buffer), _pFormat, _args);
	printf(buffer);
}
