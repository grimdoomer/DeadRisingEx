
#pragma once
#include "MtFramework/Object/cUnit.h"

// sizeof = 0xA0
struct uNpcMarker : public cUnit
{
    /* 0x28 */

    inline static void(__stdcall *_Cleanup)(uNpcMarker *thisptr) =
        (void(__stdcall*)(uNpcMarker*))GetModuleAddress(0x1402EB720);
};