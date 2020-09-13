
#pragma once
#include "DeadRisingEx.h"
#include <MtFramework/MtObject.h>

#define PROPERTY_TYPE_COUNT     0x17
static const char * MtPropertyTypeNames[PROPERTY_TYPE_COUNT] =
{
    nullptr,
    nullptr,
    nullptr,
    "BOOL",
    "BYTE",
    "WORD",
    "DWORD",
    "QWORD",
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    "float",
    nullptr,
    "MtString",
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    "Vector3",
    nullptr,
    "Quaternion",
};

class MtObjectImpl
{
protected:

public:
    static void RegisterTypeInfo();
};