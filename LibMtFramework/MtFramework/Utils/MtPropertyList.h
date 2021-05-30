
#pragma once
#include "LibMtFramework.h"
#include <xmmintrin.h>

struct MtObject;

#define MT_PROP_TYPE_POINTER1       1
#define MT_PROP_TYPE_POINTER2       2
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
#define MT_PROP_TYPE_DOUBLE         0xD
#define MT_PROP_TYPE_MTSTRING       0xE
// COLORARGB 0xf?
// float POINT/vector2 0x10?
#define MT_PROP_TYPE_VECTOR3        0x14
#define MT_PROP_TYPE_VECTOR4        0x15
#define MT_PROP_TYPE_QUATERNION     0x16
// function callback 0x18?
#define MT_PROP_TYPE_SUB_SECTION_START  0x19

#define MT_PROP_TYPE_SUB_SECTION_END    0x1F
#define MT_PROP_TYPE_STRING_PTR	    0x20

// float POINT? 0x22?
// 0x140621E1C (3 floats) 0x23
// vector4 0x24?

#define MT_PROP_FLAG_READ_ONLY      1       // Field is read only
// clickable 0x2?
// getter is function callback 0x8?
#define MT_PROP_FLAG_IS_ARRAY               0x20    // Property is an array (valid for types 1 and 2), also used to pass a second parameter to getter callback (array index?)
#define MT_PROP_FLAG_USE_FUNCTION_POINTERS  0x80    // pFieldValue and pArrayLength are function pointers

typedef unsigned long long(*MtPropertyEntry_GetValue)(void *pOwner, DWORD arrayIndex);
typedef unsigned long long(*MtPropertyEntry_GetArrayLength)(void *pResource);
typedef void(*MtPropertyEntry_ItemAction)(void *pOwner, __int64 arg);
//typedef void(*MtPropertyEntry_SetFieldValue)(void *pOwner, DWORD arrayIndex);

// sizeof = 0x50
struct MtPropertyListEntry
{
    /* 0x00 */ const char   *pPropertyName;
    /* 0x08 */ WORD         PropertyType;           // See MT_PROP_TYPE_* above
    /* 0x0A */ WORD         Flags;
    /* 0x10 */ MtObject     *pOwnerObject;                          // Object that owns the field, acts as a this pointer
    /* 0x18 */ MtPropertyEntry_GetValue         pFieldValue;        // If MT_PROP_FLAG_USE_FUNCTION_POINTERS is not set pFieldValue is the address of the field, if flag is set is a function pointer to get the field value
    /* 0x20 */ MtPropertyEntry_GetArrayLength   pArrayLength;       // If MT_PROP_FLAG_USE_FUNCTION_POINTERS is not set pArrayLength is the length of the array, if flag is set is a function pointer to get the array length
    /* 0x28 */ MtPropertyEntry_ItemAction       pSetFieldValue;     // Used when MT_PROP_FLAG_USE_FUNCTION_POINTERS is set as an "action" callback when the item is clicked in the UI
    /* 0x30 */ void         *pUnkFunc2;
    /* 0x38 */ void         *pUnkFunc3;                             // Current index in the array if MT_PROP_FLAG_IS_ARRAY flag is set
    /* 0x40 */ MtPropertyListEntry  *pBLink;
    /* 0x48 */ MtPropertyListEntry  *pFLink;

    inline static BYTE(__stdcall *_GetValueByte)(MtPropertyListEntry *thisptr) =
        (BYTE(__stdcall*)(MtPropertyListEntry*))GetModuleAddress(0x140620FF0);

    inline static WORD(__stdcall *_GetValueWord)(MtPropertyListEntry *thisptr) =
        (WORD(__stdcall*)(MtPropertyListEntry*))GetModuleAddress(0x1406219B0);

    inline static DWORD(__stdcall *_GetValueDword)(MtPropertyListEntry *thisptr) =
        (DWORD(__stdcall*)(MtPropertyListEntry*))GetModuleAddress(0x1406219F0);

    inline static unsigned long long(__stdcall *_GetValueQword)(MtPropertyListEntry *thisptr) =
        (unsigned long long(__stdcall*)(MtPropertyListEntry*))GetModuleAddress(0x140621CF0);

    inline static float(__stdcall *_GetValueFloat)(MtPropertyListEntry *thisptr) =
        (float(__stdcall*)(MtPropertyListEntry*))GetModuleAddress(0x1406214A0);

    inline static double(__stdcall *_GetValueDouble)(MtPropertyListEntry *thisptr) =
        (double(__stdcall*)(MtPropertyListEntry*))GetModuleAddress(0x1406214E0);

