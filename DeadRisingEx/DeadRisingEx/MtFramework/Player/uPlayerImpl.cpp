/*

*/

#include "uPlayerImpl.h"
#include "detours.h"
#include <locale>
#include <codecvt>
#include <MtFramework/Archive/sResource.h>

void **g_sUnitInstance = GetModuleAddress<void**>(0x141CF2620);
void **g_sItemCtrlInstance = GetModuleAddress<void**>(0x1419462A0);

void *uPlayerInstance = nullptr;

void * (__stdcall *uPlayer_ctor)(void *thisptr) = GetModuleAddress<void*(__stdcall*)(void*)>(0x1401306E0);

void * __stdcall Hook_uPlayer_ctor(void *thisptr);

void uPlayerImpl::RegisterTypeInfo()
{
    // Hook the uPlayer constructor so we can easily get the address of the player's position later on.
    DetourAttach((void**)&uPlayer_ctor, Hook_uPlayer_ctor);
}

void * __stdcall Hook_uPlayer_ctor(void *thisptr)
{
    uPlayerInstance = thisptr;
    return uPlayer_ctor(thisptr);
}