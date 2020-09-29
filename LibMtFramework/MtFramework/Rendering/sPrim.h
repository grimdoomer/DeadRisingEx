
#pragma once
#include "LibMtFramework.h"
#include "MtFramework/Game/cSystem.h"
#include "MtFramework/Graphics/cTrans.h"

// sizeof = 0x6118
struct sPrim : public cSystem
{
    // sizeof = 0x70
    struct sPrimSomething : public MtObject
    {
        // VTable: 0x141043630

        /* 0x08 */
        /* 0x0C */ // 
        /* 0x10 */ // void*
        /* 0x18 */ cTrans       *pOwner; //?
        /* 0x20 */ // Vector3/4

        /* 0x34 */ // DWORD

        /* 0x3C */ // DWORD size of primitive in bytes?

        /* 0x58 */ // DWORD
    };

    /* 0x38 */ DWORD        mPrimNum;
    /* 0x3C */ DWORD        mStateChangeNum;
    /* 0x40 */ float        mNearStart;
    /* 0x44 */ float        mNearEnd;
    /* 0x48 */ float        mFarStart;
    /* 0x4C */ float        mFarEnd;
    /* 0x50 */ DWORD        mReductionDist;
    /* 0x54 */ float        mVolumeDepth;
    /* 0x58 */ BOOL         mDynamicReductionControl;
    /* 0x60 */ //sPrimSomething   *p[6];
    /* 0x90 */ // void* size = 0x10 * 0x98
    /* 0x98 */ // DWORD prim buffer size?
    /* 0xA0 */ // void* size = 0x10 * 0x98
    /* 0xA8 */ // void* size = 0x10 * 0x98

    /* 0xB4 */ DWORD        tXfPrimStandardIndex;   // Shader technique index

    /* 0x60C5 */ BOOL       mTrans;
    /* 0x60C8 */ // cTrans::VertexDecl *
};