// dllmain.cpp : Defines the entry point for the DLL application.
#include <stdio.h>
#include <string>
#include <iostream>
#include <locale>
#include <codecvt>
#include <map>
#include <filesystem>
#include <Windows.h>
#include <shellapi.h>
#include "LibMtFramework.h"
#include <detours.h>
#include "Utilities/Module.h"
#include "MtFramework/Utils/Utilities.h"
#include "DeadRisingEx/ModConfig.h"

#include <MtFramework/Game/sMain.h>
#include <MtFramework/IO/MtDataReader.h>
#include <MtFramework/Graphics/rTexture.h>

#include "DeadRisingEx/MtFramework/MtObjectImpl.h"
#include "DeadRisingEx/MtFramework/Archive/ArchiveOverlay.h"
#include "DeadRisingEx/MtFramework/Archive/sResourceImpl.h"
#include "DeadRisingEx/MtFramework/Debug/sSnatcherToolImpl.h"
#include "DeadRisingEx/MtFramework/Game/sMainImpl.h"
#include "DeadRisingEx/MtFramework/Game/sSnatcherPadImpl.h"
#include "DeadRisingEx/MtFramework/Game/sSnatcherMainImpl.h"
#include "DeadRisingEx/MtFramework/Game/Task/cGametaskTitleImpl.h"
#include "DeadRisingEx/MtFramework/Graphics/rModelImpl.h"
#include "DeadRisingEx/MtFramework/Memory/MtHeapAllocatorImpl.h"
#include "DeadRisingEx/MtFramework/Rendering/ImGui/ImGuiRenderer.h"
#include "DeadRisingEx/MtFramework/Rendering/ImGui/ImGuiConsole.h"
#include "DeadRisingEx/MtFramework/Rendering/sRenderImpl.h"
#include "DeadRisingEx/MtFramework/Rendering/sShaderImpl.h"
#include "DeadRisingEx/MtFramework/Item/uItemImpl.h"
#include "DeadRisingEx/MtFramework/Item/Items/uOm08Impl.h"
#include "DeadRisingEx/MtFramework/Object/sUnitImpl.h"
#include "DeadRisingEx/MtFramework/Object/uPhotoImpl.h"
#include "DeadRisingEx/MtFramework/Object/Model/sSMManagerImpl.h"
#include "DeadRisingEx/MtFramework/Object/Npc/uNpcMarkerImpl.h"
#include "DeadRisingEx/MtFramework/Player/uPlayerImpl.h"
#include "DeadRisingEx/MtFramework/Object/Vehicle/Vehicle.h"
#include "DeadRisingEx/Utilities/DebugLog.h"


// Make sure the shim imports are resolved before any other static initialization routines run or else
// we run the risk of something calling into the shim instead of the game code.
#pragma init_seg(lib)

bool ResolveShimImports();

bool ShimImportsResolved = ResolveShimImports();


struct ShimExportInfo
{
    std::map<std::string, void*> SnatcherShimExportsMap;        // Map of export name -> address
    std::map<void*, void*> SnatcherShimRedirectionMap;          // Map of snatcher shim export address -> game function address
};

// Version string for update 1 of the game exe.
const char *g_SupportedGameVersionString = "Master Oct  6 2016 23:23:44";

void ForceSymbolsHelper()
{
    MtDataReader *pDataReader = nullptr;
    MtFile *pFile = nullptr;
    MtFileStream *pFileStream = nullptr;
}

void(*pOutputDebugStringA)(LPCSTR lpOutputString) = OutputDebugStringA;

void Hook_OutputDebugStringA(LPCSTR lpOutputString)
{
    // Print the message to the imgui console and the debugger.
    ImGuiConsole::Instance()->ConsolePrint(lpOutputString);
    pOutputDebugStringA(lpOutputString);

    // If the debug log is enabled write all messages to debug log.
    if (ModConfig::Instance()->DebugLog == true)
        DebugLog::WriteMessage(lpOutputString);
}

