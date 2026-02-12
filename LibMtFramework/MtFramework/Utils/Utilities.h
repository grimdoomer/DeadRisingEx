/*

*/

#pragma once
#include "LibMtFramework.h"
#include "Utilities/Module.h"

inline static bool(__stdcall *WaitForEventHelper)(HANDLE *pEvent, DWORD timeout) = 
    (bool(__stdcall*)(HANDLE*, DWORD))GetModuleAddress(0x1406239D0);

inline static void(__stdcall *SetEventHelper)(HANDLE *pEvent) = 
    (void(__stdcall*)(HANDLE*))GetModuleAddress(0x1406239B0);

class Utilities
{
public:
    static bool InstallHooks();
};