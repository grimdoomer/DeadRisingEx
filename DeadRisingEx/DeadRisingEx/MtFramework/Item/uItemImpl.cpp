/*

*/

#include "uItemImpl.h"
#include "detours.h"
#include <locale>
#include <codecvt>
#include <string>
#include <map>
#include <functional>
#include <tuple>
#include <MtFramework/Archive/sResource.h>
#include <MtFramework/Area/sAreaHit.h>
#include <MtFramework/Game/sSnatcherMain.h>
#include <MtFramework/Item/sItemCtrl.h>
#include <MtFramework/Item/rItemLayout.h>
#include <MtFramework/Memory/MtHeapAllocator.h>
#include <MtFramework/Object/cUnit.h>
#include "DeadRisingEx/MtFramework/Player/uPlayerImpl.h"

bool(__stdcall *sUnit_Something)(void *thisptr, int unk, uItem *pItem) =
    (bool(__stdcall*)(void*, int, uItem*))GetModuleAddress(0x1406300B0);

void(__stdcall *CopyMtString)(void *thisptr, MtString **ppString) =
    (void(__stdcall*)(void*, MtString**))GetModuleAddress(0x1400CCE80);

bool(__stdcall *sUnit_AddObject)(void *thisptr, DWORD Unk, void *pObject) =
    (bool(__stdcall*)(void*, DWORD, void*))GetModuleAddress(0x1406300B0);

MtDTI *g_uSnatcherModelDTI = (MtDTI*)GetModuleAddress(0x141949C20);

std::hash<std::string> stringHasher;
std::map<size_t, const char*> mObjectArchiveLookupTable;

// Randomizer state:
bool itemRandomizerEnabled = false;

// Map of item ids and a tuple: <bool itemCanBeRandomized, bool itemCanBeSpawned>
// itemCanBeRandomized indicates the item can be replaced via item randomization, and itemCanBeSpawned indicates
// the item can be spawned in place of another item.
std::vector<std::tuple<bool, bool>> itemRandomizerAllowList;

// Forward declarations for command functions.
__int64 SpawnItem(WCHAR **argv, int argc);
__int64 SpawnObject(WCHAR **argv, int argc);
__int64 SetRandomizerState(WCHAR **argv, int argc);

uItem * __stdcall Hook_SpawnAndPlaceItem(sItemCtrl *thisptr, DWORD dwItemId, Vector4 *pPosition, Vector4 *pRotation);
uItem * __stdcall Hook_SpawnAndPlaceItem2(sItemCtrl *thisptr, DWORD dwItemId, Vector4 *pPosition, Vector4 *pRotation);
void __stdcall Hook_sAreaHit_SpawnItems(sAreaHit *thisptr);

uItem * __stdcall Hook_SpawnItem(sItemCtrl *thisptr, DWORD dwItemId);

// Table of commands for uPlayer objects.
const int g_uItemCommandsLength = 3;
const ConsoleCommandInfo g_uItemCommands[g_uItemCommandsLength] =
{
    { L"spawn_item", L"Spawns an item near the player", SpawnItem },
    { L"spawn_object", L"Spawns an object by class name near the player", SpawnObject },
    { L"item_randomizer", L"Randomizes all item spawns when enabled", SetRandomizerState },
};

void uItemImpl::RegisterTypeInfo()
{
    // Register commands:
    ImGuiConsole::Instance()->RegisterCommands(g_uItemCommands, g_uItemCommandsLength);

    // Loop through all of the object names and build out object archive lookup table.
    for (int i = 0; i < OBJECT_COUNT; i++)
    {
        // Add a new entry for the current object name.
        size_t hashcode = stringHasher(ObjectInfoTable[i].ClassName);
        DbgPrint("%s -> %p\n", ObjectInfoTable[i].ClassName, hashcode);
        mObjectArchiveLookupTable.emplace(stringHasher(ObjectInfoTable[i].ClassName), ObjectInfoTable[i].ArchivePath);
    }
}

