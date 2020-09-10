/*

*/

#pragma once
#include <Windows.h>
#include "Misc/AsmHelpers.h"

struct cResource;
struct MtObject;

typedef void MtPropertyList;

// sizeof = 0x38
struct MtDTI
{
    /*
        VTable:
            0x00 ~MtDTI()
            0x08 cResource * CreateInstance()
    */

    /* 0x00 */ void     **vtable;
    /* 0x08 */ char     *pObjectName;
    /* 0x10 */ MtDTI    *pFLink;
    /* 0x18 */ MtDTI    *pBLink;
    /* 0x20 */ MtDTI    *pParentObject;

    union
    {
        /* 0x28 */ DWORD    ObjectSize : 24;        // Upper 8 bits are flags, lower 24 bits are object size
        /* 0x2B */ DWORD    Flags : 8;
    };

    /* 0x2C */ DWORD    FileTypeId;
    /* 0x30 */ void     *pUnknown3;

    inline static MtDTI * (__stdcall *_ctor)(MtDTI *thisptr, const char *psTypeName, MtDTI *pParentType, DWORD dwSizeOf, DWORD dwFileType, BYTE flags) =
        GetModuleAddress<MtDTI*(__stdcall*)(MtDTI*, const char*, MtDTI*, DWORD, DWORD, BYTE)>(0x1406184C0);

    inline static MtDTI * (__stdcall *_dtor)(MtDTI *thisptr, bool bFreeMemory) =
        GetModuleAddress<MtDTI*(__stdcall*)(MtDTI*, bool)>(0x1400AF010);

    inline static MtDTI * (__stdcall *_FindDTIByFileType)(DWORD dwFileType, MtDTI *pRoot) = 
        GetModuleAddress<MtDTI*(__stdcall*)(DWORD, MtDTI*)>((void*)0x140618590);

    inline static MtDTI * (__stdcall *_FindDTIByName)(const char *psTypeName, MtDTI *pRoot) =
        GetModuleAddress<MtDTI*(__stdcall*)(const char*, MtDTI*)>(0x1406185F0);


    // Default DTI parent object if not parent object is specified.
    inline static MtDTI *DefaultMtDTIParentObject = GetModuleAddress<MtDTI*>(0x141928468);

    /*
        Parameters:
            - psTypeName: Name of the type
            - pParentType: DTI info for the parent type if this type has a parent, or nullptr otherwise
            - dwSizeOf: Size of the type in bytes
            - dwFileType: File type ID, or 0 to calculate the file type id based on the file name
            - flags: Flag values for the DTI instance
    */
    MtDTI(const char *psTypeName, MtDTI *pParentType, DWORD dwSizeOf, DWORD dwFileType, BYTE flags)
    {
        _ctor(this, psTypeName, pParentType, dwSizeOf, dwFileType, flags);
    }

    ~MtDTI()
    {
        ThisPtrCallNoFixup<void, bool>(this->vtable[0], this, false);
    }

    /*
        Creates a new instance of this object type.
    */
    MtObject * CreateInstance()
    {
        return ThisPtrCallNoFixup<MtObject*>(this->vtable[1], this);
    }

    /*
        Creates a new instance of this object type and casts it to type T.
    */
    template<typename T> T* CreateInstance()
    {
        return (T*)CreateInstance();
    }

    /*
        Description: Checks pRoot and all children recursively for a MtDTI object with the specified file type.

        Parameters:
            - dwFileType: File type id for get DTI info for
            - pRoot: Root element to start searching at

        Returns: Pointer to the MtDTI instance for the specified file type if it was found, or nullptr otherwise.
    */
    static MtDTI * FindDTIByFileType(DWORD dwFileType, MtDTI *pRoot)
    {
        return _FindDTIByFileType(dwFileType, pRoot);
    }

    /*
        Description: Checks pRoot and all children recursively for a MtDTI object with the specified type name.

        Parameters:
            - psTypeName: Type name to search for
            - pRoot: Root element to start searching at

        Returns: Pointer to the MtDTI instance for the specified type name if it was found, or nullptr otherwise.
    */
    static MtDTI * FindDTIByName(const char *psTypeName, MtDTI *pRoot)
    {
        return _FindDTIByName(psTypeName, pRoot);
    }
};
static_assert(sizeof(MtDTI) == 0x38, "MtDTI struct has incorrect size");


/*
    Implements the generic MyDTI class for objects that register debug type info.
*/
#define IMPLEMENT_MYDTI(type, dtiAddr, dtorAddr, createInstAddr) \
struct MyDTI : public MtDTI \
{ \
    inline static MyDTI * (__stdcall *_dtor)(MyDTI *thisptr, bool bFreeMemory) = \
        GetModuleAddress<MyDTI*(__stdcall*)(MyDTI*, bool)>(dtorAddr); \
\
    inline static type * (__stdcall *_CreateInstance)(MyDTI *thisptr) = \
        GetModuleAddress<type*(__stdcall*)(MyDTI*)>(createInstAddr); \
\
    MyDTI(const char *psTypeName, MtDTI *pParentType, DWORD dwSizeOf, DWORD dwFileType, BYTE flags) : \
        MtDTI(psTypeName, pParentType, dwSizeOf, dwFileType, flags) \
    { \
    } \
}; \
\
inline static MtDTI *DebugTypeInfo = GetModuleAddress<MyDTI*>(dtiAddr)


/*
    Implements a singleton pattern for the object.
*/
#define IMPLEMENT_SINGLETON(type, instanceAddr) \
inline static type **_Instance = GetModuleAddress<type**>(instanceAddr); \
\
inline static type * Instance() \
{ \
    return *type::_Instance; \
}



// sizeof = 0x8
struct MtObject
{
    /*
        VTable:
            0x00 ~MtObject();
            0x08
            0x10 bool
            0x18 void RegisterDebugOptions(MtPropertyList *pPropList)
            0x20 MtDTI * GetDTI();
    */

    /* 0x00 */ void **vtable;

    /*
        Description: Adds debug menu options for this object to the propery list.

        Parameters:
            - pPropList: Property list to add debug options to
    */
    void RegisterDebugOptions(MtPropertyList *pPropList)
    {
        ThisPtrCallNoFixup<void, MtPropertyList*>(this->vtable[3], this, pPropList);
    }

    /*
        Gets the debug type info for this object.
    */
    MtDTI * GetDTI()
    {
        return ThisPtrCallNoFixup<MtDTI*>(this->vtable[4], this);
    }
};
static_assert(sizeof(MtObject) == 8, "MtObject incorrect struct size");