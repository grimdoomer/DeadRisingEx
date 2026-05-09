/*

*/

#pragma once
#include <Windows.h>
#include "Utilities/Module.h"
#include "MtFramework/Utils/MtPropertyList.h"

struct cResource;
struct MtObject;

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

    inline static MtDTI * (*_ctor)(MtDTI *thisptr, const char *psTypeName, MtDTI *pParentType, DWORD dwSizeOf, DWORD dwFileType, BYTE flags) =
        (MtDTI*(*)(MtDTI*, const char*, MtDTI*, DWORD, DWORD, BYTE))GetModuleAddress(0x1406184C0);

    inline static MtDTI * (*_dtor)(MtDTI *thisptr, unsigned int flags) =
        (MtDTI*(*)(MtDTI*, unsigned int))GetModuleAddress(0x1400AF010);

    inline static MtDTI * (*_FindDTIByFileType)(DWORD dwFileType, MtDTI *pRoot) = 
        (MtDTI*(*)(DWORD, MtDTI*))GetModuleAddress((void*)0x140618590);

    inline static MtDTI * (*_FindDTIByName)(const char *psTypeName, MtDTI *pRoot) =
        (MtDTI*(*)(const char*, MtDTI*))GetModuleAddress(0x1406185F0);


    // Default DTI parent object if not parent object is specified.
    inline static MtDTI *DefaultMtDTIParentObject = (MtDTI*)GetModuleAddress(0x141928468);

    /*
        Parameters:
            - psTypeName: Name of the type
            - pParentType: DTI info for the parent type if this type has a parent, or nullptr otherwise
            - dwSizeOf: Size of the type in bytes
            - dwFileType: File type ID, or 0 to calculate the file type id based on the file name
            - flags: Flag values for the DTI instance
    */
    SHIM_API MtDTI(const char* psTypeName, MtDTI* pParentType, DWORD dwSizeOf, DWORD dwFileType, BYTE flags) SHIM_BODY(0x1406184C0)

    SHIM_API ~MtDTI() SHIM_BODY_DTOR_VCALL()

    /*
        Creates a new instance of this object type.
    */
    MtObject * CreateInstance()
    {
        return (MtObject*)ThisPtrCallNoFixup(this->vtable[1], this);
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
ASSERT_STRUCT_SIZE(MtDTI, 0x38);


/*
    Implements the generic MyDTI class for objects that register debug type info.
*/
#define IMPLEMENT_MYDTI(type, dtiAddr, dtorAddr, createInstAddr) \
struct MyDTI : public MtDTI \
{ \
    inline static MyDTI * (*_dtor)(MyDTI *thisptr, unsigned int flags) = \
        (MyDTI*(*)(MyDTI*, unsigned int))GetModuleAddress(dtorAddr); \
\
    inline static type * (*_CreateInstance)(MyDTI *thisptr) = \
        (type*(*)(MyDTI*))GetModuleAddress(createInstAddr); \
\
    MyDTI(const char *psTypeName, MtDTI *pParentType, DWORD dwSizeOf, DWORD dwFileType, BYTE flags) : \
        MtDTI(psTypeName, pParentType, dwSizeOf, dwFileType, flags) \
    { \
    } \
}; \
\
inline static MtDTI *DebugTypeInfo = (MyDTI*)GetModuleAddress(dtiAddr)


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

    inline static void* (*_scalar_deleting_dtor)(MtObject* thisptr, unsigned int flags) =
        (void* (*)(MtObject*, unsigned int))GetModuleAddress(0x1400C72B0);

    SHIM_API ~MtObject() SHIM_BODY_DTOR_VCALL()

    /*
        Description: Adds debug menu options for this object to the propery list.

        Parameters:
            - pPropList: Property list to add debug options to
    */
    void RegisterDebugOptions(MtPropertyList *pPropList)
    {
        (void)ThisPtrCallNoFixup(this->vtable[3], this, pPropList);
    }

    /*
        Gets the debug type info for this object.
    */
    MtDTI * GetDTI()
    {
        return (MtDTI*)ThisPtrCallNoFixup(this->vtable[4], this);
    }

    void* operator new(size_t size);

    void operator delete(void* ptr);
};
ASSERT_STRUCT_SIZE(MtObject, 8);