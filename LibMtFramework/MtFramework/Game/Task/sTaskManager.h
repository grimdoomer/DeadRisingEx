
#pragma once
#include "MtFramework/System/cSystem.h"
#include "cGametask.h"

// sizeof = 0x2F088
struct sTaskManager : public cSystem
{
    enum GametaskType : int
    {
        None = 0,
        Init,
        Warning,
        Logo,
        Title,
        Main,
        Demo,
        Movie,
        Load,
        Convert,
        Dolby,
        Exit
    };

    // sizeof = 0x178
    struct unknown
    {
        /* 0x00 */ //DWORD ?
        /* 0x04 */ //DWORD ?
        /* 0x08 */ //DWORD ?
        /* 0x0C */ //DWORD ?s
        /* 0x10 */ //DWORD

        /* 0x28 */ //void* ?

        /* 0x70 */ //DWORD
    };

    /* 0x38 */ GametaskType    mEntryTaskNo;

    /* 0x40 */ //DWORD

    
    /* 0x4C */ //DWORD entry count for array

    /* 0x58 */ //unknown[512];
    /* 0x2F058 */ cGametask* mpTask;

    /* 0x2F068 */ //uGametaskPrim*
    /* 0x2F070 */ //void* ?
    /* 0x2F078 */ //void* ?
    /* 0x2F080 */ //void* ?

    inline static void(__stdcall *_StartNewTask)() = (void(__stdcall*)())GetModuleAddress(0x1400B7730);

    IMPLEMENT_MYDTI(sTaskManager, 0x141947010, 0x1400AF010, 0x1400B7590);

    IMPLEMENT_SINGLETON(sTaskManager, 0x141946FC0);

    /*
        Description: Starts the task specified by mEntryTaskNo
    */
    void StartNewTask()
    {
        _StartNewTask();
    }
};
