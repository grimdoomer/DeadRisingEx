
#pragma once
#include "LibMtFramework.h"
#include "MtFramework/Item/uItem.h"

// Grenade
// sizeof = 0x3740
struct uOm23 : public uItem
{
    inline static void **_vtable = (void**)GetModuleAddress(0x140CB2828);

    inline static uOm23 * (__stdcall *_ctor)(uOm23 *thisptr) =
        (uOm23*(__stdcall*)(uOm23*))GetModuleAddress(0x1400699A0);

    inline static uOm23 * (__stdcall *_dtor)(uOm23 *thisptr, bool bFreeMemory) =
        (uOm23*(__stdcall*)(uOm23*, bool))GetModuleAddress(0x14006DAC0);

    IMPLEMENT_MYDTI(uOm23, 0x14194CCB0, 0x1400AF010, 0x140079E70);
};
