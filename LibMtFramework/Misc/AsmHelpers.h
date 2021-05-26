/*

*/

#pragma once
#include <Windows.h>

extern "C" void *SnatcherModuleHandle;

/*
    Adjusts a module address for relocation.
*/
static void * GetModuleAddress(__int64 address)
{
    static void *GameBaseAddress = GetModuleHandle(NULL);
    return (void*)(address - ((__int64)0x140000000 - (__int64)GameBaseAddress));
}

/*
    Adjusts a module address for relocation.
*/
static void * GetModuleAddress(void * pAddress)
{
    static void *GameBaseAddress = GetModuleHandle(NULL);
    return (void*)((__int64)pAddress - ((__int64)0x140000000 - (__int64)GameBaseAddress));
}

/*
    Gets the T instance at the specified address after adjusting it for module relocation.
*/
template<typename T> T GetModulePointer(void *pAddress)
{
    return *(T*)GetModuleAddress(pAddress);
}

/*
    Gets a pointer value at the specified address after adjusting it for module relocation, adds the
    specified offset, and then returns the new pointer as type S.
*/
template<typename T, typename S> S* GetModulePointer(void *pAddress, int offset = 0)
{
    return (S*)(GetModulePointer<T>(pAddress) + offset);
}

/*
    Performs a this ptr call for an object adjusting functionPtr for relocation.
*/
extern "C" __int64 __stdcall ThisPtrCall(void *functionPtr, void *thisPtr, ...);

/*
    Performs a this ptr call for an object without adjusting functionPtr for relocation.
*/
extern "C" __int64 __stdcall ThisPtrCallNoFixup(void *functionPtr, void *thisPtr, ...);