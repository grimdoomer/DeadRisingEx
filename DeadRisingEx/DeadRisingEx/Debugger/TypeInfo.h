/*

*/

#pragma once
#include <Windows.h>
#include <string>
#include <map>
#include "CommandManager.h"

enum FieldType
{
    FieldType_Pointer,
    FieldType_String,
    FieldType_Number,
    FieldType_Float,
    FieldType_Array,
    FieldType_Struct,
    FieldType_Enum,
    FieldType_Bitmask,
    FieldType_Vector3,
    FieldType_Vector4,
    FieldType_Matrix,

    FieldType_Category,
    
    FieldType_Terminator
};

struct FieldInfo
{
    FieldType        Type;            // Type of field
    const char        *Name;            // Name of the field
    DWORD            Offset;            // Offset of the field relative to the start of the object
    DWORD            ElementSize;    // Size of the field type
    void            *Definition;    // Pointer to additional data describing the field

    template<typename T> T* GetDefinition() const
    {
        return (T*)this->Definition;
    }
};

struct TypeInfo
{
    const char        *Name;        // Name of the type
    DWORD            Size;        // Size of the type in bytes
    const char        *BaseType;    // Name of the parent type or null otherwise
    FieldInfo        Fields[];    // Array of fields in the type

    const FieldInfo * FindFieldByName(const char *psFieldName) const
    {
        // Loop through all of the fields and search for a matching name.
        for (const FieldInfo *pField = &this->Fields[0]; pField->Type != FieldType_Terminator; pField++)
        {
            // Check if the field names match.
            if (strcmp(psFieldName, pField->Name) == 0)
            {
                // Found the field.
                return pField;
            }
        }

        // A field with maching name was not found.
        return nullptr;
    }
};

struct ArrayFieldDefinition
{
    const TypeInfo    *pTypeInfo;            // Type info for the type of array
    bool            IsPointer;            // True if the field is a pointer to the array, false if the field is an inline array
    unsigned int    MaxLength;            // Maximum element count for the array, 0 if no max
    const char        *LengthFieldName;    // Name of the field that represents the length of the array
};

// Type info macros:
#define TYPE_INFO_BEGIN(type, base)    \
const TypeInfo type##TypeInfo =        \
{                                    \
    #type, sizeof(type), #base,        \
    {


#define TYPE_INFO_END()                \
    }                                \
};


// Field macros:
#define FIELD_NUMBER(type, field)        { FieldType_Number, #field, FIELD_OFFSET(type, field), sizeof(type::field), nullptr }
#define FIELD_FLOAT(type, field)        { FieldType_Float, #field, FIELD_OFFSET(type, field), sizeof(type::field), nullptr }
#define FIELD_VECTOR3(type, field)        { FieldType_Vector3, #field, FIELD_OFFSET(type, field), sizeof(type::field), nullptr }
#define FIELD_VECTOR4(type, field)        { FieldType_Vector4, #field, FIELD_OFFSET(type, field), sizeof(type::field), nullptr }
#define FIELD_MATRIX(type, field)        { FieldType_Matrix, #field, FIELD_OFFSET(type, field), sizeof(type::field), nullptr }

#define FIELD_ENUM(type, field, def)    { FieldType_Enum, #field, FIELD_OFFSET(type, field), sizeof(type::field), (void*)&def }

#define FIELD_ARRAY(type, field)        { FieldType_Pointer, #field, FIELD_OFFSET(type, field), sizeof(type::field), (void*)&type##field##ArrayDefinition }

#define FIELD_TERM()                    { FieldType_Terminator, nullptr, 0, 0, nullptr }

// Field definition macros:
#define ARRAY_DEFINITION_PTR(type, field, atype, max, length_field)    \
const ArrayFieldDefinition type##field##ArrayDefinition =    { &atype##TypeInfo, true, max, #length_field };


struct EnumOptionInfo
{
    const char    *Name;    // Name of the enum option
    int            Value;    // Integer value of the enum option
};

struct EnumTypeInfo
{
    const char        *Name;        // Name of the enum
    DWORD            Size;        // Size of the enum in bytes
    EnumOptionInfo    Options[];    // Array of enum options
};


struct RuntimeFieldInfo
{
    void                *pFieldAddress;        // Pointer to the field in memory
    const FieldInfo        *pFieldInfo;        // Field info describing the field
};

bool RegisterTypeInfo(const TypeInfo *pTypeInfo);

const TypeInfo *GetInfoForType(std::string typeName);

bool EvaluateFieldAccess(WCHAR **argv, int argc);

// Command table info for TypeInfo related commands.
const int g_TypeInfoCommandsLength = 1;
extern const CommandEntry g_TypeInfoCommands[];