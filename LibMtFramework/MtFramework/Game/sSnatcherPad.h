
#pragma once
#include "sPad.h"

// sizeof = 0x1BCD8
struct sSnatcherPad : public sPad
{
    inline static void(__stdcall *_SystemUpdate)(sSnatcherPad *thisptr) =
        GetModuleAddress<void(__stdcall*)(sSnatcherPad*)>(0x1400ADD00);
};