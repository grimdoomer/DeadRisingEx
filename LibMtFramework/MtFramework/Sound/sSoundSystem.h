
#pragma once
#include "MtFramework/System/cSystem.h"

typedef void rSoundSnd;
typedef void rSoundRrd;
typedef void rSoundWed;

// sizeof = 0x200
struct sSoundSystem : public cSystem
{
    inline static rSoundSnd* (__stdcall* _LoadSoundSnd)(sSoundSystem* thisptr, const char* psFileName, int) =
        (rSoundSnd*(__stdcall*)(sSoundSystem*, const char*, int))GetModuleAddress(0x1400B1EF0);

    inline static rSoundRrd* (__stdcall* _LoadSoundRrd)(sSoundSystem* thisptr, const char* psFileName) =
        (rSoundRrd * (__stdcall*)(sSoundSystem*, const char*))GetModuleAddress(0x1400B5830);

    inline static rSoundWed* (__stdcall* _LoadSoundWed)(sSoundSystem* thisptr, const char* psFileName) =
        (rSoundWed * (__stdcall*)(sSoundSystem*, const char*))GetModuleAddress(0x1400B6530);

    IMPLEMENT_SINGLETON(sSoundSystem, 0x141CF3128);


    rSoundSnd* LoadSoundSnd(const char* psFileName, int _notused)
    {
        return _LoadSoundSnd(this, psFileName, _notused);
    }

    rSoundRrd* LoadSoundRrd(const char* psFileName)
    {
        return _LoadSoundRrd(this, psFileName);
    }

    rSoundWed* LoadSoundWed(const char* psFileName)
    {
        return _LoadSoundWed(this, psFileName);
    }
};