void uItemImpl::InstallHooks()
{
    // Install hooks:
    //DetourAttach((void**)&sItemCtrl::_SpawnAndPlaceItem, Hook_SpawnAndPlaceItem);
    //DetourAttach((void**)&sItemCtrl::_SpawnAndPlaceItem2, Hook_SpawnAndPlaceItem2);
    DetourAttach((void**)&sAreaHit::_SpawnItems, Hook_sAreaHit_SpawnItems);

    DetourAttach((void**)&sItemCtrl::_SpawnItem, Hook_SpawnItem);

    // Initialize the item randomizer allow list.
    for (int i = 0; i < ITEM_COUNT; i++)
    {
        // Special case items that should not be spawned or randomized:
        if (i == 11)
            // First Aid Kit, used as medicine for Brad
            itemRandomizerAllowList.push_back(std::tuple<bool, bool> { false, false });
        else if (i == 68)
            // Book [Cult Initiation Guide]
            itemRandomizerAllowList.push_back(std::tuple<bool, bool> { false, false });
        else if (i >= 173 && i <= 179)
            // 
            itemRandomizerAllowList.push_back(std::tuple<bool, bool> { false, false });
        else
            itemRandomizerAllowList.push_back(std::tuple<bool, bool> { true, true });
    }

    // Set the item randomizer initial state.
    itemRandomizerEnabled = ModConfig::Instance()->ItemRandomizerEnabled;
}

