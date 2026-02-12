
#pragma once
#include "uVehicle.h"

// sizeof = 0x3380
struct uVehicleOm09 : public uVehicle
{
    /* 0x32A0 */

    inline static void** _vtable = (void**)GetModuleAddress(0x140D019B8);

    inline static uVehicleOm09* (__stdcall* _ctor)(uVehicleOm09* thisptr) =
        (uVehicleOm09 * (__stdcall*)(uVehicleOm09*))GetModuleAddress(0x1401C56C0);

    inline static uVehicleOm09* (__stdcall* _dtor)(uVehicleOm09* thisptr, bool bFreeMemory) =
        (uVehicleOm09 * (__stdcall*)(uVehicleOm09*, bool))GetModuleAddress(0x1401C5990);

    IMPLEMENT_MYDTI(uVehicleOm09, 0x141950910, 0x1400AF010, 0x1401C62F0);

    uVehicleOm09()
    {
        _ctor(this);
    }
};