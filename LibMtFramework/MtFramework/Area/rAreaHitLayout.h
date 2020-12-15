/*

*/

#pragma once
#include "LibMtFramework.h"
#include "MtFramework/Archive/cResource.h"

//sizeof = 0x70
struct rAreaHitLayout : public cResource
{
    // sizeof = 0x780
    struct LayoutInfo
    {
        /* 0x10 */ BYTE         Shape;
        /* 0x14 */ float        mRectX;
        /* 0x18 */ float        mRectY;
        /* 0x1C */ float        mRadius;
        
        /* 0x30 */ Vector3      TargetAngle;
        /* 0x40 */ Vector3      RangeAngle;
        /* 0x50 */ Vector3      mCursorWorldPos;

        /* 0x60 */ WORD         MessageNo;
        /* 0x62 */ WORD         MessageType;

        /* 0x70 */ Vector3      CheckMessagePos;
        /* 0x80 */ DWORD        Check;

        /* 0x8C */ BYTE         Day;
        /* 0x8D */ BYTE         Hour;
        /* 0x8E */ BYTE         Minute;
        /* 0x8F */ BYTE         Second;
        /* 0x90 */ BYTE         Day1;
        /* 0x91 */ BYTE         Hour1;
        /* 0x92 */ BYTE         Minute1;
        /* 0x93 */ BYTE         Second1;
        /* 0x94 */ DWORD        Trriger;
        /* 0x98 */ DWORD        Success;
        /* 0x9C */ DWORD        Item;
        /* 0xA0 */ WORD         CheckMessageNo;
        /* 0xA2 */ WORD         CheckMessageType;

        /* 0xA8 */ DWORD        CheckEventSystem;

        /* 0xB8 */ DWORD        CheckEventMain5;

        /* 0xC8 */ DWORD        CheckEventMain;
        /* 0xCC */ DWORD        CheckEventMain2;
        /* 0xD0 */ DWORD        CheckEventBoss;
        /* 0xD4 */ DWORD        CheckEventEvs2;
        /* 0xD8 */ DWORD        CheckEventEmSet00;
        /* 0xDC */ DWORD        CheckEmSet01;

        /* 0xE8 */ DWORD        CheckEventDoor;

        /* 0xF8 */ DWORD        CheckEventMess;

        /* 0x118 */ DWORD       CheckEventBroke;

        /* 0x128 */ DWORD       CheckEventPhoto;

        /* 0x138 */ DWORD       CheckEventEmAppear;

        /* 0x148 */ DWORD       CheckEventDie;

        /* 0x150 */ DWORD       CheckEventEtc;

        /* 0x158 */ DWORD       CheckEventTudo;

        /* 0x160 */ DWORD       CheckEventTuto;
        /* 0x164 */ DWORD       CheckEventTuto2;
        
        /* 0x1A8 */ DWORD       CheckEventTreasure;
        /* 0x1AC */ DWORD       CheckEventTreasure2;

        /* 0x1B4 */ DWORD       CheckContentsKey;
        /* 0x1B8 */ DWORD       CheckEventMessage;

        /* 0x1C8 */ DWORD       CheckEventCase;

        /* 0x208 */ DWORD       CheckAchivement;
        /* 0x20C */ DWORD       CheckAchivement2;

        /* 0x248 */ DWORD       CheckEventArea;

        /* 0x268 */ DWORD       CheckEventWaitting;

        /* 0x2A8 */ DWORD       SuccessEventSystem;

        /* 0x2B8 */ DWORD       SuccessEventMain5;

        /* 0x2C8 */ DWORD       SuccessEventMain;
        /* 0x2CC */ DWORD       SuccessEventMain2;
        /* 0x2D0 */ DWORD       SuccessEventBoss;
        /* 0x2D4 */ DWORD       SuccessEventEvs;
        union
        {
            /* 0x2D8 */ DWORD       SuccessSetMain;
            /* 0x2D8 */ DWORD       SuccessEmSet00;
        };
        /* 0x2DC */ DWORD       SuccessEmSet01;

        /* 0x2E8 */ DWORD       SuccessEventDoor;

        /* 0x2F8 */ DWORD       SuccessEventMess;

