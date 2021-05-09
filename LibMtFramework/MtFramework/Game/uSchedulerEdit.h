
#pragma once
#include "MtFramework/Object/cUnit.h"

// sizeof = 0x48
struct uSchedulerEdit : public cUnit
{
    /* 0x28 */ DWORD Frame;
    /* 0x2C */ // float
    /* 0x30 */ float mSpeed;

    /* 0x35 */ bool mPause;
};
