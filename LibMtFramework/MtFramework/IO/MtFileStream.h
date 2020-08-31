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
        GetModuleAddress<MtFileStream*(__stdcall*)(MtFileStream*, MtFile*)>(0x14062C7A0);

    inline static MtFileStream * (__stdcall *_dtor)(MtFileStream *thisptr, bool bFreeMemory) =
        GetModuleAddress<MtFileStream*(__stdcall*)(MtFileStream*, bool)>(0x1400C72B0);

    inline static MtDTI * (__stdcall *_GetDTI)(MtFileStream *thisptr) =
        GetModuleAddress<MtDTI*(__stdcall*)(MtFileStream*)>(0x14062CC40);

    inline static bool(__stdcall *_CanRead)(MtFileStream *thisptr) =
        GetModuleAddress<bool(__stdcall*)(MtFileStream*)>(0x14062CCE0);

    inline static bool(__stdcall *_CanWrite)(MtFileStream *thisptr) =
        GetModuleAddress<bool(__stdcall*)(MtFileStream*)>(0x14062CD70);

    inline static DWORD(__stdcall *_GetCurrentPosition)(MtFileStream *thisptr) =
        GetModuleAddress<DWORD(__stdcall*)(MtFileStream*)>(0x14062CCA0);

    inline static void(__stdcall *_Close)(MtFileStream *thisptr) =
        GetModuleAddress<void(__stdcall*)(MtFileStream*)>(0x14062CA20);

    inline static DWORD(__stdcall *_ReadData)(MtFileStream *thisptr, void *pBuffer, DWORD dwNumberOfBytes) =
        GetModuleAddress<DWORD(__stdcall*)(MtFileStream*, void*, DWORD)>(0x14062CF50);

    inline static void(__stdcall *_WaitForCompletion)(MtFileStream *thisptr) =
        GetModuleAddress<void(__stdcall*)(MtFileStream*)>(0x14062D120);

    inline static DWORD(__stdcall *_WriteData)(MtFileStream *thisptr, void *pBuffer, DWORD dwNumberOfBytes) =
        GetModuleAddress<DWORD(__stdcall*)(MtFileStream*, void*, DWORD)>(0x14062D440);

    inline static DWORD(__stdcall *_GetLength)(MtFileStream *thisptr) =
        GetModuleAddress<DWORD(__stdcall*)(MtFileStream*)>(0x14062CC80);

    inline static void(__stdcall *_Seek)(MtFileStream *thisptr, DWORD dwOffset, int seekOrigin) =
        GetModuleAddress<void(__stdcall*)(MtFileStream*, DWORD, int)>(0x14062D160);

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

	~MtFileStream()
	{
		ThisPtrCallNoFixup<void, bool>(this->vtable[0], this, false);
	}
};
static_assert(sizeof(MtFileStream) == 0x10, "MtFileStream incorrect struct size");