
#include "MtObjectImpl.h"
#include <locale>
#include <codecvt>
#include <string>

// Forward declarations for command functions.
__int64 PrintDebugOptions(WCHAR **argv, int argc);

// Table of commands.
const int g_MtObjectCommandsLength = 1;
const ConsoleCommandInfo g_MtObjectCommands[g_MtObjectCommandsLength] =
{
    { L"print_dbg_options", L"Prints the debug options for the specified object type", PrintDebugOptions },
};

void MtObjectImpl::RegisterTypeInfo()
{
    // Register commands:
#if _DEBUG
    ImGuiConsole::Instance()->RegisterCommands(g_MtObjectCommands, g_MtObjectCommandsLength);
#endif
}

std::string FixFieldName(const char *psFieldName)
{
    std::string sNewName;
    bool bNewWord = true;

    // Get the length of the string.
    int length = strlen(psFieldName);

    // Loop and process the field name.
    for (int i = 0; i < length; i++)
    {
        // Check if this is a space or underscore.
        if (psFieldName[i] == ' ' || psFieldName[i] == '_')
        {
            // Flag that this is a new word and continue.
            bNewWord = true;
            continue;
        }
        else
        {
            // Check if this is a new word or not.
            if (bNewWord == true)
            {
                bNewWord = false;
                sNewName += psFieldName[i];
            }
            else if (psFieldName[i] != '-')
                sNewName += psFieldName[i];
        }
    }

    return sNewName;
}

void PrintPropertyListReversed(MtPropertyListEntry *pEntry, void *pBaseAddress)
{
    // Traverse the entire list before printing the info for the current property.
    if (pEntry->pFLink != nullptr)
        PrintPropertyListReversed(pEntry->pFLink, pBaseAddress);

    // Special case for section headers.
    //if ((pEntry->Flags & MT_PROP_FLAG_SECTION) != 0)
    //{
    //    // If there is a section header name print it, else just skip a line.
    //    if (pEntry->pPropertyName[0] == 0)
    //        ImGuiConsole::Instance()->ConsolePrint(L"\n");
    //    else
    //        ImGuiConsole::Instance()->ConsolePrint(L"[%S]\n", pEntry->pPropertyName);

    //    return;
    //}

    // Check if we know the property type or not.
    std::string sFieldName = FixFieldName(pEntry->pPropertyName);
    DWORD address = (BYTE*)pEntry->pFieldValue - (BYTE*)pBaseAddress;
    if (pEntry->PropertyType < PROPERTY_TYPE_COUNT && MtPropertyTypeNames[pEntry->PropertyType] != nullptr)
        //ImGuiConsole::Instance()->ConsolePrint(L"   %S %S 0x%04x ", MtPropertyTypeNames[pEntry->PropertyType], pEntry->pPropertyName, pEntry->Flags);
        ImGuiConsole::Instance()->ConsolePrint(L"/* 0x%02X */ %S \t%S;", address, MtPropertyTypeNames[pEntry->PropertyType], sFieldName.c_str());
    else
        ImGuiConsole::Instance()->ConsolePrint(L"   0x%04x %S 0x%04x ", pEntry->PropertyType, pEntry->pPropertyName, pEntry->Flags);

    /*
    // Print function addresses.
    if (pEntry->pGetter)
        ImGuiConsole::Instance()->ConsolePrint(L"Get: %p ", pEntry->pGetter);
    if (pEntry->pGetArrayLength)
        ImGuiConsole::Instance()->ConsolePrint(L"ArrayLength: %p ", pEntry->pGetArrayLength);
    if (pEntry->pUnkFunc1)
        ImGuiConsole::Instance()->ConsolePrint(L"Func1: %p ", pEntry->pUnkFunc1);
    if (pEntry->pUnkFunc2)
        ImGuiConsole::Instance()->ConsolePrint(L"Func2: %p ", pEntry->pUnkFunc2);
    if (pEntry->pUnkFunc3)
        ImGuiConsole::Instance()->ConsolePrint(L"Func3: %p ", pEntry->pUnkFunc3);
        */

    ImGuiConsole::Instance()->ConsolePrint(L"\n");
}

__int64 PrintDebugOptions(WCHAR **argv, int argc)
{
    // Make sure at least 1 argument was passed.
    if (argc < 1)
    {
        // Invalid syntax.
        ImGuiConsole::Instance()->ConsolePrint(L"Invalid command syntax!\n");
        return 0;
    }

    // Setup the unicode converter.
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> unicConvert;

    // Convert the object name to ascii.
    std::string sObjectName = unicConvert.to_bytes(argv[0]);

    // Search the DTI root for an object with the specified name.
    MtDTI *pObjectDTI = MtDTI::FindDTIByName(sObjectName.c_str(), MtDTI::DefaultMtDTIParentObject);
    if (pObjectDTI == nullptr)
    {
        // Failed to find DTI info for the specified object.
        ImGuiConsole::Instance()->ConsolePrint(L"Failed to find DTI info for type '%s'\n", argv[0]);
        return 0;
    }

    // Create an instance of the object.
    MtObject *pObjectInstance = pObjectDTI->CreateInstance<MtObject>();
    if (pObjectInstance == nullptr)
    {
        // Failed to create an instance of the object.
        ImGuiConsole::Instance()->ConsolePrint(L"Failed to create an instance of '%s'\n", argv[0]);
        return 0;
    }

    // Create a new property list to hold the debug options.
    MtPropertyList *pPropertyList = new MtPropertyList();
    if (pPropertyList == nullptr)
    {
        // Failed to allocate memory for the property list.
        ImGuiConsole::Instance()->ConsolePrint(L"Failed to create MtPropertyList\n");
        return 0;
    }

    // Build the property list.
    pObjectInstance->RegisterDebugOptions(pPropertyList);

    // Loop and print all of the debug options in reverse.
    if (pPropertyList->pHead != nullptr)
        PrintPropertyListReversed(pPropertyList->pHead, pObjectInstance);

    // Cleanup objects.
    delete pPropertyList;

    return 0;
}