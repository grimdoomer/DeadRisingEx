
#pragma once
#include "MtFramework/MtObject.h"
#include "cExplodeData.h"

// sizeof = 0x1A30
struct cExplodeParts : public MtObject
{
    /* 0x08 */
    /* 0x10 */ //cExplodeData [32];
    /* 0x1A00 */

    /* 0x1A10 */ //void*?
    /* 0x1A18 */ //DWORD
    /* 0x1A20 */ //void*?
    /* 0x1A28 */ 
    /* 0x1A2C */ //DWORD

    inline static cExplodeParts* (__stdcall* _ctor)(cExplodeParts* thisptr) =
        (cExplodeParts * (__stdcall*)(cExplodeParts*))GetModuleAddress(0x14000E200);

    inline static cExplodeParts* (__stdcall* _dtor)(cExplodeParts* thisptr, bool bFreeMemory) =
        (cExplodeParts * (__stdcall*)(cExplodeParts*, bool))GetModuleAddress(0x14000E2E0);

    IMPLEMENT_MYDTI(cExplodeParts, 0x1419300B0, 0x1400AF010, 0x14000EF00);

    cExplodeParts()
    {
        _ctor(this);
    }
};