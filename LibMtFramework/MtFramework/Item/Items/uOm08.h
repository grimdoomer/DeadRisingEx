
#pragma once
#include "LibMtFramework.h"
#include "MtFramework/Item/uItem.h"

// Zucchini
// sizeof = 0x3740
struct uOm08 : public uItem
{
    inline static void ** _vtable = (void**)GetModuleAddress(0x140C96988);

    inline static uOm08 * (*_ctor)(uOm08 *thisptr) =
        (uOm08*(*)(uOm08*))GetModuleAddress(0x1400683E0);

    inline static void * (*_dtor)(uOm08* thisptr) = 
        (void*(*)(uOm08*))uItem::_dtor;

    inline static void * (*_scalar_deleting_dtor)(uOm08 *thisptr, unsigned int flags) = 
        (void * (*)(uOm08*, unsigned int))uItem::_scalar_deleting_dtor;

    IMPLEMENT_MYDTI(uOm08, 0x14194CFB0, 0x1400AF010, 0x140078EA0);

    SHIM_API uOm08() SHIM_BODY(0x1400683E0)

    SHIM_API ~uOm08() SHIM_BODY_DTOR_VCALL()

    IMPLEMENT_OPERATOR_NEW_DELETE(g_pUnitHeapAllocator, 32)
};