/*

*/

#pragma once
#include "MtFramework/MtObject.h"

#define MTALLOC_ATTR_LOCK_ON_ACCESS			2	// Acquire list lock on access
#define MTALLOC_ATTR_USE_GLOBAL_LOCK_FIELD	4	// Refer to byte_141CF23C8 for locking

// sizeof = 0x50
struct MtAllocator : public MtObject
{
	/*
		VTable:
			0x28 void * Alloc(ULONGLONG Size, DWORD Flags);
			0x30 void Free(void *pAddress);
			0x38 DWORD gets some info about an allocation (void *pAddress);
	*/

	/* 0x08 */ ULONGLONG	mUsedSize;
	/* 0x10 */ ULONGLONG	mTotalSize;
	/* 0x18 */ char			*mName;
	/* 0x20 */ WORD			mType;
	/* 0x22 */ WORD			mAttr;
	/* 0x24 */ DWORD		CreatorThreadId;
	/* 0x28 */ CRITICAL_SECTION		ListLock;

	void * Alloc(ULONGLONG Size, DWORD Flags)
	{
		return (void*)ThisPtrCallNoFixup(this->vtable[5], this, (void*)Size, (void*)Flags);
	}

	template<typename T> T* Alloc(ULONGLONG Size, DWORD Flags)
	{
		return (T*)Alloc(Size, Flags);
	}

	void Free(void *pAddress)
	{
		ThisPtrCallNoFixup(this->vtable[6], this, pAddress);
	}
};
static_assert(sizeof(MtAllocator) == 0x50, "MtAllocator incorrect struct size");