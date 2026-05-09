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

enum ShimFunctionType
{
    ShimFunc_Absolute,
    ShimFunc_Vcall,
    ShimFunc_DtorVcall,
    ShimFunc_ScalarDtor,
};

struct ShimFunctionInfo
{
    ShimFunctionType Type;                  // Type of shim
    void* SnatcherFuncAddress;              // Function address in the game executable
};

struct ShimExportInfo
{
    std::map<std::string, void*> SnatcherShimExportsMap;                // Map of export name -> address
    std::map<void*, ShimFunctionInfo> SnatcherShimRedirectionMap;       // Map of snatcher shim export address -> game function address
    std::map<unsigned int, void*> SnatcherShimVcallLookupMap;           // Map of vcall ordinals -> dispatch stub address
};

// Shell code for the following instruction pattern:
//      mov     rax, [rcx]
//      jmp     qword ptr [rax + 0xXXXXXXXX]
const BYTE ImportVcallCode[] = { 0x48, 0x8B, 0x01, 0xFF, 0xA0, 0x04, 0x01, 0x00, 0x00 };

// Shell code for the following instruction pattern:
//      xor     edx, edx
//      mov     rax, [rcx]
//      jmp     qword ptr[rax]
const BYTE ImportDtorVcallCode[] = { 0x31, 0xD2, 0x48, 0x8B, 0x01, 0xFF, 0x20 };

// Shell code for the following instruction pattern:
//      mov     edx, 0xXXXXXXXX
//      mov     rax, 0xXXXXXXXXXXXXXXXX
//      jmp     rax
const BYTE ImportScalarDtorCallCode[] = { 0xBA, 0x00, 0x00, 0x00, 0x00, 0x48, 0xB8, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0xFF, 0xE0 };

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

