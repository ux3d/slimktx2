// Copyright (c) 2020 UX3D GmbH. All rights reserved.

#pragma once

#include "callbacks.h"

namespace ux3d
{
	namespace slimktx2
	{
		// forward decl
		class SlimKTX2;

		class BasisTranscoder
		{
		public:
			BasisTranscoder();
			~BasisTranscoder();

			bool transcode(SlimKTX2& _image, IOHandle _file);

		private:

		};
	}
}