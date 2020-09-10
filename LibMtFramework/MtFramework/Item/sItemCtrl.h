

#pragma once
#include "LibMtFramework.h"
#include "MtFramework/Game/cSystem.h"
#include "MtFramework/Item/uItem.h"

// sizeof = 0x1470
struct sItemCtrl : public cSystem
{
    /* 0x38 */ // array, 64 items, 0x40 element size

    inline static sItemCtrl * (__stdcall *_ctor)(sItemCtrl *thisptr) =
        GetModuleAddress<sItemCtrl*(__stdcall*)(sItemCtrl*)>(0x1400680E0);

    inline static sItemCtrl * (__stdcall *_dtor)(sItemCtrl *thisptr, bool bFreeMemory) =
        GetModuleAddress<sItemCtrl*(__stdcall*)(sItemCtrl*, bool)>(0x14006D9A0);

    inline static uItem * (__stdcall *_SpawnAndPlaceItem)(sItemCtrl *thisptr, DWORD dwItemId, Vector4 *pPosition, Vector4 *pRotation) =
        GetModuleAddress<uItem*(__stdcall*)(sItemCtrl*, DWORD, Vector4*, Vector4*)>(0x140075540);

    inline static uItem * (__stdcall *_SpawnItem)(sItemCtrl *thisptr, DWORD dwItemId) =
        GetModuleAddress<uItem*(__stdcall*)(sItemCtrl*, DWORD)>(0x140075680);

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