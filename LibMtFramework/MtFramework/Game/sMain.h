
#pragma once
#include "LibMtFramework.h"
#include "MtFramework/System/cSystem.h"
#include "MtFramework/System/MtThread.h"
#include <windows.h>

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
        /* 0x20 */ void*        WorkArgument;           // Argument passed to work items for certain work types
        /* 0x28 */ DWORD        ThreadNumber;
        /* 0x2C */ BYTE         _pad[0xC];
    };
    static_assert(sizeof(WorkerThreadInfo) == 0x38, "sMain::WorkerThreadInfo incorrect struct size");

    // sizeof = 0x10
    struct WorkItem
    {
        /* 0x00 */ void *pObject;               // Common base type?
        /* 0x08 */ void *pWorkRoutine;          // ?
    };
    static_assert(sizeof(WorkItem) == 0x10, "sMain::WorkItem incorrect struct size");

    // sizeof = 0x18
    struct WorkItemWithArgument
    {
        /* 0x00 */ void *pObject;               // Common base type?
        /* 0x08 */ void *pWorkRoutine;          // ?
        /* 0x10 */ void *pArgument;             // Argument passed to the work function
    };
    static_assert(sizeof(WorkItemWithArgument) == 0x18, "sMain::WorkItemWithArgument incorrect struct size");

    // sizeof = 0x50
    struct GameTime
    {
        /*
            PerfFrequency: Normally you would divide PerformanceCounter / PerformanceCounterFrequency, instead the game
                uses will multiply by PerfFrequency which is just 1f / PerformanceCounterFrequency

            FrameCounter: Incremented once each frame until CurrentTimeInSeconds - LastFrameCounterTime > 1.0f, when this
                condition is met FramesPerSecond and LastFrameCounterTime are updated, and FrameCounter is reset to 0
        */

        /* 0x00 */ float        PerfFrequency;                  // Used to convert performance counters to time in seconds
        /* 0x04 */ float        CurrentTimeInSeconds;           // Current time in seconds
        /* 0x08 */ float        DeltaTime1InSeconds;            // Time difference between this frame and the previous frame in seconds
        /* 0x0C */ float        ElapsedTimeInSeconds;           // Time elapsed since the game started in seconds
        /* 0x10 */ float        DeltaTime2InSeconds;            // Time difference between this frame and the previous frame in seconds
        /* 0x14 */ float        LastFrameCounterTime;           // Last time (in seconds) that the fps stat was calculated
        /* 0x18 */ float        FramesPerSecond;                // Frames per second
        /* 0x1C */ DWORD        FrameCounter;                   // Incremented every frame until a whole second has elapsed and then reset to 0
        /* 0x20 */ ULONGLONG    PerfFrequencyCPS;               // Performance counter frequency (ticks per second)
        /* 0x28 */ ULONGLONG    PerformanceCounter;             // Performance counter for the current frame
        /* 0x30 */ ULONGLONG    PreviousPerformanceCounter;     // Performance counter from the previous frame
        /* 0x38 */ ULONGLONG    DeltaTime1;                     // Time difference between this frame and the previous frame
        /* 0x40 */ ULONGLONG    ElapsedTime;                    // Time elapsed since the game started
        /* 0x48 */ ULONGLONG    DeltaTime2;                     // Time difference between this frame and the previous frame
    };
    static_assert(sizeof(GameTime) == 0x50, "sMain::GameTime incorrect struct size");

    /* 0x38 */ ULONGLONG    mTimer;                 // Elapsed time in milliseconds (from start of game)
    /* 0x40 */ ULONGLONG    PreviousPerfCounter;    // Performance counter for the previous update
    /* 0x48 */ DWORD        mFrameTimer;            // Elapsed number of frames since last update?
    /* 0x4C */ BOOL         ShouldQuit;             // Indicates if sMain should break the window message loop and tear down the game
    /* 0x50 */ float        mFps;
    /* 0x54 */ BOOL         mFrameWait;
    /* 0x58 */ float        mActualFps;
    /* 0x5C */ // padding
    /* 0x60 */ ULONGLONG    CurrentFrameNumber;     // Current frame number per 1 second intervals: (PerformanceCounter - LastFpsTime) * mFps
    /* 0x68 */ ULONGLONG    NextFrameTime;          // Time in ms the next frame boundary occurs at
    /* 0x70 */ ULONGLONG    LastFpsTime;            // Performance counter for the last time the frame boundary was hit
    /* 0x78 */ DWORD        SkipCounter;            // 0x140645188 something to do with a timer when the game is paused
    /* 0x7C */ BOOL         mPause;
    /* 0x80 */ float        mDeltaTime;
    /* 0x84 */ DWORD        MainThreadId;
    /* 0x88 */ DWORD        WorkItemsQueued;
    /* 0x8C */ DWORD        WorkItemsProcessed;
    /* 0x90 */ DWORD        WorkType;               // Determines what type of work item type (WorkItem or WorkItemWithArgument) and what arguments to pass to the queued work items
    /* 0x94 */ DWORD        mJobThreadNum;          // Default value = 4
    union
    {
        /* 0x98 */ WorkItem                 WorkItems[0x2000];
        /* 0x98 */ WorkItemWithArgument     WorkItemsWithArgument[0x1555];
    };
    /* 0x20098 */ WorkerThreadInfo  WorkerThreads[6];
    /* 0x201E8 */ ULONGLONG _unused;
    /* 0x201F0 */ GameTime  GameTime;
    /* 0x20240 */ DWORD     Framerate;              // Framerate setting from config.ini, 0 = 30fps, 1 = 60fps, 2 = variable
    /* 0x20244 */

    IMPLEMENT_MYDTI(sMain, 0x141CF2AB0, 0x1400AF010, 0x1406453F0);

    IMPLEMENT_SINGLETON(sMain, 0x141CF2AA0);

    inline static const char *mBuildVersion = (const char*)GetModuleAddress(0x14103CB90);

    inline static HWND *GameWindowHandle = (HWND*)GetModuleAddress(0x141CF2A88);

    inline static LRESULT(__stdcall *_WndProc)(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) =
        (LRESULT(__stdcall*)(HWND, UINT, WPARAM, LPARAM))GetModuleAddress(0x14002BDC0);
};
static_assert(sizeof(sMain) == 0x20248, "sMain incorrect struct size");