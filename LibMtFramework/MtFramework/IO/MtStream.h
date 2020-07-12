/*

*/

#pragma once
#include "MtFramework/MtObject.h"

struct MtStream : public MtObject
{
	/*
		VTable:
			0x28 Open

			0x48 DWORD GetCurrentPosition();
			0x50 Close

			0x60 DWORD ReadData(void *pBuffer, DWORD dwLength);
			0x68 DWORD ReadDataAsync(void *pBuffer, DWORD dwLength); // I think this one is async or something
			0x70 void WaitForCompletion();

			0x88 DWORD GetLength()
			0x90 void Seek(DWORD dwOffset, int seekOrigin);
	*/
};