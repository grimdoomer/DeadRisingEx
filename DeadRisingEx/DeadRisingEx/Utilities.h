/*

*/

#pragma once
#include <Windows.h>
#include <stdio.h>
#include <string>

static void DbgPrint(LPCSTR Format, ...)
{
    CHAR Buffer[512] = { 0 };
    va_list Args;

    // Get the variable arg pointer.
    va_start(Args, Format);

    // Format print the string.
    int length = vsnprintf(Buffer, sizeof(Buffer), Format, Args);
    va_end(Args);

    // Output the string to the debugger.
    OutputDebugString(Buffer);
}

static bool ReplaceVTablePointer(void **pVtableAddress, void *pFunctionPtr)
{
    DWORD dwOldProtect = 0;

    // Make the page writable.
    if (VirtualProtect((void*)pVtableAddress, sizeof(void*), PAGE_READWRITE, &dwOldProtect) == FALSE)
    {
        // Failed to change page permissions.
        wprintf(L"Failed to change page permissions on address %p!\n", (void*)pVtableAddress);
        return false;
    }

    // Replace the vtable pointer.
    *pVtableAddress = pFunctionPtr;

    // Change the page permissions back.
    if (VirtualProtect((void*)pVtableAddress, sizeof(void*), dwOldProtect, &dwOldProtect) == FALSE)
    {
        // Failed to restore page permissions.
        wprintf(L"Failed to restore page permissions for address %p!\n", (void*)pVtableAddress);
        return false;
    }

    // Vtable pointer replaced successfully.
    return true;
}

template<typename T> static bool PatchBytes(T *pAddress, T newValue)
{
    DWORD dwOldProtect = 0;

    // Make the page writable.
    if (VirtualProtect(pAddress, sizeof(T), PAGE_READWRITE, &dwOldProtect) == FALSE)
    {
        // Failed to change page permissions.
        wprintf(L"Failed to change page permissions on address %p!\n", pAddress);
        return false;
    }

    // Poke the new value.
    *pAddress = newValue;

    // Change the page permissions back.
    if (VirtualProtect(pAddress, sizeof(T), dwOldProtect, &dwOldProtect) == FALSE)
    {
        // Failed to restore page permissions.
        wprintf(L"Failed to restore page permissions for address %p!\n", pAddress);
        return false;
    }

    // Bytes replaced successfully.
    return true;
}

static bool PatchBytes(void *pAddress, void *pBytes, DWORD length)
{
    DWORD dwOldProtect = 0;

    // Make the page writable.
    if (VirtualProtect(pAddress, length, PAGE_READWRITE, &dwOldProtect) == FALSE)
    {
        // Failed to change page permissions.
        wprintf(L"Failed to change page permissions on address %p!\n", pAddress);
        return false;
    }

    // Poke the new value.
    memcpy(pAddress, pBytes, length);

    // Change the page permissions back.
    if (VirtualProtect(pAddress, length, dwOldProtect, &dwOldProtect) == FALSE)
    {
        // Failed to restore page permissions.
        wprintf(L"Failed to restore page permissions for address %p!\n", pAddress);
        return false;
    }

    // Bytes replaced successfully.
    return true;
}

static bool WriteDword(DWORD *pAddress, DWORD newValue)
{
    DWORD dwOldProtect = 0;

    // Make the page writable.
    if (VirtualProtect(pAddress, sizeof(DWORD), PAGE_READWRITE, &dwOldProtect) == FALSE)
    {
        // Failed to change page permissions.
        wprintf(L"Failed to change page permissions on address %p!\n", pAddress);
        return false;
    }

    // Poke the new value.
    *pAddress = newValue;

    // Change the page permissions back.
    if (VirtualProtect(pAddress, sizeof(DWORD), dwOldProtect, &dwOldProtect) == FALSE)
    {
        // Failed to restore page permissions.
        wprintf(L"Failed to restore page permissions for address %p!\n", pAddress);
        return false;
    }

    // Vtable pointer replaced successfully.
    return true;
}