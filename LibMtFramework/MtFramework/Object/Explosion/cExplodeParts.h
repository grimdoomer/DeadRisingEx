
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

    inline static cExplodeParts* (* _ctor)(cExplodeParts* thisptr) =
        (cExplodeParts * (*)(cExplodeParts*))GetModuleAddress(0x14000E200);

    inline static void* (* _scalar_deleting_dtor)(cExplodeParts* thisptr, unsigned int) =
        (void * (*)(cExplodeParts*, unsigned int))GetModuleAddress(0x14000E2E0);

    IMPLEMENT_MYDTI(cExplodeParts, 0x1419300B0, 0x1400AF010, 0x14000EF00);

    SHIM_API cExplodeParts() SHIM_BODY(0x14000E200)

    SHIM_API ~cExplodeParts() SHIM_BODY_DTOR_VCALL()

    IMPLEMENT_OPERATOR_NEW_DELETE(g_pResourceHeapAllocator2, 16)
};