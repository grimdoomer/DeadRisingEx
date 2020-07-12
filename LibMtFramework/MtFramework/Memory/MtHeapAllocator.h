/*

*/

#pragma once
#include "MtAllocator.h"

// sizeof = 0x78
struct MtHeapAllocator : public MtAllocator
{
	/* 0x50 */ void			*pAllocationPtr2;		// pBaseAllocation + 0x1E0
	/* 0x58 */ void			*pAllocationPtr1;		// pBaseAllocation + 0x20
	/* 0x60 */ ULONGLONG	mRealSize;
	/* 0x68 */ DWORD		mBreakNo;
	/* 0x70 */ void			*pBaseAllocation;		// Heap data allocated by VirtualAlloc
};
static_assert(sizeof(MtHeapAllocator) == 0x78, "MtHeapAllocator incorrect struct size");

inline static MtHeapAllocator **g_pTempHeapAllocator = GetModuleAddress<MtHeapAllocator**>((void*)0x141928458);