
#pragma once
#include "DeadRisingEx.h"
#include <MtFramework/Archive/cResource.h>

TYPE_INFO_BEGIN(cResource, )
	{ FieldType_Pointer, "*vtable", FIELD_OFFSET(cResource, vtable), sizeof(void*), nullptr },
	{ FieldType_String, "mPath", FIELD_OFFSET(cResource, mPath), sizeof(char), (void*)64 },
	{ FieldType_Number, "mRefCount", FIELD_OFFSET(cResource, mRefCount), sizeof(DWORD), nullptr },
	{ FieldType_Number, "mAttr", FIELD_OFFSET(cResource, mAttr), sizeof(DWORD), nullptr },
	{ FieldType_Number, "mState", FIELD_OFFSET(cResource, mState), sizeof(DWORD), nullptr },
	{ FieldType_Number, "mSize", FIELD_OFFSET(cResource, mSize), sizeof(DWORD), nullptr },
	{ FieldType_Number, "mID", FIELD_OFFSET(cResource, mID), sizeof(ULONGLONG), nullptr },
	{ FieldType_Terminator, nullptr, 0, 0, nullptr }
TYPE_INFO_END()