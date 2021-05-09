
#include "MtHeapAllocatorImpl.h"
#include <detours.h>
#include <stdio.h>
#include <MtFramework/Memory/MtHeapAllocator.h>

// Forward declarations:
void * Hook_MtHeapAllocator_Alloc(MtHeapAllocator *thisptr, DWORD size, DWORD alignment);
void Hook_MtHeapAllocator_Free(MtHeapAllocator *thisptr, void *pAddress);
DWORD Hook_MtHeapAllocator_GetAllocationInfo(MtHeapAllocator *thisptr, void *pAddress);

void MtHeapAllocatorImpl::RegisterTypeInfo()
{
    // Register commands:
}

bool MtHeapAllocatorImpl::InstallHooks()
{
    DetourAttach((void**)&MtHeapAllocator::_Alloc, Hook_MtHeapAllocator_Alloc);
    DetourAttach((void**)&MtHeapAllocator::_Free, Hook_MtHeapAllocator_Free);
    DetourAttach((void**)&MtHeapAllocator::_GetAllocationInfo, Hook_MtHeapAllocator_GetAllocationInfo);

    return true;
}

void * Hook_MtHeapAllocator_Alloc(MtHeapAllocator *thisptr, DWORD size, DWORD alignment)
{
    // Allocate memory using non-pooled heap space.
    void *pAddress = _aligned_malloc(size, alignment);
    if (pAddress == nullptr)
    {
        // Failed to allocate memory.
        wprintf(L"### FATAL: Hook_MtHeapAllocator_Alloc failed to allocate memory %d (%d)!\n", size, alignment);
        DbgPrint("### FATAL: Hook_MtHeapAllocator_Alloc failed to allocate memory %d (%d)!\n", size, alignment);
        DebugBreak();
    }

    // Initialize the allocation and return the memory address.
    memset(pAddress, 0, size);
    return pAddress;
}

void Hook_MtHeapAllocator_Free(MtHeapAllocator *thisptr, void *pAddress)
{
    _aligned_free(pAddress);
}

DWORD Hook_MtHeapAllocator_GetAllocationInfo(MtHeapAllocator *thisptr, void *pAddress)
{
    // Not sure how to emulate this just yet, but this function doesn't seem to be called...
    wprintf(L"### FATAL: Hook_MtHeapAllocator_GetAllocationInfo not implemented!\n");
    DbgPrint("### FATAL: Hook_MtHeapAllocator_GetAllocationInfo not implemented!\n");
    DebugBreak();

    return 0;
}