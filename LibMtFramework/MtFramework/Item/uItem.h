

#pragma once
#include "LibMtFramework.h"
#include "MtFramework/MtObject.h"

// sizeof = 0x18
struct ItemInfoEntry
{
    /* 0x00 */ DWORD        Id;
    /* 0x08 */ const char * Name;
    /* 0x10 */ const char * ArchivePath;
};
static_assert(sizeof(ItemInfoEntry) == 0x18, "ItemInfoEntry incorrect struct size");

enum eItemHealthType : int
{
    HealthType_UseCount = 3
};

// sizeof = ?
struct ItemProperties0
{
    /* 0x00 */ // DWORD 0x140178F8B
    /* 0x04 */ // DWORD 0x140178F8B
    /* 0x08 */ // DWORD effect id to spawn on destruction? 0x140179004
    /* 0x0C */ eItemHealthType      HealthType;     // some kind of item use/health type, e.x.: use health or use count for calculations
    /* 0x10 */ // DWORD bitmask that describes how much damage the item takes on use? 0x1401898C6
};

// sizeof = ?
struct ItemProperties1
{
    /* 0x00 */ Vector4  vUnknown;       // Something related to getting the item's position 0x14018ACB8
};

// sizeof = 0x28
struct ItemProperties
{
    /* 0x00 */ ItemProperties0      *pProperties0;
    /* 0x08 */ ItemProperties1      *pProperties1;
    /* 0x10 */ void                 *pProperties2;
    /* 0x18 */ void                 *pProperties3;
    /* 0x20 */ void                 *pProperties4;
};
static_assert(sizeof(ItemProperties) == 0x28, "ItemProperties incorrect struct size");

// sizeof = 0x10
struct ItemWeaponProperties
{
    /* 0x00 */ WORD     Health;         // Health of the item for melee/throwable weapons
    /* 0x02 */ WORD     UseCount;       // Number of uses the item has for projectile weapons
    /* 0x04 */ // WORD 0x1401BBFEF
    /* 0x06 */ // WORD index into dword_1411BF120
    /* 0x08 */ // WORD sub_1401F56F0 and sub_1402B5DF0, index into dword_1411B37E0 or unk_1411B37F0
    /* 0x0A */ // WORD index into off_1411D9420
    /* 0x0C */ WORD     DamagePropertiesIndex1;     // Index into the damage properties for the item
    /* 0x0E */ WORD     DamagePropertiesIndex2;     // Index into the damage properties for the item
};
//static_assert(sizeof(ItemWeaponProperties) == 0x10, "ItemWeaponProperties incorrect struct size");

// sizeof = 0xA0
struct ItemCollisionProperties
{
    /* 0x00 */ DWORD    ShapeType;      // Could be hkpShapeType but not sure
    /* 0x04 */ DWORD    JointIndex1;
    /* 0x08 */ DWORD    JointIndex2;
    /* 0x10 */ Vector4  HalfExtents;    // Half extends used for box shape collision
    /* 0x20 */ // Vector4 0x140181053
    /* 0x30 */ // Vector4 0x140180CA9
    /* 0x40 */ // Vector4 0x140180CA9
    /* 0x50 */ // float
    /* 0x54 */ // float
    /* 0x58 */ // float

    /* 0x70 */ // Vector4
    /* 0x80 */ // Vector4
    /* 0x90 */ // BYTE
    /* 0x91 */ // BYTE RotationAngleType?
    /* 0x92 */ // BYTE RotationAngleType?
};

// sizeof = 0x3740
struct uItem : public MtObject //: public uSnatcherModel
{
    /* 0x1390 */

    /* 0x2E24 */ float          CurrentHealth;          // Current health of the item
    /* 0x2E28 */ float          StartingHealth;         // Initial health of the item
    /* 0x2E2C */ WORD           CurrentUseCount;        // Current ammo/use count
    /* 0x2E2E */ WORD           StartingUseCount;       // Initial use count for the item

    /* 0x2FA0 */ void           **pItemProperties;

    /* 0x2FB0 */ // void* damage properties from 0x1411EF2D0
    /* 0x2FB8 */ const char     *psModelFilePath;       // File path of the model for this item

    /* 0x36FC */ WORD           ItemId;

    /* 0x3704 */ // WORD at pItemProperties[0] + 0xC, 0x14017F5BD

    /* 0x3723 */ BYTE           DamagePropertyIndex;    // Index into the damage properties for the item

    /* 0x3731 */ bool           ItemIsDestroyed;        // True if the item has been destroyed

    /* 0x37D2 */ // BYTE index into dword_1411BF120, 0xFF to use ItemWeaponProperties index, some kinda flags list?

    /*
        Vtable:

            0x250 
    */

    inline static bool(__stdcall *_HasLowHealth)(uItem *thisptr) =
        GetModuleAddress<bool(__stdcall*)(uItem*)>(0x14018B1A0);

    inline static bool(__stdcall *_SetupItemProperties)(uItem *thisptr) =
        GetModuleAddress<bool(__stdcall*)(uItem*)>(0x14017F550);

    inline static void(__stdcall *_GetItemPosition)(uItem *thisptr, Vector4 *pPosition) =
        GetModuleAddress<void(__stdcall*)(uItem*, Vector4*)>(0x14018AC70);


#define ITEM_COUNT 314

    inline static ItemInfoEntry *ItemInfoTable =
        GetModuleAddress<ItemInfoEntry*>(0x14122C1E0);

    inline static ItemProperties **ItemProperties =
        GetModuleAddress<ItemProperties**>(0x141209CA0);

    inline static ItemCollisionProperties **ItemCollisionProperties =
        GetModuleAddress<ItemCollisionProperties**>(0x141217360);

    /*
        Description: Called when the item is destroyed

        Parameters:
            TODO

        Returns: Zero if an effect should be played for item destruction, or non-zero to skip effect.
    */
    bool OnItemDestroyed(void *unk)     // TODO: confirm return type and parameter type
    {
        return ThisPtrCallNoFixup<bool, void*>(this->vtable[74], this, unk);
    }

    /*
        Returns true if the item's health or use count reach 25% or lower, false otherwise.
    */
    bool HasLowHealth()
    {
        return _HasLowHealth(this);
    }

    /*
        Description: Sets up item properties based on the item id.

        Returns: True if setup was successfull, or false otherwise.
    */
    bool SetupItemProperties()
    {
        return _SetupItemProperties(this);
    }

    /*
        Description: Gets the world position of the item.

        Parameters:
            pPosition - Output variable that receives the item's position.
    */
    void GetItemPosition(Vector4 *pPosition)
    {
        _GetItemPosition(this, pPosition);
    }
};
