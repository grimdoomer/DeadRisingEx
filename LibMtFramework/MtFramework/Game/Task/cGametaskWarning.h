
#pragma once
#include "cGametask.h"

// sizeof = 0xA0
struct cGametaskWarning : public cGametask
{
    /* 0x40 */

    /*
        VTable:
            0x40
            0x48
    */

    inline static cGametaskWarning* (__stdcall *_ctor)(cGametaskWarning* thisptr) =
        (cGametaskWarning*(__stdcall*)(cGametaskWarning*))GetModuleAddress(0x1402116A0);

    inline static cGametaskWarning * (__stdcall *_dtor)(cGametaskWarning *thisptr, bool bFreeMemory) =
        (cGametaskWarning*(__stdcall*)(cGametaskWarning*, bool))GetModuleAddress(0x140211860);

    inline static MtDTI * (__stdcall *_GetDTI)(cGametaskWarning *thisptr) =
        (MtDTI*(__stdcall*)(cGametaskWarning*))GetModuleAddress(0x140211B20);

    IMPLEMENT_MYDTI(cGametaskWarning, 0x141951F10, 0x1400AF010, 0x1402125F0);

    cGametaskWarning()
    {
        _ctor(this);
    }

    ~cGametaskWarning()
    {
        (void)ThisPtrCallNoFixup(this->vtable[0], this, false);
    }
};
