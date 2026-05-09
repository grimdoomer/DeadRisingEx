/*

*/

#pragma once
#include "MtFramework/MtObject.h"
#include "MtFramework/Memory/MtHeapAllocator.h"
#include "MtFramework/IO/MtStream.h"
#include "MtFramework/IO/MtFile.h"

// sizeof = 0x60
struct cResource : public MtObject
{
    /*
        VTable:
            0x28 MtFile::FileInfo * GetFileInfo(MtFile::FileInfo *pFileInfo); // ???
            0x30 const char * GetFileExtension();
            0x38 bool
            0x40 bool LoadResource(MtStream *pStream);
            0x48 bool SaveResource(MtStream *pStream); ?
            0x50 bool
            0x58 void CleanupResources()
    */

#define RESF_USE_NATIVE_DIR         1       // Load the resource from the nativeWin64 directory
    // 0x80 resource allocated? uninitialized?

#define RESS_RESOURCE_LOADED        1       // pResource->LoadResource returned true
#define RESS_RESOURCE_LOAD_FAILED   0x10    // pResource->LoadResource failed

    /* 0x08 */ char         mPath[64];
    /* 0x48 */ DWORD        mRefCount;
    /* 0x4C */ DWORD        mAttr;          // See RESF_* above
    /* 0x50 */ DWORD        mState;         // See RESS_* above
    /* 0x54 */ DWORD        mSize;
    /* 0x58 */ ULONGLONG    mID;

    inline static void** _vtable = (void**)GetModuleAddress(0x14103B2B0);

    inline static cResource * (*_ctor)(cResource *thisptr) =
        (cResource*(*)(cResource*))GetModuleAddress(0x140630C90);

    inline static void * (*_scalar_deleting_dtor)(cResource *thisptr, unsigned int flags) =
        (void*(*)(cResource*, unsigned int))GetModuleAddress(0x1402B8640);

    inline static void(*_RegisterDebugOptions)(cResource *thisptr, MtPropertyList *pPropList) =
        (void(*)(cResource*, MtPropertyList*))GetModuleAddress(0x140630CC0);

    inline static MtDTI * (*_GetDTI)(cResource *thisptr) =
        (MtDTI*(*)(cResource*))GetModuleAddress(0x140631060);

    inline static MtFile::FileInfo* (* _GetFileInfo)(cResource* thisptr, MtFile::FileInfo* pFileInfo) =
        (MtFile::FileInfo * (*)(cResource*, MtFile::FileInfo*))GetModuleAddress(0x140631080);

    inline static const char * (*_GetFileExtension)(cResource *thisptr) =
        (const char*(*)(cResource*))GetModuleAddress(0x14103B2E0);

    inline static void(*_IncrementRefCount)(cResource *thisptr) =
        (void(*)(cResource*))GetModuleAddress(0x140630CB0);

    inline static void(*_DecrementRefCount)(cResource *thisptr) =
        (void(*)(cResource*))GetModuleAddress(0x140631150);

    IMPLEMENT_MYDTI(cResource, 0x141CF2698, 0x1400AF010, 0x140631100);

    SHIM_API cResource() SHIM_BODY(0x140630C90)

    SHIM_API ~cResource() SHIM_BODY_DTOR_VCALL()

    /*

    */
    MtFile::FileInfo* GetFileInfo(MtFile::FileInfo* pFileInfo)
    {
        return (MtFile::FileInfo*)ThisPtrCallNoFixup(this->vtable[5], this, pFileInfo);
    }

    /*
        Gets the file extension for this object type.
    */
    const char * GetFileExtension()
    {
        return (const char*)ThisPtrCallNoFixup(this->vtable[6], this);
    }

    /*
        Description: Parses the file contents of pStream and performs object initialization.

        Parameters:
            - pStream: Stream to read object data from.

        Returns: True if the object was loaded successfully, false otherwise.
    */
    bool LoadResource(MtStream *pStream)
    {
        return (bool)ThisPtrCallNoFixup(this->vtable[8], this, pStream);
    }

    /*
        Releases all game resources owned by this object, free'ing them from memory if their
        reference count hits 0.
    */
    void CleanupResources()
    {
        (void)ThisPtrCallNoFixup(this->vtable[11], this);
    }

    /*
        Increments the reference count for this object.
    */
    void IncrementRefCount()
    {
        _IncrementRefCount(this);
    }

    /*
        Decrements the reference count for this object, if the new ref count is 0 the object is destroyed.
    */
    void DecrementRefCount()
    {
        _DecrementRefCount(this);
    }

    IMPLEMENT_OPERATOR_NEW_DELETE(g_pResourceHeapAllocator, 16)
};
ASSERT_STRUCT_SIZE(cResource, 0x60);