/*

*/

#include "uPlayerImpl.h"
#include "detours.h"
#include <locale>
#include <codecvt>
#include <MtFramework/Archive/sResource.h>

void **g_sUnitInstance = (void**)GetModuleAddress(0x141CF2620);
void **g_sItemCtrlInstance = (void**)GetModuleAddress(0x1419462A0);

// Player instance:
uPlayer *uPlayerInstance = nullptr;
bool godModeEnabled = false;

// Forward declaractions:
__int64 ToggleGodMode(WCHAR** argv, int argc);

uPlayer* __stdcall Hook_uPlayer_ctor(uPlayer* thisptr);
DWORD __stdcall Hook_uPlayer_AdjustHealth(uPlayer* thisptr, int amount, bool setInfluenceFlag);

// Table of commands for uPlayer objects.
const int g_uPlayerCommandsLength = 1;
const ConsoleCommandInfo g_uPlayerCommands[g_uPlayerCommandsLength] =
{
    { L"god", L"Enables or disables god mode", ToggleGodMode },
};

void uPlayerImpl::RegisterTypeInfo()
{
    // Register commands:
    ImGuiConsole::Instance()->RegisterCommands(g_uPlayerCommands, g_uPlayerCommandsLength);
}

void uPlayerImpl::InstallHooks()
{
    // Hook the uPlayer constructor so we can easily get the address of the player's position later on.
    DetourAttach((void**)&uPlayer::_ctor, Hook_uPlayer_ctor);
    DetourAttach((void**)&uPlayer::_AdjustHealth, Hook_uPlayer_AdjustHealth);
}

__int64 ToggleGodMode(WCHAR** argv, int argc)
{
    // Toggle god mode on or off.
    godModeEnabled = !godModeEnabled;
    ImGuiConsole::Instance()->ConsolePrint(L"God mode %s", godModeEnabled == true ? L"enabled" : L"disabled");

    return 0;
}

uPlayer* __stdcall Hook_uPlayer_ctor(uPlayer *thisptr)
{
    // Save the player instance and call the trampoline.
    uPlayerInstance = thisptr;
    return uPlayer::_ctor(thisptr);
}

DWORD __stdcall Hook_uPlayer_AdjustHealth(uPlayer* thisptr, int amount, bool setInfluenceFlag)
{
    // If god mode is disabled call the trampoline.
    if (godModeEnabled == false)
        return uPlayer::_AdjustHealth(thisptr, amount, setInfluenceFlag);

    // Return the player's current health value (no adjustment).
    DWORD mVitalNew = *(DWORD*)((BYTE*)thisptr + 0x12EC);
    return mVitalNew;
}