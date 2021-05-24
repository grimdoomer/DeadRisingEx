
#include "sSnatcherToolImpl.h"
#include <MtFramework/Game/sSnatcherMain.h>
#include <MtFramework/Debug/sToolBase.h>

void **g_sToolBaseInstance = GetModuleAddress<void**>(0x141CF3230);

// Forward declarations for command functions.
__int64 OpenDebugMenu(WCHAR **argv, int argc);

// Table of commands.
const int g_sSnatcherToolCommandsLength = 1;
const ConsoleCommandInfo g_sSnatcherToolCommands[g_sSnatcherToolCommandsLength] =
{
    { L"open_debug_menu", L"Opens the in-game debug menu", OpenDebugMenu },
};

void sSnatcherToolImpl::RegisterTypeInfo()
{
    // Register commands:
    ImGuiConsole::Instance()->RegisterCommands(g_sSnatcherToolCommands, g_sSnatcherToolCommandsLength);
}

__int64 OpenDebugMenu(WCHAR **argv, int argc)
{
    // Get the sSnatcherTool instance from sSnatcherMain.
    sToolBase *sSnatcherTool = (sToolBase*)(*g_sToolBaseInstance);
    if (sSnatcherTool == nullptr)
    {
        // Snatcher tool instance not initialized.
        ImGuiConsole::Instance()->ConsolePrint(L"Snatcher tool instance not initialized\n");
        return 0;
    }

    // Register the debug menu options.
    sSnatcherTool->RegisterDebugOptions(&sSnatcherTool->DebugOptionList);

    // Change the menu open state.
    sSnatcherTool->ShowDebugMenu = 1;
}