    inline static __int64(__stdcall *_GetPrimitiveFieldValue)(MtPropertyListEntry *thisptr) =
        (__int64(__stdcall*)(MtPropertyListEntry*))GetModuleAddress(0x140621520);
    
    inline static void(__stdcall *_GetValueVector)(MtPropertyListEntry *thisptr, Vector4 *pVector) =
        (void(__stdcall*)(MtPropertyListEntry*,Vector4*))GetModuleAddress(0x140621D30);

    inline static MtObject *(__stdcall *_GetValueMtObject)(MtPropertyListEntry *thisptr) =
        (MtObject*(__stdcall*)(MtPropertyListEntry*))GetModuleAddress(0x140621210);

    inline static const char *(__stdcall *_GetValueString)(MtPropertyListEntry *thisptr) =
        (const char *(__stdcall*)(MtPropertyListEntry*))GetModuleAddress(0x140621B10);

    inline static const char *(__stdcall *_GetDisplayName)(const char *psPropertyName) =
        (const char*(__stdcall*)(const char*))GetModuleAddress(0x1407A5AB0);

    inline static unsigned long long(__stdcall *_GetArrayLength)(MtPropertyListEntry *thisptr) =
        (unsigned long long(__stdcall*)(MtPropertyListEntry*))GetModuleAddress(0x140621320);

    /*
        Gets the property value as a byte, used for bool, BYTE, and SBYTE
    */
    BYTE GetValueByte()
    {
        return _GetValueByte(this);
    }

    /*
        Gets the property value as a WORD, used for WORD and SWORD
    */
    WORD GetValueWord()
    {
        return _GetValueWord(this);
    }

    /*
        Gets the property value as a DWORD, used for DWORD and SINT
    */
    DWORD GetValueDword()
    {
        return _GetValueDword(this);
    }

    /*
        Gets the property value as a QWORD, used for QWORD and LONGLONG
    */
    unsigned long long GetValueQword()
    {
        return _GetValueQword(this);
    }

    /*
        Gets the value as a float
    */
    float GetValueFloat()
    {
        return _GetValueFloat(this);
    }

    /*
        Gets the value as a double
    */
    double GetValueDouble()
    {
        return _GetValueDouble(this);
    }

    /*
        Gets the field value for primitive field types, bool, S/BYTE, S/WORD, S/INT32, S/INT64, float, double. Floating
        point values are converted into 32-bit integers
    */
    __int64 GetPrimitiveFieldValue()
    {
        return _GetPrimitiveFieldValue(this);
    }

    /*
        Gets the value as a vector, used for Vector3, Vector4, Quaternion, and other multi-float types
    */
    void GetValueVector(Vector4 *pVector)
    {
        _GetValueVector(this, pVector);
    }

    /*
        Gets the value as a MtObject, used for pointer types
    */
    MtObject * GetValueMtObject()
    {
        return _GetValueMtObject(this);
    }

    /*
        Gets the value as a string
    */
    const char * GetValueString()
    {
        return _GetValueString(this);
    }

    /*
        Description: Gets a name suitable for displaying in the debug menu ui for the specified name

        Parameters:
            - psPropertyName: Property name or MtObject name string
    */
    static const char * GetDisplayName(const char *psPropertyName)
    {
        return _GetDisplayName(psPropertyName);
    }

    /*
        Gets the length of the array if the field is an array
    */
    unsigned long long GetArrayLength()
    {
        return _GetArrayLength(this);
    }
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
        (MtPropertyList*(__stdcall*)(MtPropertyList*))GetModuleAddress(0x1406188A0);

    inline static MtPropertyList * (__stdcall *_dtor)(MtPropertyList *thisptr, bool bFreeMemory) =
        (MtPropertyList*(__stdcall*)(MtPropertyList*, bool))GetModuleAddress(0x140618910);

    inline static MtPropertyListEntry * (__stdcall *_AllocatePropertyEntry)(MtPropertyList *thisptr) =
        (MtPropertyListEntry*(__stdcall*)(MtPropertyList*))GetModuleAddress(0x140618AB0);

    inline static MtPropertyListEntry * (__stdcall *_GetFirstNode)(MtPropertyList *thisptr) =
        (MtPropertyListEntry*(__stdcall*)(MtPropertyList*))GetModuleAddress(0x140618950);

    inline static MtPropertyListEntry * (__stdcall *_FindProperty)(MtPropertyList *thisptr, DWORD propertyType, const char *psPropertyName) =
        (MtPropertyListEntry*(__stdcall*)(MtPropertyList*, DWORD, const char*))GetModuleAddress(0x140618A40);

    MtPropertyList()
    {
        _ctor(this);
    }

    ~MtPropertyList()
    {
        (void)ThisPtrCallNoFixup(this->vtable[0], this, false);
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