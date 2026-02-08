
#include "MtObjectImpl.h"
#include <locale>
#include <codecvt>
#include <string>
#include <list>

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

struct ClassField
{
    int Offset;
    std::string FieldType;
    std::string FieldName;
};

bool CompareClassField(const ClassField& a, const ClassField& b)
{
    return a.Offset < b.Offset;
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

void PrintPropertyList(MtPropertyListEntry *pEntry, void *pBaseAddress)
{
    std::list<ClassField> classFields;

    // Loop until we reach the end of the property list.
    while (pEntry != nullptr)
    {
        ClassField fieldInfo;

        // Skip any fields that are section begin/end markers.
        switch (pEntry->PropertyType)
        {
        case MT_PROP_TYPE_ACTION:
        case MT_PROP_TYPE_SUB_SECTION_START:
        case MT_PROP_TYPE_SUB_SECTION_START2:
        case MT_PROP_TYPE_SUB_SECTION_END2:
        case MT_PROP_TYPE_SUB_SECTION_END:
        case MT_PROP_TYPE_STRING_PTR:
        {
            // Next node.
            pEntry = pEntry->pBLink;
            continue;
        }
        }

        // Determine the field type.
        const char* psFieldType = nullptr;
        if (pEntry->PropertyType < PROPERTY_TYPE_COUNT && MtPropertyTypeNames[pEntry->PropertyType] != nullptr)
            fieldInfo.FieldType = MtPropertyTypeNames[pEntry->PropertyType];
        else if (pEntry->PropertyType == MT_PROP_TYPE_CRESOURCE)
            fieldInfo.FieldType = "cResource*";
        else
            fieldInfo.FieldType = "UNKNOWN_TYPE";

        // Get field name and offset.
        fieldInfo.FieldName = FixFieldName(pEntry->pPropertyName);
        fieldInfo.Offset = (BYTE*)pEntry->pFieldValue - (BYTE*)pBaseAddress;
        classFields.push_back(fieldInfo);

        //if (pEntry->PropertyType < PROPERTY_TYPE_COUNT && MtPropertyTypeNames[pEntry->PropertyType] != nullptr)
        //{
        //    //ImGuiConsole::Instance()->ConsolePrint(L"   %S %S 0x%04x ", MtPropertyTypeNames[pEntry->PropertyType], pEntry->pPropertyName, pEntry->Flags);
        //    //ImGuiConsole::Instance()->ConsolePrint(L"/* 0x%02X */ %S \t%S;\n", address, MtPropertyTypeNames[pEntry->PropertyType], sFieldName.c_str());
        //    DbgPrint("/* 0x%02X */ %s \t%s;\n", address, MtPropertyTypeNames[pEntry->PropertyType], sFieldName.c_str());
        //}
        //else
        //{
        //    //ImGuiConsole::Instance()->ConsolePrint(L"   0x%04x %S 0x%04x \n", pEntry->PropertyType, pEntry->pPropertyName, pEntry->Flags);
        //    DbgPrint("/* 0x%02X */   0x%04x %s 0x%04x \n", address, pEntry->PropertyType, pEntry->pPropertyName, pEntry->Flags);
        //}

        // Next node.
        pEntry = pEntry->pBLink;
    }

    // Sort the field list by offset.
    classFields.sort([](const ClassField& a, const ClassField& b)
        {
            return a.Offset < b.Offset;
        });

    // Loop and print class fields.
    for (auto iter = classFields.begin(); iter != classFields.end(); iter++)
    {
        DbgPrint("/* 0x%02X */ %s \t\t%s;\n", iter->Offset, iter->FieldType.c_str(), iter->FieldName.c_str());
    }
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
    MtPropertyListEntry* pEntry = pPropertyList->GetFirstNode();
    if (pEntry != nullptr)
        PrintPropertyList(pEntry, pObjectInstance);

    // Cleanup objects.
    delete pPropertyList;

    return 0;
}