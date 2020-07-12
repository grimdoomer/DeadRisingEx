/*

*/

#include "Utilities.h"
#include "Misc/AsmHelpers.h"
#include "detours.h"

// Function pointers/trampolines:
void(__stdcall *DbgPrintError)(void **pPrintFunctions, const char *psFormat, __int64 arg1, __int64 arg2) = (void(__stdcall*)(void**, const char*, __int64, __int64))GetModuleAddress<__int64>((void*)0x14002F6D0);

// Forward declarations:
void __stdcall Hook_DbgPrintError(void **pPrintFunctions, const char *psFormat, __int64 arg1, __int64 arg2);

bool Utilities::InstallHooks()
{
	// Hook debug print functions.
	DetourAttach((void**)&DbgPrintError, Hook_DbgPrintError);

	return true;
}

void __stdcall Hook_DbgPrintError(void **pPrintFunctions, const char *psFormat, __int64 arg1, __int64 arg2)
{
	char sMessage[512] = { 0 };

	// Format the error message.
	snprintf(sMessage, sizeof(sMessage), psFormat, arg1, arg2);

	// TODO: Print to console

	// Print to the debugger.
	OutputDebugStringA(sMessage);
}