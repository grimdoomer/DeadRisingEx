/*

*/

#pragma once
#include "MtStream.h"

// sizeof = 0x28?
struct MtDataReader
{
	/* 0x00 */ void			**vtable;
	/* 0x08 */ MtStream		*pStream;
	/* 0x10 */ void			*pScratchBuffer;		// Scratch buffer to store decompressed data
	/* 0x18 */ DWORD		ScratchBufferOffset;	// Current offset in the scratch buffer
	/* 0x1C */ DWORD		ScratchBufferAvailable;	// Number of bytes available in the scratch buffer (number of bytes decompressed into it)
	/* 0x20 */ DWORD		ScratchBufferSize;		// Size of the scratch buffer allocation
};