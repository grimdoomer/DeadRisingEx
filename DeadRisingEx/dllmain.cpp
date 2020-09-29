// dllmain.cpp : Defines the entry point for the DLL application.
#include <stdio.h>
#include <string>
#include <iostream>
#include <locale>
#include <codecvt>
#include <Windows.h>
#include <shellapi.h>
#include "LibMtFramework.h"
#include <detours.h>
#include "Misc/AsmHelpers.h"
#include "MtFramework/Utils/Utilities.h"
#include "DeadRisingEx/ModConfig.h"
#include "DeadRisingEx/MtFramework/MtObjectImpl.h"
#include "DeadRisingEx/MtFramework/Archive/ArchiveOverlay.h"
#include "DeadRisingEx/MtFramework/Archive/sResourceImpl.h"
#include "DeadRisingEx/MtFramework/Debug/sSnatcherToolImpl.h"
#include "DeadRisingEx/MtFramework/Graphics/rModelImpl.h"
#include "DeadRisingEx/MtFramework/Graphics/sRenderImpl.h"
#include "DeadRisingEx/MtFramework/Graphics/sShaderImpl.h"
#include "DeadRisingEx/MtFramework/Item/uItemImpl.h"
#include "DeadRisingEx/MtFramework/Item/Items/uOm08Impl.h"
#include "DeadRisingEx/MtFramework/Object/sUnitImpl.h"
#include "DeadRisingEx/MtFramework/Player/uPlayerImpl.h"

void SetupConsole()
{
    // Create the console window.
    if (AllocConsole() == FALSE)
    {
        // Failed to create the console window.
        OutputDebugString("Failed to create console window!\n");
        return;
    }

    // Set the window title.
    SetConsoleTitle("DRDebugger");

    // Open input/output streams.
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
}

DWORD __stdcall ProcessConsoleWorker(LPVOID)
{
    // Setup the unicode converter.
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> unicConvert;

    // Loop forever.
    while (true)
    {
        std::string sCommand;
        std::wstring sCommandUnic;
        CommandEntry commandInfo;
        LPWSTR *pArguments;
        int ArgCount;

        // Get the next command and convert it to unicode for parsing.
        wprintf(L">");
        std::getline(std::cin, sCommand);
        sCommandUnic = unicConvert.from_bytes(sCommand);

        // Parse the command string.
        pArguments = CommandLineToArgvW(sCommandUnic.c_str(), &ArgCount);
        if (pArguments != NULL)
        {
            // Make sure there is at least one argument to process.
            if (ArgCount == 0)
                goto CommandEnd;

            // Check if we are accessing a command or local variable.
            if (pArguments[0][0] == '$')
            {

            }
            else
            {
                // Check if a command with the specified name exists.
                if (FindCommand(std::wstring(pArguments[0]), &commandInfo) == false)
                {
                    // No matching command found.
                    wprintf(L"\nUnknown command: %s\n\n", pArguments[0]);
                    goto CommandEnd;
                }
            }

            // Call the command handler.
            commandInfo.pHandlerFunction(&pArguments[1], ArgCount - 1);
            wprintf(L"\n");

        CommandEnd:
            // Free the argument buffer.
            LocalFree(pArguments);
        }

        // Sleep and loop.
        Sleep(50);
    }

    return 0;
}

__declspec(dllexport) void DummyExport()
{
    // Required for detours.
}