BOOL EnumerateShimExportsCallback(PVOID pContext, ULONG nOrdinal, LPCSTR pszName, PVOID pCode)
{
    ShimExportInfo* pExportsInfo = (ShimExportInfo*)pContext;
    ShimFunctionInfo funcInfo;

    // Check if the export name starts with "snatcher_".
    if (strncmp(pszName, "snatcher_scalar_dtor_", strlen("snatcher_scalar_dtor_")) == 0)
    {
        // Get the game address from the export name.
        void* snatcherAddress = (void*)std::stoull(pszName + strlen("snatcher_scalar_dtor_"), nullptr, 16);
        assert(snatcherAddress != nullptr);

        // Adjust the game address for relocations and add it to the redirection dictionary.
        funcInfo.Type = ShimFunc_ScalarDtor;
        funcInfo.SnatcherFuncAddress = GetModuleAddress(snatcherAddress);
        pExportsInfo->SnatcherShimRedirectionMap.emplace(pCode, funcInfo);
    }
    else if (strncmp(pszName, "snatcher_vcall_", strlen("snatcher_vcall_")) == 0)
    {
        // Get the game address from the export name.
        unsigned int vtableOrdinal = std::stoul(pszName + strlen("snatcher_vcall_"), nullptr, 10);

        // Add function info to the redirection dictionary.
        funcInfo.Type = ShimFunc_Vcall;
        funcInfo.SnatcherFuncAddress = (void*)vtableOrdinal;
        pExportsInfo->SnatcherShimRedirectionMap.emplace(pCode, funcInfo);
        pExportsInfo->SnatcherShimVcallLookupMap.emplace(vtableOrdinal, nullptr);
    }
    else if (strncmp(pszName, "snatcher_dtor_vcall__", strlen("snatcher_dtor_vcall__")) == 0)
    {
        // Add function info to the redirection dictionary.
        funcInfo.Type = ShimFunc_DtorVcall;
        pExportsInfo->SnatcherShimRedirectionMap.emplace(pCode, funcInfo);
    }
    else if (strncmp(pszName, "snatcher_", strlen("snatcher_")) == 0)
    {
        // Get the game address from the export name.
        void* snatcherAddress = (void*)std::stoull(pszName + strlen("snatcher_"), nullptr, 16);
        assert(snatcherAddress != nullptr);

        // Adjust the game address for relocations and add it to the redirection dictionary.
        funcInfo.Type = ShimFunc_Absolute;
        funcInfo.SnatcherFuncAddress = GetModuleAddress(snatcherAddress);
        pExportsInfo->SnatcherShimRedirectionMap.emplace(pCode, funcInfo);
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

    // Calculate the numer of shim functions we need to create stubs for.
    size_t stubFunctionAllocationSize = sizeof(ImportDtorVcallCode) + (exportsInfo.SnatcherShimVcallLookupMap.size() * sizeof(ImportVcallCode));
    for (auto iter = exportsInfo.SnatcherShimRedirectionMap.begin(); iter != exportsInfo.SnatcherShimRedirectionMap.end(); iter++)
    {
        // Check if this import needs a stub function.
        if (iter->second.Type == ShimFunc_ScalarDtor)
            stubFunctionAllocationSize += sizeof(ImportScalarDtorCallCode);
    }

    // Allocate memory for the stub functions.
    BYTE* pStubFunctionMemory = (BYTE*)VirtualAlloc(nullptr, stubFunctionAllocationSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (pStubFunctionMemory == nullptr)
    {
        DbgPrint("Failed to allocate %d bytes of memory for shim import stub functions\n", stubFunctionAllocationSize);
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

    // Loop and setup dispatcher stubs for vcalls.
    BYTE* pStubCodePtr = pStubFunctionMemory;
    for (auto iter = exportsInfo.SnatcherShimVcallLookupMap.begin(); iter != exportsInfo.SnatcherShimVcallLookupMap.end(); iter++)
    {
        // Setup the jump stub.
        memcpy(pStubCodePtr, ImportVcallCode, sizeof(ImportVcallCode));

        // Ordinal:
        *(unsigned int*)(pStubCodePtr + 5) = (unsigned int)iter->first * 8;

        // Update stub address.
        iter->second = pStubCodePtr;
        pStubCodePtr += sizeof(ImportVcallCode);
    }

    // Add the stub for the dtor vcall dispatcher.
    BYTE* pDtorVcallDispatchStub = pStubCodePtr;
    memcpy(pDtorVcallDispatchStub, ImportDtorVcallCode, sizeof(ImportDtorVcallCode));
    pStubCodePtr += sizeof(ImportDtorVcallCode);

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
                    DbgPrint("Failed to find entry for '%s' in redirection dictionary\n", psImportName);
                    DebugBreak();
                }

                // Check if the import needs a stub function.
                ShimFunctionInfo funcInfo = exportsInfo.SnatcherShimRedirectionMap[shimAddress];
                if (funcInfo.Type == ShimFunc_Vcall)
                {
                    // Update the import address to point to the jump stub.
                    pImportAddresses[i] = exportsInfo.SnatcherShimVcallLookupMap[(unsigned int)funcInfo.SnatcherFuncAddress];
                    pStubCodePtr += sizeof(ImportVcallCode);
                }
                else if (funcInfo.Type == ShimFunc_DtorVcall)
                {
                    // Update the import address to point to the jump stub.
                    pImportAddresses[i] = pDtorVcallDispatchStub;
                }
                else if (funcInfo.Type == ShimFunc_ScalarDtor)
                {
                    // Setup the jump stub.
                    memcpy(pStubCodePtr, ImportScalarDtorCallCode, sizeof(ImportScalarDtorCallCode));

                    // Flags:
                    *(unsigned int*)(pStubCodePtr + 1) = 0;

                    // Jump address:
                    *(void**)(pStubCodePtr + 7) = funcInfo.SnatcherFuncAddress;

                    // Update the import address to point to the jump stub.
                    pImportAddresses[i] = pStubCodePtr;
                    pStubCodePtr += sizeof(ImportScalarDtorCallCode);
                }
                else
                {
                    // Update the import address to point to the game executable instead.
                    pImportAddresses[i] = funcInfo.SnatcherFuncAddress;
                }

                DbgPrint("Redirected '%s' to 0x%p\n", psImportName, funcInfo.SnatcherFuncAddress);
            }
        }

        break;
    }

    // Mark the jump stub allocation as read-only and executable.
    DWORD scratch;
    if (VirtualProtect(pStubFunctionMemory, stubFunctionAllocationSize, PAGE_EXECUTE_READ, &scratch) == FALSE)
    {
        DbgPrint(__FUNCTION__ ": failed to mark jump function allocation as executable 0x%08x\n", GetLastError());
        DebugBreak();
        return false;
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

