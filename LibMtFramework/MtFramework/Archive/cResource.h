/*

*/

#pragma once
#include "MtFramework/MtObject.h"
#include "MtFramework/IO/MtStream.h"
#include "Misc/TypeInfo.h"

// sizeof = 0x60
struct cResource : public MtObject
{
	/*
		VTable:
			0x28
			0x30 const char * GetFileExtension();
			0x38 bool
			0x40 bool LoadResource(sResource::DecompressStream *pStream);
			0x48 bool
			0x50 bool
			0x58 void FreeResources()?
	*/

#define RESF_USE_NATIVE_DIR		1	// Load the resource from the nativeWin64 directory
	// 0x80 resource allocated? uninitialized?

#define RESS_RESOURCE_LOADED		1		// pResource->LoadResource returned true
#define RESS_RESOURCE_LOAD_FAILED	0x10	// pResource->LoadResource failed

	/* 0x08 */ char			mPath[64];
	/* 0x48 */ DWORD		mRefCount;
	/* 0x4C */ DWORD		mAttr;
	/* 0x50 */ DWORD		mState;
	/* 0x54 */ DWORD		mSize;
	/* 0x58 */ ULONGLONG	mID;

	const char * GetFileExtension()
	{
		return (const char*)ThisPtrCallNoFixup(this->vtable[6], this);
	}

	bool LoadResource(MtStream *pStream)
	{
		return (bool)ThisPtrCallNoFixup(this->vtable[8], this, pStream);
	}
};
static_assert(sizeof(cResource) == 0x60, "cResource struct has incorrect size");

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