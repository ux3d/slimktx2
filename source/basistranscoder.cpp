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
    _targetFormat = TranscodeFormat::ETC1_RGB; // TODO: remove, for debugging only

    if (_targetFormat == TranscodeFormat::UNDEFINED)
    {
        return false;
    }

    const auto targetFormat = static_cast<basist::transcoder_texture_format>(_targetFormat);
    //ktx_uint32_t ktx_transcode_flags; ktx_transcode_flag_bits_e
    const uint32_t transcodeFlags = 0u; // TODO pass flags to transcode()

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

    {
        const auto& header = _image.m_basisLZ.header;
        if (bit.decode_palettes(
            header.endpointCount, _image.m_basisLZ.pEndpoints, header.endpointsByteLength,
            header.selectorCount, _image.m_basisLZ.pSelectors, header.selectorsByteLength) == false)
        {
            return false;
        }

        if (bit.decode_tables(_image.m_basisLZ.pTables, header.tablesByteLength) == false)
        {
            return false;
        }
    }

    if (_image.allocateMipLevelArray() != Result::Success)
    {
        return false;
    }

    const Header& ktx = _image.getHeader();
    const uint32_t levelCount = _image.getLevelCount();
    const uint32_t faceCount = _image.getFaceCount();
    const uint32_t layerCount = _image.getLayerCount();

    const LevelIndex& baseLevel = _image.m_pLevels[0u];
    uint8_t* pLevelData = _image.allocateArray<uint8_t>(baseLevel.byteLength);
    if (pLevelData == nullptr)
    {
        return false;
    }

    //for (uint32_t level = levelCount - 1u, image = 0u; level <= levelCount; --level)
    for(uint32_t level = 0u, image = 0u; level < levelCount; ++level)
    {
        const LevelIndex& lvl = _image.m_pLevels[level];

        const uint64_t faceSize = getFaceSize(ktx.vkFormat, level, ktx.pixelWidth, ktx.pixelHeight, ktx.pixelDepth);

        basist::basisu_image_desc imageDesc(
            basist::basis_tex_format::cETC1S,
            max(1u, ktx.pixelWidth >> level),
            max(1u, ktx.pixelHeight >> level),
            level);

        if (_image.seek(_file, lvl.byteOffset) == false)
        {
            return false;
        }

        // read the whole level array
        if (_image.read(_file, pLevelData, lvl.byteLength) == false)
        {
            return false;
        }

        for (uint32_t layer = 0; layer < layerCount; ++layer)
        {
            for (uint32_t face = 0; face < faceCount; ++face, ++image)
            {
                const auto& basisImg = _image.m_basisLZ.pImageDescs[image];
                imageDesc.m_rgb_byte_offset = basisImg.rgbSliceByteOffset;
                imageDesc.m_rgb_byte_length = basisImg.rgbSliceByteLength;
                imageDesc.m_flags = basisImg.imageFlags;

                // TODO: apha slice offset

                uint8_t* pDecoded = nullptr;
                if (_image.getImage(pDecoded, level, face, layer, static_cast<uint32_t>(faceSize)) != Result::Success)
                {
                    return false;
                }

                if (bit.transcode_image(targetFormat, pDecoded, static_cast<uint32_t>(faceSize), pLevelData, imageDesc, transcodeFlags) == false)
                {
                    return false;
                }
            }
        }
    }

    _image.free(pLevelData);
    
    // TODO: update DFD with new color model

	return false;
}

