
#pragma once
#include "cGametask.h"

// sizeof = 0x88
struct cGametaskInit : public cGametask
{
    /* 0x40 */

    /*
        VTable:
            0x40
            0x48
    */

    inline static cGametaskInit* (__stdcall *_ctor)(cGametaskInit* thisptr) =
        (cGametaskInit*(__stdcall*)(cGametaskInit*))GetModuleAddress(0x1401FB4B0);

    inline static cGametaskInit * (__stdcall *_dtor)(cGametaskInit *thisptr, bool bFreeMemory) =
        (cGametaskInit*(__stdcall*)(cGametaskInit*, bool))GetModuleAddress(0x1401FB5B0);

    inline static MtDTI * (__stdcall *_GetDTI)(cGametaskInit *thisptr) =
        (MtDTI*(__stdcall*)(cGametaskInit*))GetModuleAddress(0x1401FBA40);

    IMPLEMENT_MYDTI(cGametaskInit, 0x141951C68, 0x1400AF010, 0x1401FBC50);

    cGametaskInit()
    {
        _ctor(this);
    }

    ~cGametaskInit()
    {
        (void)ThisPtrCallNoFixup(this->vtable[0], this, false);
    }
};
