
#pragma once
#include "cGametask.h"

// sizeof = 0x9C20
struct cGametaskMain : public cGametask
{
    /* 0x40 */

    /* 0x182 */ BYTE    mBack_menu_disp_flag;

    IMPLEMENT_MYDTI(cGametaskMain, 0x141951DB8, 0x1400AF010, 0x1402045B0);
};