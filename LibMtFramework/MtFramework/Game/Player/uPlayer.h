
#pragma once
#include "LibMtFramework.h"
#include "MtFramework/Object/Model/uSnatcherModel.h"

// sizeof = 0x2530
struct uPlayer : public uSnatcherModel
{
    /* 0x12E8 */ DWORD mVitalMax;               // Maximum health value
    /* 0x12EC */ DWORD mVitalNew;               // Current health value

    /* 0x237C */ DWORD mInfluenceFlag;

    IMPLEMENT_MYDTI(uPlayer, 0x141949540, 0x1400AF010, 0x140132A90);

    inline static uPlayer* (__stdcall* _ctor)(uPlayer* thisptr) =
        (uPlayer * (__stdcall*)(uPlayer*))GetModuleAddress(0x1401306E0);

    inline static uPlayer* (__stdcall* _dtor)(uPlayer* thisptr, bool bFreeMemory) =
        (uPlayer * (__stdcall*)(uPlayer*, bool))GetModuleAddress(0x140130CC0);

    inline static DWORD(__stdcall* _AdjustHealth)(uPlayer* thisptr, int amount, bool setInfluenceFlag) =
        (DWORD(__stdcall*)(uPlayer*, int, bool))GetModuleAddress(0x1401F4590);

    uPlayer()
    {
        _ctor(this);
    }

    ~uPlayer()
    {
        (void)ThisPtrCallNoFixup(this->vtable[0], this, false);
    }

    /*
        Description: Adjusts the player health (increase or decrease) and flags them as dead if health is depleted

        Parameters:
            amount: positive value to increase health, negative value to decrease health

            setInfluenceFlag: true to set some influence flag on the player (unknown purpose)

        Returns: The new player health value
    */
    DWORD AdjustHealth(int amount, bool setInfluenceFlag)
    {
        return _AdjustHealth(this, amount, setInfluenceFlag);
    }
};
