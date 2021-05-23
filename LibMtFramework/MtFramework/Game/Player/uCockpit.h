
#pragma once
#include "MtFramework/Object/cUnit.h"

// sizeof = 0x4340
struct uCockpit : public cUnit
{
    // Indices into mGrobalCockpitAlpha array:
    #define GROUP_ALPHA_MAIN        0
    #define GROUP_ALPHA_BACK_MENU   1
    #define GROUP_ALPHA_DATE        2
    //
    #define GROUP_ALPHA_ETC         4

    /* 0x28 */

    /* 0x15A */ WORD        mMesKind;

    /* 0x160 */ float       mGrobalCockpitAlpha[5];     // Typo is intentional, see GROUP_ALPHA_* above

    /* 0x198 */ bool        MESSLISTSTATUS;
    /* 0x199 */ bool        MESSLISTTODO;
    /* 0x19A */ bool        MESSLISTNOTE;
    /* 0x19B */ bool        MESSLISTVOICE;

    /* 0x19D */ bool        mDispTimeFlag;

    /* 0x302 */ INT16       mDebugTimer;

    /* 0x30E */ WORD        mTestMesId;
    /* 0x310 */ INT16       mTestMesCnt;
    /* 0x312 */ INT16       mTestMesTimer;
    /* 0x314 */ BYTE        mTestMesPrio;
    /* 0x315 */ BYTE        mTestMesType;
    /* 0x316 */ BYTE        PrimType;
    /* 0x317 */ BYTE        PrimNo;                     // PrimCmd
    /* 0x318 */ BYTE        PolyNo;

    /* 0x320 */ Vector2     Point;
    /* 0x328 */ float       ScaleW;
    /* 0x32C */ float       ScaleH;
    /* 0x330 */ float       Ratio;
    /* 0x334 */ DWORD       Color;
    /* 0x338 */ bool        CmdStock;
    /* 0x339 */ BYTE        PrimNo1;                    // PrimParam
    /* 0x33A */ BYTE        PolyNo1;
    /* 0x33B */ BYTE        PolyNo2;
    /* 0x33C */ BYTE        PolyNo3;

    /* 0x341 */ bool        Reflect[4];

    struct // sizeof = 0x24
    {
        /* 0x34C */ float       PosX;
        /* 0x350 */ float       PosY;
        /* 0x354 */ float       Width;
        /* 0x358 */ float       Height;
        /* 0x35C */ float       ScaleX;
        /* 0x360 */ float       ScaleY;
        /* 0x364 */ BYTE        SrcAlphaMode;
        /* 0x365 */ BYTE        DestAlphaMode;
        /* 0x366 */ BYTE        AlphaOpt;

        /* 0x368 */ DWORD       Color1;
        /* 0x36C */ DWORD       Color2;
    } Polygon[4];

    /* 0x3D4 */ Vector2     mDebugPPPos;
    /* 0x3DC */ DWORD       mDebugPP;
    /* 0x3E0 */ bool        WEAPONDISP;
    /* 0x3E1 */ bool        LIFEDISP;
    /* 0x3E2 */ bool        RESOURCEDISP;
    /* 0x3E3 */ bool        INFOGETDISP;
    /* 0x3E4 */ bool        NPCDISPAREA;
    /* 0x3E5 */ bool        DISPSTATUSALL;
    /* 0x3E6 */ bool        DISPNPCPROFILEALL;
    /* 0x3E7 */ bool        SIGNBOARDPOSDISP;
    /* 0x3E8 */ bool        ISINTERSECTOFF;
    /* 0x3E9 */ bool        NPCNAMEPLATE;

    // mItemIcon:
    struct
    {
        /* 0x588 */ INT     ItemDispNumRate;
        /* 0x58C */ INT     ItemDispNumRateOld;
        /* 0x590 */ INT     ItemDispRate;
        /* 0x594 */ INT     ItemDevelopSpd;

        /* 0x5AC */ WORD    ItemNo;

        /* 0x5B0 */ BYTE    ItemDispNum;
        /* 0x5B1 */ BYTE    ItemDispNumNext;
    } mItemIcon;

    // mBreakingNews:
    struct
    {
        /* 0x5E8 */ DWORD       Color[2];
        /* 0x5F0 */ float       PosX[2];
        /* 0x5F8 */ float       PosY[2];
        /* 0x600 */ float       Spd[2];

        /* 0x60C */ float       StrWidth;
        /* 0x610 */ float       StrHeight;
        /* 0x614 */ float       ScreenUp;
        /* 0x618 */ float       ScreenLow;
        /* 0x61C */ INT16       Timer;

        /* 0x626 */ INT16       MesId;

        /* 0x62A */ INT8        Cnt;
    } mBreakingNews;

    // mPlLife:
    struct
    {
        /* 0x74D */ BYTE    BarCnt;

        /* 0x74F */ BYTE    BarCntMax;
    } mPlLife;

    // mNpcLead:
    struct
    {
        /* 0x758 */ BYTE    NpcNum;

        /* 0x75A */ BYTE    Mode;
    } mNpcLead;

    // mExperience:
    struct
    {
        /* 0x7D0 */ INT32       Now;
        /* 0x7D4 */ INT32       Old;
        /* 0x7D8 */ INT32       Disp;
        /* 0x7DC */ INT32       Max;
    } mExperience;

    /* 0x27D4 */ BOOL       SUB;
};