
#pragma once
#include "DeadRisingEx.h"

class DebugLog
{
public:
    static bool Initialize();

    static void WriteMessage(LPCSTR lpOutputString);
};
