/*

*/

#pragma once
#include "MtFramework/MtObject.h"

// sizeof = 0x50
struct MtAllocator : public MtObject
{
    /*
        VTable:
            0x28 void * Alloc(ULONGLONG Size, DWORD Flags);
            0x30 void Free(void *pAddress);
            0x38 DWORD gets some info about an allocation (void *pAddress);
    */

    #define MTALLOC_ATTR_LOCK_ON_ACCESS            2    // Acquire list lock on access
    #define MTALLOC_ATTR_USE_GLOBAL_LOCK_FIELD    4    // Refer to byte_141CF23C8 for locking

    /* 0x08 */ ULONGLONG    mUsedSize;
    /* 0x10 */ ULONGLONG    mTotalSize;
    /* 0x18 */ char            *mName;
    /* 0x20 */ WORD            mType;
    /* 0x22 */ WORD            mAttr;                  // See MTALLOC_ATTR_* above
    /* 0x24 */ DWORD        CreatorThreadId;
    /* 0x28 */ CRITICAL_SECTION        ListLock;

    inline static MtAllocator * (__stdcall *_ctor)(MtAllocator *thisptr, const char *psName, DWORD type) =
        GetModuleAddress<MtAllocator*(__stdcall*)(MtAllocator*, const char*, DWORD)>(0x140623A00);

    inline static MtAllocator * (__stdcall *_dtor)(MtAllocator *thisptr, bool bFreeMemory) =
        GetModuleAddress<MtAllocator*(__stdcall*)(MtAllocator*, bool)>(0x140623E80);

    /*
        Description: Creates a new MtAllocator with the specified name and type.

        Parameters:
            - psName: Name of the allocator
            - type: Type of allocator
    */
    MtAllocator(const char *psName, DWORD type)
    {
        _ctor(this, psName, type);
    }

    ~MtAllocator()
    {
        ThisPtrCallNoFixup<void, bool>(this->vtable[0], this, false);
    }

    /*
        Description: Allocates a new block of memory from the allocator memory pool.

        Parameters:
            - size: Size of the allocation in bytes
            - flags: Allocation flags

        Returns: Address of the allocated memory, for nullptr if the allocation failed.
    */
    void * Alloc(ULONGLONG size, DWORD flags)
    {
        return ThisPtrCallNoFixup<void*, ULONGLONG, DWORD>(this->vtable[5], this, size, flags);
    }

    /*
        Description: Allocates a new block of memory from the allocator memory pool.

        Parameters:
            - size: Size of the allocation in bytes
            - flags: Allocation flags

        Returns: Address of the allocated memory, for nullptr if the allocation failed.
    */
    template<typename T> T* Alloc(ULONGLONG size, DWORD flags)
    {
        return (T*)Alloc(size, flags);
    }

    /*
        Description: Free's the specified block of memory.

        Parameters:
            - pAddress: Allocation address to free
    */
    void Free(void *pAddress)
    {
        ThisPtrCallNoFixup<void, void*>(this->vtable[6], this, pAddress);
    }
};
static_assert(sizeof(MtAllocator) == 0x50, "MtAllocator incorrect struct size");