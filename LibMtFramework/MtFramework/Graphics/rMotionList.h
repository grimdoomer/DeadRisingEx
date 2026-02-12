
#pragma once
#include "MtFramework/Archive/cResource.h"

// sizeof = 0x68
struct rMotionList : public cResource
{
    /* 0x60 */ void* pFileBuffer;           // Allocation containing the entire rMotionList file

    IMPLEMENT_MYDTI(rMotionList, 0x141D18080, 0x1400AF010, 0x1406BE750);
};
static_assert(sizeof(rMotionList) == 0x68, "rMotionList incorrect struct size");