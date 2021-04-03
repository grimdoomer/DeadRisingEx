/*

*/

#include "sResourceImpl.h"
#include "cResourceImpl.h"
#include <stdio.h>
#include <string>
#include <locale>
#include <codecvt>
#include <MtFramework/MtObject.h>
#include <MtFramework/Archive/sResource.h>

// Forward declarations for command functions.
__int64 PrintLoadedResources(WCHAR **argv, int argc);
__int64 GetResourceByIndex(WCHAR **argv, int argc);
//__int64 CalculateResourceIdTest(WCHAR **argv, int argc);
__int64 LoadArchive(WCHAR **argv, int argc);

// Table of commands for sResource objects.
const int g_sResourceCommandsLength = 3;
const CommandEntry g_sResourceCommands[g_sResourceCommandsLength] =
{
    { L"list_resources", L"Lists all loaded resources", PrintLoadedResources },
    { L"GetResourceByIndex", L"Gets the resource at the specified index", GetResourceByIndex },
    //{ L"CalculateResourceId", L"", CalculateResourceIdTest }
    { L"load_archive", L"Loads the specified archive by file path", LoadArchive }
};

void sResourceImpl::InitializeTypeInfo()
{
    // Register types:
    RegisterTypeInfo(&cResourceTypeInfo);

    // Register commands:
    RegisterCommands(g_sResourceCommands, g_sResourceCommandsLength);
}

__int64 PrintLoadedResources(WCHAR **argv, int argc)
{
    std::string sFilter;
    bool bFilterFront = false;

    // Setup the unicode converter.
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> unicConvert;

    // Check if there is a filter argument.
    if (argc > 0)
    {
        // Check if this is a front filter or rear filter.
        if (argv[0][0] == '*')
        {
            // Rear filter.
            sFilter = unicConvert.to_bytes(std::wstring(&argv[0][1]));
        }
        else
        {
            // Front filter.
            argv[0][lstrlenW(argv[0]) - 1] = (wchar_t)0;
            sFilter = unicConvert.to_bytes(std::wstring(argv[0]));
            bFilterFront = true;
        }
    }

    // Acquire the list lock.
    sResource *p_sResource = sResource::Instance();
    EnterCriticalSection(&p_sResource->ListLock);

    // Loop through the hash table and print each entry.
    for (int i = 0; i < 8192; i++)
    {
        // Check if this entry is allocated.
        if (p_sResource->pResourceEntries[i] == nullptr)
            continue;

        // Check if there is a filter and handle accordingly.
        if (sFilter.size() > 0)
        {
            // Check if this is a front filter or rear filter.
            std::string sFileName(p_sResource->pResourceEntries[i]->mPath);
            if (bFilterFront == true && sFileName.find(sFilter, 0) == 0)
            {
                // Print the name of the object.
                wprintf(L"Resource %d: %S\n", i, p_sResource->pResourceEntries[i]->mPath);
            }
            else if (bFilterFront == false && sFileName.rfind(sFilter) == sFileName.size() - sFilter.size())
            {
                // Print the name of the object.
                wprintf(L"Resource %d: %S\n", i, p_sResource->pResourceEntries[i]->mPath);
            }
        }
        else
        {
            // Print the name of the object.
            wprintf(L"Resource %d: %S\n", i, p_sResource->pResourceEntries[i]->mPath);
        }
    }

    // Release the list lock.
    LeaveCriticalSection(&p_sResource->ListLock);

    return 0;
}

__int64 GetResourceByIndex(WCHAR **argv, int argc)
{
    cResource *pResource = nullptr;

    // Make sure there is at least 1 argument to parse.
    if (argc != 1)
    {
        // Invalid command syntax.
        wprintf(L"Invalid command syntax\n");
        return 0;
    }

    // Parse the index argument and make sure it is valid.
    int index = std::stoi(std::wstring(argv[0]));
    if (index < 0 || index >= 8192)
    {
        // Index is out of range.
        wprintf(L"Index is out of range, must be [0, 8191]\n");
        return 0;
    }

    // Acquire the list lock.
    sResource *p_sResource = sResource::Instance();
    EnterCriticalSection(&p_sResource->ListLock);

    // Get the resource list table pointer and make sure the resource we want is not null.
    if (p_sResource->pResourceEntries[index] != nullptr)
    {
        // Set the return value.
        pResource = p_sResource->pResourceEntries[index];

        // Increment the ref count of the resource.
        pResource->IncrementRefCount();
    }

    // Release the list lock.
    LeaveCriticalSection(&p_sResource->ListLock);

    // Return the resource.
    return (__int64)pResource;
}

//__int64 CalculateResourceIdTest(WCHAR **argv, int argc)
//{
//    // Setup the unicode converter.
//    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> unicConvert;
//
//    // Make sure there is at least 1 argument to parse.
//    if (argc != 1)
//    {
//        // Invalid command syntax.
//        wprintf(L"Invalid command syntax\n");
//        return 0;
//    }
//
//    // Convert the unicode file name to ascii.
//    std::string sFileName = unicConvert.to_bytes(argv[0]);
//
//    // Calculate the resource id.
//    ULONGLONG resourceId = sResource::Instance()->CalculateResourceId(*g_rTextureDTI, (char*)sFileName.c_str());
//    wprintf(L"0x%016llx\n", resourceId);
//
//    return 0;
//}

__int64 LoadArchive(WCHAR **argv, int argc)
{
    // Make sure there is at least 1 argument to parse.
    if (argc != 1)
    {
        // Invalid command syntax.
        wprintf(L"Invalid command syntax\n");
        return 0;
    }

    // Setup the unicode converter.
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> unicConvert;

    // Convert the unicode file name to ascii.
    std::string sFileName = unicConvert.to_bytes(argv[0]);

    // Load the archive.
    cResource *pResource = sResource::Instance()->LoadGameResource<cResource>(rArchive::DebugTypeInfo, sFileName.c_str(), RLF_SYNCHRONOUS | RLF_LOAD_AS_ARCHIVE);
    if (pResource == nullptr)
    {
        // Failed to force load resource.
        wprintf(L"Failed to load archive '%S'!\n", sFileName.c_str());
        return 0;
    }

    // BUGBUG: This creates a memory leak by leaving the archive in memory, we should add something to unload resources to prevent this.

    return 0;
}