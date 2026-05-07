/*

*/

#pragma once
#include "LibMtFramework.h"
#include "Utilities/Module.h"

inline static bool(*WaitForEventHelper)(HANDLE *pEvent, DWORD timeout) = 
    (bool(*)(HANDLE*, DWORD))GetModuleAddress(0x1406239D0);

inline static void(*SetEventHelper)(HANDLE *pEvent) = 
    (void(*)(HANDLE*))GetModuleAddress(0x1406239B0);

class Utilities
{
public:
    static bool InstallHooks();
};