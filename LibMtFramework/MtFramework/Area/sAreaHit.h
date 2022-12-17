

#pragma once
#include "LibMtFramework.h"
#include "MtFramework/System/cSystem.h"

enum eAreaHitEntryType : int
{
    TYPE_INVALID = 0xFF,
    TYPE_AREAJUMP = 0,
    TYPE_EVENT = 1,
    TYPE_ITEM = 2,
    TYPE_NPC = 3,
    TYPE_NPC_EVENT = 4,
    TYPE_WATER = 5,
    TYPE_EM = 6
};

// sizeof = 0x18
struct sAreaHitResourceEntry
{
    /* 0x00 */ eAreaHitEntryType    Type;
    /* 0x08 */ cResource            *pResource;     // Can be one of rAreaHitLayout, rItemLayout
    /* 0x10 */ const char           *psFilePath;    // File path of the layout file
};
static_assert(sizeof(sAreaHitResourceEntry) == 0x18, "sAreaHitResourceEntry incorrect struct size");

// sizeof = 0x28
struct sAreaHitStringEntry
{
    /* 0x00 */ // void*?
    /* 0x08 */ // void*?
    /* 0x10 */ // void*?
    /* 0x18 */ // DWORD?
    /* 0x20 */ // void* pointer to some string allocation thing
};

// sizeof = 0x20be0
struct sAreaHit : public cSystem
{
    /* 0x38 */ DWORD        ResourceCount;      // Number of items in the ResourceEntries array

    /* 0x71 */ // BYTE[64] related to ResourceEntries array

    /* 0xF8 */ sAreaHitResourceEntry    ResourceEntries[64];
    /* 0x6F8 */ sAreaHitStringEntry     StringEntries[32];      // This may be 31 entries not 32

    inline static void(__stdcall *_SpawnItems)(sAreaHit *thisptr) =
        (void(__stdcall*)(sAreaHit*))GetModuleAddress(0x14005E080);
};
