// Copyright (c) 2020 UX3D GmbH. All rights reserved.

#include "basistranscoder.h"
#include <basisu_transcoder.h>
#include "slimktx2.h"

ux3d::slimktx2::BasisTranscoder::BasisTranscoder()
{
	basist::basisu_transcoder_init();
}

ux3d::slimktx2::BasisTranscoder::~BasisTranscoder()
{
}

bool ux3d::slimktx2::BasisTranscoder::transcode(SlimKTX2& _image, IOHandle _file)
{
    auto* pBlock = _image.getDFD().pBlocks;

    if (pBlock == nullptr)
    {
        return false;
    }

    // currently only work with ETC1S
    if (pBlock->header.colorModel != ColorModel_ETC1S)
    {
        return false;
    }

    static basist::etc1_global_selector_codebook sel_codebook(basist::g_global_selector_cb_size, basist::g_global_selector_cb);

    basist::basisu_etc1s_image_transcoder bit(&sel_codebook);

    const auto& header = _image.m_basisLZ.header;

    bit.decode_palettes(header.endpointCount, _image.m_basisLZ.pEndpoints, header.endpointsByteLength,
                        header.selectorCount, _image.m_basisLZ.pSelectors, header.selectorsByteLength);

    bit.decode_tables(_image.m_basisLZ.pTables, header.tablesByteLength);

	return false;
}

