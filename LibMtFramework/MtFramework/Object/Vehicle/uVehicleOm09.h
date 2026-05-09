
#pragma once
#include "uVehicle.h"

// sizeof = 0x3380
struct uVehicleOm09 : public uVehicle
{
    /* 0x32A0 */

    inline static void** _vtable = (void**)GetModuleAddress(0x140D019B8);

    inline static uVehicleOm09* (* _ctor)(uVehicleOm09* thisptr) =
        (uVehicleOm09 * (*)(uVehicleOm09*))GetModuleAddress(0x1401C56C0);

    inline static void* (* _dtor)(uVehicleOm09* thisptr, unsigned int flags) =
        (void * (*)(uVehicleOm09*, unsigned int))GetModuleAddress(0x1401C5990);

    IMPLEMENT_MYDTI(uVehicleOm09, 0x141950910, 0x1400AF010, 0x1401C62F0);

    SHIM_API uVehicleOm09() SHIM_BODY(0x1401C56C0)

    SHIM_API ~uVehicleOm09() SHIM_BODY_DTOR_VCALL()

    IMPLEMENT_OPERATOR_NEW_DELETE(g_pUnitHeapAllocator, 32)
};