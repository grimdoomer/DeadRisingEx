
#pragma once
#include "cGametask.h"

// sizeof = 0x40
struct cGametaskLoad : public cGametask
{
    /*
        VTable:
            0x40
            0x48
    */

    inline static cGametaskLoad* (__stdcall *_ctor)(cGametaskLoad* thisptr) =
        (cGametaskLoad*(__stdcall*)(cGametaskLoad*))GetModuleAddress(0x1401FBE70);

    inline static cGametaskLoad * (__stdcall *_dtor)(cGametaskLoad *thisptr, bool bFreeMemory) =
        (cGametaskLoad*(__stdcall*)(cGametaskLoad*, bool))GetModuleAddress(0x1401F9A00);

    inline static MtDTI * (__stdcall *_GetDTI)(cGametaskLoad *thisptr) =
        (MtDTI*(__stdcall*)(cGametaskLoad*))GetModuleAddress(0x1401FBEA0);

    IMPLEMENT_MYDTI(cGametaskLoad, 0x141951CE0, 0x1400AF010, 0x1401FC000);

    cGametaskLoad()
    {
        _ctor(this);
    }

    ~cGametaskLoad()
    {
        (void)ThisPtrCallNoFixup(this->vtable[0], this, false);
    }
};
static_assert(sizeof(cGametaskLoad) == 0x40, "cGametaskLoad incorrect struct size");