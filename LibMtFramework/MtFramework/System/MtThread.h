/*

*/

#pragma once
#include <Windows.h>

// sizeof = 0x58
struct MtThread
{
    /*
        VTable:
            0x00 dtor
    */

    /* 0x00 */ void                     **vtable;
    /* 0x08 */ CRITICAL_SECTION         ThreadLock;
    /* 0x30 */ HANDLE                   hThreadHandle;
    /* 0x38 */ LPTHREAD_START_ROUTINE   pThreadRoutine;
    /* 0x40 */ void                     *pThreadArgument;
    /* 0x48 */ BYTE                     Unk1;
    /* 0x4C */ DWORD                    ThreadId;
    /* 0x50 */ BOOL                     ThreadStarted;
};
static_assert(sizeof(MtThread) == 0x58, "MtThread incorrect struct size");