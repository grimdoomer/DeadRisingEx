/*

*/

#pragma once
#include <Windows.h>
#include "Utilities/Module.h"

#pragma warning(disable: 4200) // nonstandard extension used: zero-sized array in struct/union

struct Vector2
{
    float x, y;
};
ASSERT_STRUCT_SIZE(Vector2, 0x8);

struct Vector3 
{
    float x, y, z;
};
ASSERT_STRUCT_SIZE(Vector3, 0xC);

__declspec(align(16)) struct Vector3Aligned
{
    union
    {
        struct
        {
            float x, y, z;
        };
        Vector3 AsVec3;
    };
};
ASSERT_STRUCT_SIZE(Vector3, 0xC);

__declspec(align(16)) struct Vector4
{
    float x, y, z, w;
};
ASSERT_STRUCT_SIZE(Vector4, 0x10);

struct Matrix4x4
{
    Vector4 Rows[4];
};
ASSERT_STRUCT_SIZE(Matrix4x4, 0x40);

// sizeof = 0x8+N
struct MtString
{
    /* 0x00 */ DWORD    RefCount; // Not 100% sure on this
    /* 0x04 */ DWORD    Length;
    /* 0x08 */ CHAR     String[0];
};