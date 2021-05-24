
#include "DebugLog.h"
#include <detours.h>
#include <Windows.h>

// File handle for debug log:
HANDLE hLogFile = INVALID_HANDLE_VALUE;

bool DebugLog::Initialize()
{
    CHAR sLogFilePath[MAX_PATH] = { 0 };

    // Format the log file path.
    snprintf(sLogFilePath, sizeof(sLogFilePath), "%s\\DebugLog.txt", ModConfig::Instance()->GameDirectory);

    // Create the debug log file.
    hLogFile = CreateFile(sLogFilePath, GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hLogFile == INVALID_HANDLE_VALUE)
    {
        // Failed to create the log file.
        ImGuiConsole::Instance()->ConsolePrint(L"DebugLog::InstallHooks(): failed to create log file %d\n", GetLastError());
        return false;
    }
}

void DebugLog::WriteMessage(LPCSTR lpOutputString)
{
    DWORD BytesWritten = 0;

    // Write the debug message to the log file.
    WriteFile(hLogFile, lpOutputString, lstrlenA(lpOutputString), &BytesWritten, NULL);

    // Flush to file.
    FlushFileBuffers(hLogFile);
}