/*

*/

#pragma once
#include "LibMtFramework.h"
#include "MtFramework/MtObject.h"
#include <d3d11.h>

struct sPrim;

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
struct cTrans : public MtObject
{
    // sizeof = 0x18
    struct Element : public MtObject
    {
        // Flag values:
        #define ELEMENT_FLAGS_WRITABLE      0x40    // this might not be the correct meaning

        // It seems there are 2 "IsBeingAccessed" values, for what appears to
        // be 2 render calls back to back. Perhaps this is something like
        // double buffering?
        /* 0x08 */ DWORD    mProtect[2];
        /* 0x10 */ DWORD    mRefCount;
        /* 0x14 */ DWORD    Flags;
    };
    static_assert(sizeof(Element) == 0x18, "cTrans::Element incorrect struct size");

    // sizeof = 0x70
    struct TextureBase : public Element
    {
        // Flag values:
        #define TEXTURE_BASE_FLAGS_USE_TEXTURE_SCALE    0x800   // The texture is scaled for the render target view

        /* 0x18 */ ID3D11Resource   *pTexture;      // Interface pointer for ID3D11Texture1D/ID3D11Texture2D/ID3D11Texture3D
        /* 0x20 */ // void* some resource heap allocation
        /* 0x28 */ DWORD    mWidth;
        /* 0x2C */ DWORD    mHeight;
        /* 0x30 */ //DWORD gets set to width on init
        /* 0x34 */ //DWORD gets set to height on init
        /* 0x38 */ DWORD    mDepth;
        /* 0x3C */ DWORD    mLevelCount;
        /* 0x40 */ DWORD    mFormat;
        /* 0x48 */ float    WidthScale;     // Used to scale the texture if it is a render target
        /* 0x4C */ float    HeightScale;    // Used to scale the texture if it is a render target
        /* 0x50 */ DWORD    Usage;          // See D3D11_USAGE
        /* 0x54 */ DWORD    CPUAccess;      // See D3D11_CPU_ACCESS_FLAG
        /* 0x58 */ DWORD    BindFlags;      // See D3D11_BIND_FLAG
        /* 0x5C */ DWORD    SampleCount;    // Number of multisamples per pixel
        /* 0x60 */ ID3D11ShaderResourceView *pShaderResourceView;
        /* 0x68 */ ID3D11DepthStencilView   *pDepthStencilView;
    };

    // sizeof = 0x108 */
    struct Texture : public TextureBase
    {
        /* 0x70 */

        /* 0x78 */ ID3D11RenderTargetView   *pRenderTargetViews[16];    // Render target views, 1 for each mip map
        /* 0xF8 */
    };

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
        /* 0x18 */ ID3D11Buffer     *pVertexBuffer;
        /* 0x20 */ void             *pRawBuffer;
        /* 0x28 */ DWORD            Size;
    };
    static_assert(sizeof(VertexBuffer) == 0x30, "cTrans::VertexBuffer incorrect struct size");

    // sizeof = 0x30
    struct IndexBuffer : public Element
    {
        /* 0x18 */ ID3D11Buffer     *pIndexBuffer;
        /* 0x20 */ void             *pRawBuffer;
        /* 0x28 */ DWORD            Size;
    };
    static_assert(sizeof(IndexBuffer) == 0x30, "cTrans::IndexBuffer incorrect struct size");

    // sizeof = 0x10
    struct RenderCommandInfo
    {
        /* 0x00 */ void     *pCmdBuffer;    // Pointer to the command buffer
        /* 0x08 */ DWORD    Tag;            // Tag value used to sort the commands before processing
    };
    static_assert(sizeof(RenderCommandInfo) == 0x10, "cTrans::RenderCommandInfo incorrect struct size");

    // sizeof = ?
    struct RenderCommandBase
    {
        /* 0x00 */ DWORD    Command;        // Command type
    };
    static_assert(sizeof(RenderCommandBase) == 4, "cTrans::RenderCommandBase incorrect struct size");

    // sizeof = 0x14?
    struct RenderCommand_SetScissorRect : public RenderCommandBase  // 0x140662FAF
    {
        /* 0x04 */ D3D11_RECT   ScissorRect;
    };
    static_assert(sizeof(RenderCommand_SetScissorRect) == 0x14, "cTrans::RenderCommand_SetScissorRect incorrect struct size");

    // sizeof = 0xC?
    struct RenderCommand_2 : public RenderCommandBase   // 0x140662F93
    {
        /* 0x04 */ DWORD    SubCommand;
        /* 0x08 */ DWORD    Value;
    };

    // sizeof = 0x50
    struct RenderCommand_13 : public RenderCommandBase
    {
        /* 0x04 */ // padding
        /* 0x08 */ // void* some kind of shader object

        /* 0x38 */ sPrim    *pPrimOwner;
        /* 0x40 */ // void* rendering function
        /* 0x48 */ // void* some other data buffer
    };

    /* 0x08 */
    /* 0x10 */ // array length 3 size 0x1250, this might not be correct
        /* 0x00 */ // array count=4 entry size=0x10
        /* 0x40 */ // array count=4 entry size=0x10
        /* 0x80 */ // array count=4 entry size=0x10
        /* 0xC0 */ // array count=4 entry size=0x10
        /* 0x100 */ // array count=4 entry size=0x10
        /* 0x140 */ // array count=4 entry size=0x10
        /* 0x180 */ // array count=6 entry size=0x10
        /* 0x1E0 */

        /* 0x200 */ // void*?
        /* 0x208 */ // void*?

    /* 0x3700 */

    /* 0x4960 */ // buffer 0x190 bytes

    /* 0x4AF8 */ // void* some kind of shader object from sShader (0x14068FD33)

    /* 0x4B08 */ // DWORD shader technique index for sShader
    /* 0x4B0C */ // DWORD some kind of flag value for shaders
    /* 0x4B10 */ RenderCommandInfo      *pCommandBuffer;    // Holds render commands to be sorted before processing (buffer is allocated in sRender and reset every frame)
    /* 0x4B18 */ DWORD                  mTagNum;            // Number of RenderCommandInfo entries in the command buffer
    /* 0x4B20 */ void                   *pCommandBufferEnd; // Used to allocate memory backwards for command buffer data (buffer is allocated in sRender and reset every frame)
    /* 0x4B28 */ // void* memory region same as pCommandBuffer
    /* 0x4B30 */ // void* end of region same as pCommandBufferEnd

    IMPLEMENT_MYDTI(cTrans, 0x141CF38B0, 0x1400AF010, 0x14068B4C0);

    inline static cTrans * (__stdcall *_ctor)(cTrans *thisptr) =
        (cTrans*(__stdcall*)(cTrans*))GetModuleAddress(0x140688FD0);
};