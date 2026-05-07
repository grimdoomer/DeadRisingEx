
#pragma once
#include "MtFramework/MtObject.h"

// sizeof = 0xD0
struct cExplodeData : public MtObject
{
    /* 0x08 */ //void* ?

    /* 0xC0 */ //BYTE

    inline static cExplodeData* (* _ctor)(cExplodeData* thisptr) =
        (cExplodeData * (*)(cExplodeData*))GetModuleAddress(0x14000E1E0);

    inline static cExplodeData* (* _dtor)(cExplodeData* thisptr, bool bFreeMemory) =
        (cExplodeData * (*)(cExplodeData*, bool))GetModuleAddress(0x1400C72B0);

    IMPLEMENT_MYDTI(cExplodeData, 0x141930078, 0x1400AF010, 0x14000EEC0);

    cExplodeData()
    {
        _ctor(this);
    }
};