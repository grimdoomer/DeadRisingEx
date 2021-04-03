
#pragma once
#include "MtFramework/Archive/cResource.h"
#include "MtFramework/Rendering/cTrans.h"

// sizeof = 0x120
struct rTexture : public cResource
{
    /* 0x60 */ float    CubePoints[27];     // Not sure if this is ever used or what exactly it is
    /* 0xCC */ // padding
    /* 0xD0 */ Vector4  Range;
    /* 0xE0 */ Vector4  *pRangeUsed;        // Range vector in use, if no range vector is specified in the texture the vector (1,1,1,0) is used

    /* 0xEC */ //BYTE
    /* 0xED */ BYTE		Type;	// TextureType enum
    /* 0xEE */ BYTE		Attr;	// Flags from rTexture header
    /* 0xEF */ BYTE		Level;
    /* 0xF0 */ DWORD	Width;
    /* 0xF4 */ DWORD	Height;
    /* 0xF8 */ DWORD	Depth;
    /* 0xFC */ DWORD	Format;	// DXGI_FORMAT?
    /* 0x100 */ cTrans::Texture *pTransTexture;

    /* 0x110 */ float UVScaleWidth;     // 1.0f / Width
    /* 0x114 */ float UVScaleHeight;    // 1.0f / Height

    IMPLEMENT_MYDTI(rTexture, 0x141CF2AE0, 0x1400AF010, 0x1406462E0);
};
