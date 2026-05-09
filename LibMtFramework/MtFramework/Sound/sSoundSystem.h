
#pragma once
#include "MtFramework/System/cSystem.h"

typedef void rSoundSnd;
typedef void rSoundRrd;
typedef void rSoundWed;

// sizeof = 0x200
struct sSoundSystem : public cSystem
{
    inline static rSoundSnd* (* _LoadSoundSnd)(sSoundSystem* thisptr, const char* psFileName, int) =
        (rSoundSnd*(*)(sSoundSystem*, const char*, int))GetModuleAddress(0x1400B1EF0);

    inline static rSoundRrd* (* _LoadSoundRrd)(sSoundSystem* thisptr, const char* psFileName) =
        (rSoundRrd * (*)(sSoundSystem*, const char*))GetModuleAddress(0x1400B5830);

    inline static rSoundWed* (* _LoadSoundWed)(sSoundSystem* thisptr, const char* psFileName) =
        (rSoundWed * (*)(sSoundSystem*, const char*))GetModuleAddress(0x1400B6530);

    IMPLEMENT_SINGLETON(sSoundSystem, 0x141CF3128);

    sSoundSystem()
    {
        // TODO:
        DebugBreak();
    }

    ~sSoundSystem()
    {
        // TODO:
        DebugBreak();
    }


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