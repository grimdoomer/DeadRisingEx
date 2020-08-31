/*

*/

#pragma once
#include "MtFramework/MtObject.h"
#include "MtFramework/IO/MtStream.h"

// sizeof = 0x60
struct cResource : public MtObject
{
    /*
        VTable:
            0x28 MtFile::FileInfo * GetFileInfo(MtFile::FileInfo *pFileInfo); // ???
            0x30 const char * GetFileExtension();
            0x38 bool
            0x40 bool LoadResource(MtStream *pStream);
            0x48 bool
            0x50 bool
            0x58 void CleanupResources()
    */

#define RESF_USE_NATIVE_DIR        1    // Load the resource from the nativeWin64 directory
    // 0x80 resource allocated? uninitialized?

#define RESS_RESOURCE_LOADED        1        // pResource->LoadResource returned true
#define RESS_RESOURCE_LOAD_FAILED    0x10    // pResource->LoadResource failed

    /* 0x08 */ char            mPath[64];
    /* 0x48 */ DWORD        mRefCount;
    /* 0x4C */ DWORD        mAttr;          // See RESF_* above
    /* 0x50 */ DWORD        mState;         // See RESS_* above
    /* 0x54 */ DWORD        mSize;
    /* 0x58 */ ULONGLONG    mID;

    inline static cResource * (__stdcall *_ctor)(cResource *thisptr) =
        GetModuleAddress<cResource*(__stdcall*)(cResource*)>(0x140630C90);

    inline static cResource * (__stdcall *_dtor)(cResource *thisptr) =
        GetModuleAddress<cResource*(__stdcall*)(cResource*)>(0x1402B8640);

    inline static void(__stdcall *_RegisterDebugOptions)(cResource *thisptr, MtPropertyList *pPropList) =
        GetModuleAddress<void(__stdcall*)(cResource*, MtPropertyList*)>(0x140630CC0);

    inline static MtDTI * (__stdcall *_GetDTI)(cResource *thisptr) =
        GetModuleAddress<MtDTI*(__stdcall*)(cResource*)>(0x140631060);

    inline static const char * (__stdcall *_GetFileExtension)(cResource *thisptr) =
        GetModuleAddress<const char*(__stdcall*)(cResource*)>(0x14103B2E0);

    inline static void(__stdcall *_IncrementRefCount)(cResource *thisptr) =
        GetModuleAddress<void(__stdcall*)(cResource*)>(0x140630CB0);

    inline static void(__stdcall *_DecrementRefCount)(cResource *thisptr) =
        GetModuleAddress<void(__stdcall*)(cResource*)>(0x140631150);

    IMPLEMENT_MYDTI(cResource, 0x141CF2698, 0x1400AF010, 0x140631100);

    /*
        Gets the file extension for this object type.
    */
    const char * GetFileExtension()
    {
        return ThisPtrCallNoFixup<const char*>(this->vtable[6], this);
    }

    /*
        Description: Parses the file contents of pStream and performs object initialization.

        Parameters:
            - pStream: Stream to read object data from.

        Returns: True if the object was loaded successfully, false otherwise.
    */
    bool LoadResource(MtStream *pStream)
    {
        return ThisPtrCallNoFixup<bool>(this->vtable[8], this, pStream);
    }

    /*
        Releases all game resources owned by this object, free'ing them from memory if their
        reference count hits 0.
    */
    void CleanupResources()
    {
        ThisPtrCallNoFixup<void>(this->vtable[11], this);
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
};
static_assert(sizeof(cResource) == 0x60, "cResource struct has incorrect size");