
#include "uNpcMarkerImpl.h"
#include <MtFramework/Object/Npc/uNpcMarker.h>
#include <detours.h>

// Forward declarations:
void __stdcall Hook_uNpcMarker_Cleanup(uNpcMarker *thisptr);

// Last uNpcMarker instance to be cleaned up.
uNpcMarker *pLastNpcMarkerCleaned = nullptr;

bool uNpcMarkerImpl::InstallHooks()
{
    DetourAttach((void**)&uNpcMarker::_Cleanup, Hook_uNpcMarker_Cleanup);

    return true;
}

void __stdcall Hook_uNpcMarker_Cleanup(uNpcMarker *thisptr)
{
    // Check if this npc marker was already cleaned up.
    if (thisptr == pLastNpcMarkerCleaned)
        return;

    // Cleanup the npc marker.
    uNpcMarker::_Cleanup(thisptr);
    pLastNpcMarkerCleaned = thisptr;
}