void ForcePatchInfinityMode()
{
    void *pPatchAddr1 = (void*)GetModuleAddress(0x14021155F);
    void *pPatchAddr2 = (void*)GetModuleAddress(0x1402115EF);

    BYTE NopBytes[2] = { 0x90, 0x90 };

    PatchBytes(pPatchAddr1, NopBytes, sizeof(NopBytes));
    PatchBytes(pPatchAddr2, NopBytes, sizeof(NopBytes));
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

BOOL EnumerateShimExportsCallback(PVOID pContext, ULONG nOrdinal, LPCSTR pszName, PVOID pCode)
{
    ShimExportInfo* pExportsInfo = (ShimExportInfo*)pContext;

    // Check if the export name starts with "snatcher_".
    if (strncmp(pszName, "snatcher_", strlen("snatcher_")) == 0)
    {
        // Get the game address from the export name.
        void* snatcherAddress = (void*)std::stoull(pszName + strlen("snatcher_"), nullptr, 16);
        assert(snatcherAddress != nullptr);

        // Adjust the game address for relocations and add it to the redirection dictionary.
        void* snatcherAddressReloc = GetModuleAddress(snatcherAddress);
        pExportsInfo->SnatcherShimRedirectionMap.emplace(pCode, snatcherAddressReloc);
    }
    else
    {
        // Add the export entry to the exports list.
        pExportsInfo->SnatcherShimExportsMap.emplace(pszName, pCode);
    }

    return TRUE;
}

bool RunningInGameProcess()
{
    CHAR sModulePath[MAX_PATH] = { 0 };
    CHAR sModuleName[32] = { 0 };

    // Get the name of the exe we are running under.
    GetModuleFileName(GetModuleHandle(NULL), sModulePath, sizeof(sModulePath));
    _splitpath_s(sModulePath, nullptr, 0, nullptr, 0, sModuleName, sizeof(sModuleName), nullptr, 0);

    // Check if we were run from the launcher or the game process.
    if (_stricmp(sModuleName, "DeadRisingLauncher") == 0)
    {
        // Bail out as we are not in the game process.
        return false;
    }

    return true;
}

bool ResolveShimImports()
{
    ShimExportInfo exportsInfo;

    // Bail out if we're not running in the game process.
    if (RunningInGameProcess() == false)
        return false;

    // Get the handle for SnatcherShim.dll and enumerate exports.
    HMODULE hSnatcherShim = GetModuleHandleA("SnatcherShim.dll");
    if (DetourEnumerateExports(hSnatcherShim, &exportsInfo, EnumerateShimExportsCallback) == FALSE)
    {
        DbgPrint("Failed to enumerate SnatcherShim.dll exports 0x%08x\n", GetLastError());
        DebugBreak();
        return false;
    }

    // Get the import table info for SnatcherShim.dll.
    IMAGE_DOS_HEADER* pDosHeader = (IMAGE_DOS_HEADER*)GetModuleHandle("DeadRisingEx.dll");
    if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
    {
        DbgPrint(__FUNCTION__ ": image header has invalid signature\n");
        return false;
    }

    IMAGE_NT_HEADERS* pNtHeader = (IMAGE_NT_HEADERS*)((BYTE*)pDosHeader + pDosHeader->e_lfanew);
    if (pNtHeader->Signature != IMAGE_NT_SIGNATURE)
    {
        DbgPrint(__FUNCTION__ ": NT image header has invalid signature\n");
        return false;
    }

    if (pNtHeader->FileHeader.SizeOfOptionalHeader == 0)
    {
        DbgPrint(__FUNCTION__ ": optional header size is invalid\n");
        return false;
    }

    if (pNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress == 0)
    {
        DbgPrint(__FUNCTION__ ": import directory virtual address is invalid\n");
        return false;
    }

    IMAGE_IMPORT_DESCRIPTOR* pImportDescriptor = (IMAGE_IMPORT_DESCRIPTOR*)((BYTE*)pDosHeader + pNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

    // Make the import table writable so we can just the import addresses.
    DWORD oldProtect;
    if (VirtualProtect(pImportDescriptor, pNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size, PAGE_READWRITE, &oldProtect) == FALSE)
    {
        DbgPrint(__FUNCTION__ ": failed to make import table writable 0x%08x\n", GetLastError());
        DebugBreak();
        return false;
    }

    // Loop and walk the import directory until we find the entry for SnatcherShim.dll.
    for (; pImportDescriptor->OriginalFirstThunk != 0; pImportDescriptor++)
    {
        // Skip any module that's not the shim dll.
        const char* psImportModuleName = (const char*)((BYTE*)pDosHeader + pImportDescriptor->Name);
        if (strncmp(psImportModuleName, "SnatcherShim.dll", strlen("SnatcherShim.dll")) != 0)
            continue;

        IMAGE_THUNK_DATA* pThunkData = (IMAGE_THUNK_DATA*)((BYTE*)pDosHeader + pImportDescriptor->OriginalFirstThunk);
        void** pImportAddresses = (void**)((BYTE*)pDosHeader + pImportDescriptor->FirstThunk);

        // Loop through all the imports and fixup each one.
        for (int i = 0; pThunkData[i].u1.Ordinal; i++)
        {
            // Only check imports linked by name.
            if (IMAGE_SNAP_BY_ORDINAL(pThunkData[i].u1.Ordinal) == false)
            {
                // Get the name of the import.
                const char* psImportName = (const char*)((BYTE*)pDosHeader + ((DWORD)pThunkData[i].u1.AddressOfData + 2));

                // Make sure the function exists in the dictionary.
                if (exportsInfo.SnatcherShimExportsMap.find(psImportName) == exportsInfo.SnatcherShimExportsMap.end())
                {
                    DbgPrint("SnatcherShim.dll import '%s' not found in exports dictionary\n", psImportName);
                    DebugBreak();
                }

                // Make sure the redirection dictionary contains an entry for this address.
                void* shimAddress = exportsInfo.SnatcherShimExportsMap[psImportName];
                if (exportsInfo.SnatcherShimRedirectionMap.find(shimAddress) == exportsInfo.SnatcherShimRedirectionMap.end())
                {
                    DbgPrint("Failed to entry for '%s' in redirection dictionary\n", psImportName);
                    DebugBreak();
                }

                // Update the import address to point to the game executable instead.
                void* snatcherAddress = exportsInfo.SnatcherShimRedirectionMap[shimAddress];
                pImportAddresses[i] = snatcherAddress;

                DbgPrint("Redirected '%s' to 0x%p\n", psImportName, snatcherAddress);
            }
        }

        break;
    }

    // Restore page protections on the import table.
    if (VirtualProtect(pImportDescriptor, pNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size, oldProtect, &oldProtect) == FALSE)
    {
        DbgPrint(__FUNCTION__ ": failed to restore import table page permissions 0x%08x\n", GetLastError());
        DebugBreak();
        return false;
    }

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
        // Check if we were run from the launcher or the game process.
        if (RunningInGameProcess() == false)
        {
            // Bail out as we are not in the game process.
            return TRUE;
        }

        // Set the module handle.
        SnatcherModuleHandle = GetModuleHandle(NULL);

        // Check the game version string to make sure we are loading with the correct game version.
        if (strncmp(sMain::mBuildVersion, g_SupportedGameVersionString, strlen(g_SupportedGameVersionString)) != 0)
        {
            // Game version not supported, display an error and kill the process.
            MessageBoxW(NULL, L"This version of Dead Rising is not supported by DeadRisingEx! Please update the game to the Oct 6th 2016 version in order to use DeadRisingEx.",
                L"Game version not supported", MB_OK | MB_ICONERROR | MB_APPLMODAL);
            TerminateProcess(GetCurrentProcess(), 0xBAD0C0DE);
        }

        // Fixup imports to SnatcherShim.dll.
        if (ShimImportsResolved == false)
        {
            MessageBoxW(NULL, L"Failed to initialize DeadRisingEx, something is very wrong.",
                L"DeadRisingEx", MB_OK | MB_ICONERROR | MB_APPLMODAL);
            TerminateProcess(GetCurrentProcess(), 0xBAD0C0DE);
        }

        // Load the mod config file.
        if (ModConfig::Instance()->LoadConfigFile("DeadRisingEx.ini") == false)
        {
            // Failed to load the mod config.
            DbgPrint("Failed to load mod config file, using default settings!\n");
        }

        // Register built in types.
        /*RegisterTypeInfo(&Vector3TypeInfo);
        RegisterTypeInfo(&Vector4TypeInfo);
        RegisterTypeInfo(&Matrix4x4TypeInfo);*/

        // Register types and commands.
        MtObjectImpl::RegisterTypeInfo();
        sSnatcherToolImpl::RegisterTypeInfo();
        sResourceImpl::InitializeTypeInfo();
        rModelImpl::RegisterCommands();
        sRenderImpl::RegisterTypeInfo();
        sShaderImpl::RegisterTypeInfo();
        uItemImpl::RegisterTypeInfo();
        sUnitImpl::RegisterTypeInfo();
        sMainImpl::RegisterTypeInfo();
        sSnatcherMainImpl::RegisterTypeInfo();
        MtHeapAllocatorImpl::RegisterTypeInfo();
        uDrexVehicle::RegisterCommands();
        ImGuiRenderer::RegisterTypeInfo();

        if (ModConfig::Instance()->RecursiveGrenade == true)
            uOm08Impl::RegisterTypeInfo();

        // Begin the detour transaction.
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());

        // If the debug log is enabled hook debug output to file.
        if (ModConfig::Instance()->DebugLog == true)
            DebugLog::Initialize();

#ifdef _DEBUG
        ForcePatchInfinityMode();
#endif

        // Hook debug output to the imgui console.
        DetourAttach((void**)&pOutputDebugStringA, Hook_OutputDebugStringA);

        //DetourAttach((void**)&LoadSpriteData, Hook_LoadSpriteData);

        // Hook functions.
        Utilities::InstallHooks();
        sRenderImpl::InstallHooks();
        sSnatcherPadImpl::InstallHooks();
        sMainImpl::InstallHooks();
        sSnatcherMainImpl::InstallHooks();
        sSMManagerImpl::InstallHooks();
        //MtHeapAllocatorImpl::InstallHooks();
        cGametaskTitleImpl::InstallHooks();
        uNpcMarkerImpl::InstallHooks();
        uPhotoImpl::InstallHooks();
        uItemImpl::InstallHooks();

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

        // Dummy function to force non-used symbols to be emitted in the pdb file.
        ForceSymbolsHelper();
        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

