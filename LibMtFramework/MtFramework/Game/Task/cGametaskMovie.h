
#pragma once
#include "cGametask.h"

// sizeof = 0x70
struct cGametaskMovie : public cGametask
{
    /* 0x40 */

    /*
        VTable:
            0x40
            0x48
    */

    inline static cGametaskMovie* (__stdcall *_ctor)(cGametaskMovie* thisptr) =
        (cGametaskMovie*(__stdcall*)(cGametaskMovie*))GetModuleAddress(0x14020B250);

    inline static cGametaskMovie * (__stdcall *_dtor)(cGametaskMovie *thisptr, bool bFreeMemory) =
        (cGametaskMovie*(__stdcall*)(cGametaskMovie*, bool))GetModuleAddress(0x14020B340);

    inline static MtDTI * (__stdcall *_GetDTI)(cGametaskMovie *thisptr) =
        (MtDTI*(__stdcall*)(cGametaskMovie*))GetModuleAddress(0x14020B380);

    IMPLEMENT_MYDTI(cGametaskMovie, 0x141951E30, 0x1400AF010, 0x14020B6C0);

    cGametaskMovie()
    {
        _ctor(this);
    }

    ~cGametaskMovie()
    {
        (void)ThisPtrCallNoFixup(this->vtable[0], this, false);
    }
};
