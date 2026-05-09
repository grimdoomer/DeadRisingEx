
#include "MtPropertyList.h"
#include "MtFramework/Memory/MtHeapAllocator.h"

void* MtPropertyList::operator new(size_t size)
{
    return g_pResourceHeapAllocator2->Alloc(size, 16);
}

void MtPropertyList::operator delete(void* ptr)
{
    g_pResourceHeapAllocator2->Free(ptr);
}