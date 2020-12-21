
#pragma once
#include "LibMtFramework.h"
#include "MtFramework/System/cSystem.h"
#include "MtFramework/Utils/MtPropertyList.h"

// sizeof = 0x2270
struct sToolBase : public cSystem
{
    /* 0x38 */ MtPropertyList   DebugOptionList;
    /* 0x48 */ DWORD            Unk1;       // Number of options?
    /* 0x4C */ DWORD            Unk4;
    /* 0x50 */ BYTE             Unk2[8704];
    /* 0x2250 */ DWORD          Unk3;
    /* 0x2254 */ BOOL           ShowDebugMenu;
    /* 0x2255 */ BYTE           mSysMenuDisp;
    /* 0x2258 */ DWORD          mSysMenuFontW;
    /* 0x225C */ DWORD          mSysMenuFontH;
    /* 0x2260 */ DWORD          mSysMenuMargin;
    /* 0x2264 */ DWORD          Unk7; // might be padding, previous field might be qword
    /* 0x2268 */ DWORD          Unk8;
};
static_assert(sizeof(sToolBase) == 0x2270, "sToolBase incorrect struct size");