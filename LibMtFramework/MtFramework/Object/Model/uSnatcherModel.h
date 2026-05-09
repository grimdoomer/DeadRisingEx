
#pragma once
#include "LibMtFramework.h"
#include "MtFramework/Physics/Model/uHavokModel.h"

// sizeof = 0x1390
struct uSnatcherModel : public uHavokModel
{
    /* 0xD60 */

    /* 0xFC0 */ Vector3Aligned     mItemHavingOfs;
    /* 0xFD0 */ Vector3Aligned     mItemHavingAngle;

    /* 0x1000 */ Vector3Aligned    mOld;
    /* 0x1010 */ //cResource *mprSMAdd;

    /* 0x1018 */ // mpSectioning
    /* 0x1020 */ // mpAdhesion

    /* 0x1090 */ //rSoundSnd*

    /* 0x10A0 */ //rSoundSnd*

    /* 0x1248 */ DWORD      mWork;

    /* 0x12D4 */ DWORD      mSynchroFlag;
    /* 0x12D8 */ DWORD      mSynchroTransFlag;

    /* 0x12E4 */ DWORD      mKindAttr;
    /* 0x12E8 */ int        mVitalMax;
    /* 0x12EC */ int        mVitalNew;

    /*
        VTable:
            0xE0
            0xE8
            0xF0
            0xF8
            0x100
            0x108
            0x110
            0x118
            0x120
            0x128
            0x130
            0x138
            0x140
            0x148
            0x150
            0x158
            0x160
            0x168
            0x170
            0x178
            0x180
            0x188
            0x190
            0x198
            0x1A0
            0x1A8
    */

    inline static uSnatcherModel* (* _ctor)(uSnatcherModel* thisptr) =
        (uSnatcherModel * (*)(uSnatcherModel*))GetModuleAddress(0x140145930);

    inline static void* (* _scalar_deleting_dtor)(uSnatcherModel* thisptr, unsigned int flags) =
        (void * (*)(uSnatcherModel*, unsigned int))GetModuleAddress(0x140145F00);

    inline static void (* _LoadAndAssignModel)(uSnatcherModel* thisptr, const char* psFileName) =
        (void(*)(uSnatcherModel*, const char*))GetModuleAddress(0x14014D630);

    IMPLEMENT_MYDTI(uSnatcherModel, 0x141949C20, 0x1400AF010, 0x1401E94B0);

    SHIM_API uSnatcherModel() SHIM_BODY(0x140145930)

    SHIM_API ~uSnatcherModel() SHIM_BODY_DTOR_VCALL()

    /*

    */
    void LoadAndAssignModel(const char* psFileName)
    {
        _LoadAndAssignModel(this, psFileName);
    }

    IMPLEMENT_OPERATOR_NEW_DELETE(g_pUnitHeapAllocator, 32)
};