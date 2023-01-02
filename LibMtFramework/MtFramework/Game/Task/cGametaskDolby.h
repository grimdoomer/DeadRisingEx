
#pragma once
#include "cGametask.h"

// sizeof = 0x58
struct cGametaskDolby : public cGametask
{
    /* 0x40 */

    /*
        VTable:
            0x40
            0x48
    */

    inline static cGametaskDolby* (__stdcall *_ctor)(cGametaskDolby* thisptr) =
        (cGametaskDolby*(__stdcall*)(cGametaskDolby*))GetModuleAddress(0x1401FAEE0);

    inline static cGametaskDolby * (__stdcall *_dtor)(cGametaskDolby *thisptr, bool bFreeMemory) =
        (cGametaskDolby*(__stdcall*)(cGametaskDolby*, bool))GetModuleAddress(0x1401FAF20);

    inline static MtDTI * (__stdcall *_GetDTI)(cGametaskDolby *thisptr) =
        (MtDTI*(__stdcall*)(cGametaskDolby*))GetModuleAddress(0x1401FAFB0);

    IMPLEMENT_MYDTI(cGametaskDolby, 0x141951B90, 0x1400AF010, 0x1401FB240);

    cGametaskDolby()
    {
        _ctor(this);
    }

    ~cGametaskDolby()
    {
        (void)ThisPtrCallNoFixup(this->vtable[0], this, false);
    }
};
