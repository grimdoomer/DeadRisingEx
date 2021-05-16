
#include "sMainImpl.h"
#include <MtFramework/Game/sMain.h>

// Forward declarations for command functions.
__int64 EnableDebugZombieSpawner(WCHAR **argv, int argc);

// Table of commands for sRender objects.
const int g_sMainCommandsLength = 1;
const CommandEntry g_sMainCommands[g_sMainCommandsLength] =
{
    { L"enable_zombie_spawner", L"Enables the sMManager debug zombie spawner", EnableDebugZombieSpawner }
};

void sMainImpl::RegisterTypeInfo()
{
    // Register commands:
    RegisterCommands(g_sMainCommands, g_sMainCommandsLength);
}

bool sMainImpl::InstallHooks()
{
    // Pre-emptively patch the debug zombie spawner to work on button held instead of button press.
    PatchBytes<WORD>(GetModuleAddress<WORD*>(0x1400A07BB + 3), 0x45C);

    return true;
}

__int64 EnableDebugZombieSpawner(WCHAR **argv, int argc)
{
    // Get the sMain singleton instance and set the flag to enable the spawner.
    BYTE *psMain = (BYTE*)sMain::Instance();
    *(DWORD*)(psMain + 0x203D4) |= 0x800;

    // Set the debug controller index to input index 1.
    *(DWORD*)(psMain + 0x2106C) = 1;

    return 0;
}