
#pragma once
#include "DeadRisingEx.h"

class MtHeapAllocatorImpl
{
public:
    static void RegisterTypeInfo();
    static bool InstallHooks();
};