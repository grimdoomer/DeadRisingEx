
#pragma once
#include "LibMtFramework.h"
#include "MtFramework/Game/cSystem.h"
#include <dinput.h>

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
        /* 0x2B9 */ INT16   Trigger_free;
        /* 0x2BA */ INT16   Analog_free;
        /* 0x2BC */ INT16   Analog_cross_free;
        /* 0x2C0 */ // DWORD (0x140655605)
        /* 0x2C4 */ // DWORD (0x140655605)

        /* 0x2D0 */ // buffer 0x60 bytes
        /* 0x330 */ // buffer 0xCC bytes
        /* 0x3FC */ DWORD   On;

        /* 0x414 */ INT32   Rx;
        /* 0x418 */ INT32   Ry;
        /* 0x41C */ INT32   Lx;
        /* 0x420 */ INT32   Ly;
        /* 0x424 */ INT32   Rz_RT;
        /* 0x428 */ INT32   Lz_LT;
        /* 0x42C */ //DWORD Initialized? (0x1406553F4)
        /* 0x430 */ BYTE    mTriggerVibLow;
        /* 0x431 */ BYTE    mTriggerVibHigh;
        /* 0x432 */ WORD    mVibStartValue;
        /* 0x434 */ WORD    mVibEndValue;
        /* 0x436 */ WORD    mVibTime;
        /* 0x438 */ sPad::Map   JoyPadMapping;
        /* 0x678 */

        IMPLEMENT_MYDTI(Pad, 0x141CF3198, 0x1400AF010, 0x140656BB0);
    };

    /* 0x38 */
    /* 0x40 */ // DWORD (0x1406523AA)
    /* 0x44 */ // DWORD (0x1406523AA)
    /* 0x48 */ // DWORD (0x1406523AA)

    /* 0x60 */ Pad          mPad[64];
    /* 0x19E60 */ // some kind of buffer 0x1010 in size
    /* 0x1AE70 */ // 0x400 bytes big
    /* 0x1B270 */ // BOOL
    /* 0x1B271 */ BOOL      mXInput;

    /* 0x1B278 */ IDirectInputA         *pInputInterface;
    /* 0x1B280 */ // 0x200 bytes
    /* 0x1B480 */ IDirectInputDeviceA   *pKeyboardDevice;
    /* 0x1B488 */ IDirectInputDeviceA   *pMouseDevice;

    /* 0x1B4D8 */ // 0x400 bytes
    /* 0x1B8D8 */ // 0x400 bytes

    IMPLEMENT_MYDTI(sPad, 0x141CF3198, 0x1400AF010, 0x140656BE0);

    IMPLEMENT_SINGLETON(sPad, 0x141CF3190);
};