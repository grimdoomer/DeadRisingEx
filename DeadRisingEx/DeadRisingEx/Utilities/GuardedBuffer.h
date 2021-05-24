
#pragma once
#include <Windows.h>
#include <stdio.h>

class GuardedBuffer
{
public:
    template<typename T> static T * AllocateGuardedBuffer(size_t size)
    {
        // Round the allocation size up to the next page size.
        size_t newSize = size + 4096 - (size % 4096);

        // Allocate a buffer that is 2 pages larger than the requested size.
        BYTE *pbBuffer = (BYTE*)VirtualAlloc(NULL, newSize + (2 * 4096), MEM_COMMIT, PAGE_READWRITE);
        if (pbBuffer == nullptr)
        {
            // Failed to allocate memory for buffer.
            ImGuiConsole::Instance()->ConsolePrint(L"AllocateGuardedBuffer(): failed to allocate buffer of size %d!\n", size);
            return (T*)nullptr;
        }

        // Mark the guard pages as read-only to catch memory corruption around the allocation.
        DWORD dwOldProtect = 0;
        VirtualProtect(pbBuffer, 4096, PAGE_READONLY, &dwOldProtect);
        VirtualProtect(pbBuffer + 4096 + newSize, 4096, PAGE_READONLY, &dwOldProtect);

        // Zero init the allocation.
        memset(pbBuffer + 4096, 0, size);

        // Print the allocation info so we can set break points in the debugger.
        ImGuiConsole::Instance()->ConsolePrint(L"Guarded allocation info:\n");
        ImGuiConsole::Instance()->ConsolePrint(L"\tGuard address1: %p\n", pbBuffer);
        ImGuiConsole::Instance()->ConsolePrint(L"\tData address: %p\n", pbBuffer + 4096);
        ImGuiConsole::Instance()->ConsolePrint(L"\tGuard address2: %p\n", pbBuffer + 4096 + newSize);

        // Return the data address for the allocation.
        return (T*)(pbBuffer + 4096);
    }

    bool CheckGuardedAllocation(void *pAddress, size_t size)
    {
        return false;
    }
};
