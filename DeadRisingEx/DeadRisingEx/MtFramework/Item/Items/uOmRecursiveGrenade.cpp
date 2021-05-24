
#include "uOmRecursiveGrenade.h"
#include <MtFramework/Archive/sResource.h>
#include "DeadRisingEx/MtFramework/Item/uItemImpl.h"
#include "DeadRisingEx/MtFramework/Player/uPlayerImpl.h"

void **g_sMainInstance = GetModuleAddress<void**>(0x141CF2AA0);

float(__stdcall *GetRandomFloat)(void *pThing) = GetModuleAddress<float(__stdcall*)(void*)>(0x14000ADA0);

void(__stdcall *CalculateMatrix)(void *pMatrix, void *pVector) = GetModuleAddress<void(__stdcall*)(void*, void*)>(0x14061B880);

float velocity = 3.0f;
Vector3 UnkVec = { 0.0f, 0.0f, 0.0f };

void uOmRecursiveGrenade::SpawnGrenades()
{
    void *pThing = (BYTE*)*g_sMainInstance + 0x203F4;

    // Pick a random number of grenades to spawn.
    int grenadeCount = (int)(GetRandomFloat(pThing) * 15.0f) % 5;
    //ImGuiConsole::Instance()->ConsolePrint(L"Spawning %d grenades\n", grenadeCount);

    // Loop and spawn grenades.
    for (int i = 0; i < grenadeCount; i++)
    {
        // Get the parent item's position and modify it so the new grenade spawns nearby.
        Vector4 position = { 0 };
        Vector4 rotation = { 0 };
        this->GetItemPosition(&position);

        position.x += GetRandomFloat(pThing) * 350.0f * (round(GetRandomFloat(pThing)) == 1 ? 1.0f : -1.0f);
        position.y += 50.0f;
        position.z += GetRandomFloat(pThing) * 350.0f * (round(GetRandomFloat(pThing)) == 1 ? 1.0f : -1.0f);
        //ImGuiConsole::Instance()->ConsolePrint(L"Grenade #%d: %f %f %f\n", i, position.x, position.y, position.z);

        // Create a new grenade object.
        uOmRecursiveGrenade *pGrenade = uOmRecursiveGrenade::CreateInstance();
        if (pGrenade == nullptr)
        {
            // Failed to spawn the grenage.
            ImGuiConsole::Instance()->ConsolePrint(L"Failed to spawn new grenade!\n");
            return;
        }

        if (pGrenade->SetupItemProperties() == false)
        {
            // Failed to setup properties for the item.
            ImGuiConsole::Instance()->ConsolePrint(L"Failed to setup item properties\n");
            return;
        }

        // Set some flag value on the item.
        *(DWORD*)(((BYTE*)pGrenade) + 0x2F48) |= 0x1000;

        // Set some flag to do physics simulation.
        *(DWORD*)(((BYTE*)pGrenade) + 0x2F4C) |= 2;

        // Set initial velocity.
        //*(float*)(((BYTE*)pGrenade) + 0x1540) = 10.0f;
        *(float*)(((BYTE*)pGrenade) + 0x2F64) = velocity; // 1.0f;

        // Calculate some matrix?
        Vector3 vector = { 0.0f, 0.0f, 0.0f };
        CalculateMatrix(((BYTE*)pGrenade) + 0x2F00, &UnkVec);

        *(Vector4*)(((BYTE*)pGrenade) + 0x2F30) = position;
        *(Matrix4x4*)(((BYTE*)pGrenade) + 0xB0) = *(Matrix4x4*)(((BYTE*)pGrenade) + 0x2F00);

        // Set the recursion counter.
        pGrenade->RecursionCounter = this->RecursionCounter - 1;

        // Call some sUnit function to add the item to a list?
        if (sUnit_AddObject(*g_sUnitInstance, 9, pGrenade) == false)
        {
            ImGuiConsole::Instance()->ConsolePrint(L"sUnit function failed!\n");
            return;
        }

        // Set the grenade's position and spawn it in.
        *(Vector4*)(((BYTE*)pGrenade) + 0x40) = position;

        //// Spawn and place the grenade.
        //uOm23 *pGrenade = (uOm23*)sItemCtrl::Instance()->SpawnAndPlaceItem(29, &position, &rotation);
        //if (pGrenade == nullptr)
        //{
        //    // Failed to spawn the grenage.
        //    ImGuiConsole::Instance()->ConsolePrint(L"Failed to spawn new grenade!\n");
        //    return;
        //}
    }
}

void RecursiveGrenade_OnExplode_Hook(uOmRecursiveGrenade *thiptr, DWORD unk)
{
    // Call the original function.
    uOm23_OnExplode(thiptr, unk);

    // Check if the grenade has been marked as destroyed and if so spawn more grenades.
    if (*(BYTE*)((BYTE*)thiptr + 0x3731) == 1)
    {
        // Check if the recursion level has hit 0, if not spawn more grenades.
        if (thiptr->RecursionCounter > 0)
        {
            // Grenade has blown, spawn more.
            thiptr->SpawnGrenades();
        }
    }
}