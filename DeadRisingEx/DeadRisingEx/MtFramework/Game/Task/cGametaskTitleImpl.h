
#pragma once
#include "DeadRisingEx.h"

extern "C" void Hook_cGametaskTitle_UnkUpdateFunc();

class cGametaskTitleImpl
{
public:
    static bool InstallHooks();
};