        /* 0x318 */ DWORD       SuccessEventBroke;

        /* 0x328 */ DWORD       SuccessEventPhoto;

        /* 0x338 */ DWORD       SuccessEventEmAppear;

        /* 0x348 */ DWORD       SuccessEventDie;

        /* 0x350 */ DWORD       SuccessEventEtc;

        /* 0x358 */ DWORD       SuccessEventTudo;

        /* 0x360 */ DWORD       SuccessEventTuto;
        /* 0x364 */ DWORD       SuccessEventTuto2;

        /* 0x3A8 */ DWORD       SuccessEventTreasure;
        /* 0x3AC */ DWORD       SuccessEventTreasure2;

        /* 0x3B4 */ DWORD       SuccessContentsKey;
        /* 0x3B8 */ DWORD       SuccessEventMessage;

        /* 0x3C8 */ DWORD       SuccessEventCase;
        
        /* 0x408 */ DWORD       SuccessAchivement;
        /* 0x40C */ DWORD       SuccessAchivement2;
        
        /* 0x448 */ DWORD       SuccessEventArea;

        /* 0x468 */ DWORD       SuccessEventWaitting;

        /* 0x4A8 */ DWORD       CheckNotEventSystem;

        /* 0x4B8 */ DWORD       CheckNotEventMain5;

        /* 0x4C8 */ DWORD       CheckNotEventMain;
        /* 0x4CC */ DWORD       CheckNotEventMain2;
        /* 0x4D0 */ DWORD       CheckNotEventBoss;
        /* 0x4D4 */ DWORD       CheckNotEventEvs;
        /* 0x4D8 */ DWORD       CheckNotEventEmSet00;
        /* 0x4DC */ DWORD       CheckNotEmSet01;

        /* 0x4E8 */ DWORD       CheckNotEventDoor;

        /* 0x4F8 */ DWORD       CheckNotEventMess;

        /* 0x518 */ DWORD       CheckNotEventBroke;

        /* 0x528 */ DWORD       CheckNotEventPhoto;

        /* 0x538 */ DWORD       CheckNotEventEmAppear;

        /* 0x548 */ DWORD       CheckNotEventDie;

        /* 0x550 */ DWORD       CheckNotEventEtc;

        /* 0x558 */ DWORD       CheckNotEventTudo;

        /* 0x560 */ DWORD       CheckNotEventTuto;
        /* 0x564 */ DWORD       CheckNotEventTuto2;

        /* 0x5A8 */ DWORD       CheckNotEventTreasure;
        /* 0x5AC */ DWORD       CheckNotEventTreasure2;

        /* 0x5B4 */ DWORD       CheckNotContentsKey;
        /* 0x5B8 */ DWORD       CheckNotEventMessage;

        /* 0x5C8 */ DWORD       CheckNotEventCase;
        
        /* 0x608 */ DWORD       CheckNotAchivement;
        /* 0x60C */ DWORD       CheckNotAchivement2;

        /* 0x648 */ DWORD       CheckNotEventArea;

        /* 0x668 */ DWORD       CheckNotEventWaitting;

        /* 0x6A8 */ DWORD       EventTimeNo;
        
        /* 0x6B0 */ MtString    *mFileName[4];
        /* 0x6D0 */ MtString    *AreaJumpName;
        /* 0x6E0 */ Vector3     AreaJumpPos;
        /* 0x6F0 */ Vector3     AreaJumpAngle;
        /* 0x700 */ MtString    *AreaCheckName;
        /* 0x708 */ MtString    *EventName;
        /* 0x710 */ DWORD       SqoopNo;
        /* 0x714 */ DWORD       AreaDoorNo;
        /* 0x718 */ INT32       ClothNo;
        /* 0x71C */ INT32       ClothId;
        /* 0x720 */ DWORD       ActType;
        /* 0x724 */ DWORD       BankNo;
        /* 0x728 */ DWORD       MoitonNo;
        /* 0x72C */ DWORD       IconNo;
    };

    /* 0x60 */ LayoutInfo   *pLayoutInfoList;
    /* 0x68 */ DWORD        LayoutCount;        // Number of items in the list
};