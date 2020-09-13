
#pragma once
#include "LibMtFramework.h"
#include <MtFramework/MtObject.h>

// sizeof = 0x21110
struct sSnatcherMain //: public sMain
{
    /* 0x20248 */

    /* 0x20258 */ void  *pSnatcherTool;

    IMPLEMENT_SINGLETON(sSnatcherMain, 0x141944DD8);
};