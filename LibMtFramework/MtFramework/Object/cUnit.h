

#pragma once
#include "LibMtFramework.h"
#include "MtFramework/MtObject.h"

// sizeof = 0x28
struct cUnit : public MtObject // abstract
{
    // This is like an anon struct with bit indices
    /* 0x08 */ //WORD
    /* 0x0A */ BYTE     mTransMode;
    /* 0x0B */ BYTE     mTransView;

    /* 0x20 */ DWORD    mDeltaTime; // float?
    /* 0x24 */ DWORD    mRno;

    /*
        VTable:
            0x28
            0x30
            0x38
            0x40
            0x48
            0x50 const char * GetObjectName();
    */
};