__int64 SpawnItem(WCHAR **argv, int argc)
{
    char sItemClassName[64];

    // Make sure at least 1 argument was passed.
    if (argc < 1)
    {
        // Invalid syntax.
        ImGuiConsole::Instance()->ConsolePrint(L"Invalid command syntax!\n");
        return 0;
    }

    if (uPlayerInstance == nullptr)
    {
        ImGuiConsole::Instance()->ConsolePrint(L"Player instance not set!\n");
        return 0;
    }

    // Setup the unicode converter.
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> unicConvert;

    // Parse item id and make sure it is valid.
    std::string sItemId = unicConvert.to_bytes(argv[0]);
    DWORD itemId = atoi(sItemId.c_str());
    if (itemId >= ITEM_COUNT)
    {
        // Item id is invalid.
        ImGuiConsole::Instance()->ConsolePrint(L"Item id is invalid, should be [0, 313]\n");
        return 0;
    }

    // Get the item info for this item.
    ItemInfoEntry *pItemInfo = &uItem::ItemInfoTable[itemId];
    if (lstrlenA(pItemInfo->ArchivePath) == 0)
    {
        // Item is invalid.
        ImGuiConsole::Instance()->ConsolePrint(L"Item %d has no archive associated with it!\n", itemId);
        return 0;
    }

    // Make sure the item has valid properties or else we might crash.
    if (uItem::ItemProperties[itemId] == nullptr)
    {
        // Item does not have item properties and may crash us trying to spawn it.
        ImGuiConsole::Instance()->ConsolePrint(L"Item %d does not have an item properties structure!\n", itemId);
        return 0;
    }

    // Find the index of the last slash so we can get the Om object name.
    std::string sItemPath = pItemInfo->ArchivePath;
    int index = sItemPath.find_last_of('\\');

    // Get the om index from the string.
    std::string sItemName = sItemPath.substr(index + 3);
    DWORD itemFileId = strtoul(sItemName.c_str(), nullptr, 16);

    // Load the item archive so we can spawn objects that are not part of this area.
    cResource *pResource = sResource::Instance()->LoadGameResource<cResource>(rArchive::DebugTypeInfo, sItemPath.c_str(), RLF_SYNCHRONOUS | RLF_LOAD_AS_ARCHIVE);
    if (pResource == nullptr)
    {
        // Failed to force load resource.
        ImGuiConsole::Instance()->ConsolePrint(L"Failed to force load object '%S'!\n", sItemPath.c_str());
        return 0;
    }

    // Certain items have two DTI instances, one for static objects and one for item objects. Check the item id to see if
    // we need to use the item DTI instance instead of the static object DTI.
    switch (itemId)
    {
    case 69:        // Queen bee
    case 135:       // Sniper rifle
    {
        // Format the item class name using the item id.
        snprintf(sItemClassName, sizeof(sItemClassName), "uOm%02x_1", itemFileId);
        break;
    }
    default:
    {
        // Format the item class name using the item id.
        snprintf(sItemClassName, sizeof(sItemClassName), "uOm%02x", itemFileId);
        break;
    }
    }

    // If an item class name was provided then use that instead of the one we calculated.
    if (argc >= 2)
        snprintf(sItemClassName, sizeof(sItemClassName), "%S", argv[1]);

    // Try to find the type info for the object name.
    MtDTI *pItemDTI = MtDTI::FindDTIByName(sItemClassName, MtDTI::DefaultMtDTIParentObject);
    if (pItemDTI == nullptr)
    {
        // Failed to find type info for the item class name.
        ImGuiConsole::Instance()->ConsolePrint(L"Failed to find type info for object '%S'\n", sItemClassName);
        return 0;
    }

    // Create a new instance of the item.
    uItem *pItem = pItemDTI->CreateInstance<uItem>();
    if (pItem == nullptr)
    {
        // Failed to create item instance.
        ImGuiConsole::Instance()->ConsolePrint(L"Failed to create item instance for item '%S'\n", sItemClassName);
        return 0;
    }

    // Initialize item properties.
    if (pItem->SetupItemProperties() == false)
    {
        // Failed to setup properties for the item.
        ImGuiConsole::Instance()->ConsolePrint(L"Failed to setup item properties\n");
        return 0;
    }

    // Set some flag value on the item.
    *(DWORD*)(((BYTE*)pItem) + 0x2F48) |= 0x1000;

    // Call some sUnit function to add the item to a list?
    if (sUnit_Something(*g_sUnitInstance, 9, pItem) == false)
    {
        ImGuiConsole::Instance()->ConsolePrint(L"sUnit function failed!\n");
        return 0;
    }

    // Get the uPlayer instance from sUnit.
    BYTE *pPlayerInst = (BYTE*)uPlayerInstance;

    // Get the player's position and rotation.
    Vector4 position = *(Vector4*)(pPlayerInst + 0x40);
    position.y += 50.0f;

    // Set the item position.
    *(Vector4*)(((BYTE*)pItem) + 0x40) = position;

    return 0;
}

