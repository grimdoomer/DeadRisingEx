
#pragma once
#include "MtFramework/System/cSystem.h"

// (abstract) sizeof = 0x40
struct cGametask : public cSystem
{
    /* 0x38 */ BYTE     mRno[4];        // Menu ids for four levels of menus
    /* 0x3C */ DWORD    Unknown1;

    /*
        VTable:
            0x40
            0x48
    */

    IMPLEMENT_MYDTI(cGametask, 0x141946FE0, 0x1400AF010, ReturnNullPtrFuncAddr);
};
static_assert(sizeof(cGametask) == 0x40, "cGametask incorrect struct size");