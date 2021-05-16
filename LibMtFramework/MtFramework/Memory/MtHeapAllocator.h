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

    inline static MtHeapAllocator * (__stdcall *_ctor)(MtHeapAllocator *thisptr, const char *psName, DWORD type, DWORD size) =
        GetModuleAddress<MtHeapAllocator*(__stdcall*)(MtHeapAllocator*, const char*, DWORD, DWORD)>(0x140623B80);

    inline static void * (__stdcall *_Alloc)(MtHeapAllocator *thisptr, DWORD size, DWORD alignment) =
        GetModuleAddress<void*(__stdcall*)(MtHeapAllocator*, DWORD, DWORD)>(0x140624790);

    inline static void(__stdcall *_Free)(MtHeapAllocator *thisptr, void *pAddress) =
        GetModuleAddress<void(__stdcall*)(MtHeapAllocator*, void*)>(0x140624A70);

    inline static DWORD(__stdcall *_GetAllocationInfo)(MtHeapAllocator *thisptr, void *pAddress) =
        GetModuleAddress<DWORD(__stdcall*)(MtHeapAllocator*, void*)>(0x140624C50);
};
static_assert(sizeof(MtHeapAllocator) == 0x78, "MtHeapAllocator incorrect struct size");

// Global memory allocators:
inline static MtHeapAllocator **g_pResourceHeapAllocator = GetModuleAddress<MtHeapAllocator**>(0x141CF2688);
inline static MtHeapAllocator **g_pResourceHeapAllocator2 = GetModuleAddress<MtHeapAllocator**>(0x141928450);
inline static MtHeapAllocator **g_pResourceHeapAllocator3 = GetModuleAddress<MtHeapAllocator**>(0x14192FEF0);
inline static MtHeapAllocator **g_pTempHeapAllocator = GetModuleAddress<MtHeapAllocator**>(0x141928458);
inline static MtHeapAllocator **g_pSystemHeapAllocator = GetModuleAddress<MtHeapAllocator**>(0x141CF2A48);
inline static MtHeapAllocator **g_pUnitHeapAllocator = GetModuleAddress<MtHeapAllocator**>(0x141CF2B70);
inline static MtHeapAllocator **g_pUnitHeapAllocator2 = GetModuleAddress<MtHeapAllocator**>(0x141928750);
inline static MtHeapAllocator **g_pTransHeapAllocator = GetModuleAddress<MtHeapAllocator**>(0x141CF36F8);
inline static MtHeapAllocator **g_pHavokHeapAllocator = GetModuleAddress<MtHeapAllocator**>(0x141947550);
inline static MtHeapAllocator **g_pStringHeapAllocator = GetModuleAddress<MtHeapAllocator**>(0x141928608);