/*

*/

#pragma once
#include "MtFile.h"

// sizeof = 0x10
struct MtFileStream : public MtStream
{
	/* 0x08 */ MtFile *pFile;

	static MtFileStream * ctor(MtFileStream *thisptr, MtFile *pFile)
	{
		return (MtFileStream*)ThisPtrCall((void*)0x14062C7A0, thisptr, pFile);
	}

	void dtor(bool bFreeMemory = false)
	{
		ThisPtrCallNoFixup(this->vtable[0], this, (void*)bFreeMemory);
	}

	bool Open()
	{
		return (bool)ThisPtrCallNoFixup(this->vtable[5], this);
	}

	DWORD GetCurrentPosition()
	{
		return (DWORD)ThisPtrCallNoFixup(this->vtable[9], this);
	}

	void Close()
	{
		ThisPtrCallNoFixup(this->vtable[10], this);
	}

	DWORD ReadData(void *pBuffer, DWORD dwSize)
	{
		return (DWORD)ThisPtrCallNoFixup(this->vtable[12], this, pBuffer, (void*)dwSize);
	}

	DWORD ReadDataAsync(void *pBuffer, DWORD dwSize)
	{
		return (DWORD)ThisPtrCallNoFixup(this->vtable[13], this, pBuffer, (void*)dwSize);
	}

	void WaitForCompletion()
	{
		ThisPtrCallNoFixup(this->vtable[14], this);
	}

	void Seek(DWORD dwOffset, int seekOrigin)
	{
		ThisPtrCallNoFixup(this->vtable[18], this, (void*)dwOffset, (void*)seekOrigin);
	}
};
static_assert(sizeof(MtFileStream) == 0x10, "MtFileStream incorrect struct size");