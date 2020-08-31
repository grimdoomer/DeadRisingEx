/*

*/

#include "uPlayerImpl.h"
#include "detours.h"
#include <locale>
#include <codecvt>
#include <MtFramework/Archive/sResource.h>

void *g_sUnitInstance = GetModuleAddress<void*>(0x141CF2620);
void *g_sItemCtrlInstance = GetModuleAddress<void*>(0x1419462A0);

void *uPlayerInstance = nullptr;

MtDTI *g_uItemDTI = GetModuleAddress<MtDTI*>(0x14194C470);
MtDTI *g_rArchiveDTI = GetModuleAddress<MtDTI*>(0x141CF30F8);

MtObject * (__stdcall *sItemCtrl_SpawnItem)(void *thisptr, DWORD dwItemId, Vector4 *pPosition, Vector4 *pRotation) =
    GetModuleAddress<MtObject*(__stdcall*)(void*, DWORD, Vector4*, Vector4*)>(0x140075540);

void * (__stdcall *uPlayer_ctor)(void *thisptr) = GetModuleAddress<void*(__stdcall*)(void*)>(0x1401306E0);

void * __stdcall Hook_uPlayer_ctor(void *thisptr);

// Forward declarations for command functions.
__int64 SpawnItem(WCHAR **argv, int argc);

// Table of commands for uPlayer objects.
const int g_uPlayerCommandsLength = 1;
const CommandEntry g_uPlayerCommands[g_uPlayerCommandsLength] =
{
    { L"spawn_item", L"Spawns an item near the player", SpawnItem }
};

void uPlayerImpl::RegisterTypeInfo()
{
    // Register commands:
    RegisterCommands(g_uPlayerCommands, g_uPlayerCommandsLength);

    DetourAttach((void**)&uPlayer_ctor, Hook_uPlayer_ctor);
}

void * __stdcall Hook_uPlayer_ctor(void *thisptr)
{
    uPlayerInstance = thisptr;
    return uPlayer_ctor(thisptr);
}

__int64 SpawnItem(WCHAR **argv, int argc)
{
    CHAR sItemPath[100] = { 0 };
    CHAR sItemName[16] = { 0 };

    // Make sure at least 1 argument was passed.
    if (argc < 1)
    {
        // Invalid syntax.
        wprintf(L"Invalid command syntax!\n");
        return 0;
    }

    if (uPlayerInstance == nullptr)
    {
        wprintf(L"Player instance not set!\n");
        return 0;
    }

    // Setup the unicode converter.
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> unicConvert;

    // Parse item id.
    std::string sItemId = unicConvert.to_bytes(argv[0]);
    DWORD itemId = atoi(sItemId.c_str());

    // Get the uPlayer instance from sUnit.
    BYTE *pPlayerInst = (BYTE*)uPlayerInstance; //*(BYTE**)((BYTE*)g_sUnitInstance + 0x50 + 3 * 40);

    // Get the player's position and rotation.
    Vector4 position = *(Vector4*)(pPlayerInst + 0x40);
    position.y += 50.0f;
    Vector4 rotation = { 0 };

    // Spawn the item.
    MtObject *pObject = sItemCtrl_SpawnItem(g_sItemCtrlInstance, itemId, &position, &rotation);
    if (pObject == nullptr)
    {
        // Failed to spawn item.
        wprintf(L"Failed to spawn item %d!\n", itemId);
        return 0;
    }

    // Get the DTI info for this object.
    MtDTI *pObjectDTI = pObject->GetDTI();
    if (pObjectDTI == nullptr)
    {
        // Failed to find DTI info for object name.
        wprintf(L"Failed to find DTI info for object '%S'!\n", sItemName);
        return 0;
    }

    // Guess the item name based on the object name.
    DWORD itemFileId = strtoul(&pObjectDTI->pObjectName[3], nullptr, 16);

    // Using the item name format the file path for the item archive.
    snprintf(sItemPath, sizeof(sItemPath), "arc\\rom\\om\\om%04x\\om%04x", itemFileId, itemFileId);

    // Load the resource which will trigger the archive to load.
    cResource *pResource = sResource::Instance()->LoadGameResource<cResource>(g_rArchiveDTI, sItemPath, RLF_SYNCHRONOUS | RLF_LOAD_AS_ARCHIVE);
    if (pResource == nullptr)
    {
        // Failed to force load resource.
        wprintf(L"Failed to force load object '%S'!\n", sItemPath);
    }
}