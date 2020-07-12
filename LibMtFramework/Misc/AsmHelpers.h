/*

*/

#pragma once
#include <Windows.h>

extern "C" void *SnatcherModuleHandle;

/*
	Adjusts a module address for relocation.
*/
template<typename T> T GetModuleAddress(void *pAddress)
{
	static void *GameBaseAddress = GetModuleHandle(NULL);
	return (T)((__int64)pAddress - ((__int64)0x140000000 - (__int64)GameBaseAddress));
}

/*
	Gets the T instance at the specified address after adjusting it for module relocation.
*/
template<typename T> T GetModulePointer(void *pAddress)
{
	return *(T*)GetModuleAddress<void*>(pAddress);
}

/*
	Gets a pointer value at the specified address after adjusting it for module relocation, adds the
	specified offset, and then returns the new pointer as type S.
*/
template<typename T, typename S> S* GetModulePointer(void *pAddress, int offset = 0)
{
	return (S*)(GetModulePointer<T>(pAddress) + offset);
}

extern "C" __int64 __stdcall ThisPtrCall(void *functionPtr, void *thisPtr, void *arg1 = nullptr, void *arg2 = nullptr, void *arg3 = nullptr, void *arg4 = nullptr);

extern "C" __int64 __stdcall ThisPtrCallNoFixup(void *functionPtr, void *thisPtr, void *arg1 = nullptr, void *arg2 = nullptr, void *arg3 = nullptr, void *arg4 = nullptr);

/*
	Performs a this ptr call for an object without adjusting functionPtr for relocation.
*/
template<typename T> T __stdcall ThisPtrCallNoFixup(void *functionPtr, void *thisPtr, void *arg1 = nullptr, void *arg2 = nullptr, void *arg3 = nullptr, void *arg4 = nullptr)
{
	return (T)ThisPtrCallNoFixup(functionPtr, thisPtr, arg1, arg2, arg3, arg4);
}