
#pragma once
#include "DeadRisingEx.h"
#include <MtFramework/Item/uItem.h>
#include <MtFramework/Item/Items/uOm23.h>
#include <MtFramework/Memory/MtHeapAllocator.h>

inline static DWORD(__stdcall *uOm23_OnExplode)(uOm23 *thisptr, DWORD unk) =
    GetModuleAddress<DWORD(__stdcall*)(uOm23*, DWORD)>(0x14006E2C0);

struct uOmRecursiveGrenade;

void RecursiveGrenade_OnExplode_Hook(uOmRecursiveGrenade *thiptr, DWORD unk);

static void *uOmRecursiveGrenade_vtable[84] = { 0 };

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
            ImGuiConsole::Instance()->ConsolePrint(L"Failed to allocate memory for uOmRecursiveGrenade!\n");
            return nullptr;
        }

        // Initialize by calling the ctor for the real grenade.
        uOm23::_ctor((uOm23*)pObject);

        // Check if we need to initialize the recursive grenade vtable.
        if (uOmRecursiveGrenade_vtable[0] == nullptr)
        {
            // Copy the vtable from uOm23.
            memcpy(uOmRecursiveGrenade_vtable, pObject->vtable, sizeof(uOmRecursiveGrenade_vtable));

            // Update the vtable pointer for the explode function
            uOmRecursiveGrenade_vtable[61] = RecursiveGrenade_OnExplode_Hook;
        }

        // Update the vtable pointer to our own.
        pObject->vtable = uOmRecursiveGrenade_vtable;

        // Set the recursion counter to 4.
        pObject->RecursionCounter = 4;

        return pObject;
    }
};