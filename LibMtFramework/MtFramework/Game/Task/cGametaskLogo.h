
#pragma once
#include "cGametask.h"

// sizeof = 0x68
struct cGametaskLogo : public cGametask
{
    /* 0x40 */

    /*
        VTable:
            0x40
            0x48
    */

    inline static cGametaskLogo* (__stdcall *_ctor)(cGametaskLogo* thisptr) =
        (cGametaskLogo*(__stdcall*)(cGametaskLogo*))GetModuleAddress(0x1401FC060);

    inline static cGametaskLogo * (__stdcall *_dtor)(cGametaskLogo *thisptr, bool bFreeMemory) =
        (cGametaskLogo*(__stdcall*)(cGametaskLogo*, bool))GetModuleAddress(0x1401FC130);

    inline static MtDTI * (__stdcall *_GetDTI)(cGametaskLogo *thisptr) =
        (MtDTI*(__stdcall*)(cGametaskLogo*))GetModuleAddress(0x1401FC170);

    IMPLEMENT_MYDTI(cGametaskLogo, 0x141951D48, 0x1400AF010, 0x1401FC4A0);

    cGametaskLogo()
    {
        _ctor(this);
    }

    ~cGametaskLogo()
    {
        (void)ThisPtrCallNoFixup(this->vtable[0], this, false);
    }
};
