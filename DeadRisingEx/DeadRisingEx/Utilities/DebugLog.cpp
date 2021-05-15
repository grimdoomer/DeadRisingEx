
#include "DebugLog.h"
#include <detours.h>
#include <Windows.h>

// Forward declarations:
void __stdcall Hook_OutputDebugStringA(LPCSTR lpOutputString);

// Trampoline function:
void(__stdcall *pOutputDebugStringA)(LPCSTR lpOutputString) = nullptr;

// File handle for debug log:
HANDLE hLogFile = INVALID_HANDLE_VALUE;

bool DebugLog::InstallHooks()
{
    CHAR sLogFilePath[MAX_PATH] = { 0 };

    // Get the address of OutputDebugStringA in kernel32.
    pOutputDebugStringA = (void(__stdcall *)(LPCSTR lpOutputString))DetourFindFunction("kernel32.dll", "OutputDebugStringA");
    if (pOutputDebugStringA == nullptr)
    {
        // Failed to resolve import.
        wprintf(L"DebugLog::InstallHooks(): failed to find OutputDebugStringA export!\n");
        return false;
    }

    // Format the log file path.
    snprintf(sLogFilePath, sizeof(sLogFilePath), "%s\\DebugLog.txt", ModConfig::Instance()->GameDirectory);

    // Create the debug log file.
    hLogFile = CreateFile(sLogFilePath, GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hLogFile == INVALID_HANDLE_VALUE)
    {
        // Failed to create the log file.
        wprintf(L"DebugLog::InstallHooks(): failed to create log file %d\n", GetLastError());
        return false;
    }

    // Hook the function.
    DetourAttach((void**)&pOutputDebugStringA, Hook_OutputDebugStringA);
}

void __stdcall Hook_OutputDebugStringA(LPCSTR lpOutputString)
{
    DWORD BytesWritten = 0;

    // Write the debug message to the log file.
    WriteFile(hLogFile, lpOutputString, lstrlenA(lpOutputString), &BytesWritten, NULL);

    // Flush to file.
    FlushFileBuffers(hLogFile);
}