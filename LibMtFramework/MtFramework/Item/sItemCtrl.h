

#pragma once
#include "LibMtFramework.h"
#include "MtFramework/System/cSystem.h"
#include "MtFramework/Item/uItem.h"

// sizeof = 0x1470
struct sItemCtrl : public cSystem
{
    /* 0x38 */ // array, 64 items, 0x40 element size

    inline static sItemCtrl * (__stdcall *_ctor)(sItemCtrl *thisptr) =
        (sItemCtrl*(__stdcall*)(sItemCtrl*))GetModuleAddress(0x1400680E0);

    inline static sItemCtrl * (__stdcall *_dtor)(sItemCtrl *thisptr, bool bFreeMemory) =
        (sItemCtrl*(__stdcall*)(sItemCtrl*, bool))GetModuleAddress(0x14006D9A0);

    inline static uItem * (__stdcall *_SpawnAndPlaceItem)(sItemCtrl *thisptr, DWORD dwItemId, Vector4 *pPosition, Vector4 *pRotation) =
        (uItem*(__stdcall*)(sItemCtrl*, DWORD, Vector4*, Vector4*))GetModuleAddress(0x140075540);

    inline static uItem * (__stdcall *_SpawnAndPlaceItem2)(sItemCtrl *thisptr, DWORD dwItemId, Vector4 *pPosition, Vector4 *pRotation) =
        (uItem*(__stdcall*)(sItemCtrl*, DWORD, Vector4*, Vector4*))GetModuleAddress(0x1400754E0);

    inline static uItem * (__stdcall *_SpawnItem)(sItemCtrl *thisptr, DWORD dwItemId) =
        (uItem*(__stdcall*)(sItemCtrl*, DWORD))GetModuleAddress(0x140075680);

    IMPLEMENT_SINGLETON(sItemCtrl, 0x141CF2620);

    /*
        Description: Spawns the specified item and updates its position and rotation.

        Parameters:
            dwItemId - Id of the item to spawn
            pPosition - World position to spawn the item at
            pRotation - Rotational vector for the item

        Returns: Instance of the spawned item, or nullptr otherwise.
    */
    uItem * SpawnAndPlaceItem(DWORD dwItemId, Vector4 *pPosition, Vector4 *pRotation)
    {
        return _SpawnAndPlaceItem(this, dwItemId, pPosition, pRotation);
    }

    /*
        Description: Spawns the specified item and updates its position and rotation. Also sets some unknown physics flag.

        Parameters:
            dwItemId - Id of the item to spawn
            pPosition - World position to spawn the item at
            pRotation - Rotational vector for the item

        Returns: Instance of the spawned item, or nullptr otherwise.
    */
    uItem * SpawnAndPlaceItem2(DWORD dwItemId, Vector4 *pPosition, Vector4 *pRotation)
    {
        return _SpawnAndPlaceItem2(this, dwItemId, pPosition, pRotation);
    }

    /*
        Description: Spawns the specified item

        Parameters:
            dwItemId - Id of the item to spawn

        Returns: Instance of the spawned item, or nullptr otherwise.
    */
    uItem * SpawnItem(DWORD dwItemId)
    {
        return _SpawnItem(this, dwItemId);
    }
};