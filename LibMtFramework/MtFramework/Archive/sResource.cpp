/*

*/

#include "sResource.h"
#include <stdio.h>
#include <string>
#include <locale>
#include <codecvt>
#include "Misc/AsmHelpers.h"
#include "MtFramework/MtObject.h"

sResource **g_sResourceInstance = GetModuleAddress<sResource**>((void*)0x141CF27F8);

/*
cResource* sResourceImpl::GetGameResourceAsType(void *pTypeDTI, char *psFileName, int dwFlags)
{
	return (cResource*)ThisPtrCall((void*)0x14063BC60, GetModulePointer<void*>(g_sResourceSingletonInst), pTypeDTI, psFileName, (void*)dwFlags, nullptr);
}

void sResourceImpl::IncrementResourceRefCount(cResource *pResource)
{
	ThisPtrCall((void*)0x14063B3B0, GetModulePointer<void*>(g_sResourceSingletonInst), pResource, nullptr, nullptr, nullptr);
}

// Forward declarations for command functions.
__int64 PrintLoadedResources(WCHAR **argv, int argc);
__int64 GetResourceByIndex(WCHAR **argv, int argc);

// Table of commands for sResource objects.
const int g_sResourceCommandsLength = 2;
const CommandEntry g_sResourceCommands[g_sResourceCommandsLength] =
{
	{ L"list_resources", L"Lists all loaded resources", PrintLoadedResources },
	{ L"GetResourceByIndex", L"Gets the resource at the specified index", GetResourceByIndex }
};

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
	EnterCriticalSection((LPCRITICAL_SECTION)(GetModulePointer<__int64>(g_sResourceSingletonInst) + 8));

	// Loop through the hash table and print each entry.
	cResource **pTableEntries = (cResource**)(GetModulePointer<__int64>(g_sResourceSingletonInst) + 0x2248);
	for (int i = 0; i < 8192; i++)
	{
		// Check if this entry is allocated.
		if (pTableEntries[i] == nullptr)
			continue;

		// Check if there is a filter and handle accordingly.
		if (sFilter.size() > 0)
		{
			// Check if this is a front filter or rear filter.
			std::string sFileName(pTableEntries[i]->mPath);
			if (bFilterFront == true && sFileName.find(sFilter, 0) == 0)
			{
				// Print the name of the object.
				wprintf(L"Resource %d: %S\n", i, pTableEntries[i]->mPath);
			}
			else if (bFilterFront == false && sFileName.rfind(sFilter) == sFileName.size() - sFilter.size())
			{
				// Print the name of the object.
				wprintf(L"Resource %d: %S\n", i, pTableEntries[i]->mPath);
			}
		}
		else
		{
			// Print the name of the object.
			wprintf(L"Resource %d: %S\n", i, pTableEntries[i]->mPath);
		}
	}

	// Release the list lock.
	LeaveCriticalSection((LPCRITICAL_SECTION)(GetModulePointer<__int64>(g_sResourceSingletonInst) + 8));

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
	EnterCriticalSection((LPCRITICAL_SECTION)(GetModulePointer<__int64>(g_sResourceSingletonInst) + 8));

	// Get the resource list table pointer and make sure the resource we want is not null.
	cResource **pTableEntries = (cResource**)(GetModulePointer<__int64>(g_sResourceSingletonInst) + 0x2248);
	if (pTableEntries[index] != nullptr)
	{
		// Set the return value.
		pResource = pTableEntries[index];

		// Increment the ref count of the resource.
		sResourceImpl::IncrementResourceRefCount(pResource);
	}

	// Release the list lock.
	LeaveCriticalSection((LPCRITICAL_SECTION)(GetModulePointer<__int64>(g_sResourceSingletonInst) + 8));

	// Return the resource.
	return (__int64)pResource;
}
*/

void sResourceImpl::InitializeTypeInfo()
{
	// Register types:
	RegisterTypeInfo(&cResourceTypeInfo);

	// Register commands:
	//RegisterCommands(g_sResourceCommands, g_sResourceCommandsLength);
}