/*

*/

#include "uItemImpl.h"
#include "detours.h"
#include <locale>
#include <codecvt>
#include <string>
#include <map>
#include <functional>
#include <MtFramework/Archive/sResource.h>
#include <MtFramework/Item/uItem.h>
#include <MtFramework/Object/cUnit.h>
#include "DeadRisingEx/MtFramework/Player/uPlayerImpl.h"

bool(__stdcall *sUnit_Something)(void *thisptr, int unk, uItem *pItem) =
    GetModuleAddress<bool(__stdcall*)(void*, int, uItem*)>(0x1406300B0);

void(__stdcall *CopyMtString)(void *thisptr, MtString **ppString) =
    GetModuleAddress<void(__stdcall*)(void*, MtString**)>(0x1400CCE80);

void(__stdcall *uCoord_SetRotation)(void *thisptr, Vector4 *pRotation) =
    GetModuleAddress<void(__stdcall*)(void*, Vector4*)>(0x14063EB90);

bool(__stdcall *sUnit_AddObject)(void *thisptr, DWORD Unk, void *pObject) =
    GetModuleAddress<bool(__stdcall*)(void*, DWORD, void*)>(0x1406300B0);

MtDTI *g_uSnatcherModelDTI = GetModuleAddress<MtDTI*>(0x141949C20);

std::hash<std::string> stringHasher;
std::map<size_t, const char*> mObjectArchiveLookupTable;

// Forward declarations for command functions.
__int64 SpawnItem(WCHAR **argv, int argc);
__int64 SpawnObject(WCHAR **argv, int argc);

// Table of commands for uPlayer objects.
const int g_uItemCommandsLength = 2;
const CommandEntry g_uItemCommands[g_uItemCommandsLength] =
{
    { L"spawn_item", L"Spawns an item near the player", SpawnItem },
    { L"spawn_object", L"Spawns an object by class name near the player", SpawnObject },
};

void uItemImpl::RegisterTypeInfo()
{
    // Register commands:
    RegisterCommands(g_uItemCommands, g_uItemCommandsLength);

    // Loop through all of the object names and build out object archive lookup table.
    for (int i = 0; i < OBJECT_COUNT; i++)
    {
        // Add a new entry for the current object name.
        size_t hashcode = stringHasher(ObjectInfoTable[i].ClassName);
        DbgPrint("%s -> %p\n", ObjectInfoTable[i].ClassName, hashcode);
        mObjectArchiveLookupTable.emplace(stringHasher(ObjectInfoTable[i].ClassName), ObjectInfoTable[i].ArchivePath);
    }
}

__int64 SpawnItem(WCHAR **argv, int argc)
{
    char sItemClassName[64];

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

    // Parse item id and make sure it is valid.
    std::string sItemId = unicConvert.to_bytes(argv[0]);
    DWORD itemId = atoi(sItemId.c_str());
    if (itemId >= ITEM_COUNT)
    {
        // Item id is invalid.
        wprintf(L"Item id is invalid, should be [0, 313]\n");
        return 0;
    }

    // Get the item info for this item.
    ItemInfoEntry *pItemInfo = &uItem::ItemInfoTable[itemId];
    if (lstrlenA(pItemInfo->ArchivePath) == 0)
    {
        // Item is invalid.
        wprintf(L"Item %d has no archive associated with it!\n", itemId);
        return 0;
    }

    // Make sure the item has valid properties or else we might crash.
    if (uItem::ItemProperties[itemId] == nullptr)
    {
        // Item does not have item properties and may crash us trying to spawn it.
        wprintf(L"Item %d does not have an item properties structure!\n", itemId);
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
        wprintf(L"Failed to force load object '%S'!\n", sItemPath.c_str());
        return 0;
    }
    
    // Format the item class name using the item id.
    snprintf(sItemClassName, sizeof(sItemClassName), "uOm%02x", itemFileId);

    // If an item class name was provided then use that instead of the one we calculated.
    if (argc >= 2)
        snprintf(sItemClassName, sizeof(sItemClassName), "%S", argv[1]);

    // Try to find the type info for the object name.
    MtDTI *pItemDTI = MtDTI::FindDTIByName(sItemClassName, MtDTI::DefaultMtDTIParentObject);
    if (pItemDTI == nullptr)
    {
        // Failed to find type info for the item class name.
        wprintf(L"Failed to find type info for object '%S'\n", sItemClassName);
        return 0;
    }

    // Create a new instance of the item.
    uItem *pItem = pItemDTI->CreateInstance<uItem>();
    if (pItem == nullptr)
    {
        // Failed to create item instance.
        wprintf(L"Failed to create item instance for item '%S'\n", sItemClassName);
        return 0;
    }

    // Initialize item properties.
    if (pItem->SetupItemProperties() == false)
    {
        // Failed to setup properties for the item.
        wprintf(L"Failed to setup item properties\n");
        return 0;
    }

    // Set some flag value on the item.
    *(DWORD*)(((BYTE*)pItem) + 0x2F48) |= 0x1000;

    // Call some sUnit function to add the item to a list?
    if (sUnit_Something(*g_sUnitInstance, 9, pItem) == false)
    {
        wprintf(L"sUnit function failed!\n");
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
        wprintf(L"Invalid command syntax!\n");
        return 0;
    }

    if (uPlayerInstance == nullptr)
    {
        wprintf(L"Player instance not set!\n");
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
            wprintf(L"Failed to force load object '%S'!\n", sArchivePath.c_str());
            return 0;
        }
    }
    else
    {
        // Check if we have an entry for this object in the object archive lookup table.
        size_t hashcode = stringHasher(sObjectClassName);
        wprintf(L"hashcode = %p\n", hashcode);
        if (mObjectArchiveLookupTable.count(hashcode) > 0 && mObjectArchiveLookupTable[hashcode] != nullptr)
        {
            // Load the object archive so we can load objects not part of this area.
            pArchive = sResource::Instance()->LoadGameResource<cResource>(rArchive::DebugTypeInfo, mObjectArchiveLookupTable[hashcode], RLF_SYNCHRONOUS | RLF_LOAD_AS_ARCHIVE);
            if (pArchive == nullptr)
            {
                // Failed to force load resource.
                wprintf(L"Failed to force load object '%S'!\n", mObjectArchiveLookupTable[hashcode]);
                return 0;
            }
        }
        else
        {
            // No archive path for this object.
            wprintf(L"No known archive path for '%S'\n", sObjectClassName.c_str());
            return 0;
        }
    }

    // Get the class name and try to find DTI info for it.
    MtDTI *pObjectDTI = MtDTI::FindDTIByName(sObjectClassName.c_str(), g_uSnatcherModelDTI);
    if (pObjectDTI == nullptr)
    {
        // No DTI info found for the object type.
        wprintf(L"No DTI info found for '%s'\n", argv[0]);
        return 0;
    }

    // Create an instance of the object.
    cUnit *pObject = pObjectDTI->CreateInstance<cUnit>();
    if (pObject == nullptr)
    {
        // Failed to create the object.
        wprintf(L"Failed to create object type '%s'\n", argv[0]);
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
        wprintf(L"sUnit_AddObject failed\n");
        return 0;
    }

    // TODO: destroy pArchive if it was loaded.

    return 0;
}