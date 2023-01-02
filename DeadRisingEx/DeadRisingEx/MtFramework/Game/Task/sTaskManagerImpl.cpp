
#include "sTaskManagerImpl.h"
#include <MtFramework/Game/sMain.h>
#include <MtFramework/Game/Task/sTaskManager.h>
#include <MtFramework/Game/Task/cGametaskConvert.h>
#include <MtFramework/Game/Task/cGametaskDemo.h>
#include <MtFramework/Game/Task/cGametaskDolby.h>
#include <MtFramework/Game/Task/cGametaskExit.h>
#include <MtFramework/Game/Task/cGametaskInit.h>
#include <MtFramework/Game/Task/cGametaskLoad.h>
#include <MtFramework/Game/Task/cGametaskLogo.h>
#include <MtFramework/Game/Task/cGametaskMain.h>
#include <MtFramework/Game/Task/cGametaskMovie.h>
#include <MtFramework/Game/Task/cGametaskTitle.h>
#include <MtFramework/Game/Task/cGametaskWarning.h>
#include <MtFramework/Memory/MtHeapAllocator.h>
#include <detours.h>

void Hook_sTaskManager_StartNewTask(sTaskManager* thisptr);

bool sTaskManagerImpl::InstallHooks()
{
    DetourAttach((void**)&sTaskManager::_StartNewTask, Hook_sTaskManager_StartNewTask);

    return true;
}

void Hook_sTaskManager_StartNewTask(sTaskManager* thisptr)
{
    // Check the task type and handle accordingly.
    cGametask* pNewTask = nullptr;
    switch (*(sTaskManager::GametaskType*)((BYTE*)thisptr + 0x38))
    {
    case sTaskManager::GametaskType::Init:
    {
        pNewTask = (cGametask*)(*g_pSystemHeapAllocator)->Alloc(0x88, 0x10);
        if (pNewTask)
            cGametaskInit::_ctor((cGametaskInit*)pNewTask);
        break;
    }
    case sTaskManager::GametaskType::Warning:
    {
        pNewTask = (cGametask*)(*g_pSystemHeapAllocator)->Alloc(0xA0, 0x10);
        if (pNewTask)
            cGametaskWarning::_ctor((cGametaskWarning*)pNewTask);
        break;
    }
    case sTaskManager::GametaskType::Logo:
    {
        pNewTask = (cGametask*)(*g_pSystemHeapAllocator)->Alloc(0x68, 0x10);
        if (pNewTask)
            cGametaskLogo::_ctor((cGametaskLogo*)pNewTask);
        break;
    }
    case sTaskManager::GametaskType::Title:
    {
        pNewTask = (cGametask*)(*g_pSystemHeapAllocator)->Alloc(0x1F8, 0x10);
        if (pNewTask)
            cGametaskTitle::_ctor((cGametaskTitle*)pNewTask);
        break;
    }
    case sTaskManager::GametaskType::Main:
    {
        // Check some flag value on sMain.
        DWORD flag = *(DWORD*)((BYTE*)sMain::Instance() + 0x210A0);
        if ((flag & 1) == 0)
        {
            pNewTask = (cGametask*)(*g_pSystemHeapAllocator)->Alloc(0x9C20, 0x10);
            if (pNewTask)
                cGametaskMain::_ctor((cGametaskMain*)pNewTask);
        }
        else
        {
            pNewTask = *(cGametask**)((BYTE*)thisptr + 0x2F060);
            *(cGametask**)((BYTE*)thisptr + 0x2F060) = nullptr;
        }
        break;
    }
    case sTaskManager::GametaskType::Demo:
    {
        pNewTask = (cGametask*)(*g_pSystemHeapAllocator)->Alloc(0x88, 0x10);
        if (pNewTask)
            cGametaskDemo::_ctor((cGametaskDemo*)pNewTask);
        break;
    }
    case sTaskManager::GametaskType::Movie:
    {
        pNewTask = (cGametask*)(*g_pSystemHeapAllocator)->Alloc(0x70, 0x10);
        if (pNewTask)
            cGametaskMovie::_ctor((cGametaskMovie*)pNewTask);
        break;
    }
    case sTaskManager::GametaskType::Load:
    {
        pNewTask = (cGametask*)(*g_pSystemHeapAllocator)->Alloc(0x40, 0x10);
        if (pNewTask)
            cGametaskLoad::_ctor((cGametaskLoad*)pNewTask);
        break;
    }
    case sTaskManager::GametaskType::Convert:
    {
        pNewTask = (cGametask*)(*g_pSystemHeapAllocator)->Alloc(0x198, 0x10);
        if (pNewTask)
            cGametaskConvert::_ctor((cGametaskConvert*)pNewTask);
        break;
    }
    case sTaskManager::GametaskType::Dolby:
    {
        pNewTask = (cGametask*)(*g_pSystemHeapAllocator)->Alloc(0x58, 0x10);
        if (pNewTask)
            cGametaskDolby::_ctor((cGametaskDolby*)pNewTask);
        break;
    }
    case sTaskManager::GametaskType::Exit:
    {
        pNewTask = (cGametask*)(*g_pSystemHeapAllocator)->Alloc(0x40, 0x10);
        if (pNewTask)
            cGametaskExit::_ctor((cGametaskExit*)pNewTask);
        break;
    }
    }

    // Assign the new task.
    *(cGametask**)((BYTE*)thisptr + 0x2F058) = pNewTask;
    *(sTaskManager::GametaskType*)((BYTE*)thisptr + 0x38) = sTaskManager::GametaskType::None;
}