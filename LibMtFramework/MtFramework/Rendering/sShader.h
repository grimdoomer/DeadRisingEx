/*

*/

#pragma once
#include "LibMtFramework.h"

// sizeof = 0xB560
struct sShader
{
    // sizeof = 0x30
    struct ParameterDesc
    {
        /* 0x00 */ void     *vtable;
        /* 0x08 */ char     *mName;
        /* 0x10 */ DWORD    mID;
        /* 0x14 */ BYTE     Type;
        /* 0x15 */ BYTE     RegCount;
        /* 0x16 */ BYTE     SomethingIndex;        // Index into sShader->0xA29C
        union
        {
            /* 0x18 */ DWORD    IntValue;
            /* 0x18 */ void     *pValue;
        };

        /*
            0x140692767

            Type = 0: SomethingIndex
            Type = 1: return IntValue != 0
            Type = 2: return IntValue
            Type = 3: return (float)IntValue
            Type = 4: return pValue
        */

        /* 0x20 */ BYTE        Unknown[16];    // Not sure if this is ever used...
    };
    static_assert(sizeof(ParameterDesc) == 0x30, "sShader::ParameterDesc incorrect struct size");

    // sizeof = 0x18
    struct TechniqueDesc
    {
        /* 0x00 */ void     *vtable;
        /* 0x08 */ char     *mName;     // Shader name
        /* 0x10 */ DWORD    mID;        // Hashcode of the shader name
    };
    static_assert(sizeof(TechniqueDesc) == 0x18, "sShader::TechniqueDesc incorrect struct size");
};