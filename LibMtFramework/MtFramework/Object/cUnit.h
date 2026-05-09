

#pragma once
#include "LibMtFramework.h"
#include "MtFramework/MtObject.h"
#include "MtFramework/Memory/MtHeapAllocator.h"

// sizeof = 0x28
struct cUnit : public MtObject // abstract
{
    /* 0x08 */
    struct
    {
        /* 00 */ WORD Move : 3;
        /* 03 */ WORD mMoveLine : 7;
        struct
        {
            /* 10 */ WORD Bit10 : 3;
            /* 13 */ WORD Fix : 1;
            /* 14 */ WORD Bit14 : 1;
            /* 15 */ WORD Bit15 : 1;
        } mUnitAttr;
    } mAttr;
    /* 0x0A */ BYTE     mTransMode;
    /* 0x0B */ BYTE     mTransView;

    /* 0x10 */ cUnit    *pBLink;
    /* 0x18 */ cUnit    *pFLink;
    /* 0x20 */ float    mDeltaTime;
    /* 0x24 */ DWORD    mRno;

    /*
        VTable:
            0x28 void LoadUnitResources();
            0x30 RenderFrame() ?
            0x38
            0x40
            0x48
            0x50 const char * GetObjectName();
    */

    inline static cUnit * (*_ctor)(cUnit *thisptr) =
        (cUnit*(*)(cUnit*))GetModuleAddress(0x1406466A0);

    inline static void * (*_scalar_deleting_dtor)(cUnit *thisptr, unsigned int flags) =
        (void*(*)(cUnit*, unsigned int))GetModuleAddress(0x1402178A0);

    inline static const char * (*_GetObjectName)(cUnit *thisptr) =
        (const char*(*)(cUnit*))GetModuleAddress(0x140646C80);

    IMPLEMENT_MYDTI(cUnit, 0x141CF2B78, 0x1400AF010, 0x1401E94B0);

    SHIM_API cUnit() SHIM_BODY(0x1406466A0)

    SHIM_API ~cUnit() SHIM_BODY_DTOR_VCALL()

    /*
        Gets the name of the code class for this item instance.
    */
    const char * GetObjectName()
    {
        return (const char*)ThisPtrCallNoFixup(this->vtable[10], this);
    }

    /*
        Loads resources for the unit and assigns it to a move line?
    */
    void LoadUnitResources()
    {
        (void)ThisPtrCallNoFixup(this->vtable[5], this);
    }

    IMPLEMENT_OPERATOR_NEW_DELETE(g_pUnitHeapAllocator, 32)
};
ASSERT_STRUCT_SIZE(cUnit, 0x28);