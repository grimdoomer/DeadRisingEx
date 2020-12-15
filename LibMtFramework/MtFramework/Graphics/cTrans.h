/*

*/

#pragma once
#include "LibMtFramework.h"
#include "MtFramework/MtObject.h"

static const char *SemanticNames[14] =
{
    "POSITION",
    "BLENDWEIGHT",
    "BLENDINDICES",
    "NORMAL",
    "PSIZE",
    "TEXCOORD",
    "TANGENT",
    "BINORMAL",
    "TESSFACTOR",
    "POSITIONT",
    "COLOR",
    "FOG",
    "DEPTH",
    "SAMPLE"
};

static const char *FormatName[122] =
{
    "UNKNOWN",
    "R32G32B32A32_TYPELESS",
    "R32G32B32A32_FLOAT",
    "R32G32B32A32_UINT",
    "R32G32B32A32_SINT",
    "R32G32B32_TYPELESS",
    "R32G32B32_FLOAT",
    "R32G32B32_UINT",
    "R32G32B32_SINT",
    "R16G16B16A16_TYPELESS",
    "R16G16B16A16_FLOAT",
    "R16G16B16A16_UNORM",
    "R16G16B16A16_UINT",
    "R16G16B16A16_SNORM",
    "R16G16B16A16_SINT",
    "R32G32_TYPELESS",
    "R32G32_FLOAT",
    "R32G32_UINT",
    "R32G32_SINT",
    "R32G8X24_TYPELESS",
    "D32_FLOAT_S8X24_UINT",
    "R32_FLOAT_X8X24_TYPELESS",
    "X32_TYPELESS_G8X24_UINT",
    "R10G10B10A2_TYPELESS",
    "R10G10B10A2_UNORM",
    "R10G10B10A2_UINT",
    "R11G11B10_FLOAT",
    "R8G8B8A8_TYPELESS",
    "R8G8B8A8_UNORM",
    "R8G8B8A8_UNORM_SRGB",
    "R8G8B8A8_UINT",
    "R8G8B8A8_SNORM",
    "R8G8B8A8_SINT",
    "R16G16_TYPELESS",
    "R16G16_FLOAT",
    "R16G16_UNORM",
    "R16G16_UINT",
    "R16G16_SNORM",
    "R16G16_SINT",
    "R32_TYPELESS",
    "D32_FLOAT",
    "R32_FLOAT",
    "R32_UINT",
    "R32_SINT",
    "R24G8_TYPELESS",
    "D24_UNORM_S8_UINT",
    "R24_UNORM_X8_TYPELESS",
    "X24_TYPELESS_G8_UINT",
    "R8G8_TYPELESS",
    "R8G8_UNORM",
    "R8G8_UINT",
    "R8G8_SNORM",
    "R8G8_SINT",
    "R16_TYPELESS",
    "R16_FLOAT",
    "D16_UNORM",
    "R16_UNORM",
    "R16_UINT",
    "R16_SNORM",
    "R16_SINT",
    "R8_TYPELESS",
    "R8_UNORM",
    "R8_UINT",
    "R8_SNORM",
    "R8_SINT",
    "A8_UNORM",
    "R1_UNORM",
    "R9G9B9E5_SHAREDEXP",
    "R8G8_B8G8_UNORM",
    "G8R8_G8B8_UNORM",
    "BC1_TYPELESS",
    "BC1_UNORM",
    "BC1_UNORM_SRGB",
    "BC2_TYPELESS",
    "BC2_UNORM",
    "BC2_UNORM_SRGB",
    "BC3_TYPELESS",
    "BC3_UNORM",
    "BC3_UNORM_SRGB",
    "BC4_TYPELESS",
    "BC4_UNORM",
    "BC4_SNORM",
    "BC5_TYPELESS",
    "BC5_UNORM",
    "BC5_SNORM",
    "B5G6R5_UNORM",
    "B5G5R5A1_UNORM",
    "B8G8R8A8_UNORM",
    "B8G8R8X8_UNORM",
    "R10G10B10_XR_BIAS_A2_UNORM",
    "B8G8R8A8_TYPELESS",
    "B8G8R8A8_UNORM_SRGB",
    "B8G8R8X8_TYPELESS",
    "B8G8R8X8_UNORM_SRGB",
    "BC6H_TYPELESS",
    "BC6H_UF16",
    "BC6H_SF16",
    "BC7_TYPELESS",
    "BC7_UNORM",
    "BC7_UNORM_SRGB",
    "AYUV",
    "Y410",
    "Y416",
    "NV12",
    "P010",
    "P016",
    "420_OPAQUE",
    "YUY2",
    "Y210",
    "Y216",
    "NV11",
    "AI44",
    "IA44",
    "P8",
    "A8P8",
    "B4G4R4A4_UNORM",
    "P208",
    "V208",
    "V408",
    "SAMPLER_FEEDBACK_MIN_MIP_OPAQUE",
    "SAMPLER_FEEDBACK_MIP_REGION_USED_OPAQUE",
    "FORCE_UINT"
};

// sizeof = 0x4B80
struct cTrans
{
    // sizeof = 0x18
    struct Element : public MtObject
    {
        // It seems there are 2 "IsBeingAccessed" values, for what appears to
        // be 2 render calls back to back. Perhaps this is something like
        // double buffering?
        /* 0x08 */ DWORD    mProtect[2];
        /* 0x10 */ DWORD    mRefCount;
        /* 0x14 */ DWORD    Flags;
    };
    static_assert(sizeof(Element) == 0x18, "cTrans::Element incorrect struct size");

    // sizeof = 0x8
    struct VertexDeclElement
    {
        /* 0x00 */ WORD Slot;
        /* 0x02 */ WORD Offset;
        /* 0x04 */ WORD Format;
        /* 0x06 */ BYTE SemanticNameIndex;
        /* 0x07 */ BYTE SemanticIndex;
    };
    static_assert(sizeof(VertexDeclElement) == 8, "VertexDeclElement inccorect struct size");

    // sizeof = 0x30
    struct VertexDecl : public Element
    {
        /* 0x18 */ void                 *Unk1;
        /* 0x20 */ VertexDeclElement    *pElements;        // Element array, last element slot = 0xFF
        /* 0x28 */ DWORD                mElementNum;
        /* 0x2C */ DWORD                Id;                // Checksum? (0x140688ED0)
    };
    static_assert(sizeof(VertexDecl) == 0x30, "cTrans::VertexDecl incorrect struct size");

    // sizeof = 0x30
    struct VertexBuffer : public Element
    {
        /* 0x18 */ void     *pVertexBuffer;        // ID3D11Buffer
        /* 0x20 */ void     *pRawBuffer;
        /* 0x28 */ DWORD    Size;
    };
    static_assert(sizeof(VertexBuffer) == 0x30, "cTrans::VertexBuffer incorrect struct size");

    // sizeof = 0x30
    struct IndexBuffer : public Element
    {
        /* 0x18 */ void     *pIndexBuffer;        // ID3D11Buffer
        /* 0x20 */ void     *pRawBuffer;
        /* 0x28 */ DWORD    Size;
    };
    static_assert(sizeof(IndexBuffer) == 0x30, "cTrans::IndexBuffer incorrect struct size");
};