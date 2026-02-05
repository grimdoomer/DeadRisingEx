
#include "TestHooks.h"
#include "detours.h"
#include <MtFramework/Memory/MtHeapAllocator.h>
#include <MtFramework/Game/sSnatcherMain.h>
#include <MtFramework/Area/sAreaManager.h>

inline static bool(__stdcall* _sNpcMarker_Initialize)(void* thisptr, int count) =
(bool(__stdcall*)(void*, int))GetModuleAddress(0x1400914B0);

bool Hook_sNpcMarker_Initialize(void* thisptr, int count);
void Hook_sSnatcherMain_SystemUpdate(sSnatcherMain* thisptr);
__int64 ToggleMallTour(WCHAR** argv, int argc);

DWORD OldAccess = 0;

bool mallTourEnabled = false;
ULONGLONG mallTourLastSwitchTime = 0;
int mallTourAreaIndex = 0;

const char* MallTourAreas[] =
{
    "s100",
    "s200",
    "s300",
    "s400",
    "s500",
    "s600",
    "s700",
    "s800",
    "sa00",
};

// Table of commands.
const int g_TestHookCommandsLength = 1;
const ConsoleCommandInfo g_TestHookCommands[g_TestHookCommandsLength] =
{
    { L"mall_tour", L"Enables or disables mall tour mode", ToggleMallTour },
};

void TestHooks::RegisterTypeInfo()
{
    // Register commands:
    //ImGuiConsole::Instance()->RegisterCommands(g_TestHookCommands, g_TestHookCommandsLength);
}

void TestHooks::InstallHooks()
{
    //DetourAttach((void**)&_sNpcMarker_Initialize, Hook_sNpcMarker_Initialize);
    //DetourAttach((void**)&sSnatcherMain::_SystemUpdate, Hook_sSnatcherMain_SystemUpdate);
}

__int64 ToggleMallTour(WCHAR** argv, int argc)
{
    mallTourEnabled = !mallTourEnabled;
    mallTourLastSwitchTime = sMain::Instance()->mTimer;
    mallTourAreaIndex = 0;

    return 0;
}

bool Hook_sNpcMarker_Initialize(void* thisptr, int count)
{
    void* pOldAlloc = *(void**)((BYTE*)thisptr + 0x50);
    if (pOldAlloc != nullptr)
        (*g_pResourceHeapAllocator2)->Free(pOldAlloc);

    *(void**)((BYTE*)thisptr + 0x50) = nullptr;
    *(DWORD*)((BYTE*)thisptr + 0x4C) = 0;

    void* pArrayAlloc = (*g_pResourceHeapAllocator2)->Alloc(0x1000 + (count * 8), 0x1000);
    if (pArrayAlloc == nullptr)
    {
        DbgPrint("Failed to allocate array for sNpcMarker!\n");
        DebugBreak();
        return false;
    }

    memset(pArrayAlloc, 0x41, 0x1000);
    memset((BYTE*)pArrayAlloc + 0x1000, 0, count * 8);

    DbgPrint("!!! Placing sNpcMarker guard page at %p\n", pArrayAlloc);
    if (VirtualProtect(pArrayAlloc, 0x1000, PAGE_NOACCESS, &OldAccess) == FALSE)
    {
        DbgPrint("Failed to protect array for sNpcMarker 0x%08x\n", GetLastError());
        DebugBreak();
        return false;
    }

    *(void**)((BYTE*)thisptr + 0x50) = (BYTE*)pArrayAlloc + 0x1000;
    *(DWORD*)((BYTE*)thisptr + 0x4C) = count;

    return true;
}

void Hook_sSnatcherMain_SystemUpdate(sSnatcherMain* thisptr)
{
    // Check if mall tour mode is enabled.
    if (mallTourEnabled == true)
    {
        // Check if it's been 30 seconds.
        if (thisptr->mTimer - mallTourLastSwitchTime >= 30 * 1000)
        {
            mallTourLastSwitchTime = thisptr->mTimer;
            mallTourAreaIndex = (mallTourAreaIndex + 1) % 9;

            // Load next area.
            DWORD areaId = sAreaManager::Instance()->GetAreaIdFromName(MallTourAreas[mallTourAreaIndex]);

            // Get the gametask object from sMain instance.
            BYTE* pcGametaskMain = *(BYTE**)((BYTE*)thisptr + 0x20DC0);

            // Set the area id and force and area load.
            *(WORD*)(pcGametaskMain + 0x1B8) = (WORD)areaId;
            *(DWORD*)(pcGametaskMain + 0x1E0) = 0;
        }
    }

    // Call the trampoline.
    sSnatcherMain::_SystemUpdate(thisptr);
}