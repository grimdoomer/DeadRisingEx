/*

*/

#pragma once
#include "DeadRisingEx.h"

extern "C" void sRender_DrawFrame_Hook_Start();
extern "C" void sRender_DrawFrame_Hook_End();

extern "C" __int64 sRender_CaptureFrame_Start();
extern "C" __int64 sRender_CaptureFrame_End();

class sRenderImpl
{
protected:

public:
    static void RegisterTypeInfo();
    static bool InstallHooks();
};