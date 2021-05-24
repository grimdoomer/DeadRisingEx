
#include "sSMManagerImpl.h"
#include <MtFramework/Object/Model/sSMManager.h>
#include <detours.h>

BOOL *pGlobalLockStatus = GetModuleAddress<BOOL*>(0x141CF2A50);

void __stdcall Hook_sSMManager_AddModel(sSMManager *thisptr, uSnatcherModel *pModel);

bool sSMManagerImpl::InstallHooks()
{
    DetourAttach((void**)&sSMManager::_AddModel, Hook_sSMManager_AddModel);

    return true;
}

void __stdcall Hook_sSMManager_AddModel(sSMManager *thisptr, uSnatcherModel *pModel)
{
    // Check if we need to lock on list access.
    if (thisptr->IgnoreGlobalLock != FALSE || *pGlobalLockStatus != FALSE)
        EnterCriticalSection(&thisptr->Lock);

    DWORD *pdwModelCount = (DWORD*)((BYTE*)thisptr + 0x40A0);
    uSnatcherModel **ppModelList = (uSnatcherModel**)((BYTE*)thisptr + 0x40A8);

    // Check if there is room in the model list.
    if (*pdwModelCount < 2048)
    {
        // Add the model to the list.
        ppModelList[*pdwModelCount] = pModel;
        (*pdwModelCount)++;

        if (*pdwModelCount == 2048 - 15)
            ImGuiConsole::Instance()->ConsolePrint(L"sSMManager->ModelCount hit threshold\n");
    }
    else
    {
        // TODO: Eventually we need to handle the memory leak we create from not tracking the model. Perhaps
        // we can add the model to a list that will be destroyed on the next area load?

        // Model list is full, do nothing and report an error.
        ImGuiConsole::Instance()->ConsolePrint(L"sSMManager::AddModel(): model list is full!\n");
    }

    // Check if we need to release the list lock.
    if (thisptr->IgnoreGlobalLock != FALSE || *pGlobalLockStatus != FALSE)
        LeaveCriticalSection(&thisptr->Lock);
}