bool __declspec(dllexport) LaunchDeadRisingEx(const char *psGameDirectory)
{
    CHAR sGameExe[MAX_PATH];
    CHAR sExDll[MAX_PATH];
    STARTUPINFO StartupInfo = { 0 };
    PROCESS_INFORMATION ProcInfo = { 0 };

    // Initialize the startup info structure.
    StartupInfo.cb = sizeof(STARTUPINFO);

    // Format the file paths for the game exe and ex dll.
    snprintf(sGameExe, sizeof(sGameExe), "%s\\DeadRising.exe", psGameDirectory);
    snprintf(sExDll, sizeof(sExDll), "%s\\DeadRisingEx.dll", psGameDirectory);

    // Build our list of dlls to inject.
    LPCSTR DllsToInject[1] =
    {
        sExDll
    };

    // Create the game process in a suspended state with our dll.
    if (DetourCreateProcessWithDllsA(sGameExe, NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, 
        psGameDirectory, &StartupInfo, &ProcInfo, 1, DllsToInject, NULL) == FALSE)
    {
        // Failed to create the game process.
        return false;
    }

    // Resume the process.
    ResumeThread(ProcInfo.hThread);

    // Close the process and thread handles.
    CloseHandle(ProcInfo.hProcess);
    CloseHandle(ProcInfo.hThread);
    return true;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        CHAR sModulePath[MAX_PATH] = { 0 };
        CHAR sModuleName[32] = { 0 };

        // Set the module handle.
        SnatcherModuleHandle = GetModuleHandle(NULL);

        // Get the name of the exe we are running under.
        GetModuleFileName(GetModuleHandle(NULL), sModulePath, sizeof(sModulePath));
        _splitpath_s(sModulePath, nullptr, 0, nullptr, 0, sModuleName, sizeof(sModuleName), nullptr, 0);

        // Check if we were run from the launcher or the game process.
        if (_stricmp(sModuleName, "DeadRisingLauncher") == 0)
        {
            // Bail out as we are not in the game process.
            return TRUE;
        }

        // Load the mod config file.
        if (ModConfig::Instance()->LoadConfigFile("DeadRisingEx.ini") == false)
        {
            // Failed to load the mod config.
            DbgPrint("Failed to load mod config file, using default settings!\n");
        }

        // Register all commands.
        RegisterCommands(g_CommandManagerCommands, g_CommandManagerCommandsLength);
        RegisterCommands(g_TypeInfoCommands, g_TypeInfoCommandsLength);

        // Register built in types.
        RegisterTypeInfo(&Vector3TypeInfo);
        RegisterTypeInfo(&Vector4TypeInfo);
        RegisterTypeInfo(&Matrix4x4TypeInfo);

        // Register types and commands.
        MtObjectImpl::RegisterTypeInfo();
        sSnatcherToolImpl::RegisterTypeInfo();
        sResourceImpl::InitializeTypeInfo();
        rModelImpl::InitializeTypeInfo();
        sRenderImpl::RegisterTypeInfo();
        sShaderImpl::RegisterTypeInfo();
        uItemImpl::RegisterTypeInfo();
        sUnitImpl::RegisterTypeInfo();

        if (ModConfig::Instance()->RecursiveGrenade == true)
            uOm08Impl::RegisterTypeInfo();

        // Begin the detour transaction.
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());

        // Hook functions.
        Utilities::InstallHooks();

        uPlayerImpl::RegisterTypeInfo();

        // Initialize the archive file overlay system.
        if (ArchiveOverlay::Instance()->Initialize() == false)
        {
            // Failed to initialize the overlay system.
            DbgPrint("Failed to initialize the overlay system!\n");
            TerminateProcess(GetCurrentProcess(), 0xBAD0C0DE);
        }

        // Commit the transaction.
        if (DetourTransactionCommit() != NO_ERROR)
        {
            // Failed to hook into the process, terminate.
            TerminateProcess(GetCurrentProcess(), 0xBAD0C0DE);
        }

        // Check if we should enable the console window or not.
        if (ModConfig::Instance()->EnableConsole == true)
        {
            // Setup the console window.
            OutputDebugString("DRDebugger DllMain\n");
            SetupConsole();

            // Create a worker thread to process console commands.
            HANDLE hThread = CreateThread(NULL, NULL, ProcessConsoleWorker, NULL, NULL, NULL);
            CloseHandle(hThread);
        }
        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

