
#pragma once
#include "LibMtFramework.h"
#include "MtFramework/Item/uItem.h"

// Zucchini
// sizeof = 0x3740
struct uOm08 : public uItem
{
    inline static void ** _vtable = GetModuleAddress<void**>(0x140C96988);

    inline static uOm08 * (__stdcall *_ctor)(uOm08 *thisptr) =
        GetModuleAddress<uOm08*(__stdcall*)(uOm08*)>(0x1400683E0);

    inline static uOm08 * (__stdcall *_dtor)(uOm08 *thisptr, bool bFreeMemory) =
        GetModuleAddress<uOm08*(__stdcall*)(uOm08*, bool)>(0x14006DAC0);

    IMPLEMENT_MYDTI(uOm08, 0x14194CFB0, 0x1400AF010, 0x140078EA0);
};