/*

*/

#pragma once
#include "MtFile.h"
#include "MtStream.h"

// sizeof = 0x10
struct MtFileStream : public MtStream
{
    /* 0x08 */ MtFile *pFile;

    inline static MtFileStream * (*_ctor)(MtFileStream *thisptr, MtFile *pFile) =
        (MtFileStream*(*)(MtFileStream*, MtFile*))GetModuleAddress(0x14062C7A0);

    inline static MtFileStream * (*_dtor)(MtFileStream *thisptr, bool bFreeMemory) =
        (MtFileStream*(*)(MtFileStream*, bool))GetModuleAddress(0x1400C72B0);

    inline static MtDTI * (*_GetDTI)(MtFileStream *thisptr) =
        (MtDTI*(*)(MtFileStream*))GetModuleAddress(0x14062CC40);

    inline static bool(*_CanRead)(MtFileStream *thisptr) =
        (bool(*)(MtFileStream*))GetModuleAddress(0x14062CCE0);

    inline static bool(*_CanWrite)(MtFileStream *thisptr) =
        (bool(*)(MtFileStream*))GetModuleAddress(0x14062CD70);

    inline static DWORD(*_GetCurrentPosition)(MtFileStream *thisptr) =
        (DWORD(*)(MtFileStream*))GetModuleAddress(0x14062CCA0);

    inline static void(*_Close)(MtFileStream *thisptr) =
        (void(*)(MtFileStream*))GetModuleAddress(0x14062CA20);

    inline static DWORD(*_ReadData)(MtFileStream *thisptr, void *pBuffer, DWORD dwNumberOfBytes) =
        (DWORD(*)(MtFileStream*, void*, DWORD))GetModuleAddress(0x14062CF50);

    inline static void(*_WaitForCompletion)(MtFileStream *thisptr) =
        (void(*)(MtFileStream*))GetModuleAddress(0x14062D120);

    inline static DWORD(*_WriteData)(MtFileStream *thisptr, void *pBuffer, DWORD dwNumberOfBytes) =
        (DWORD(*)(MtFileStream*, void*, DWORD))GetModuleAddress(0x14062D440);

    inline static DWORD(*_GetLength)(MtFileStream *thisptr) =
        (DWORD(*)(MtFileStream*))GetModuleAddress(0x14062CC80);

    inline static void(*_Seek)(MtFileStream *thisptr, DWORD dwOffset, int seekOrigin) =
        (void(*)(MtFileStream*, DWORD, int))GetModuleAddress(0x14062D160);

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
ASSERT_STRUCT_SIZE(MtFileStream, 0x10);