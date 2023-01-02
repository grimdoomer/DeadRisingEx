
#pragma once
#include "LibMtFramework.h"
#include "MtFramework/System/cSystem.h"
#include <dinput.h>

enum InputButtonId
{
    CheckWatch = 19,
};

// sizeof = 0x1BCD8
struct sPad : public cSystem
{
    // sizeof = 0x240
    struct Map : public MtObject
    {
        /* 0x08 */

        /* 0x228 */ BYTE    Rup;
        /* 0x229 */ BYTE    Rright;
        /* 0x22A */ BYTE    Rdown;
        /* 0x22B */ BYTE    Rleft;
        /* 0x22C */ BYTE    L2_LB;
        /* 0x22D */ BYTE    R2_RB;
        /* 0x22E */ BYTE    L1_LT;
        /* 0x22F */ BYTE    R1_RT;
        /* 0x230 */ BYTE    Select;
        /* 0x231 */ BYTE    Start;
        /* 0x232 */ BYTE    L3_LS;
        /* 0x233 */ BYTE    R3_RS;
        /* 0x234 */ BYTE    Lup;
        /* 0x235 */ BYTE    Lright;
        /* 0x236 */ BYTE    Ldown;
        /* 0x237 */ BYTE    Lleft;
        /* 0x238 */ BYTE    AnalogLx;
        /* 0x239 */ BYTE    AnalogLy;
        /* 0x23A */ BYTE    AnalogRx;
        /* 0x23B */ BYTE    AnalogRy;

        IMPLEMENT_MYDTI(Map, 0x141CF31C8, 0x1400AF010, 0x140656B80);
    };

    // sizeof = 0x678
    struct Pad : public MtObject
    {
        // Button values for input:
        #define PAD_BTN_BACK            1           // Gamepad back button
        #define PAD_BTN_LEFT_THUMB      2           // Gamepad left thumbstick
        #define PAD_BTN_RIGHT_THUMB     4           // Gamepad right thumbstick
        #define PAD_BTN_START           8           // Gamepad start button
        #define PAD_BTN_DPAD_UP         0x10        // Gamepad Dpad up
        #define PAD_BTN_DPAD_RIGHT      0x20        // Gmaepad Dpad right
        #define PAD_BTN_DPAD_DOWN       0x40        // Gamepad Dpad down
        #define PAD_BTN_DPAD_LEFT       0x80        // Gmaepad Dpad left
        #define PAD_BTN_LEFT_BUMPER     0x100       // Gamepad left bumper
        #define PAD_BTN_RIGHT_BUMPER    0x200       // Gamepad right bumper
        #define PAD_BTN_LEFT_TRIGGER    0x400       // Gamepad left trigger
        #define PAD_BTN_RIGHT_TRIGGER   0x800       // Gamepad right trigger
        #define PAD_BTN_Y               0x1000      // Gamepad Y button
        #define PAD_BTN_B               0x2000      // Gamepad B button
        #define PAD_BTN_A               0x4000      // Gamepad A button
        #define PAD_BTN_X               0x8000      // Gamepad X button
        #define PAD_LEFT_THUMB_UP       0x10000     // Gamepad left thumbstick up
        #define PAD_LEFT_THUMB_LEFT     0x20000     // Gamepad left thumbstick left
        #define PAD_LEFT_THUMB_DOWN     0x40000     // Gamepad left thumbstick down
        #define PAD_LEFT_THUMB_RIGHT    0x80000     // Gamepad left thumbstick right
        #define PAD_RIGHT_THUMB_UP      0x100000    // Gamepad right thumbstick up
        #define PAD_RIGHT_THUMB_LEFT    0x200000    // Gamepad right thumbstick left
        #define PAD_RIGHT_THUMB_DOWN    0x400000    // Gamepad right thumbstick down
        #define PAD_RIGHT_THUMB_RIGHT   0x800000    // Gamepad right thumbstick right

        /* 0x08 */ BYTE     Be_flag;        // Big endian flag?
        /* 0x09 */ // BYTE (0x1406555E0)
        /* 0x0A */ BYTE     Rno;
        /* 0x0B */ BYTE     Pad_no;
        /* 0x0C */ BYTE     UserIndex;
        /* 0x0D */ BYTE     Kind;

        /* 0x10 */ DWORD    Ability;
        /* 0x14 */ DWORD    Input_attr;
        /* 0x18 */ DWORD    Socket_no;

        /* 0x2B8 */ INT8    Press_free;
        /* 0x2B9 */ INT16   Trigger_free;       // Threshold for trigger presses (dead zone)
        /* 0x2BA */ INT16   Analog_free;
        /* 0x2BC */ INT16   Analog_cross_free;
        /* 0x2C0 */ // DWORD (0x140655605)
        /* 0x2C4 */ // DWORD (0x140655605)

