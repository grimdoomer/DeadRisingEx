/*

*/

#pragma once
#include "DeadRisingEx.h"
#include <MtFramework/Game/Player/uPlayer.h>

extern void **g_sUnitInstance;
extern uPlayer *uPlayerInstance;

class uPlayerImpl
{
protected:

public:
    static void RegisterTypeInfo();
    static void InstallHooks();
};