
#pragma once
#include "cGametask.h"

// sizeof = 0x88
struct cGametaskDemo : public cGametask
{
    /* 0x40 */

    /*
        VTable:
            0x40
            0x48
    */

    inline static cGametaskDemo* (__stdcall *_ctor)(cGametaskDemo* thisptr) =
        (cGametaskDemo*(__stdcall*)(cGametaskDemo*))GetModuleAddress(0x1401FA640);

    inline static cGametaskDemo * (__stdcall *_dtor)(cGametaskDemo *thisptr, bool bFreeMemory) =
        (cGametaskDemo*(__stdcall*)(cGametaskDemo*, bool))GetModuleAddress(0x1401FA7B0);

    inline static MtDTI * (__stdcall *_GetDTI)(cGametaskDemo *thisptr) =
        (MtDTI*(__stdcall*)(cGametaskDemo*))GetModuleAddress(0x1401FA920);

    IMPLEMENT_MYDTI(cGametaskDemo, 0x141951B10, 0x1400AF010, 0x1401FAE60);

    cGametaskDemo()
    {
        _ctor(this);
    }

    ~cGametaskDemo()
    {
        (void)ThisPtrCallNoFixup(this->vtable[0], this, false);
    }
};
