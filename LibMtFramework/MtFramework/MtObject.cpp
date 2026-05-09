
#include "MtObject.h"
#include "MtFramework/Memory/MtHeapAllocator.h"

// The operator new/delete functions must be declared in a source file in order to satisfy some requirements
// on header includes when trying to include MtHeapAllocator which depends on MtObject...

void* MtObject::operator new(size_t size)
{
    return g_pResourceHeapAllocator2->Alloc(size, 16);
}

void MtObject::operator delete(void* ptr)
{
    g_pResourceHeapAllocator2->Free(ptr);
}