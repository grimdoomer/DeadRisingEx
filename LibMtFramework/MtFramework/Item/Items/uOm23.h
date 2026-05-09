
#pragma once
#include "LibMtFramework.h"
#include "MtFramework/Item/uItem.h"

// Grenade
// sizeof = 0x3740
struct uOm23 : public uItem
{
    inline static void **_vtable = (void**)GetModuleAddress(0x140CB2828);

    inline static uOm23 * (*_ctor)(uOm23 *thisptr) =
        (uOm23*(*)(uOm23*))GetModuleAddress(0x1400699A0);

    inline static void * (*_dtor)(uOm23* thisptr) =
        (void*(*)(uOm23*))uItem::_dtor;

    inline static void* (*_scalar_deleting_dtor)(uOm23* thisptr, unsigned int flags) =
        (void * (*)(uOm23*, unsigned int))uItem::_scalar_deleting_dtor;

    IMPLEMENT_MYDTI(uOm23, 0x14194CCB0, 0x1400AF010, 0x140079E70);

    SHIM_API uOm23() SHIM_BODY(0x1400699A0)

    SHIM_API ~uOm23() SHIM_BODY_DTOR_VCALL()

    IMPLEMENT_OPERATOR_NEW_DELETE(g_pUnitHeapAllocator, 32)
};
