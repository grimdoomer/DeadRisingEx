
#pragma once
#include "LibMtFramework.h"
#include "MtFramework/MtObject.h"

// sizeof = 0x1390
struct uSnatcherModel : public MtObject // public uHavokModel
{
    /* 0xD60 */

    /* 0xFC0 */ Vector3     mItemHavingOfs;
    /* 0xFD0 */ Vector3     mItemHavingAngle;

    /* 0x1000 */ Vector3    mOld;
    /* 0x1010 */ //MtObject *mprSMAdd;

    /* 0x1018 */ // mpSectioning

    /* 0x1020 */ // mpAdhesion

    /* 0x1248 */ DWORD      mWork;

    /* 0x12D4 */ DWORD      mSynchroFlag;
    /* 0x12D8 */ DWORD      mSynchroTransFlag;

    /* 0x12E4 */ DWORD      mKindAttr;
    /* 0x12E8 */ DWORD      mVitalMax;
    /* 0x12EC */ DWORD      mVitalNew;
};