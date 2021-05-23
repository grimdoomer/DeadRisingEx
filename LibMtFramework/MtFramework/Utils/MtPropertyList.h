
#pragma once
#include "LibMtFramework.h"

struct MtObject;

// pointer 2?
#define MT_PROP_TYPE_BOOL           3
#define MT_PROP_TYPE_BYTE           4
#define MT_PROP_TYPE_WORD           5
#define MT_PROP_TYPE_DWORD          6
#define MT_PROP_TYPE_QWORD          7
#define MT_PROP_TYPE_SBYTE          8
#define MT_PROP_TYPE_SWORD          9
#define MT_PROP_TYPE_SINT           0xA
#define MT_PROP_TYPE_LONGLONG       0xB
#define MT_PROP_TYPE_FLOAT          0xC
//
#define MT_PROP_TYPE_MTSTRING       0xE
// COLORARGB 0xf?
// float POINT/vector2 0x10?
#define MT_PROP_TYPE_VECTOR3        0x14
// vector4 0x15?
#define MT_PROP_TYPE_QUATERNION     0x16
// function callback 0x18?
// section header 0x19?
// line break 0x1F?
#define MT_PROP_TYPE_STRING_PTR	    0x20

// float POINT/vector2 0x22?

#define MT_PROP_FLAG_READ_ONLY      1       // Field is read only
// section header 0x4?

typedef void(*MtPropertyEntry_ItemAction)(void *pObject);
typedef void(*MtPropertyEntry_GetArrayLength)(void *pResource);

// sizeof = 0x50
struct MtPropertyListEntry
{
    /* 0x00 */ const char   *pPropertyName;
    /* 0x08 */ WORD         PropertyType;           // See MT_PROP_TYPE_* above
    /* 0x0A */ WORD         Flags;
    /* 0x10 */ MtObject     *pOwnerObject;
    /* 0x18 */ void         *pGetter;
    /* 0x20 */ MtPropertyEntry_GetArrayLength   *pGetArrayLength;
    /* 0x28 */ void         *pUnkFunc1;
    /* 0x30 */ void         *pUnkFunc2;
    /* 0x38 */ void         *pUnkFunc3;
    /* 0x40 */ MtPropertyListEntry  *pBLink;
    /* 0x48 */ MtPropertyListEntry  *pFLink;
};
static_assert(sizeof(MtPropertyListEntry) == 0x50, "MtPropertyListEntry incorrect struct size");

// sizeof = 0x10
struct MtPropertyList
{
    /* 0x00 */ void                 **vtable;
    /* 0x08 */ MtPropertyListEntry  *pHead;

    /*
        VTable:
            0x00 dtor
    */

    inline static MtPropertyList * (__stdcall *_ctor)(MtPropertyList *thisptr) =
        GetModuleAddress<MtPropertyList*(__stdcall*)(MtPropertyList*)>(0x1406188A0);

    inline static MtPropertyList * (__stdcall *_dtor)(MtPropertyList *thisptr, bool bFreeMemory) =
        GetModuleAddress<MtPropertyList*(__stdcall*)(MtPropertyList*, bool)>(0x140618910);

    inline static MtPropertyListEntry * (__stdcall *_AllocatePropertyEntry)(MtPropertyList *thisptr) =
        GetModuleAddress<MtPropertyListEntry*(__stdcall*)(MtPropertyList*)>(0x140618AB0);

    inline static MtPropertyListEntry * (__stdcall *_GetFirstNode)(MtPropertyList *thisptr) =
        GetModuleAddress<MtPropertyListEntry*(__stdcall*)(MtPropertyList*)>(0x140618950);

    inline static MtPropertyListEntry * (__stdcall *_FindProperty)(MtPropertyList *thisptr, DWORD propertyType, const char *psPropertyName) =
        GetModuleAddress<MtPropertyListEntry*(__stdcall*)(MtPropertyList*, DWORD, const char*)>(0x140618A40);

    MtPropertyList()
    {
        _ctor(this);
    }

    ~MtPropertyList()
    {
        ThisPtrCallNoFixup<void, bool>(this->vtable[0], this, false);
    }

    /*
        Description: Allocates a new property entry from the global property list buffer. The entry is NOT
            linked to the MtPropertyList that allocated it.

        Returns: Pointer to a new MtPropertyListEntry object.
    */
    MtPropertyListEntry * AllocatePropertyEntry()
    {
        return _AllocatePropertyEntry(this);
    }

    /*
        Gets the first node in the linked list or nullptr if no nodes exist.
    */
    MtPropertyListEntry * GetFirstNode()
    {
        return _GetFirstNode(this);
    }

    /*
        Description: Finds a property with matching type and name in the property list.

        Parameters:
            propertyType: Property type, see MT_PROP_TYPE_* above
            psPropertyName: Name of the property (string comparison is case sensitive)

        Returns: A pointer to the MtPropertyListEntry for the property if it was found, nullptr otherwise
    */
    MtPropertyListEntry * FindProperty(DWORD propertyType, const char *psPropertyName)
    {
        return _FindProperty(this, propertyType, psPropertyName);
    }
};
static_assert(sizeof(MtPropertyList) == 0x10, "MtPropertyList incorrect struct size");