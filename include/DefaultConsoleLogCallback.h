// Copyright (c) 2020 UX3D GmbH. All rights reserved.

#pragma once

#include "callbacks.h"

namespace ux3d
{
	namespace slimktx2
	{
		class DefaultConsoleLogCallback
		{
		public:
			slimktx2::Callbacks getCallback() const;

			operator slimktx2::Callbacks() const;

		private:
			static void log(void* _pUserData, const char* _pFormat, va_list _args);
		};
	} // !slimktx2
} // !ux3d