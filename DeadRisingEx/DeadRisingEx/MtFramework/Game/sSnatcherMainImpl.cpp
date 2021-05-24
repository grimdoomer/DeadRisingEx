
#include "sSnatcherMainImpl.h"
#include <detours.h>
#include <string>
#include <locale>
#include <codecvt>
#include <MtFramework/Game/sSnatcherMain.h>
#include <MtFramework/Memory/MtDefaultAllocator.h>
#include <MtFramework/Memory/MtHeapAllocator.h>
#include <MtFramework/Area/sAreaManager.h>

// Forward declarations:
__int64 PrintMemoryUsage(WCHAR **argv, int argc);
__int64 LoadArea(WCHAR **argv, int argc);

void Hook_sSnatcherMain_InitializeAllocators(sSnatcherMain *thisptr);

// Table of commands for sRender objects.
const int g_sSnatcherMainCommandsLength = 2;
const ConsoleCommandInfo g_sSnatcherMainCommands[g_sSnatcherMainCommandsLength] =
{
    { L"print_mem_stats", L"Prints memory usage stats", PrintMemoryUsage },
    { L"load_area", L"Loads the specified area by id", LoadArea }
};

void sSnatcherMainImpl::RegisterTypeInfo()
{
    // Register commands:
    ImGuiConsole::Instance()->RegisterCommands(g_sSnatcherMainCommands, g_sSnatcherMainCommandsLength);
}

bool sSnatcherMainImpl::InstallHooks()
{
    DetourAttach((void**)&sSnatcherMain::_InitializeAllocators, Hook_sSnatcherMain_InitializeAllocators);

    return true;
}

__int64 PrintMemoryUsage(WCHAR **argv, int argc)
{
    // Create a list of memory allocators to print stats on.
    MtAllocator* pMemoryAllocators[8] =
    {
        *g_pDefaultAllocator,
        *g_pResourceHeapAllocator,
        *g_pTempHeapAllocator,
        *g_pSystemHeapAllocator,
        *g_pUnitHeapAllocator,
        *g_pTransHeapAllocator,
        *g_pHavokHeapAllocator,
        *g_pStringHeapAllocator
    };

    // Loop and print memory usage stats.
    ImGuiConsole::Instance()->ConsolePrint(L"Pool | Size (MB) | Used (MB)\n");
    ImGuiConsole::Instance()->ConsolePrint(L"----------------------------\n");
    for (int i = 0; i < 8; i++)
    {
        // Pad the allocator name for consistency.
        std::string sName = pMemoryAllocators[i]->mName;
        sName.append(15 - sName.length(), ' ');

        // Calculate the memory allocated and used.
        DWORD memAllocated = (DWORD)pMemoryAllocators[i]->mTotalSize / (DWORD)(1024 * 1024);
        float memUsed = (float)pMemoryAllocators[i]->mUsedSize / (float)(1024 * 1024);

        // Print the memory stats for the current allocator.
        ImGuiConsole::Instance()->ConsolePrint(L"%S %d\t%.2f\n", sName.c_str(), memAllocated, memUsed);
    }

    ImGuiConsole::Instance()->ConsolePrint(L"\n");
    return 0;
}

__int64 LoadArea(WCHAR **argv, int argc)
{
    // Make sure there is at least one argument for the area id.
    if (argc < 1)
    {
        // Invalid syntax.
        ImGuiConsole::Instance()->ConsolePrint(L"Invalid command syntax!\n");
        return 0;
    }

    // Setup the unicode converter.
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> unicConvert;

    // Get the area id for the area name provided.
    std::string sAreaId = unicConvert.to_bytes(argv[0]);
    DWORD areaId = sAreaManager::Instance()->GetAreaIdFromName(sAreaId.c_str());
    ImGuiConsole::Instance()->ConsolePrint(L"Loading area id %d\n", areaId);

    // Get the gametask object from sMain instance.
    BYTE *psSnatcherMain = (BYTE*)sSnatcherMain::Instance();
    BYTE *pcGametaskMain = *(BYTE**)(psSnatcherMain + 0x20DC0);

    // Set the area id and force and area load.
    *(WORD*)(pcGametaskMain + 0x1B8) = (WORD)areaId;
    *(DWORD*)(pcGametaskMain + 0x1E0) = 0;

    return 0;
}

void Hook_sSnatcherMain_InitializeAllocators(sSnatcherMain *thisptr)
{
    // Resource allocators:
    *g_pResourceHeapAllocator = DefaultMemoryAlloc<MtHeapAllocator>();
    if (*g_pResourceHeapAllocator != nullptr)
    {
        // 400MB for resources -> increased to 800mb
        MtHeapAllocator::_ctor(*g_pResourceHeapAllocator, "Resource", 0, 800 * 1024 * 1024);
    }
    *g_pResourceHeapAllocator2 = *g_pResourceHeapAllocator;
    *g_pResourceHeapAllocator3 = *g_pResourceHeapAllocator;

    // Temp allocator:
    *g_pTempHeapAllocator = DefaultMemoryAlloc<MtHeapAllocator>();
    if (*g_pTempHeapAllocator != nullptr)
    {
        // 100MB for temp
        MtHeapAllocator::_ctor(*g_pTempHeapAllocator, "Temp", 0, 100 * 1024 * 1024);
    }

    // System allocator:
    *g_pSystemHeapAllocator = DefaultMemoryAlloc<MtHeapAllocator>();
    if (*g_pSystemHeapAllocator != nullptr)
    {
        // 100MB for system -> increased to 200mb
        MtHeapAllocator::_ctor(*g_pSystemHeapAllocator, "System", 3, 200 * 1024 * 1024);
    }

    // Unit allocators:
    *g_pUnitHeapAllocator = DefaultMemoryAlloc<MtHeapAllocator>();
    if (*g_pUnitHeapAllocator != nullptr)
    {
        // 24MB for unit
        MtHeapAllocator::_ctor(*g_pUnitHeapAllocator, "Unit", 3, 100 * 1024 * 1024);
    }
    *g_pUnitHeapAllocator2 = *g_pUnitHeapAllocator;

    // Trans allocator:
    *g_pTransHeapAllocator = DefaultMemoryAlloc<MtHeapAllocator>();
    if (*g_pTransHeapAllocator != nullptr)
    {
        // 1MB for trans
        MtHeapAllocator::_ctor(*g_pTransHeapAllocator, "Trans", 3, 1 * 1024 * 1024);
    }

    // Havok allocator:
    *g_pHavokHeapAllocator = DefaultMemoryAlloc<MtHeapAllocator>();
    if (*g_pHavokHeapAllocator != nullptr)
    {
        // 1MB for havok
        MtHeapAllocator::_ctor(*g_pHavokHeapAllocator, "Havok", 3, 10 * 1024 * 1024);
    }

    // String and array allocator:
    *g_pStringHeapAllocator = DefaultMemoryAlloc<MtHeapAllocator>();
    if (*g_pStringHeapAllocator != nullptr)
    {
        // 20MB for string and array
        MtHeapAllocator::_ctor(*g_pStringHeapAllocator, "String&Array", 3, 20 * 1024 * 1024);
    }
    *(MtAllocator**)g_pDefaultAllocator = *g_pStringHeapAllocator;

    // Set locking flags.
    (*g_pHavokHeapAllocator)->mAttr |= MTALLOC_ATTR_LOCK_ON_ACCESS;
    (*g_pResourceHeapAllocator3)->mAttr = (*g_pHavokHeapAllocator)->mAttr;
}