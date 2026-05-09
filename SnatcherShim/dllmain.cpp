// dllmain.cpp : Defines the entry point for the DLL application.
#include <Windows.h>
#include <filesystem>
#include <MtFramework/MtObject.h>
#include <MtFramework/Archive/cResource.h>
#include <MtFramework/Archive/rArchive.h>
#include <MtFramework/Archive/sResource.h>
#include <MtFramework/Area/rAreaHitLayout.h>

#include <MtFramework/Graphics/rModel.h>

#include <MtFramework/IO/MtDataReader.h>
#include <MtFramework/IO/MtFile.h>
#include <MtFramework/IO/MtFileStream.h>
#include <MtFramework/IO/MtStream.h>
#include <MtFramework/Item/Items/uOm08.h>
#include <MtFramework/Item/Items/uOm23.h>
#include <MtFramework/Item/sItemCtrl.h>
#include <MtFramework/Item/uItem.h>
#include <MtFramework/Memory/MtAllocator.h>
#include <MtFramework/Memory/MtHeapAllocator.h>
#include <MtFramework/Object/Explosion/cExplodeData.h>
#include <MtFramework/Object/Explosion/cExplodeParts.h>
#include <MtFramework/Object/Model/uModel.h>
#include <MtFramework/Object/Model/uSnatcherModel.h>
#include <MtFramework/Object/Vehicle/uVehicle.h>
#include <MtFramework/Object/Vehicle/uVehicleOm09.h>
#include <MtFramework/Object/cUnit.h>
#include <MtFramework/Object/sUnit.h>
#include <MtFramework/Object/uCoord.h>
#include <MtFramework/Physics/Model/uHavokModel.h>
#include <MtFramework/Physics/Vehicle/rHavokVehicleData.h>

#include <MtFramework/System/cSystem.h>
#include <MtFramework/Utils/MtPropertyList.h>

__declspec(dllexport) void DummyFunc()
{

}

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
    OutputDebugStringA(Buffer);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        CHAR sModulePath[MAX_PATH] = { 0 };

        // Format the file path of the drex dll using the file path for ourself.
        GetModuleFileNameA(hModule, sModulePath, sizeof(sModulePath));
        std::filesystem::path sDrexDllPath = std::filesystem::path(sModulePath).replace_filename("DeadRisingEx");

        // Load the drex dll, this ensures when we run this under a debugger we never fail to load the drex dll due to not being able to find the shim dll.
        HMODULE hDrex = LoadLibraryA(sDrexDllPath.generic_string().c_str());
        if (hDrex == nullptr)
        {
            DbgPrint("Failed to load '%s' 0x%08x\n", sDrexDllPath.generic_string().c_str(), GetLastError());
            DebugBreak();

            MessageBoxW(NULL, L"Failed to load DeadRisingEx.dll.",
                L"DeadRisingEx", MB_OK | MB_ICONERROR | MB_APPLMODAL);
            TerminateProcess(GetCurrentProcess(), 0xBAD0C0DE);
        }
        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

