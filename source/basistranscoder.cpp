// Copyright (c) 2020 UX3D GmbH. All rights reserved.

#include "basistranscoder.h"

#include <basisu_transcoder.h>

ux3d::slimktx2::BasisTranscoder::BasisTranscoder()
{
	basist::basisu_transcoder_init();
}

ux3d::slimktx2::BasisTranscoder::~BasisTranscoder()
{
}

bool ux3d::slimktx2::BasisTranscoder::transcode(SlimKTX2& _image)
{
    static basist::etc1_global_selector_codebook sel_codebook(basist::g_global_selector_cb_size, basist::g_global_selector_cb);

    basist::basisu_transcoder bit(&sel_codebook);
    //basist::basisu_etc1s_image_transcoder bit(&sel_codebook);

    //bit.decode_palettes(bgdh.endpointCount, BGD_ENDPOINTS_ADDR(bgd, imageCount),
    //    bgdh.endpointsByteLength,
    //    bgdh.selectorCount, BGD_SELECTORS_ADDR(bgd, bgdh, imageCount),
    //    bgdh.selectorsByteLength);

    //bit.decode_tables(BGD_TABLES_ADDR(bgd, bgdh, imageCount),
    //    bgdh.tablesByteLength);

	return false;
}