__int64 SpawnObject(WCHAR **argv, int argc)
{
    char sItemClassName[64];
    cResource *pArchive = nullptr;

    // Make sure at least 1 argument was passed.
    if (argc < 1)
    {
        // Invalid syntax.
        ImGuiConsole::Instance()->ConsolePrint(L"Invalid command syntax!\n");
        return 0;
    }

    if (uPlayerInstance == nullptr)
    {
        ImGuiConsole::Instance()->ConsolePrint(L"Player instance not set!\n");
        return 0;
    }

    // Setup the unicode converter and convert the object class name to ascii.
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> unicConvert;
    std::string sObjectClassName = unicConvert.to_bytes(argv[0]);

    // Check if the object archive path was specified.
    if (argc > 1)
    {
        // Get the archive path.
        std::string sArchivePath = unicConvert.to_bytes(argv[1]);

        // Load the object archive so we can load objects not part of this area.
        pArchive = sResource::Instance()->LoadGameResource<cResource>(rArchive::DebugTypeInfo, sArchivePath.c_str(), RLF_SYNCHRONOUS | RLF_LOAD_AS_ARCHIVE);
        if (pArchive == nullptr)
        {
            // Failed to force load resource.
            ImGuiConsole::Instance()->ConsolePrint(L"Failed to force load object '%S'!\n", sArchivePath.c_str());
            return 0;
        }
    }
    else
    {
        // Check if we have an entry for this object in the object archive lookup table.
        size_t hashcode = stringHasher(sObjectClassName);
        ImGuiConsole::Instance()->ConsolePrint(L"hashcode = %p\n", hashcode);
        if (mObjectArchiveLookupTable.count(hashcode) > 0 && mObjectArchiveLookupTable[hashcode] != nullptr)
        {
            // Load the object archive so we can load objects not part of this area.
            pArchive = sResource::Instance()->LoadGameResource<cResource>(rArchive::DebugTypeInfo, mObjectArchiveLookupTable[hashcode], RLF_SYNCHRONOUS | RLF_LOAD_AS_ARCHIVE);
            if (pArchive == nullptr)
            {
                // Failed to force load resource.
                ImGuiConsole::Instance()->ConsolePrint(L"Failed to force load object '%S'!\n", mObjectArchiveLookupTable[hashcode]);
                return 0;
            }
        }
        else
        {
            // No archive path for this object.
            ImGuiConsole::Instance()->ConsolePrint(L"No known archive path for '%S'\n", sObjectClassName.c_str());
            return 0;
        }
    }

    // Get the class name and try to find DTI info for it.
    MtDTI *pObjectDTI = MtDTI::FindDTIByName(sObjectClassName.c_str(), g_uSnatcherModelDTI);
    if (pObjectDTI == nullptr)
    {
        // No DTI info found for the object type.
        ImGuiConsole::Instance()->ConsolePrint(L"No DTI info found for '%s'\n", argv[0]);
        return 0;
    }

    // Create an instance of the object.
    cUnit *pObject = pObjectDTI->CreateInstance<cUnit>();
    if (pObject == nullptr)
    {
        // Failed to create the object.
        ImGuiConsole::Instance()->ConsolePrint(L"Failed to create object type '%s'\n", argv[0]);
        return 0;
    }

    // Set a null resource path.
    //MtString *pString = nullptr;
    //CopyMtString(pObject, &pString);

    // Get the uPlayer instance from sUnit.
    BYTE *pPlayerInst = (BYTE*)uPlayerInstance;

    // Get the player's position and rotation.
    Vector4 position = *(Vector4*)(pPlayerInst + 0x40);
    position.y += 50.0f;
    Vector4 rotation = { 0 };

    // Set the item position.
    *(Vector4*)(((BYTE*)pObject) + 0x40) = position;

    // Determine what move line to add the object to.
    DWORD moveLine = 9;
    if (sObjectClassName.find("uNpc") == 0)
        moveLine = 4;

    // Spawn the object.
    if (sUnit_AddObject(*g_sUnitInstance, moveLine, pObject) == false)
    {
        // Failed to add object to sUnit.
        ImGuiConsole::Instance()->ConsolePrint(L"sUnit_AddObject failed\n");
        return 0;
    }

    // TODO: destroy pArchive if it was loaded.

    return 0;
}

__int64 SetRandomizerState(WCHAR **argv, int argc)
{
    // Make sure at least 1 argument was passed.
    if (argc < 1)
    {
        // Invalid syntax.
        ImGuiConsole::Instance()->ConsolePrint(L"Invalid command syntax!\n");
        return 0;
    }

    // Determine the state from the first parameter.
    if (argv[0][0] == '0')
        itemRandomizerEnabled = false;
    else if (argv[0][0] == '1')
        itemRandomizerEnabled = true;
    else
    {
        // Invalid randomizer state.
        ImGuiConsole::Instance()->ConsolePrint(L"Invalid value provided, should be 0 to disable or 1 to enable!\n");
    }

    return 0;
}

