
#pragma once
#include "MtFramework/System/cSystem.h"
#include "MtFramework/Utils/MtArray.h"
#include "uSnatcherModel.h"

// sizeof = 0x80D0
struct sSMManager : public cSystem
{
    /* 0x38 */

    /* 0x48 */ //void* 0x80 in size

    /* 0x70 */ // array length = 512, elem size = 0x20
    /* 0x4000 */

    /* 0x40A0 */ DWORD          ModelCount;
    /* 0x40A8 */ uSnatcherModel *pModels[2048];
    /* 0x80A8 */ // float FrameDelay;   // number of frames to stop system updates for "frame hit effect"
    /* 0x80B0 */ // MtArray

    IMPLEMENT_MYDTI(sSMManager, 0x141946AC8, 0x1400AF010, 0x14009F9700);

    IMPLEMENT_SINGLETON(sSMManager, 0x141946AB0);

    inline static void(*_AddModel)(sSMManager *thisptr, uSnatcherModel *pModel) =
        (void(*)(sSMManager*, uSnatcherModel*))GetModuleAddress(0x14009EC10);

    inline static bool(*_RemoveModel)(sSMManager *thisptr, uSnatcherModel *pModel) =
        (bool(*)(sSMManager*, uSnatcherModel*))GetModuleAddress(0x14009F9C0);

    sSMManager()
    {
        // TODO:
        DebugBreak();
    }

    ~sSMManager()
    {
        // TODO:
        DebugBreak();
    }
};