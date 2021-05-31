
#include "uPhotoImpl.h"
#include "MtFramework/Utils/Utilities.h"

bool uPhotoImpl::InstallHooks()
{
    // Patch the uPhoto::dtor to not set moveline flags for the uMotionBlurFilter instance as
    // it is a use after free and causes crashes (or memory corruption). Because the uMotionBlurFilter
    // instance is added to a move line in sUnit it will get cleaned up accordingly there.
    BYTE Nops[7] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
    PatchBytes((void*)GetModuleAddress(0x140125852), Nops, sizeof(Nops));

    return true;
}