        /* 0x2D0 */ // float [24] might be time in ms buttons were pressed
        /* 0x330 */ // buffer 0xCC bytes
        /* 0x3FC */ DWORD   On;                 // Buttons from wButtons in XINPUT_GAMEPAD active this update
        /* 0x400 */ DWORD   PreviousButtons;    // Pad->On from the previous update
        /* 0x404 */ DWORD   ButtonsPressed;     // Buttons active this update but not the previous update
        /* 0x408 */ DWORD   ButtonsReleased;    // Buttons active the previous update but not this update
        /* 0x40C */ DWORD   ButtonsChanged;     // Buttons that changed state between the previous update and this update (on or off)

        /* 0x414 */ INT32   Rx;
        /* 0x418 */ INT32   Ry;
        /* 0x41C */ INT32   Lx;             // Set to sThumbLX of XINPUT_GAMEPAD
        /* 0x420 */ INT32   Ly;
        /* 0x424 */ INT32   Rz_RT;          // Set to bRightTrigger of XINPUT_GAMEPAD
        /* 0x428 */ INT32   Lz_LT;          // Set to bLeftTrigger of XINPUT_GAMEPAD
        /* 0x42C */ //DWORD Initialized? (0x1406553F4)
        /* 0x430 */ BYTE    mTriggerVibLow;
        /* 0x431 */ BYTE    mTriggerVibHigh;
        /* 0x432 */ WORD    mVibStartValue;
        /* 0x434 */ WORD    mVibEndValue;
        /* 0x436 */ WORD    mVibTime;
        /* 0x438 */ sPad::Map   JoyPadMapping;

        IMPLEMENT_MYDTI(Pad, 0x141CF3198, 0x1400AF010, 0x140656BB0);
    };

    /* 0x38 */
    /* 0x3C */ DWORD        UpdatedPadSocket;   // Socket_no of the last gamepad with input
    /* 0x40 */ DWORD        NewInputAvailable;  // 1 if a gamepad had active input (0x1406523AA)
    /* 0x44 */ DWORD        MouseSensitivity;
    /* 0x48 */ DWORD        CameraSensitivity;

    /* 0x50 */ ULONGLONG    LastUpdateTime;     // Time of the last input update in ms (pulled from sMain->mTimer)
    /* 0x58 */ DWORD        ElapsedTime;        // Elapsed time since the last update in ms

    /* 0x60 */ Pad          mPad[64];
    /* 0x19E60 */ // some kind of buffer 0x1010 in size
    /* 0x1AE70 */ // 0x400 bytes big
    /* 0x1B270 */ // BOOL
    /* 0x1B271 */ bool      mXInput;

    /* 0x1B278 */ IDirectInputA         *pInputInterface;
    /* 0x1B280 */ // void *[64]
    /* 0x1B480 */ IDirectInputDeviceA   *pKeyboardDevice;
    /* 0x1B488 */ IDirectInputDeviceA   *pMouseDevice;

    /* 0x1B4A0 */ DIMOUSESTATE          MouseState;

    /* 0x1B4D8 */ //BYTE KeyboardState 0x400 bytes
    /* 0x1B8D8 */ //BYTE previous keyboard state? 0x400 bytes

    IMPLEMENT_MYDTI(sPad, 0x141CF3198, 0x1400AF010, 0x140656BE0);

    IMPLEMENT_SINGLETON(sPad, 0x141CF3190);

    inline static void(__stdcall *_SystemUpdate)(sPad *thisptr) =
        (void(__stdcall*)(sPad*))GetModuleAddress(0x140655C50);

    inline static DWORD(__stdcall *_GetLastInputSocketNumber)(sPad *thisptr) =
        (DWORD(__stdcall*)(sPad*))GetModuleAddress(0x140652610);

    inline static bool(__stdcall *_GetButtonPressed)(InputButtonId buttonId) =
        (bool(__stdcall*)(InputButtonId))GetModuleAddress(0x1400291A0);

    inline static bool(__stdcall *_GetKeyPressed)(int key) =
        (bool(__stdcall*)(int))GetModuleAddress(0x140029460);

    /*
        Description: Gets the socket number for the last device to register input

        Returns: 0 if the keyboard was the last device, or the socket number of the game pad that last registered input
    */
    DWORD GetLastInputSocketNumber()
    {
        return _GetLastInputSocketNumber(this);
    }

    /*
        Description: Returns a value indicating if the specified button was pressed this frame

        Parameters:
            buttonId: Button id to query
    */
    static bool GetButtonPressed(InputButtonId buttonId)
    {
        return _GetButtonPressed(buttonId);
    }

    /*
        Description: Returns a value indicating if the specified key was pressed this frame

        Parameters:
            key: key id to query
    */
    static bool GetKeyPressed(int key)
    {
        return _GetKeyPressed(key);
    }
};