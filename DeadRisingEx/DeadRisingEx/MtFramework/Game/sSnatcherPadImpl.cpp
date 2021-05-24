
#include "sSnatcherPadImpl.h"
#include "DeadRisingEx/MtFramework/Rendering/ImGui/ImGuiRenderer.h"
#include "DeadRisingEx/MtFramework/Rendering/ImGui/imgui.h"
#include <MtFramework/Game/sSnatcherPad.h>
#include <MtFramework/Game/sMain.h>
#include <detours.h>
#include <WinUser.h>
#include <dinput.h>

// Forward declarations:
void __stdcall Hook_sSnatcherPad_SystemUpdate(sSnatcherPad *thisptr);

bool sSnatcherPadImpl::InstallHooks()
{
    DetourAttach((void**)&sSnatcherPad::_SystemUpdate, Hook_sSnatcherPad_SystemUpdate);

    return true;
}

void __stdcall Hook_sSnatcherPad_SystemUpdate(sSnatcherPad *thisptr)
{
    // Call the trampoline and update input.
    sSnatcherPad::_SystemUpdate(thisptr);

    // Get the keyboard and mouse state for easy access.
    BYTE *pKeyboardState = ((BYTE*)sSnatcherPad::Instance() + 0x1B4D8);
    BYTE *pPreviousKeyboardState = ((BYTE*)sSnatcherPad::Instance() + 0x1B8D8);
    DIMOUSESTATE *pMouseState = (DIMOUSESTATE*)((BYTE*)sSnatcherPad::Instance() + 0x1B4A0);

    // Get the IO structure.
    ImGuiIO& io = ImGui::GetIO();

    // Set special case keys.
    io.KeyCtrl = pKeyboardState[VK_CONTROL] != 0;
    io.KeyShift = pKeyboardState[VK_SHIFT] != 0;
    io.KeyAlt = pKeyboardState[VK_MENU] != 0;
    io.KeySuper = false;

    // Get the position of the cursor within the window.
    POINT mousePosition = { 0 };
    GetCursorPos(&mousePosition);
    ScreenToClient(*sMain::GameWindowHandle, &mousePosition);

    // Update imgui mouse state.
    io.MousePos = ImVec2((float)mousePosition.x, (float)mousePosition.y);

    // Start a new imgui frame.
    ImGuiRenderer::Instance()->BeginFrame();

    // Check if imgui is being rendered on top of the game.
    if (ImGuiRenderer::Instance()->IsVisible() == true)
    {
        // Draw the mouse cursor.
        while (ShowCursor(TRUE) < 0);

        // Mask input from the game.
        DWORD socketNumber = thisptr->GetLastInputSocketNumber();
        if (socketNumber == 0)
        {
            // Clear keyboard input.
            memset(pKeyboardState, 0, 0x400);
        }
        else
        {
            // Clear gamepad input.
            BYTE *pGamepad = (BYTE*)sSnatcherPad::Instance() + 0x60 + (socketNumber * 0x678);
            *(DWORD*)(pGamepad + 0x3FC) = 0;
            *(DWORD*)(pGamepad + 0x404) = 0;
            *(INT32*)(pGamepad + 0x414) = 0;
            *(INT32*)(pGamepad + 0x418) = 0;
            *(INT32*)(pGamepad + 0x41C) = 0;
            *(INT32*)(pGamepad + 0x420) = 0;
            *(INT32*)(pGamepad + 0x424) = 0;
            *(INT32*)(pGamepad + 0x428) = 0;
        }

        // Mask mouse input.
        memset((BYTE*)sSnatcherPad::Instance() + 0x1B4A0, 0, sizeof(DIMOUSESTATE));
    }
}