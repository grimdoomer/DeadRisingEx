

#pragma once
#include "LibMtFramework.h"
#include "MtFramework/MtObject.h"

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
    /* 0x20 */ DWORD    mDeltaTime; // float?
    /* 0x24 */ DWORD    mRno;

    /*
        VTable:
            0x28
            0x30 RenderFrame() ?
            0x38
            0x40
            0x48
            0x50 const char * GetObjectName();
    */

    inline static cUnit * (__stdcall *_ctor)(cUnit *thisptr) =
        (cUnit*(__stdcall*)(cUnit*))GetModuleAddress(0x1406466A0);

    inline static cUnit * (__stdcall *_dtor)(cUnit *thisptr, bool bFreeMemory) =
        (cUnit*(__stdcall*)(cUnit*, bool))GetModuleAddress(0x1402178A0);

    inline static const char * (__stdcall *_GetObjectName)(cUnit *thisptr) =
        (const char*(__stdcall*)(cUnit*))GetModuleAddress(0x140646C80);

    IMPLEMENT_MYDTI(cUnit, 0x141CF2B78, 0x1400AF010, 0x1401E94B0);

    cUnit()
    {
        _ctor(this);
    }

    ~cUnit()
    {
        (void)ThisPtrCallNoFixup(this->vtable[0], this, false);
    }

    /*
        Gets the name of the code class for this item instance.
    */
    const char * GetObjectName()
    {
        return (const char*)ThisPtrCallNoFixup(this->vtable[10], this);
    }
};
static_assert(sizeof(cUnit) == 0x28, "cUnit incorrect struct size");