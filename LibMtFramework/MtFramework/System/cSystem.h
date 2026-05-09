

#pragma once
#include "LibMtFramework.h"
#include "MtFramework/MtObject.h"
#include "MtFramework/Memory/MtHeapAllocator.h"

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

    inline static cSystem * (*_ctor)(cSystem *thisptr) =
        (cSystem*(*)(cSystem*))GetModuleAddress(0x140643CE0);

    inline static void * (*_scalar_deleting_dtor)(cSystem *thisptr, unsigned int flags) =
        (void*(*)(cSystem*, unsigned int))GetModuleAddress(0x140643D40);

    inline static MtDTI * (*_GetDTI)(cSystem *thisptr) =
        (MtDTI*(*)(cSystem*))GetModuleAddress(0x140643DA0);

    IMPLEMENT_MYDTI(cSystem, 0x141CF2A58, 0x1400AF010, 0x140643DB0);

    SHIM_API cSystem() SHIM_BODY(0x140643CE0)

    SHIM_API ~cSystem() SHIM_BODY_DTOR_VCALL()

    /*
        Description: Called during game shutdown to free resources.
    */
    void SystemCleanup()
    {
        (void)ThisPtrCallNoFixup(this->vtable[5], this);
    }

    /*
        Description: Called once per frame for the system to update.
    */
    void SystemUpdate()
    {
        (void)ThisPtrCallNoFixup(this->vtable[6], this);
    }

    /*
        Description: Called when the sysmenu for this system has been opened.

        Parameters:
            - pPropertyList: Property list to add debug menu options to
    */
    void BuildSystemMenu(MtPropertyList *pPropertyList)
    {
        (void)ThisPtrCallNoFixup(this->vtable[7], this, pPropertyList);
    }

    IMPLEMENT_OPERATOR_NEW_DELETE(g_pSystemHeapAllocator, 16)
};
ASSERT_STRUCT_SIZE(cSystem, 0x38);