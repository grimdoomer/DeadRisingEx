
#pragma once
#include "cGametask.h"
#include "MtFramework/Game/uSchedulerEdit.h"

// sizeof = 0x1F8
struct cGametaskTitle : public cGametask
{
    /* 0x40 */

    /* 0xC0 */ // cResource*
    /* 0xC8 */ // cResource*
    /* 0xD0 */ // cResource*

    /* 0xE0 */ //uSchedulerEdit
    /* 0xE8 */ //rSchedulerXml

    IMPLEMENT_MYDTI(cGametaskTitle, 0x141951E98, 0x1400AF010, 0x14020D2A0);
};