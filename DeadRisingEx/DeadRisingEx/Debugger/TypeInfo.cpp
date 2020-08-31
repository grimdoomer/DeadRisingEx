/*

*/

#include "DeadRisingEx.h"
#include <locale>
#include <codecvt>
#include <assert.h>
#include <map>

// Forward declarations for command table.
__int64 PrintTypeInfo(WCHAR **argv, int argc);

const CommandEntry g_TypeInfoCommands[g_TypeInfoCommandsLength] =
{
    { L"dt", L"Prints type info for the specified type or variable", PrintTypeInfo }
};

std::map<std::string, const TypeInfo*> g_TypeInfoDictionary;

bool RegisterTypeInfo(const TypeInfo *pTypeInfo)
{
    // Make sure we don't already have an entry for this type.
    if (g_TypeInfoDictionary.find(std::string(pTypeInfo->Name)) != g_TypeInfoDictionary.end())
        return false;

    // Add the type info to the dictionary.
    g_TypeInfoDictionary.emplace(std::string(pTypeInfo->Name), pTypeInfo);
    return true;
}

const TypeInfo *GetInfoForType(std::string typeName)
{
    // Check to see if the dictionary contains the type name.
    if (g_TypeInfoDictionary.find(typeName) != g_TypeInfoDictionary.end())
    {
        // Return the type info object.
        return g_TypeInfoDictionary.at(typeName);
    }

    // A type with matching name was not found in the dictionary.
    return nullptr;
}

bool GetRuntimeFieldInfo(std::wstring sFieldAccess, RuntimeFieldInfo *pFieldInfo)
{
    const LocalVariable *pVariable = nullptr;
    void *pFieldAddress = nullptr;
    const TypeInfo *pTypeInfo = nullptr;
    const FieldInfo *pSubFieldInfo = nullptr;

    // Setup the unicode converter.
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> unicConvert;

    // Loop and tokenize the field access string.
    for (int i = 0; i < sFieldAccess.length(); )
    {
        std::wstring sFieldName;
        bool bIsArrayAccess = false;
        int arrayIndex = 0;

        // Loop and get the name of the next field.
        int foundOffset = (int)sFieldAccess.find('.', i);
        if (foundOffset != -1)
        {
            // Get the field name.
            sFieldName = sFieldAccess.substr(i, foundOffset - i);

            // Update the starting offset.
            i = foundOffset + 1;
        }
        else
        {
            // Get the field name.
            sFieldName = sFieldAccess.substr(i);

            // Update the starting offset so we break the loop.
            i = sFieldAccess.length();
        }

        // Check if the field name string has an array index.
        foundOffset = (int)sFieldName.find('[');
        if (foundOffset != -1)
        {
            // Make sure the closing bracket is present.
            if (sFieldName.at(sFieldName.length() - 1) != ']')
            {
                // Invalid syntax.
                wprintf(L"'%s' invalid syntax\n", sFieldName.c_str());
                return false;
            }

            try
            {
                // Get the array index from the string.
                std::wstring sIndex = sFieldName.substr(foundOffset, sFieldName.length() - foundOffset - 1);
                arrayIndex = std::stoi(sIndex);
            }
            catch (std::exception e)
            {
                // Array index conversion failed.
                wprintf(L"'%s' invalid array index\n", sFieldName.c_str());
                return false;
            }

            // Remove the array index from the string.
            sFieldName = sFieldName.substr(0, foundOffset);

            // Flag that there is a valid array index.
            bIsArrayAccess = true;
        }

        // If this is the first loop iteration we must find the local variable being referenced.
        if (pVariable == nullptr)
        {
            // Make sure the first part of the field access string is a local variable.
            if (sFieldName.length() == 0 || sFieldName[0] != '$')
            {
                // Invalid field access string.
                return false;
            }

            // Try to find a local variable by this name.
            pVariable = GetLocalVar(sFieldName);
            if (pVariable == nullptr)
            {
                // Failed to find a local variable with the same name.
                wprintf(L"No such local variable '%s'!\n", sFieldName.c_str());
                return false;
            }

            // Check if the local variable is a pointer and handle accordingly.
            if (pVariable->Type == VariableType_Pointer)
            {
                // Get type info for the variable field type.
                pTypeInfo = GetInfoForType(unicConvert.to_bytes(pVariable->TypeName));
                if (pTypeInfo == nullptr)
                {
                    // No type info found for the variable type.
                    wprintf(L"No type info for '%s'\n", pVariable->TypeName.c_str());
                    return false;
                }

                // Check if there is an array index and handle accordingly.
                if (bIsArrayAccess == true)
                {
                    // TODO: Check array index bounds and print a warning.

                    // Set the field address.
                    pFieldAddress = (BYTE*)pVariable->Value + (arrayIndex * pTypeInfo->Size);
                }
                else
                {
                    // Set the field address.
                    pFieldAddress = pVariable->Value;
                }
            }
            else
            {
                // TODO: This isn't really supported yet.

                // Make sure there is no array index set.
                if (bIsArrayAccess == true)
                {
                    // Invalid syntax.
                    wprintf(L"'%s' invalid syntax\n", sFieldName.c_str());
                    return false;
                }

                // Set the field address which is really the variable value.
                pFieldAddress = pVariable->Value;
            }
        }
        else
        {
            // Non-pointer variables not supported.
            if (pTypeInfo == nullptr)
            {
                // Syntax error.
                wprintf(L"'%s' invalid syntax\n", sFieldName.c_str());
                return false;
            }

            // Try to find a field with matching name.
            pSubFieldInfo = pTypeInfo->FindFieldByName(unicConvert.to_bytes(sFieldName).c_str());
            if (pSubFieldInfo == nullptr)
            {
                // Type does not contain such a field.
                wprintf(L"Type '%s' does not contain a field '%S'\n", pTypeInfo->Name, sFieldName.c_str());
                return false;
            }

            // Check if this field is a pointer or not.
            if (pSubFieldInfo->Type == FieldType_Pointer)
            {
                assert(pSubFieldInfo->Definition);
                assert(pSubFieldInfo->GetDefinition<ArrayFieldDefinition>()->pTypeInfo);

                // Update the field and pointer values.
                pTypeInfo = pSubFieldInfo->GetDefinition<ArrayFieldDefinition>()->pTypeInfo;
                pFieldAddress = (void*)(*(ULONGLONG*)pFieldAddress + (arrayIndex * pTypeInfo->Size));
            }
            else
            {
                // Update the field pointer values.
                pTypeInfo == nullptr;
                pFieldAddress = (BYTE*)pFieldAddress + pSubFieldInfo->Offset;
            }
        }
    }

    // Update the field info with the evaluation results.
    pFieldInfo->pFieldAddress = pFieldAddress;
    pFieldInfo->pFieldInfo = pSubFieldInfo;
    return true;
}

