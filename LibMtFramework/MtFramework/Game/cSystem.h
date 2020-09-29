

#pragma once
#include "LibMtFramework.h"
#include "MtFramework/MtObject.h"

// sizeof = 0x38
struct cSystem : public MtObject
{
    /* 0x08 */ CRITICAL_SECTION     Lock;
    /* 0x30 */ BOOL                 IgnoreGlobalLock;

    /*
        VTable:
            0x28 void SystemCleanup()
            0x30 void SystemUpdate() systems registered with sSnatcherMain call this once per message loop after the next frame has been drawn
            0x38 void BuildSystemMenu(MtPropertyList *pPropertyList)
    */

    inline static cSystem * (__stdcall *_ctor)(cSystem *thisptr) =
        GetModuleAddress<cSystem*(__stdcall*)(cSystem*)>(0x140643CE0);

    inline static cSystem * (__stdcall *_dtor)(cSystem *thisptr, bool bFreeMemory) =
        GetModuleAddress<cSystem*(__stdcall*)(cSystem*, bool)>(0x140643D40);

    inline static MtDTI * (__stdcall *_GetDTI)(cSystem *thisptr) =
        GetModuleAddress<MtDTI*(__stdcall*)(cSystem*)>(0x140643DA0);

    IMPLEMENT_MYDTI(cSystem, 0x141CF2A58, 0x1400AF010, 0x140643DB0);

    cSystem()
    {
        _ctor(this);
    }

    ~cSystem()
    {
        ThisPtrCallNoFixup<void, bool>(this->vtable[0], this, false);
    }

    /*
        Description: Called during game shutdown to free resources.
    */
    void SystemCleanup()
    {
        ThisPtrCallNoFixup<void>(this->vtable[5], this);
    }

    /*
        Description: Called once per frame for the system to update.
    */
    void SystemUpdate()
    {
        ThisPtrCallNoFixup<void>(this->vtable[6], this);
    }

    /*
        Description: Called when the sysmenu for this system has been opened.

        Parameters:
            - pPropertyList: Property list to add debug menu options to
    */
    void BuildSystemMenu(MtPropertyList *pPropertyList)
    {
        ThisPtrCallNoFixup<void, MtPropertyList*>(this->vtable[7], this, pPropertyList);
    }
};
static_assert(sizeof(cSystem) == 0x38, "cSystem incorrect struct size");