
#pragma once
#include "MtFramework/Archive/cResource.h"

// sizeof = 0x70
struct rHavokVertexLayout : public cResource
{
    // sizeof = 0x20
    struct LayoutInfo : public MtObject
    {

    };

    /* 0x60 */ LayoutInfo* mpLayoutInfoList;
    /* 0x68 */ //void*

    IMPLEMENT_MYDTI(rHavokVertexLayout, 0x1419474F0, 0x1400AF010, 0x1400C9DF0);
};