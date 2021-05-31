

#include "uOm08Impl.h"
#include <MtFramework/Item/Items/uOm08.h>
#include <MtFramework/Item/Items/uOm23.h>
#include <MtFramework/Item/sItemCtrl.h>
#include <MtFramework/Archive/sResource.h>
#include "DeadRisingEx/MtFramework/Item/uItemImpl.h"
#include "DeadRisingEx/MtFramework/Player/uPlayerImpl.h"
#include "DeadRisingEx/MtFramework/Item/Items/uOmRecursiveGrenade.h"

DWORD uOm08_OnItemDestroyed_Hook(uOm08 *thisptr, DWORD unk);

void uOm08Impl::RegisterTypeInfo()
{
    // Hook the OnItemDestroyed vtable pointer for the zucchini.
    ReplaceVTablePointer(&uOm08::_vtable[74], uOm08_OnItemDestroyed_Hook);
}

DWORD uOm08_OnItemDestroyed_Hook(uOm08 *thisptr, DWORD unk)
{
    // Load the grenade archive to ensure the model has been loaded.
    cResource *pArchive = sResource::Instance()->LoadGameResource<cResource>(rArchive::DebugTypeInfo, "arc\\rom\\om\\om0023\\om0023", RLF_SYNCHRONOUS | RLF_LOAD_AS_ARCHIVE);
    if (pArchive == nullptr)
    {
        // Failed to force load resource.
        ImGuiConsole::Instance()->ConsolePrint(L"Failed to force load grenade archive!\n");
        return 0;
    }

    // Get the parent item's position and modify it so the new grenade spawns nearby.
    Vector4 position = { 0 };
    thisptr->GetItemPosition(&position);

    // Create a new recursive grenade object.
    uOmRecursiveGrenade *pGrenade = uOmRecursiveGrenade::CreateInstance();
    if (pGrenade == nullptr)
    {
        // Failed to spawn the grenage.
        ImGuiConsole::Instance()->ConsolePrint(L"Failed to spawn new grenade!\n");
        return 0;
    }

    if (pGrenade->SetupItemProperties() == false)
    {
        // Failed to setup properties for the item.
        ImGuiConsole::Instance()->ConsolePrint(L"Failed to setup item properties\n");
        return 0;
    }

    // Set some flag value on the item.
    *(DWORD*)(((BYTE*)pGrenade) + 0x2F48) |= 0x1000;

    // Call some sUnit function to add the item to a list?
    if (sUnit_AddObject(*g_sUnitInstance, 9, pGrenade) == false)
    {
        ImGuiConsole::Instance()->ConsolePrint(L"sUnit function failed!\n");
        return 0;
    }

    // Set the grenade's position and spawn it in.
    *(Vector4*)(((BYTE*)pGrenade) + 0x40) = position;

    return 0;
}