bool EvaluateFieldAccess(WCHAR **argv, int argc)
{
    RuntimeFieldInfo FieldInfo = { 0 };

    // Make sure there are at least 3 arguments.
    if (argc != 3)
    {
        // Invalid syntax.
        wprintf(L"Invalid syntax!\n");
        return false;
    }

    // Evaluate the destination operand.
    if (GetRuntimeFieldInfo(argv[0], &FieldInfo) == false)
    {
        // Failed to evaluate field expression.
        return false;
    }

    //
}

void PrintFieldsForType(const TypeInfo *pType, const LocalVariable *pLocal)
{
    // Check if the type has a base type.
    if (pType->BaseType[0] != 0)
    {
        // Get the type info for the base type.
        const TypeInfo *pBaseTypeInfo = GetInfoForType(pType->BaseType);
        if (pBaseTypeInfo == nullptr)
        {
            // Failed to get base type info.
            wprintf(L"Failed to get type info for type %S!\n", pType->BaseType);
            return;
        }

        // Recursively print type info for the base type.
        PrintFieldsForType(pBaseTypeInfo, pLocal);
    }

    // Loop and print all of the fields for the type.
    const FieldInfo *pField = &pType->Fields[0];
    while (pField->Type != FieldType_Terminator)
    {
        // If a local variable was provided fill in the values for the type.
        if (pLocal == nullptr)
        {
            // No local variable, just print the field names and offsets.
            wprintf(L"\t[0xx] %S\n", pField->Offset, pField->Name);
        }
        else
        {
            // Get a pointer to the field value for easy access.
            BYTE *pFieldPtr = (BYTE*)pLocal->Value + pField->Offset;

            // Check the field type and handle accordingly.
            switch (pField->Type)
            {
            case FieldType_Pointer:
            {
                wprintf(L"\t[0x%x] %S: %llu\n", pField->Offset, pField->Name, *(ULONGLONG*)pFieldPtr);
                break;
            }
            case FieldType_String:
            {
                wprintf(L"\t[0x%x] %S: %S\n", pField->Offset, pField->Name, (CHAR*)pLocal->Value + pField->Offset);
                break;
            }
            case FieldType_Float:
            {
                wprintf(L"\t[0xx] %S: %f\n", pField->Offset, pField->Name, *(float*)pFieldPtr);
                break;
            }
            case FieldType_Number:
            {
                // Check the field size and read accordingly.
                __int64 fieldValue = 0;
                switch (pField->ElementSize)
                {
                case 1: fieldValue = *(BYTE*)pFieldPtr; break;
                case 2: fieldValue = *(WORD*)pFieldPtr; break;
                case 4: fieldValue = *(DWORD*)pFieldPtr; break;
                case 8: fieldValue = *(ULONGLONG*)pFieldPtr; break;
                default: DebugBreak(); break;
                }

                wprintf(L"\t[0x%x] %S: %llu\n", pField->Offset, pField->Name, fieldValue);
                break;
            }
            //case FieldType_Array:
            //{
            //    // TODO
            //    break;
            //}
            //case FieldType_Struct:
            //{
            //    // TODO
            //    break;
            //}
            case FieldType_Enum:
            {
                // Check the field size and read accordingly.
                DWORD fieldValue = 0;
                switch (pField->ElementSize)
                {
                case 1: fieldValue = *(BYTE*)pFieldPtr; break;
                case 2: fieldValue = *(WORD*)pFieldPtr; break;
                case 4: fieldValue = *(DWORD*)pFieldPtr; break;
                default: DebugBreak(); break;
                }

                // If the field info has a definition address try to get the name of the enum option.
                const char *psOptionName = nullptr;
                if (pField->Definition != nullptr)
                {
                    // Cast the definition address to an EnumTypeInfo struct.
                    EnumTypeInfo *pEnumInfo = (EnumTypeInfo*)pField->Definition;

                    // Loop through all of the enum options and find one for the value read.
                    for (EnumOptionInfo *pOption = &pEnumInfo->Options[0]; pOption->Name != nullptr; pOption++)
                    {
                        // Check if the enum value matches the current option.
                        if (pOption->Value == fieldValue)
                        {
                            // Set the option name and break.
                            psOptionName = pOption->Name;
                            break;
                        }
                    }
                }

                // Check if we found an enum option name for the value and print accordingly.
                if (psOptionName != nullptr)
                {
                    wprintf(L"\t[0x%x] %S: %S\n", pField->Offset, pField->Name, psOptionName);
                }
                else
                {
                    wprintf(L"\t[0x%x] %S: %d\n", pField->Offset, pField->Name, fieldValue);
                }
                break;
            }
            // case FieldType_Bitmask
            case FieldType_Vector3:
            {
                // Read and print the vector3 at the field address.
                Vector3 vector = *(Vector3*)pFieldPtr;
                wprintf(L"\t[0x%x] %S: x=%f y=%f z=%f\n", pField->Offset, pField->Name, vector.x, vector.y, vector.z);
                break;
            }
            case FieldType_Vector4:
            {
                // Read and print the vector4 at the field address.
                Vector4 vector = *(Vector4*)pFieldPtr;
                wprintf(L"\t[0x%x] %S: x=%f y=%f z=%f w=%f\n", pField->Offset, pField->Name, vector.x, vector.y, vector.z, vector.w);
                break;
            }
            default:
            {
                // Unsupported field type.
                wprintf(L"Unsupported field type: %d\n", pField->Type);
                break;
            }
            }
        }

        // Next field.
        pField++;
    }
}

