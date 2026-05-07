/*

*/

#pragma once
#include "MtAllocator.h"

// sizeof = 0x78
struct MtHeapAllocator : public MtAllocator
{
    /* 0x50 */ void         *pAllocationPtr2;        // pBaseAllocation + 0x1E0
    /* 0x58 */ void         *pAllocationPtr1;        // pBaseAllocation + 0x20
    /* 0x60 */ ULONGLONG    mRealSize;
    /* 0x68 */ DWORD        mBreakNo;
    /* 0x70 */ void         *pBaseAllocation;        // Heap data allocated by VirtualAlloc

    inline static MtHeapAllocator * (*_ctor)(MtHeapAllocator *thisptr, const char *psName, DWORD type, DWORD size) =
        (MtHeapAllocator*(*)(MtHeapAllocator*, const char*, DWORD, DWORD))GetModuleAddress(0x140623B80);

    inline static void * (*_Alloc)(MtHeapAllocator *thisptr, DWORD size, DWORD alignment) =
        (void*(*)(MtHeapAllocator*, DWORD, DWORD))GetModuleAddress(0x140624790);

    inline static void(*_Free)(MtHeapAllocator *thisptr, void *pAddress) =
        (void(*)(MtHeapAllocator*, void*))GetModuleAddress(0x140624A70);

    inline static DWORD(*_GetAllocationInfo)(MtHeapAllocator *thisptr, void *pAddress) =
        (DWORD(*)(MtHeapAllocator*, void*))GetModuleAddress(0x140624C50);
};
ASSERT_STRUCT_SIZE(MtHeapAllocator, 0x78);

// Global memory allocators:
inline static WrappedPtr<MtHeapAllocator> g_pResourceHeapAllocator = (MtHeapAllocator**)GetModuleAddress(0x141CF2688);
inline static WrappedPtr<MtHeapAllocator> g_pResourceHeapAllocator2 = (MtHeapAllocator**)GetModuleAddress(0x141928450);
inline static WrappedPtr<MtHeapAllocator> g_pResourceHeapAllocator3 = (MtHeapAllocator**)GetModuleAddress(0x14192FEF0);
inline static WrappedPtr<MtHeapAllocator> g_pTempHeapAllocator = (MtHeapAllocator**)GetModuleAddress(0x141928458);
inline static WrappedPtr<MtHeapAllocator> g_pSystemHeapAllocator = (MtHeapAllocator**)GetModuleAddress(0x141CF2A48);
inline static WrappedPtr<MtHeapAllocator> g_pUnitHeapAllocator = (MtHeapAllocator**)GetModuleAddress(0x141CF2B70);
inline static WrappedPtr<MtHeapAllocator> g_pUnitHeapAllocator2 = (MtHeapAllocator**)GetModuleAddress(0x141928750);
inline static WrappedPtr<MtHeapAllocator> g_pTransHeapAllocator = (MtHeapAllocator**)GetModuleAddress(0x141CF36F8);
inline static WrappedPtr<MtHeapAllocator> g_pHavokHeapAllocator = (MtHeapAllocator**)GetModuleAddress(0x141947550);
inline static WrappedPtr<MtHeapAllocator> g_pStringHeapAllocator = (MtHeapAllocator**)GetModuleAddress(0x141928608);
inline static WrappedPtr<MtHeapAllocator> g_pArrayHeapAllocator = (MtHeapAllocator**)GetModuleAddress(0x1419284E8);
