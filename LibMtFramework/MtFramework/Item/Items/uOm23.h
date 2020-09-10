
#pragma once
#include "LibMtFramework.h"
#include "MtFramework/Item/uItem.h"

// Grenade
// sizeof = 0x3740
struct uOm23 : public uItem
{
    inline static void **_vtable = GetModuleAddress<void**>(0x140CB2828);

    inline static uOm23 * (__stdcall *_ctor)(uOm23 *thisptr) =
        GetModuleAddress<uOm23*(__stdcall*)(uOm23*)>(0x1400699A0);

    inline static uOm23 * (__stdcall *_dtor)(uOm23 *thisptr, bool bFreeMemory) =
        GetModuleAddress<uOm23*(__stdcall*)(uOm23*, bool)>(0x14006DAC0);

    IMPLEMENT_MYDTI(uOm23, 0x14194CCB0, 0x1400AF010, 0x140079E70);
};
