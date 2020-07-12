/*

*/

#pragma once
#include <Windows.h>
#include "CommandManager.h"
#include "Misc/TypeInfo.h"


struct Vector3
{
	float x, y, z;
};
static_assert(sizeof(Vector3) == 0xC, "Vector3 incorrect struct size");

TYPE_INFO_BEGIN(Vector3,)
	FIELD_FLOAT(Vector3, x),
	FIELD_FLOAT(Vector3, y),
	FIELD_FLOAT(Vector3, z),
TYPE_INFO_END()

__declspec(align(16)) struct Vector4
{
	float x, y, z, w;
};
static_assert(sizeof(Vector4) == 0x10, "Vector4 incorrect struct size");

TYPE_INFO_BEGIN(Vector4, )
	FIELD_FLOAT(Vector4, x),
	FIELD_FLOAT(Vector4, y),
	FIELD_FLOAT(Vector4, z),
	FIELD_FLOAT(Vector4, w),
TYPE_INFO_END()

struct Matrix4x4
{
	Vector4 Rows[4];
};
static_assert(sizeof(Matrix4x4) == 0x40, "Matrix4x4 incorrect struct size");

TYPE_INFO_BEGIN(Matrix4x4, )
	FIELD_VECTOR4(Matrix4x4, Rows[0]),
	FIELD_VECTOR4(Matrix4x4, Rows[1]),
	FIELD_VECTOR4(Matrix4x4, Rows[2]),
	FIELD_VECTOR4(Matrix4x4, Rows[3]),
TYPE_INFO_END()