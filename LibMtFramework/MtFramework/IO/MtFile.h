/*

*/

#pragma once
#include <Windows.h>
#include "Misc/AsmHelpers.h"

// sizeof = 0x148
struct MtFile
{
	/*
		VTable:
			0x00 ~dtor();

			0x38 DWORD ReadData(void *pBuffer, DWORD dwSize);
			0x40 DWORD ReadDataAsync(void *pBuffer, DWORD dwSize);
			0x48 void WaitForCompletion();

			0x58 void Seek(DWORD dwOffset, int seekOrigin);
			0x60 DWORD GetCurrentPosition();
			0x68 DWORD GetFileSize();

	*/

	/* 0x00 */ void **vtable;
	/* 0x08 */ HANDLE hFile;

	/* 0x1C */

	BYTE _[0x138];

	static MtFile * ctor(MtFile *thisptr, const char *psFileName, DWORD flags)
	{
		return (MtFile*)ThisPtrCall((void*)0x140618ED0, thisptr, (void*)psFileName, (void*)flags);
	}

	void dtor(bool bFreeMemory = false)
	{
		ThisPtrCallNoFixup(this->vtable[0], this, (void*)bFreeMemory);
	}

	static DWORD GetFileSize(MtFile *thisptr)
	{
		return (DWORD)ThisPtrCall((void*)0x140619860, thisptr);
	}
};
static_assert(sizeof(MtFile) == 0x148, "MtFile incorrect struct size");