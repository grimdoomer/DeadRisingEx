
#pragma once
#include "LibMtFramework.h"
#include "MtFramework/MtObject.h"

// sizeof = 0x20
struct MtArray : public MtObject
{
    /* 0x08 */ DWORD    mLength;

    /* 0x10 */ BYTE     mAutoDelete;

    /* 0x18 */ void     **mpArray;
};