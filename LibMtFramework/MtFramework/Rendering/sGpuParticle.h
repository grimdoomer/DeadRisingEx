/*

*/

#pragma once
#include "LibMtFramework.h"
#include "MtFramework/System/cSystem.h"
#include "MtFramework/Rendering/cTrans.h"

// sizeof = 0xA0
struct sGpuParticle : public cSystem
{
    /* 0x38 */ // DWORD count for array below
    /* 0x40 */ // void* entry size = 0x38
        /* 0x00 */ // cTrans::VertexBuffer *[4]

    /* 0x48 */ // BYTE/BOOL indicates if vertex buffers should be updated
    /* 0x50 */ // void*
    /* 0x58 */ // DWORD cTrans::VertexBuffer index in 0x40 array
    /* 0x5C */ // BYTE/BOOL
    /* 0x60 */ // DWORD
    /* 0x64 */ // BYTE/BOOL
    /* 0x68 */ DWORD                tXfPrimGpuParticleBatchIndex;   // sShader::TechniqueDesc index
    /* 0x70 */ cTrans::VertexDecl   *pVertexDeclaration;
    /* 0x78 */ DWORD                InvViewportSizeIndex;           // sShader::ParameterDesc index
    /* 0x7C */ DWORD                InvTextureSizeIndex;            // sShader::ParameterDesc index
    /* 0x80 */ DWORD                PatternSizeIndex;               // sShader::ParameterDesc index
    /* 0x84 */ DWORD                PatternRowNumIndex;             // sShader::ParameterDesc index
    /* 0x88 */ DWORD                PatternTotalNumIndex;           // sShader::ParameterDesc index
    /* 0x8C */ DWORD                XfPrimBaseLinearSamplerIndex;   // sShader::ParameterDesc index
    /* 0x90 */ DWORD                gXPrimDepthBlendIndex;          // sShader::ParameterDesc index
    /* 0x94 */ DWORD                ViewZIndex;                     // sShader::ParameterDesc index
    /* 0x98 */ DWORD                DepthBlendIndex;                // sShader::ParameterDesc index
};