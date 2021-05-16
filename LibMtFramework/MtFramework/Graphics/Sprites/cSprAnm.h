
#pragma once
#include "MtFramework/MtObject.h"

// sizeof = 0x8
struct rSprAnmHeader
{
    /* 0x00 */ DWORD	Magic;
    /* 0x04 */ WORD		SpriteCount;
    /* 0x06 */ // WORD
};

// sizeof = 0x5C?
struct cAnmSprData
{
    /* 0x00 */ void		        **vtable;
    /* 0x08 */ void		        *pSpriteAnm;
    /* 0x10 */ void             *Unk1;
    /* 0x18 */ rSprAnmHeader	*pSpriteAnmHeader;
    /* 0x20 */ void             *Unk2;                      // void* pointer to second sprite data block
    /* 0x28 */ void		        *pSpriteAnmSpriteData;
    /* 0x30 */ void             *Unk3;
    /* 0x38 */ void		        **ppSpriteEntryBuffers;		// 
    /* 0x40 */ DWORD	        *pSpriteEntryCounts;		// Number of sprite entries in each pSpriteEntryBuffers element
    /* 0x48 */ // void*
    /* 0x50 */ // WORD second count in sprite anm header

    /* 0x54 */ // DWORD/BOOL
    /* 0x58 */ // DWORD/BOOL
};

// sizeof = 0x88
struct cSprAnm : public MtObject
{
    /* 0x08 */
    /* 0x10 */ cAnmSprData  SpriteData;
};
