
#pragma once
#include "cGametask.h"

// sizeof = 0x40
struct cGametaskExit : public cGametask
{
    /*
        VTable:
            0x40
            0x48
    */

    inline static cGametaskExit* (__stdcall *_ctor)(cGametaskExit* thisptr) =
        (cGametaskExit*(__stdcall*)(cGametaskExit*))GetModuleAddress(0x1401FB3E0);

    inline static cGametaskExit * (__stdcall *_dtor)(cGametaskExit *thisptr, bool bFreeMemory) =
        (cGametaskExit*(__stdcall*)(cGametaskExit*, bool))GetModuleAddress(0x1401F9A00);

    inline static MtDTI * (__stdcall *_GetDTI)(cGametaskExit *thisptr) =
        (MtDTI*(__stdcall*)(cGametaskExit*))GetModuleAddress(0x1401FB410);

    IMPLEMENT_MYDTI(cGametaskExit, 0x141951C00, 0x1400AF010, 0x1401FB450);

    cGametaskExit()
    {
        _ctor(this);
    }

    ~cGametaskExit()
    {
        (void)ThisPtrCallNoFixup(this->vtable[0], this, false);
    }
};
static_assert(sizeof(cGametaskExit) == 0x40, "cGametaskExit incorrect struct size");