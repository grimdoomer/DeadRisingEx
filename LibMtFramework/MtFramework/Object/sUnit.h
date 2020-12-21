
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

        inline static MoveLine * (__stdcall *_ctor)(MoveLine *thisptr) =
            GetModuleAddress<MoveLine*(__stdcall*)(MoveLine*)>(0x14062FF60);

        inline static MoveLine * (__stdcall *_dtor)(MoveLine *thisptr, bool bFreeMemory) =
            GetModuleAddress<MoveLine*(__stdcall*)(MoveLine*, bool)>(0x1400C72B0);

        IMPLEMENT_MYDTI(MoveLine, 0x141CF2628, 0x1400AF010, 0x140630900);

        MoveLine()
        {
            _ctor(this);
        }

        ~MoveLine()
        {
            ThisPtrCallNoFixup<void, bool>(this->vtable[0], this, false);
        }
    };
    static_assert(sizeof(MoveLine) == 0x28, "sUnit::MoveLine incorrect struct size");


    /* 0x38 */ MoveLine     MoveLines[32];

    /*
        VTable:
            0x40
            0x48
            0x50 const char * GetMoveLineName(DWORD dwLineIndex)
            0x58
    */

    inline static sUnit * (__stdcall *_ctor)(sUnit *thisptr) =
        GetModuleAddress<sUnit*(__stdcall*)(sUnit*)>(0x14062FF90);

    inline static sUnit * (__stdcall *_dtor)(sUnit *thisptr, bool bFreeMemory) =
        GetModuleAddress<sUnit*(__stdcall*)(sUnit*, bool)>(0x140630030);

    inline static bool(__stdcall *_AddObjectToMoveLine)(sUnit *thisptr, DWORD dwLineIndex, cUnit *pObject) =
        GetModuleAddress<bool(__stdcall*)(sUnit*, DWORD, cUnit*)>(0x1406300B0);

    inline static const char * (__stdcall *_GetMoveLineName)(sUnit *thisptr, DWORD dwLineIndex) =
        GetModuleAddress<const char*(__stdcall*)(sUnit*, DWORD)>(0x1406305C0);

    IMPLEMENT_MYDTI(sUnit, 0x141CF2658, 0x1400AF010, 0x140630950);

    IMPLEMENT_SINGLETON(sUnit, 0x141CF2620);

    sUnit()
    {
        _ctor(this);
    }

    ~sUnit()
    {
        ThisPtrCallNoFixup<void, bool>(this->vtable[0], this, false);
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
        return ThisPtrCallNoFixup<const char*, DWORD>(this->vtable[10], this, dwLineIndex);
    }
};
static_assert(sizeof(sUnit) == 0x538, "sUnit incorrect struct size");