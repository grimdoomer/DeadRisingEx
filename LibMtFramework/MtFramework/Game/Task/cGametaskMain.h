
#pragma once
#include "cGametask.h"

// sizeof = 0x9C20
struct cGametaskMain : public cGametask
{
    /* 0x40 */

    /* 0x14C */ DWORD   mTimer;

    /* 0x150 */ DWORD   mGameFlowNo;
    /* 0x154 */ DWORD   mEmSetCnt;

    /* 0x182 */ BYTE    mBack_menu_disp_flag;
    /* 0x183 */ BYTE    mAttention_flag;

    /* 0x8340 */ INT    mShowTimer;
    /* 0x8344 */ DWORD  mShowTimerLimit;

    /* 0x8390 */ DWORD  mMessageCount;

    /* 0x8B10 */ Vector3    mNpcZombiePos;
    /* 0x8B20 */ Vector3    mNpcZombieAngle;
    /* 0x8B30 */ Vector3    mEventTargetCameraOffs;
    /* 0x8B40 */ Vector3    mEventTargetPosOffs;

    /*
        VTable:
            0x40
            0x48
    */

    inline static cGametaskMain* (__stdcall *_ctor)(cGametaskMain* thisptr) =
        (cGametaskMain*(__stdcall*)(cGametaskMain*))GetModuleAddress(0x1401FC5F0);

    inline static cGametaskMain * (__stdcall *_dtor)(cGametaskMain *thisptr, bool bFreeMemory) =
        (cGametaskMain*(__stdcall*)(cGametaskMain*, bool))GetModuleAddress(0x1401FD6E0);

    inline static MtDTI * (__stdcall *_GetDTI)(cGametaskMain *thisptr) =
        (MtDTI*(__stdcall*)(cGametaskMain*))GetModuleAddress(0x140203000);

    IMPLEMENT_MYDTI(cGametaskMain, 0x141951DB8, 0x1400AF010, 0x1402045B0);

    cGametaskMain()
    {
        _ctor(this);
    }

    ~cGametaskMain()
    {
        (void)ThisPtrCallNoFixup(this->vtable[0], this, false);
    }
};