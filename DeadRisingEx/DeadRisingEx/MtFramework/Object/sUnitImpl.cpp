
#include "sUnitImpl.h"
#include <MtFramework/Object/sUnit.h>
#include <locale>
#include <codecvt>
#include <string>

// Forward declarations for command functions.
__int64 PrintMoveLines(WCHAR **argv, int argc);

// Table of commands.
const int g_sUnitCommandsLength = 1;
const CommandEntry g_sUnitCommands[g_sUnitCommandsLength] =
{
    { L"print_move_lines", L"Prints all objects in the sUnit move line lists", PrintMoveLines },
};

void sUnitImpl::RegisterTypeInfo()
{
    // Register commands:
    RegisterCommands(g_sUnitCommands, g_sUnitCommandsLength);
}

__int64 PrintMoveLines(WCHAR **argv, int argc)
{
    // Get the sUnit singleton instance.
    sUnit *p_sUnit = sUnit::Instance();

    // Check if an index parameter was supplied.
    if (argc == 1)
    {
        // Setup the unicode converter.
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> unicConvert;

        // Convert the index parameter to an integer.
        std::string sIndex = unicConvert.to_bytes(argv[0]);
        int index = atoi(sIndex.c_str());
        if (index < 0 || index >= ARRAYSIZE(sUnit::MoveLines))
        {
            // Index is invalid.
            wprintf(L"Index must be [0, 31]\n");
            return 0;
        }

        // Loop through all the items in the move list and print info on each one.
        wprintf(L"\nMove List %d: %S 0x%08x\n", index, p_sUnit->MoveLines[index].mName, p_sUnit->MoveLines[index].Flags);
        for (cUnit *pItem = p_sUnit->MoveLines[index].pFLink; pItem != nullptr; pItem = pItem->pFLink)
        {
            // Print item info.
            wprintf(L"   %S\n", pItem->GetObjectName());
        }
    }
    else
    {
        // Loop through all of the move line lists and print the item contents of each one.
        for (int i = 0; i < ARRAYSIZE(sUnit::MoveLines); i++)
        {
            // Loop through all the items in the move list and print info on each one.
            wprintf(L"\nMove List %d: %S 0x%08x\n", i, p_sUnit->MoveLines[i].mName, p_sUnit->MoveLines[i].Flags);
            for (cUnit *pItem = p_sUnit->MoveLines[i].pFLink; pItem != nullptr; pItem = pItem->pFLink)
            {
                // Print item info.
                wprintf(L"   %S\n", pItem->GetObjectName());
            }
        }
    }

    return 0;
}