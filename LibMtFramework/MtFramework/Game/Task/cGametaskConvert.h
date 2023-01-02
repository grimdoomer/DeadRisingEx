
#pragma once
#include "cGametask.h"

// sizeof = 0x198
struct cGametaskConvert : public cGametask
{
    /* 0x40 */

    /*
        VTable:
            0x40
            0x48
    */

    inline static cGametaskConvert* (__stdcall *_ctor)(cGametaskConvert* thisptr) =
        (cGametaskConvert*(__stdcall*)(cGametaskConvert*))GetModuleAddress(0x1401F9960);

    inline static cGametaskConvert * (__stdcall *_dtor)(cGametaskConvert *thisptr, bool bFreeMemory) =
        (cGametaskConvert*(__stdcall*)(cGametaskConvert*, bool))GetModuleAddress(0x1401F9A00);

    inline static MtDTI * (__stdcall *_GetDTI)(cGametaskConvert *thisptr) =
        (MtDTI*(__stdcall*)(cGametaskConvert*))GetModuleAddress(0x1401F9C50);

    IMPLEMENT_MYDTI(cGametaskConvert, 0x141951AB0, 0x1400AF010, 0x1401FA510);

    cGametaskConvert()
    {
        _ctor(this);
    }

    ~cGametaskConvert()
    {
        (void)ThisPtrCallNoFixup(this->vtable[0], this, false);
    }
};
