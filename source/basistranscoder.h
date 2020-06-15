// Copyright (c) 2020 UX3D GmbH. All rights reserved.

#pragma once

#include "callbacks.h"
#include "format.h"

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

			bool decompress(SlimKTX2& _image, IOHandle _file, TranscodeFormat _targetFormat);

		private:

		};
	}
}