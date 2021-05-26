
#pragma once
#include "MtAllocator.h"

// sizeof = 0x50
struct MtDefaultAllocator : public MtAllocator
{

};
static_assert(sizeof(MtDefaultAllocator), "MtDefaultAllocator incorrect struct size");

// Global default memory allocator instance:
inline static MtDefaultAllocator* g_DefaultAllocator = (MtDefaultAllocator*)GetModuleAddress(0x141C95BE0);
inline static MtDefaultAllocator** g_pDefaultAllocator = (MtDefaultAllocator**)GetModuleAddress(0x1419284E8);

// Default memory allocation function:
inline static void * (__stdcall *_DefaultMemoryAlloc)(DWORD size) =
(void*(__stdcall*)(DWORD))GetModuleAddress(0x140623DD0);

template<typename T> T* DefaultMemoryAlloc()
{
    return (T*)_DefaultMemoryAlloc(sizeof(T));
}

template<typename T> T* DefaultMemoryAlloc(DWORD size)
{
    return (T*)_DefaultMemoryAlloc(size);
}