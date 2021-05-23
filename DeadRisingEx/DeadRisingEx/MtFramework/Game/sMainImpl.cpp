
#include "sMainImpl.h"
#include <MtFramework/Game/sMain.h>
#include "DeadRisingEx/MtFramework/Rendering/ImGui/imgui.h"
#include "DeadRisingEx/MtFramework/Rendering/ImGui/ImGuiRenderer.h"
#include <detours.h>

LRESULT Hook_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL Hook_SetCursorPos(int x, int y);
BOOL Hook_ClipCursor(const RECT *lpRect);

BOOL(__stdcall *pSetCursorPosition)(int x, int y) = SetCursorPos;
BOOL(__stdcall *pClipCursor)(const RECT *lpRect) = ClipCursor;

// Forward declarations for command functions.
__int64 EnableDebugZombieSpawner(WCHAR **argv, int argc);

// Table of commands for sRender objects.
const int g_sMainCommandsLength = 1;
const CommandEntry g_sMainCommands[g_sMainCommandsLength] =
{
    { L"enable_zombie_spawner", L"Enables the sMManager debug zombie spawner", EnableDebugZombieSpawner }
};

void sMainImpl::RegisterTypeInfo()
{
    // Register commands:
    RegisterCommands(g_sMainCommands, g_sMainCommandsLength);
}

bool sMainImpl::InstallHooks()
{
    // Pre-emptively patch the debug zombie spawner to work on button held instead of button press.
    PatchBytes<WORD>(GetModuleAddress<WORD*>(0x1400A07BB + 3), 0x45C);

    DetourAttach((void**)&sMain::_WndProc, Hook_WndProc);
    DetourAttach((void**)&pSetCursorPosition, Hook_SetCursorPos);
    DetourAttach((void**)&pClipCursor, Hook_ClipCursor);

    return true;
}

LRESULT Hook_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // If imgui is not initialized bail out.
    if (ImGuiRenderer::Instance()->IsInitialized() == false)
        return sMain::_WndProc(hwnd, msg, wParam, lParam);

    // Get the IO structure.
    ImGuiIO& io = ImGui::GetIO();

    // Check the message type and handle accordingly.
    switch (msg)
    {
    case WM_LBUTTONDOWN: case WM_LBUTTONDBLCLK:
    case WM_RBUTTONDOWN: case WM_RBUTTONDBLCLK:
    case WM_MBUTTONDOWN: case WM_MBUTTONDBLCLK:
    case WM_XBUTTONDOWN: case WM_XBUTTONDBLCLK:
    {
        int button = 0;
        if (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONDBLCLK) { button = 0; }
        if (msg == WM_RBUTTONDOWN || msg == WM_RBUTTONDBLCLK) { button = 1; }
        if (msg == WM_MBUTTONDOWN || msg == WM_MBUTTONDBLCLK) { button = 2; }
        if (msg == WM_XBUTTONDOWN || msg == WM_XBUTTONDBLCLK) { button = (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) ? 3 : 4; }
        if (!ImGui::IsAnyMouseDown() && GetCapture() == NULL)
            SetCapture(hwnd);
        io.MouseDown[button] = true;
        break;
    }
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    case WM_MBUTTONUP:
    case WM_XBUTTONUP:
    {
        int button = 0;
        if (msg == WM_LBUTTONUP) { button = 0; }
        if (msg == WM_RBUTTONUP) { button = 1; }
        if (msg == WM_MBUTTONUP) { button = 2; }
        if (msg == WM_XBUTTONUP) { button = (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) ? 3 : 4; }
        io.MouseDown[button] = false;
        if (!ImGui::IsAnyMouseDown() && GetCapture() == hwnd)
            ReleaseCapture();
        break;
    }
    case WM_MOUSEWHEEL:
    {
        io.MouseWheel += (float)GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA;
        break;
    }
    case WM_MOUSEHWHEEL:
    {
        io.MouseWheelH += (float)GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA;
        break;
    }
    case WM_KEYDOWN:
    {
        if ((int)wParam < 256)
            io.KeysDown[(int)wParam] = true;

        // Check if the key pressed was tilde.
        if ((int)wParam == VK_OEM_3)
        {
            // Toggle the visible state of the imgui layer.
            ImGuiRenderer::Instance()->SetVisible(!ImGuiRenderer::Instance()->IsVisible());
        }
        break;
    }
    case WM_KEYUP:
    {
        if ((int)wParam < 256)
            io.KeysDown[(int)wParam] = false;
        break;
    }
    case  WM_CHAR:
    {
        if ((int)wParam > 0 && (int)wParam < 0x10000)
            io.AddInputCharacterUTF16((ImWchar16)wParam);
        break;
    }
    default:
    {
        // Call the trampoline.
        return sMain::_WndProc(hwnd, msg, wParam, lParam);
    }
    }

    return 0;
}

BOOL Hook_SetCursorPos(int x, int y)
{
    // If the imgui layer is visible bail out to prevent the cursor from being reset to center of the window.
    if (ImGuiRenderer::Instance()->IsInitialized() == false || ImGuiRenderer::Instance()->IsVisible() == false)
        return pSetCursorPosition(x, y);

    return TRUE;
}

BOOL Hook_ClipCursor(const RECT *lpRect)
{
    // If the imgui layer is visible bail out to prevent the cursor from being clipped to the game window.
    if (ImGuiRenderer::Instance()->IsInitialized() == false || ImGuiRenderer::Instance()->IsVisible() == false)
        return pClipCursor(lpRect);

    return pClipCursor(NULL);
}

__int64 EnableDebugZombieSpawner(WCHAR **argv, int argc)
{
    // Get the sMain singleton instance and set the flag to enable the spawner.
    BYTE *psMain = (BYTE*)sMain::Instance();
    *(DWORD*)(psMain + 0x203D4) |= 0x800;

    // Set the debug controller index to input index 1.
    *(DWORD*)(psMain + 0x2106C) = 1;

    return 0;
}