/*

*/

#pragma once
#include "MtFramework/Archive/cResource.h"
#include "MtFramework/IO/MtFileStream.h"
#include "MtFramework/Utils/Zlib.h"

// sizeof = 8
struct rArchiveHeader
{
#define ARC_HEADER_MAGIC		0x00435241
#define ARC_HEADER_VERSION		4

	/* 0x00 */ DWORD Magic;
	/* 0x04 */ WORD Version;
	/* 0x06 */ WORD NumberOfFiles;
};
static_assert(sizeof(rArchiveHeader) == 0x8, "rArchiveHeader incorrect struct size");

// sizeof = 0x50
struct rArchiveFileEntry
{
	/* 0x00 */ char FileName[64];
	/* 0x40 */ DWORD FileType;
	/* 0x44 */ DWORD CompressedSize;
	/* 0x48 */ DWORD DecompressedSize;
	/* 0x4C */ DWORD DataOffset;
};
static_assert(sizeof(rArchiveFileEntry) == 0x50, "rArchiveFileEntry incorrect struct size");

// sizeof = 0x78
struct rArchive : public cResource
{
	/* 0x60 */ cResource	**pResources;
	/* 0x68 */ DWORD		ResourceNum;
	/* 0x6C */ DWORD		DecompressedSize;		// Size of all decompressed files
	/* 0x70 */

	// sizeof = 0xA0
	struct DecompressStream
	{
		/* 0x00 */ void **vtable;
		/* 0x08 */ MtFileStream *pFileStream;
		/* 0x10 */ BYTE _[0x38];
		/* 0x48 */ z_stream_s zStream;

		static DecompressStream * ctor(DecompressStream *thisptr, MtFileStream *pFileStream)
		{
			return (DecompressStream*)ThisPtrCall((void*)0x14064FD90, thisptr, pFileStream);
		}

		void dtor(bool bFreeMemory = false)
		{
			ThisPtrCallNoFixup(this->vtable[0], this, (void*)bFreeMemory);
		}
	};
	static_assert(sizeof(DecompressStream) == 0xA0, "rArchive::DecompressStream incorrect struct size");
};