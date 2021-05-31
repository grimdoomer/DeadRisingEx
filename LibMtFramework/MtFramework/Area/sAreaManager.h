
#pragma once
#include "MtFramework/MtObject.h"
#include "MtFramework/System/cSystem.h"

// sizeof = 0xA5C0
struct sAreaManager : public cSystem
{
    /* 0x38 */ void     *mpArea;    // cAreaBase?
    /* 0x40 */ DWORD    mAreaNo;
    /* 0x44 */ DWORD    mBlockNo;

    /* 0x4C */ DWORD    mOldAreaNo;

    /* 0x8098 */ void   *mpAreaModel[10];   // uLowScroll

    /* 0x8CAC */ // DWORD number of items to load
    /* 0x8CB0 */ // file load list? entry size = 0x10

    IMPLEMENT_MYDTI(sAreaManager, 0x141945F88, 0x1400AF010, 0x140061F50);

    IMPLEMENT_SINGLETON(sAreaManager, 0x141945F70);

    inline static DWORD(__stdcall *_GetAreaIdFromName)(sAreaManager *thisptr, const char *psAreaName) =
        (DWORD(__stdcall*)(sAreaManager*, const char*))GetModuleAddress(0x14005FEB0);

    /*
        Description: Gets the area id from the specified area name.

        Parameters:
            - psAreaName: Name of the area

        Returns: Id of the area if found
    */
    DWORD GetAreaIdFromName(const char *psAreaName)
    {
        return _GetAreaIdFromName(this, psAreaName);
    }
};