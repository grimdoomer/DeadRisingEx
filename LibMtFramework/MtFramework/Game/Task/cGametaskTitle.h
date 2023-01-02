
#pragma once
#include "cGametask.h"
#include "MtFramework/Game/uSchedulerEdit.h"
#include "MtFramework/Graphics/rTexture.h"

// sizeof = 0x1F8
struct cGametaskTitle : public cGametask
{
    // sizeof = 0x1C
    struct MenuOption
    {
        /* 0x00 */ DWORD OptionNameStringIndex;         // String index into the rMessage file containing the option text
        /* 0x04 */ BYTE UIFlags;                        // UI flags that get set on sSnatcherMain before drawing
        /* 0x08 */ Vector2 Position;
        /* 0x10 */ Vector2 Scale;
        /* 0x18 */ BYTE _unused[4];
    };
    static_assert(sizeof(MenuOption) == 0x1C, "cGametaskTitle::MenuOption incorrect struct size");

    inline static MenuOption* MainMenuOptionList = (MenuOption*)GetModuleAddress(0x14122B5E0);
    inline static MenuOption* SubMenuOptionList = (MenuOption*)GetModuleAddress(0x14122B6B0);

    /* 0x40 */

    /* 0x4C */ //DWORD        SelectedOptionIndex; ?

    /* 0x90 */ Vector2      MenuOptionsPosition;        // Base position of the menu options list

    /* 0xA8 */ DWORD        MenuId;                     // 0 = main menu, 1 = sub menu

    /* 0xC0 */ rTexture*    TitleScreenSplashTexture;   // "DEAD RISING" image that fades out
    /* 0xC8 */ rTexture*    MenuScreenUITexture;        // Helper textures for option selection
    /* 0xD0 */ // rSprAnm*  MenuScreenUITextureSpriteSheet;     // Sprite data for the UI texture
    /* 0xD8 */ // cSprAnm*

    /* 0xE0 */ //uSchedulerEdit
    /* 0xE8 */ //rSchedulerXml

    /* 0x140 */ //rSoundSnd*    sound effects for main menu?

    /* 0x164 */ // DWORD LoadState; // 0x14020E51F

    /* 0x1D0 */ // void* allocated with g_pStringHeapAllocator

    /*
        VTable:
            0x40
            0x48
    */

    inline static cGametaskTitle* (__stdcall *_ctor)(cGametaskTitle* thisptr) =
        (cGametaskTitle*(__stdcall*)(cGametaskTitle*))GetModuleAddress(0x14020B900);

    inline static cGametaskTitle * (__stdcall *_dtor)(cGametaskTitle *thisptr, bool bFreeMemory) =
        (cGametaskTitle*(__stdcall*)(cGametaskTitle*, bool))GetModuleAddress(0x14020BF20);

    inline static MtDTI * (__stdcall *_GetDTI)(cGametaskTitle *thisptr) =
        (MtDTI*(__stdcall*)(cGametaskTitle*))GetModuleAddress(0x14020CDC0);

    IMPLEMENT_MYDTI(cGametaskTitle, 0x141951E98, 0x1400AF010, 0x14020D2A0);

    cGametaskTitle()
    {
        _ctor(this);
    }

    ~cGametaskTitle()
    {
        (void)ThisPtrCallNoFixup(this->vtable[0], this, false);
    }
};