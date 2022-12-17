
#pragma once
#include "LibMtFramework.h"
#include "MtFramework/Archive/cResource.h"

//typedef void uSnatcherModel;

// sizeof = 0x70
struct rItemLayout : public cResource
{
    // sizeof = 0x3F0
    struct LayoutInfo : public MtObject
    {
        /* 0x08 */
        /* 0x10 */ BYTE     Shape;
        /* 0x14 */ float    mRectX;
        /* 0x18 */ float    mRectY;
        /* 0x1C */ float    mRadius;

        /* 0x30 */ Vector3  mAngle;
        /* 0x40 */ Vector3  mRangeAngle;
        /* 0x50 */ Vector3  mCursorWorldPos;
        /* 0x60 */ WORD     mMessNo;
        /* 0x62 */ WORD     mMessType;
        // padding?
        /* 0x70 */ Vector3  mMessOffs;
        /* 0x80 */ DWORD    Check;

        /* 0x90 */ Vector3  mModelAngle;
        /* 0xA0 */ Vector3  mCursorWorldPosOffs;
        /* 0xB0 */ DWORD    ItemNo;
        /* 0xB4 */ DWORD    ItemId;
        /* 0xB8 */ DWORD    ItemPhotoId;
        /* 0xBC */ DWORD    ItemUnique;
        /* 0xC0 */ DWORD    ItemSetType;
        /* 0xC4 */ DWORD    ChangeItemNo;
        /* 0xC8 */ DWORD    ChangeItemId;
        /* 0xCC */ DWORD    ChangeItemSetType;
        /* 0xD0 */ uSnatcherModel   *pObject;
        /* 0xD8 */ MtString *pClassName;

        /* 0xE0 */ DWORD    MobSetType;
        /* 0xE4 */ DWORD    MobSetX;
        /* 0xE8 */ DWORD    MobSetZ;
        /* 0xEC */ DWORD    MobSetY;
        /* 0xF0 */ DWORD    MobAdjustX;
        /* 0xF4 */ DWORD    MobAdjustZ;
        /* 0xF8 */ DWORD    MobAdjustY;

        /* 0x18C */ DWORD   BrokenFlag;
        /* 0x190 */ DWORD   BrokenCheckFlag;

        /* 0x198 */ DWORD   EventSystem;

        /* 0x1B8 */ DWORD   EventMain;
        /* 0x1BC */ DWORD   EventMain2;

        /* 0x1C8 */ DWORD   EventSet;

        /* 0x1D8 */ DWORD   EventDoor;

        /* 0x1E8 */ DWORD   EventMess;

        /* 0x208 */ DWORD   EventBroke;

        /* 0x218 */ DWORD   EventPhoto;

        /* 0x228 */ DWORD   EventAppear;

        /* 0x238 */ DWORD   EventDie;

        /* 0x248 */ DWORD   EventTodo;

        /* 0x250 */ DWORD   EventTuto;
        /* 0x254 */ DWORD   EventTuto2;

        /* 0x298 */ DWORD   EventTreasure;

        /* 0x2A8 */ DWORD   Message;

        /* 0x2B8 */ DWORD   EventCase;

        /* 0x358 */ DWORD   EventWaiting;

        /* 0x398 */ DWORD   OneSetFlag0;
        /* 0x39C */ DWORD   OneSetFlag1;
        /* 0x3A0 */ DWORD   OneSetFlag2;
        /* 0x3A4 */ DWORD   OneSetFlag3;
        /* 0x3A8 */ MtString    *pResourceName;
        /* 0x3B0 */ float   HoseiRadius;
        /* 0x3B4 */ float   HoseiHeight;
        /* 0x3B8 */ DWORD   HoseiJntNo;
        /* 0x3C0 */ Vector3 HoseiOffsetPos;
        /* 0x3D0 */ DWORD   ActType;
        /* 0x3D4 */ DWORD   BankNo;
        /* 0x3D8 */ DWORD   MotionNo;
        /* 0x3DC */ DWORD   IconNo;
        /* 0x3E0 */ DWORD   ScrNo1;
        /* 0x3E4 */ DWORD   SrcNo2;
    };

    /* 0x60 */ LayoutInfo   *pLayoutInfoList;
    /* 0x68 */ DWORD        LayoutCount;        // Number of items in the list

    inline static void(__stdcall *_SpawnItems)(rItemLayout *thisptr) =
        (void(__stdcall*)(rItemLayout*))GetModuleAddress(0x1400CCB00);

    /*
        Description: Loops through all item placements and spawns all items on the map.
    */
    void SpawnItems()
    {
        _SpawnItems(this);
    }
};
static_assert(sizeof(rItemLayout) == 0x70, "rItemLayout incorrect struct size");