
#pragma once
#include "sToolBase.h"

// sizeof = 0x2270
struct sTool : public sToolBase
{

};
static_assert(sizeof(sTool) == 0x2270, "sTool incorrect struct size");