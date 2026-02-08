
#pragma once
#include "DeadRisingEx.h"
#include <MtFramework/MtObject.h>

#define PROPERTY_TYPE_COUNT     0x17
static const char * MtPropertyTypeNames[PROPERTY_TYPE_COUNT] =
{
    nullptr,
    "void*",
    "void*",
    "BOOL",
    "BYTE",
    "WORD",
    "DWORD",
    "QWORD",
    "INT8",
    "INT16",
    "INT32",
    "LONGLONG",
    "float",
    "double",
    "MtString",
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    "Vector3",
    "Vector4",
    "Quaternion",
};

class MtObjectImpl
{
protected:

public:
    static void RegisterTypeInfo();
};