DWORD GetRandomItemId()
{
    // Get the sSnatcherMain instance so we can access rng.
    sSnatcherMain *pSnatcherMain = sSnatcherMain::Instance();
    sSnatcherMain::RNGState *pRng = (sSnatcherMain::RNGState*)((BYTE*)pSnatcherMain + 0x203F4);

    // Loop until we find a valid item to spawn.
    DWORD itemId = 0;
    do
    {
        // Generate new item id.
        itemId = sSnatcherMain::_GetRandomInt(pRng) % ITEM_COUNT;

        // Check if the selected item is restricted.
        if (std::get<1>(itemRandomizerAllowList[itemId]) == false)
            continue;

    } while (uItem::ItemInfoTable[itemId].ArchivePath == nullptr || lstrlen(uItem::ItemInfoTable[itemId].ArchivePath) == 0 ||
        uItem::ItemInfoTable[itemId].Name == nullptr || uItem::ItemInfoTable[itemId].Name[0] == '-' || uItem::ItemProperties[itemId] == nullptr);

    // Return the item id.
    return itemId;
}

uItem * __stdcall Hook_SpawnAndPlaceItem(sItemCtrl *thisptr, DWORD dwItemId, Vector4 *pPosition, Vector4 *pRotation)
{
    // Check if the item randomize mod is enabled and if not bail out.
    if (itemRandomizerEnabled == false)
        return sItemCtrl::_SpawnAndPlaceItem(thisptr, dwItemId, pPosition, pRotation);

    // Choose a random item to spawn.
    dwItemId = GetRandomItemId();

    // Load the item archive so we can spawn objects that are not part of this area.
    cResource *pResource = sResource::Instance()->LoadGameResource<cResource>(rArchive::DebugTypeInfo, uItem::ItemInfoTable[dwItemId].ArchivePath, RLF_SYNCHRONOUS | RLF_LOAD_AS_ARCHIVE);
    if (pResource == nullptr)
    {
        // Failed to force load resource.
        ImGuiConsole::Instance()->ConsolePrint(L"Failed to force load object '%S'!\n", uItem::ItemInfoTable[dwItemId].ArchivePath);
        return 0;
    }

    // Call the trampoline with the new item id.
    return sItemCtrl::_SpawnAndPlaceItem(thisptr, dwItemId, pPosition, pRotation);
}

uItem * __stdcall Hook_SpawnAndPlaceItem2(sItemCtrl *thisptr, DWORD dwItemId, Vector4 *pPosition, Vector4 *pRotation)
{
    // Check if the item randomize mod is enabled and if not bail out.
    if (itemRandomizerEnabled == false)
        return sItemCtrl::_SpawnAndPlaceItem2(thisptr, dwItemId, pPosition, pRotation);

    // Choose a random item to spawn.
    dwItemId = GetRandomItemId();

    // Load the item archive so we can spawn objects that are not part of this area.
    cResource *pResource = sResource::Instance()->LoadGameResource<cResource>(rArchive::DebugTypeInfo, uItem::ItemInfoTable[dwItemId].ArchivePath, RLF_SYNCHRONOUS | RLF_LOAD_AS_ARCHIVE);
    if (pResource == nullptr)
    {
        // Failed to force load resource.
        ImGuiConsole::Instance()->ConsolePrint(L"Failed to force load object '%S'!\n", uItem::ItemInfoTable[dwItemId].ArchivePath);
        return 0;
    }

    // Call the trampoline with the new item id.
    return sItemCtrl::_SpawnAndPlaceItem2(thisptr, dwItemId, pPosition, pRotation);
}

