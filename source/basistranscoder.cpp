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

bool ux3d::slimktx2::BasisTranscoder::transcode(SlimKTX2& _image, IOHandle _file, TranscodeFormat _targetFormat)
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

    // from libktx:
    //ktxTexture2_TranscodeBasis(ktxTexture2* This,
    //ktx_transcode_fmt_e outputFormat,
    //    ktx_transcode_flags transcodeFlags)

    const bool sRGB = pBlock->header.transferFunction == TransferFunction_SRGB;

    enum alpha_content_e {
        eNone,
        eAlpha,
        eGreen
    };

    alpha_content_e alphaContent = eNone;

    if (pBlock->getSampleCount() == 2)
    {
        if (pBlock->pSamples[1].channelType == ColorChannels_ETC1S_AAA)
        {
            alphaContent = eAlpha;
        }
        else if (pBlock->pSamples[1].channelType == ColorChannels_ETC1S_GGG)
        {
            alphaContent = eGreen;
        }
        else
        {
            return false;
        }
    }

    // update ktx header with decoded vk format to be able to allocate the right amount of memory
    _image.m_header.vkFormat = transcodeToVkFormat(_targetFormat, sRGB);

    // TODO: check for UASTC

    //assert(colorModel == KHR_DF_MODEL_UASTC);
    //uint32_t channelId = KHR_DFDSVAL(BDB, 0, CHANNELID);
    //if (channelId == KHR_DF_CHANNEL_UASTC_RGBA)
    //    alphaContent = eAlpha;
    //else if (channelId == KHR_DF_CHANNEL_UASTC_RRRG)
    //    alphaContent = eGreen;

    static basist::etc1_global_selector_codebook sel_codebook(basist::g_global_selector_cb_size, basist::g_global_selector_cb);

    basist::basisu_etc1s_image_transcoder bit(&sel_codebook);

    const auto& header = _image.m_basisLZ.header;

    bit.decode_palettes(header.endpointCount, _image.m_basisLZ.pEndpoints, header.endpointsByteLength,
                        header.selectorCount, _image.m_basisLZ.pSelectors, header.selectorsByteLength);

    bit.decode_tables(_image.m_basisLZ.pTables, header.tablesByteLength);

    

    if (_image.allocateMipLevelArray() != Result::Success)
    {
        return false;
    }

    const uint32_t levelCount = _image.getLevelCount();

    for (uint32_t level = levelCount - 1u; level <= levelCount; --level)
    {
        const LevelIndex& lvl = _image.m_pLevels[level];

        // skip to first level
        if (_image.seek(_file, lvl.byteOffset) == false)
        {
            return false;
        }

        //if (_image.read(_file, _image.m_pMipLevelArray[level], lvl.byteLength) == false)
        //{
        //    return false;
        //}
    }

    //uint32_t layerPixelDepth = max(_image.m_header.pixelDepth, 1u);
    //for (uint32_t i = 1; i < m_header.levelCount; ++i)
    //{
    //    layerPixelDepth += max(m_header.pixelDepth >> i, 1u);
    //}

    //return max(m_header.layerCount, 1u) * m_header.faceCount * layerPixelDepth;
    
    // TODO: update DFD with new color model

	return false;
}

