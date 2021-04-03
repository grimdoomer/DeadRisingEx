/*

*/

#pragma once
#include "LibMtFramework.h"
#include "MtFramework/System/cSystem.h"

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

    /* 0x38 */
};