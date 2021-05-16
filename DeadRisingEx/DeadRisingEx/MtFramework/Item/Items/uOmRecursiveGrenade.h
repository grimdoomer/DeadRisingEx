
#pragma once
#include "DeadRisingEx.h"
#include <MtFramework/Item/uItem.h>
#include <MtFramework/Item/Items/uOm23.h>
#include <MtFramework/Memory/MtHeapAllocator.h>

inline static DWORD(__stdcall *uOm23_OnExplode)(uOm23 *thisptr, DWORD unk) =
    GetModuleAddress<DWORD(__stdcall*)(uOm23*, DWORD)>(0x14006E2C0);

struct uOmRecursiveGrenade;

void RecursiveGrenade_OnExplode_Hook(uOmRecursiveGrenade *thiptr, DWORD unk);

// sizeof = 0x3744
struct uOmRecursiveGrenade : public uOm23
{
    BYTE _padding[0x3740 - sizeof(uOm23)];

    DWORD   RecursionCounter;       // Number of recursion levels remaining.

    /*class MyDTI : public MtDTI
    {
        static
    };*/

    void SpawnGrenades();

    static uOmRecursiveGrenade * CreateInstance()
    {
        // Allocate memory for the object instance.
        uOmRecursiveGrenade *pObject = (*g_pUnitHeapAllocator)->Alloc<uOmRecursiveGrenade>(sizeof(uOmRecursiveGrenade), 0x20);
        if (pObject == nullptr)
        {
            // Failed to allocate memory for object.
            wprintf(L"Failed to allocate memory for uOmRecursiveGrenade!\n");
            return nullptr;
        }

        // Initialize by calling the ctor for the real grenade.
        uOm23::_ctor((uOm23*)pObject);

        // Update the vtable pointer for the explode function
        ReplaceVTablePointer(&pObject->vtable[61], RecursiveGrenade_OnExplode_Hook);

        // Set the recursion counter to 4.
        pObject->RecursionCounter = 4;

        return pObject;
    }
};