
#pragma once
#include "LibMtFramework.h"
#include "MtFramework/System/cSystem.h"
#include "MtFramework/System/MtThread.h"

// sizeof = 0x20248
struct sMain : public cSystem
{
    // sizeof = 0x38
    struct WorkerThreadInfo
    {
        /* 0x00 */ MtThread     *pThread;
        /* 0x08 */ DWORD        ThreadId;
        /* 0x10 */ HANDLE       hDoWorkEvent;
        /* 0x18 */ HANDLE       hWorkCompletedEvent;
        /* 0x20 */ // void*     argument passed to work items for certain work types
        /* 0x28 */ DWORD        ThreadNumber;
        /* 0x2C */ //DWORD Initialized?
    };

    // sizeof = 0x18
    struct WorkItem
    {
        /* 0x00 */ void *pObject;               // Common base type?
        /* 0x08 */ void *pWorkRoutine;          // ?
        /* 0x10 */ void *pArgument;             // Argument passed to the work function
    };
    static_assert(sizeof(WorkItem) == 0x18, "sMain::WorkItem incorrect struct size");

    /* 0x38 */ ULONGLONG    mTimer;                 // Elapsed time in milliseconds (from start of game)
    /* 0x40 */ ULONGLONG    PreviousPerfCounter;    // Performance counter for the previous update
    /* 0x48 */ DWORD        mFrameTimer;            // Elapsed number of frames since last update?

    /* 0x50 */ float        mFps;
    /* 0x54 */ BOOL         mFrameWait;
    /* 0x58 */ float        mActualFps;

    /* 0x60 */ // ULONGLONG
    /* 0x68 */ // ULONGLONG
    /* 0x70 */ // ULONGLONG
    /* 0x78 */ // DWORD 0x140645188
    /* 0x7C */ BOOL         mPause;
    /* 0x80 */ float        mDeltaTime;
    /* 0x84 */ DWORD        MainThreadId;
    /* 0x88 */ DWORD        WorkItemsQueued;
    /* 0x8C */ DWORD        WorkItemsProcessed;
    /* 0x90 */ // DWORD type of work to do, 0 = process queued work items
    /* 0x94 */ DWORD        mJobThreadNum;      // Default value = 4
    /* 0x98 */ WorkItem     WorkItems[0x1555];
    /* 0x20090 */
    /* 0x20098 */ WorkerThreadInfo  WorkerThreads[6];
    /* 0x201E8 */
    /* 0x201F0 */ // buffer size=0x50
        /* 0x00 */ float        PerfFrequency;      // 1f / PerfFrequencyCPS

        /* 0x20 */ ULONGLONG    PerfFrequencyCPS;
        /* 0x28 */ ULONGLONG    StartingPerfCounter;
        /* 0x30 */ // ULONGLONG perf counter

    /* 0x20240 */ DWORD     Framerate;              // Framerate setting from config.ini, 0 = 30fps, 1 = 60fps, 2 = variable

    IMPLEMENT_MYDTI(sMain, 0x141CF2AB0, 0x1400AF010, 0x1406453F0);

    IMPLEMENT_SINGLETON(sMain, 0x141CF2AA0);

    inline static const char *mBuildVersion = GetModuleAddress<const char*>(0x14103CB90);
};