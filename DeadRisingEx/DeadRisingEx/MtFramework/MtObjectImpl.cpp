
#include "MtObjectImpl.h"
#include <locale>
#include <codecvt>
#include <string>

// Forward declarations for command functions.
__int64 PrintDebugOptions(WCHAR **argv, int argc);

// Table of commands.
const int g_MtObjectCommandsLength = 1;
const CommandEntry g_MtObjectCommands[g_MtObjectCommandsLength] =
{
    { L"print_dbg_options", L"Prints the debug options for the specified object type", PrintDebugOptions },
};

void MtObjectImpl::RegisterTypeInfo()
{
    // Register commands:
    RegisterCommands(g_MtObjectCommands, g_MtObjectCommandsLength);
}

void PrintPropertyListReversed(MtPropertyListEntry *pEntry)
{
    // Traverse the entire list before printing the info for the current property.
    if (pEntry->pFLink != nullptr)
        PrintPropertyListReversed(pEntry->pFLink);

    // Special case for section headers.
    //if ((pEntry->Flags & MT_PROP_FLAG_SECTION) != 0)
    //{
    //    // If there is a section header name print it, else just skip a line.
    //    if (pEntry->pPropertyName[0] == 0)
    //        wprintf(L"\n");
    //    else
    //        wprintf(L"[%S]\n", pEntry->pPropertyName);

    //    return;
    //}

    // Check if we know the property type or not.
    if (pEntry->PropertyType < PROPERTY_TYPE_COUNT && MtPropertyTypeNames[pEntry->PropertyType] != nullptr)
        wprintf(L"   %S %S 0x%04x ", MtPropertyTypeNames[pEntry->PropertyType], pEntry->pPropertyName, pEntry->Flags);
    else
        wprintf(L"   0x%04x %S 0x%04x ", pEntry->PropertyType, pEntry->pPropertyName, pEntry->Flags);

    // Print function addresses.
    if (pEntry->pGetter)
        wprintf(L"Get: %p ", pEntry->pGetter);
    if (pEntry->pGetArrayLength)
        wprintf(L"ArrayLength: %p ", pEntry->pGetArrayLength);
    if (pEntry->pUnkFunc1)
        wprintf(L"Func1: %p ", pEntry->pUnkFunc1);
    if (pEntry->pUnkFunc2)
        wprintf(L"Func2: %p ", pEntry->pUnkFunc2);
    if (pEntry->pUnkFunc3)
        wprintf(L"Func3: %p ", pEntry->pUnkFunc3);

    wprintf(L"\n");
}

__int64 PrintDebugOptions(WCHAR **argv, int argc)
{
    // Make sure at least 1 argument was passed.
    if (argc < 1)
    {
        // Invalid syntax.
        wprintf(L"Invalid command syntax!\n");
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
        wprintf(L"Failed to find DTI info for type '%s'\n", argv[0]);
        return 0;
    }

    // Create an instance of the object.
    MtObject *pObjectInstance = pObjectDTI->CreateInstance<MtObject>();
    if (pObjectInstance == nullptr)
    {
        // Failed to create an instance of the object.
        wprintf(L"Failed to create an instance of '%s'\n", argv[0]);
        return 0;
    }

    // Create a new property list to hold the debug options.
    MtPropertyList *pPropertyList = new MtPropertyList();
    if (pPropertyList == nullptr)
    {
        // Failed to allocate memory for the property list.
        wprintf(L"Failed to create MtPropertyList\n");
        return 0;
    }

    // Build the property list.
    pObjectInstance->RegisterDebugOptions(pPropertyList);

    // Loop and print all of the debug options in reverse.
    if (pPropertyList->pHead != nullptr)
        PrintPropertyListReversed(pPropertyList->pHead);

    // Cleanup objects.
    delete pPropertyList;

    return 0;
}