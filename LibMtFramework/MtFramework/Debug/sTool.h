
#pragma once
#include "sToolBase.h"

// sizeof = 0x2270
struct sTool : public sToolBase
{
    sTool()
    {
        // TODO:
        DebugBreak();
    }

    ~sTool()
    {
        // TODO:
        DebugBreak();
    }
};
ASSERT_STRUCT_SIZE(sTool, 0x2270);