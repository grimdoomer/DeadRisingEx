
#pragma once
#include "LibMtFramework.h"
#include "MtFramework/MtObject.h"

// sizeof = 0x218
struct MtRegistry : public MtObject
{
    /* 0x08 */ WCHAR    ConfigFilePath[MAX_PATH];
    /* 0x210 */ DWORD   Unk1;                           // Gets set in ctor to 1 and doesn't seemed to be used after
    /* 0x214 */ BOOL    ConfigFileExists;

    IMPLEMENT_MYDTI(MtRegistry, 0x141CF22C8, 0x1400AF012, 0x140620BA0);

    IMPLEMENT_SINGLETON(MtRegistry, 0x141CF20B0);
};
static_assert(sizeof(MtRegistry) == 0x218, "MtRegistry incorrect struct size");