
#pragma once
#include "LibMtFramework.h"
#include "MtFramework/System/cSystem.h"
#include "MtFramework/Object/cUnit.h"

// sizeof = 0x538
struct sUnit : public cSystem
{
    // sizeof = 0x28
    struct MoveLine : public MtObject
    {
        #define MOVE_LINE_FLAGS_PARALLEL        1
        #define MOVE_LINE_FLAGS_PAUSE           2

        #define MOVE_LINE_FLAGS_INDEX_MASK      0x3F
        #define MOVE_LINE_FLAGS_INDEX_SHIFT     2

        /* 0x08 */ const char   *mName;
        /* 0x10 */ DWORD        Flags;      // Lower 2 are MOVE_LINE_FLAGS_*, next 6 are line index
        /* 0x18 */ cUnit        *pBLink;
        /* 0x20 */ cUnit        *pFLink;

        inline static MoveLine * (*_ctor)(MoveLine *thisptr) =
            (MoveLine*(*)(MoveLine*))GetModuleAddress(0x14062FF60);

        inline static MoveLine * (*_dtor)(MoveLine *thisptr, bool bFreeMemory) =
            (MoveLine*(*)(MoveLine*, bool))GetModuleAddress(0x1400C72B0);

        IMPLEMENT_MYDTI(MoveLine, 0x141CF2628, 0x1400AF010, 0x140630900);

        MoveLine()
        {
            _ctor(this);
        }
    };
    ASSERT_STRUCT_SIZE(MoveLine, 0x28);


    /* 0x38 */ MoveLine     MoveLines[32];

    /*
        VTable:
            0x40
            0x48
            0x50 const char * GetMoveLineName(DWORD dwLineIndex)
            0x58
    */

    inline static sUnit * (*_ctor)(sUnit *thisptr) =
        (sUnit*(*)(sUnit*))GetModuleAddress(0x14062FF90);

    inline static sUnit * (*_dtor)(sUnit *thisptr, bool bFreeMemory) =
        (sUnit*(*)(sUnit*, bool))GetModuleAddress(0x140630030);

    inline static bool(*_AddObjectToMoveLine)(sUnit *thisptr, DWORD dwLineIndex, cUnit *pObject) =
        (bool(*)(sUnit*, DWORD, cUnit*))GetModuleAddress(0x1406300B0);

    inline static const char * (*_GetMoveLineName)(sUnit *thisptr, DWORD dwLineIndex) =
        (const char*(*)(sUnit*, DWORD))GetModuleAddress(0x1406305C0);

    IMPLEMENT_MYDTI(sUnit, 0x141CF2658, 0x1400AF010, 0x140630950);

    IMPLEMENT_SINGLETON(sUnit, 0x141CF2620);

    sUnit()
    {
        _ctor(this);
    }

    /*
        Description: Adds an object to the specified move line list.

        Parameters:
            - dwLineIndex: Move line index to add the object to
            - pObject: Object to be added to the move line list

        Returns: True if the object was added to the move line list successfully, false otherwise.
    */
    bool AddObjectToMoveLine(DWORD dwLineIndex, cUnit *pObject)
    {
        return _AddObjectToMoveLine(this, dwLineIndex, pObject);
    }

    /*
        Description: Gets the name of the MoveLine for the specified index.
    */
    const char * GetMoveLineName(DWORD dwLineIndex)
    {
        return (const char*)ThisPtrCallNoFixup(this->vtable[10], this, dwLineIndex);
    }
};
ASSERT_STRUCT_SIZE(sUnit, 0x538);