void __stdcall Hook_sAreaHit_SpawnItems(sAreaHit *thisptr)
{
    char sItemClassName[64];

    // Check if the item randomizer mod is enabled and if not bail out.
    if (itemRandomizerEnabled == false)
    {
        sAreaHit::_SpawnItems(thisptr);
        return;
    }

    // Hacky pointer math:
    BYTE *pbAreaHit = (BYTE*)thisptr;
    DWORD *pResourceCount = (DWORD*)(pbAreaHit + 0x38);
    sAreaHitResourceEntry *pResources = (sAreaHitResourceEntry*)(pbAreaHit + 0xF8);

    // If no resources are loaded for this area bail out.
    if (*pResourceCount == 0)
        return;

    // Loop through all the resources and spawn item resources.
    for (int i = 0; i < *pResourceCount; i++)
    {
        // Check if this resource is an item.
        if (pResources[i].Type != eAreaHitEntryType::TYPE_ITEM)
            continue;

        // Make sure the file has been loaded and parsed.
        if (pResources[i].pResource == nullptr)
            continue;

        // Get the item layout from the resource pointer.
        rItemLayout *pItemLayout = (rItemLayout*)pResources[i].pResource;

        // Loop and randomize all the item spawns.
        for (int x = 0; x < pItemLayout->LayoutCount; x++)
        {
            // Get the current item layout entry.
            rItemLayout::LayoutInfo *pLayoutInfo = (rItemLayout::LayoutInfo*)((BYTE*)pItemLayout->pLayoutInfoList + (x * 0x3F0));
            DWORD *pItemId = (DWORD*)((BYTE*)pLayoutInfo + 0xB0);
            DWORD *pCheck = (DWORD*)((BYTE*)pLayoutInfo + 0x80);
            MtString **ppClassName = (MtString**)((BYTE*)pLayoutInfo + 0xD8);

            if ((*pCheck & 1) != 0)
                continue;

            // Ignore special case items.
            if (!(*pItemId - 65534 <= 1) && (*pCheck & 0x400) == 0)
            {
                // Check if the item is allowed to be randomized.
                if (*pItemId >= 0 && *pItemId < ITEM_COUNT && std::get<0>(itemRandomizerAllowList[*pItemId]) == false)
                    continue;

                // Randomize the item spawned.
                DWORD oldItemId = *pItemId;
                *pItemId = GetRandomItemId();
                DbgPrint("Randomizing item %d -> %d\n", oldItemId, *pItemId);

                // Find the index of the last slash so we can get the Om object name.
                std::string sItemPath = uItem::ItemInfoTable[*pItemId].ArchivePath;
                int index = sItemPath.find_last_of('\\');

                // Get the om index from the string.
                std::string sItemName = sItemPath.substr(index + 3);
                DWORD itemFileId = strtoul(sItemName.c_str(), nullptr, 16);

                // Load the item archive so we can spawn objects that are not part of this area.
                cResource *pResource = sResource::Instance()->LoadGameResource<cResource>(rArchive::DebugTypeInfo, sItemPath.c_str(), RLF_SYNCHRONOUS | RLF_LOAD_AS_ARCHIVE);
                if (pResource == nullptr)
                {
                    // Failed to force load resource.
                    ImGuiConsole::Instance()->ConsolePrint(L"Failed to force load object '%S'!\n", sItemPath.c_str());
                    continue;
                }

                // Format the item class name using the item id.
                snprintf(sItemClassName, sizeof(sItemClassName), "uOm%02x", itemFileId);

                // Allocate memory for a class name string.
                DWORD stringLength = lstrlen(sItemClassName);
                MtString *pClassName = (*g_pStringHeapAllocator)->Alloc<MtString>(sizeof(MtString) + stringLength + 1, 16);
                if (pClassName != nullptr)
                {
                    // Initialize the string.
                    pClassName->RefCount = 1;
                    pClassName->Length = stringLength;
                    strcpy(pClassName->String, sItemClassName);

                    // Free the old string pointer.
                    if (*ppClassName != nullptr)
                        (*g_pStringHeapAllocator)->Free(*ppClassName);

                    // Re-assign the class name pointer.
                    *ppClassName = pClassName;
                }
            }
        }

        // Spawn the randomized items.
        pItemLayout->SpawnItems();
    }
}

uItem * __stdcall Hook_SpawnItem(sItemCtrl *thisptr, DWORD dwItemId)
{
    DbgPrint("Spawning item %d\n", dwItemId);
    return sItemCtrl::_SpawnItem(thisptr, dwItemId);
}