__int64 PrintTypeInfo(WCHAR **argv, int argc)
{
    const TypeInfo *pTypeInfo = nullptr;
    const LocalVariable *pLocal = nullptr;

    // Setup the unicode converter.
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> unicConvert;

    // Make sure there is at least one argument to process.
    if (argc != 1)
    {
        // Invalid syntax.
        wprintf(L"Invalid command syntax\n");
        return 0;
    }

    // Check if the argument is a local variable or regular type.
    std::wstring sTypeToDisplay = argv[0];
    if (sTypeToDisplay.size() > 0 && sTypeToDisplay.at(0) == '$')
    {
        // Argument is a local variable.
        pLocal = GetLocalVar(sTypeToDisplay);
        if (pLocal == nullptr)
        {
            // No local variable found with matching name.
            wprintf(L"No local variable with name '%s' found\n", sTypeToDisplay.c_str());
            return 0;
        }

        // Try to get the type info from the local variable type.
        pTypeInfo = GetInfoForType(unicConvert.to_bytes(pLocal->TypeName));
        if (pTypeInfo == nullptr)
        {
            // No type info found for local variable type.
            wprintf(L"No type info found for type '%s'\n", pLocal->TypeName.c_str());
            return 0;
        }

        // Print the type info header.
        wprintf(L"%s : %p\n", pLocal->TypeName.c_str(), pLocal->Value);
    }
    else
    {
        // Argument is a type name, check if we have typeinfo registered for it.
        pTypeInfo = GetInfoForType(unicConvert.to_bytes(sTypeToDisplay));
        if (pTypeInfo == nullptr)
        {
            // No type info found for local variable type.
            wprintf(L"No type info found for type '%s'\n", sTypeToDisplay.c_str());
            return 0;
        }
    }

    // Recursively print fields for the type.
    PrintFieldsForType(pTypeInfo, pLocal);

    return 0;
}