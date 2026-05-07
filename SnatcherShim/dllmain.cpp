// dllmain.cpp : Defines the entry point for the DLL application.
#include <Windows.h>
#include <filesystem>
#include <MtFramework/MtObject.h>

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

        // Format the file path of the drex dll using the file path for ourself. This ensures when we
        // run this under a debugger we never fail to load the drex dll due to not being able to find the shim dll.
        GetModuleFileNameA(hModule, sModulePath, sizeof(sModulePath));
        std::filesystem::path sDrexDllPath = std::filesystem::path(sModulePath).replace_filename("DeadRisingEx");

        // Load the drex dll.
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

