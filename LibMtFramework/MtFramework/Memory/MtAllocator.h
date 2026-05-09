/*

*/

#pragma once
#include "MtFramework/MtObject.h"

// sizeof = 0x50
struct MtAllocator : public MtObject
{
    /*
        VTable:
            0x28 void * Alloc(ULONGLONG Size, DWORD Alignment);
            0x30 void Free(void *pAddress);
            0x38 DWORD gets some info about an allocation (void *pAddress);
    */

    #define MTALLOC_ATTR_LOCK_ON_ACCESS           2    // Acquire list lock on access
    #define MTALLOC_ATTR_USE_GLOBAL_LOCK_FIELD    4    // Refer to byte_141CF23C8 for locking

    /* 0x08 */ ULONGLONG    mUsedSize;
    /* 0x10 */ ULONGLONG    mTotalSize;
    /* 0x18 */ char         *mName;
    /* 0x20 */ WORD         mType;
    /* 0x22 */ WORD         mAttr;                  // See MTALLOC_ATTR_* above
    /* 0x24 */ DWORD        CreatorThreadId;
    /* 0x28 */ CRITICAL_SECTION        ListLock;

    inline static MtAllocator * (*_ctor)(MtAllocator *thisptr, const char *psName, DWORD type) =
        (MtAllocator*(*)(MtAllocator*, const char*, DWORD))GetModuleAddress(0x140623A00);

    inline static void * (*_scalar_deleting_dtor)(MtAllocator *thisptr, unsigned int flags) =
        (void*(*)(MtAllocator*, unsigned int))GetModuleAddress(0x140623E80);

#ifdef SHIMDLL

    // Satisy the compiler by providing a default constructor for MtAllocator.
    MtAllocator()
    {

    }

#endif

    /*
        Description: Creates a new MtAllocator with the specified name and type.

        Parameters:
            - psName: Name of the allocator
            - type: Type of allocator
    */
    SHIM_API MtAllocator(const char *psName, DWORD type) SHIM_BODY(0x140623A00)

    SHIM_API ~MtAllocator() SHIM_BODY_DTOR_VCALL()

    /*
        Description: Allocates a new block of memory from the allocator memory pool.

        Parameters:
            - size: Size of the allocation in bytes
            - alignment: Byte boundary to align the allocation to

        Returns: Address of the allocated memory, for nullptr if the allocation failed.
    */
    void * Alloc(ULONGLONG size, DWORD alignment)
    {
        return (void*)ThisPtrCallNoFixup(this->vtable[5], this, size, alignment);
    }

    /*
        Description: Allocates a new block of memory from the allocator memory pool.

        Parameters:
            - size: Size of the allocation in bytes
            - alignment: Byte boundary to align the allocation to

        Returns: Address of the allocated memory, for nullptr if the allocation failed.
    */
    template<typename T> T* Alloc(ULONGLONG size, DWORD alignment)
    {
        return (T*)Alloc(size, alignment);
    }

    /*
        Description: Free's the specified block of memory.

        Parameters:
            - pAddress: Allocation address to free
    */
    void Free(void *pAddress)
    {
        (void)ThisPtrCallNoFixup(this->vtable[6], this, pAddress);
    }

    // MtAllocator has a single static instance in the game executable. The game incorrectly implements an operator
    // delete override but it can never be used so we ignore it here.
};
ASSERT_STRUCT_SIZE(MtAllocator, 0x50);