
#pragma once
#include "cUnit.h"

// sizeof = 0xF0
struct uCoord : public cUnit
{
    /* 0x28 */ void*        mpParent;
    /* 0x30 */ int          mParentNo;
    /* 0x34 */ DWORD        mOrder;                // Value determines rotation type for mQuat

    /* 0x40 */ Vector3Aligned       mPos;
    /* 0x50 */ Vector4              mQuat;
    /* 0x60 */ Vector3Aligned       mScale;

    /* 0xB0 */ // Matrix4x4 0x140176B52 some sort of rotation, gets copied to mQuat after XMQuaternionRotationMatrix

    BYTE _padding[0xF0 - 0x70];

    /*
        VTable:
            0x58
            0x60
            0x68
    */

    inline static uCoord* (__stdcall* _ctor)(uCoord* thisptr) =
        (uCoord * (__stdcall*)(uCoord*))GetModuleAddress(0x14063E520);

    inline static uCoord* (__stdcall* _dtor)(uCoord* thisptr, bool bFreeMemory) =
        (uCoord * (__stdcall*)(uCoord*, bool))GetModuleAddress(0x14063E5A0);

    inline static void(__stdcall* _SetRotation)(uCoord* thisptr, Vector4* pRotation) =
        (void(__stdcall*)(uCoord*, Vector4*))GetModuleAddress(0x14063EB90);

    IMPLEMENT_MYDTI(uCoord, 0x141CF28B8, 0x1400AF010, 0x1401E94B0);

    uCoord()
    {
        _ctor(this);
    }

    void SetRotation(Vector4* pRotation)
    {
        _SetRotation(this, pRotation);
    }
};
static_assert(sizeof(uCoord) == 0xF0, "uCoord incorrect struct size");