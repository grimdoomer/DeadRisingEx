
#pragma once
#include "sPad.h"

// sizeof = 0x1BCD8
struct sSnatcherPad : public sPad
{
    inline static void(*_SystemUpdate)(sSnatcherPad *thisptr) =
        (void(*)(sSnatcherPad*))GetModuleAddress(0x1400ADD00);
};