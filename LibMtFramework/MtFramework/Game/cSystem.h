

#pragma once
#include "LibMtFramework.h"
#include "MtFramework/MtObject.h"

// sizeof = 0x38
struct cSystem : public MtObject
{
    /* 0x08 */ CRITICAL_SECTION     Lock;
    /* 0x30 */ BYTE                 Unknown;

    inline static cSystem * (__stdcall *_ctor)(cSystem *thisptr) =
        GetModuleAddress<cSystem*(__stdcall*)(cSystem*)>(0x140643CE0);

    inline static cSystem * (__stdcall *_dtor)(cSystem *thisptr, bool bFreeMemory) =
        GetModuleAddress<cSystem*(__stdcall*)(cSystem*, bool)>(0x140643D40);

    inline static MtDTI * (__stdcall *_GetDTI)(cSystem *thisptr) =
        GetModuleAddress<MtDTI*(__stdcall*)(cSystem*)>(0x140643DA0);

    IMPLEMENT_MYDTI(cSystem, 0x141CF2A58, 0x1400AF010, 0x140643DB0);
};
static_assert(sizeof(cSystem) == 0x38, "cSystem incorrect struct size");