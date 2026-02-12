/*

*/

#pragma once
#include "MtFile.h"
#include "MtStream.h"

// sizeof = 0x10
struct MtFileStream : public MtStream
{
    /* 0x08 */ MtFile *pFile;

    inline static MtFileStream * (__stdcall *_ctor)(MtFileStream *thisptr, MtFile *pFile) =
        (MtFileStream*(__stdcall*)(MtFileStream*, MtFile*))GetModuleAddress(0x14062C7A0);

    inline static MtFileStream * (__stdcall *_dtor)(MtFileStream *thisptr, bool bFreeMemory) =
        (MtFileStream*(__stdcall*)(MtFileStream*, bool))GetModuleAddress(0x1400C72B0);

    inline static MtDTI * (__stdcall *_GetDTI)(MtFileStream *thisptr) =
        (MtDTI*(__stdcall*)(MtFileStream*))GetModuleAddress(0x14062CC40);

    inline static bool(__stdcall *_CanRead)(MtFileStream *thisptr) =
        (bool(__stdcall*)(MtFileStream*))GetModuleAddress(0x14062CCE0);

    inline static bool(__stdcall *_CanWrite)(MtFileStream *thisptr) =
        (bool(__stdcall*)(MtFileStream*))GetModuleAddress(0x14062CD70);

    inline static DWORD(__stdcall *_GetCurrentPosition)(MtFileStream *thisptr) =
        (DWORD(__stdcall*)(MtFileStream*))GetModuleAddress(0x14062CCA0);

    inline static void(__stdcall *_Close)(MtFileStream *thisptr) =
        (void(__stdcall*)(MtFileStream*))GetModuleAddress(0x14062CA20);

    inline static DWORD(__stdcall *_ReadData)(MtFileStream *thisptr, void *pBuffer, DWORD dwNumberOfBytes) =
        (DWORD(__stdcall*)(MtFileStream*, void*, DWORD))GetModuleAddress(0x14062CF50);

    inline static void(__stdcall *_WaitForCompletion)(MtFileStream *thisptr) =
        (void(__stdcall*)(MtFileStream*))GetModuleAddress(0x14062D120);

    inline static DWORD(__stdcall *_WriteData)(MtFileStream *thisptr, void *pBuffer, DWORD dwNumberOfBytes) =
        (DWORD(__stdcall*)(MtFileStream*, void*, DWORD))GetModuleAddress(0x14062D440);

    inline static DWORD(__stdcall *_GetLength)(MtFileStream *thisptr) =
        (DWORD(__stdcall*)(MtFileStream*))GetModuleAddress(0x14062CC80);

    inline static void(__stdcall *_Seek)(MtFileStream *thisptr, DWORD dwOffset, int seekOrigin) =
        (void(__stdcall*)(MtFileStream*, DWORD, int))GetModuleAddress(0x14062D160);

    IMPLEMENT_MYDTI(MtFileStream, 0x141CF2508, 0x1400AF010, 0x14062CDE0);

    /*
        Description: Constructs a new MtFileStream object using the MtFile instance specified.

        Parameters:
            - pFile: Underlying file to read/write
    */
    MtFileStream(MtFile *pFile)
    {
        _ctor(this, pFile);
    }
};
static_assert(sizeof(MtFileStream) == 0x10, "MtFileStream incorrect struct size");