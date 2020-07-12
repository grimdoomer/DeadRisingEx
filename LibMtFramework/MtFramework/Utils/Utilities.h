/*

*/

#pragma once
#include "DRDebugger.h"
#include "Misc/AsmHelpers.h"

inline static bool(__stdcall *WaitForEventHelper)(HANDLE *pEvent, DWORD timeout) = GetModuleAddress<bool(__stdcall*)(HANDLE*, DWORD)>((void*)0x1406239D0);
inline static void(__stdcall *SetEventHelper)(HANDLE *pEvent) = GetModuleAddress<void(__stdcall*)(HANDLE*)>((void*)0x1406239B0);

class Utilities
{
public:
	static bool InstallHooks();
};