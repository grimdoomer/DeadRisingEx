/*

*/

#pragma once
#include <Windows.h>
#include "Misc/AsmHelpers.h"

#pragma warning(disable: 4200) // nonstandard extension used: zero-sized array in struct/union

struct Vector2
{
    float x, y;
};
static_assert(sizeof(Vector2) == 0x8, "Vector2 incorrect struct size");

// I think this needs to be aligned to 16 bytes
struct Vector3 
{
    float x, y, z;
};
static_assert(sizeof(Vector3) == 0xC, "Vector3 incorrect struct size");

__declspec(align(16)) struct Vector4
{
    float x, y, z, w;
};
static_assert(sizeof(Vector4) == 0x10, "Vector4 incorrect struct size");

struct Matrix4x4
{
    Vector4 Rows[4];
};
static_assert(sizeof(Matrix4x4) == 0x40, "Matrix4x4 incorrect struct size");

// sizeof = 0x8+N
struct MtString
{
    /* 0x00 */ DWORD    RefCount; // Not 100% sure on this
    /* 0x04 */ DWORD    Length;
    /* 0x08 */ CHAR     String[0];
};


/*
    Returns a null pointer (or integer)
*/
inline static void* (__stdcall *ReturnNullPtr)() = (void*(__stdcall*)())GetModuleAddress(0x1401E94B0);
inline static __int64 ReturnNullPtrFuncAddr = 0x1401E94B0;