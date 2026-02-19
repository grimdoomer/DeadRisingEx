/*

*/

#pragma once
#include "LibMtFramework.h"
#include "MtFramework/System/cSystem.h"
#include "MtFramework/Graphics/rTexture.h"

// sizeof = 0xB560
struct sShader : public cSystem
{
    // sizeof = 0x30
    struct ParameterDesc : public MtObject
    {
        /* 0x08 */ char     *mName;
        /* 0x10 */ DWORD    mID;
        /* 0x14 */ BYTE     Type;
        /* 0x15 */ BYTE     RegCount;
        /* 0x16 */ BYTE     SomethingIndex;        // Index into sShader->0xA29C, I think these are sampler states?
        union
        {
            /* 0x18 */ DWORD    IntValue;
            /* 0x18 */ void     *pValue;
        };

        /*
            0x140692767

            Type = 0: SomethingIndex // sampler state index?
			Type = 1: return IntValue != 0
			Type = 2: return IntValue
			Type = 3: return (float)IntValue
			Type = 4: return pValue // vector
			Type = 5: array
        */

        /* 0x20 */ BYTE        Unknown[16];    // Not sure if this is ever used...
    };
    static_assert(sizeof(ParameterDesc) == 0x30, "sShader::ParameterDesc incorrect struct size");

    // sizeof = 0x18
    struct TechniqueDesc : public MtObject
    {
        /* 0x08 */ char     *mName;     // Shader name
        /* 0x10 */ DWORD    mID;        // Hashcode of the shader name
    };
    static_assert(sizeof(TechniqueDesc) == 0x18, "sShader::TechniqueDesc incorrect struct size");

    /* 0x38 */ float                mDistanceFogStart;
    /* 0x3C */ float                mDistanceFogEnd;
    /* 0x40 */ float                mHeightFogStart;
    /* 0x44 */ float                mHeightFogEnd;

    /* 0x50 */ Vector4              mFogColor;

    /* 0x70 */ float                mLightMapLerp;
    /* 0x74 */ signed int           mDebugView;

    /* 0x298 */ ParameterDesc       mParameter[512];
    /* 0x6298 */ TechniqueDesc      mTechnique[512];
    /* 0x9298 */ // 0x1000 bytes
    /* 0xA298 */

    /* 0xB298 */ DWORD              mParameterNum;
    /* 0xB29C */ DWORD              mTechniqueNum;

    /* 0xB4D0 */ rTexture*          pNullWhiteTexture;
    /* 0xB4D8 */ rTexture*          pNullNormalTexture;
    /* 0xB4E0 */ rTexture*          pNullBlackTexture;
    /* 0xB4E8 */ rTexture*          mpEnvTexture;
    /* 0xB4F0 */ rTexture*          pFontTexture;

    /* 0xB548 */ BOOL               mDisableBaseMap;
    /* 0xB549 */ BOOL               mDisableNormalMap;
    /* 0xB54A */ BOOL               mDisableMaskMap;
    /* 0xB54B */ BOOL               mDisableLightMap;
    /* 0xB54C */ BOOL               mDisableShadowMap;
    /* 0xB54D */ BOOL               mDisableEnvMap;
    /* 0xB550 */ Vector4            mTestParam;

    IMPLEMENT_SINGLETON(sShader, 0x141D179A0);
};