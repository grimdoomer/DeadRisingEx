
#include "cGametaskTitleImpl.h"
#include <detours.h>

// Asm hook functions:
void *cGametaskTitle_UnkUpdateFunc = GetModuleAddress<void*>(0x14020CFAB);

bool cGametaskTitleImpl::InstallHooks()
{
    // Install hooks:
    DetourAttach((void**)&cGametaskTitle_UnkUpdateFunc, Hook_cGametaskTitle_UnkUpdateFunc);

    return true;
}