/*

*/

#pragma once
#include "MtFramework/Archive/cResource.h"
#include "MtFramework/IO/MtFileStream.h"
#include "MtFramework/Utils/Zlib.h"

// sizeof = 8
struct rArchiveHeader
{
#define ARC_HEADER_MAGIC        0x00435241
#define ARC_HEADER_VERSION        4

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
    /* 0x60 */ cResource    **pResources;           // Array of resource instances loaded from the archive
    /* 0x68 */ DWORD        ResourceNum;            // Number of resources in the pResources array
    /* 0x6C */ DWORD        DecompressedSize;        // Size of all decompressed files
    /* 0x70 */

    /*
        Performs ZLib decompression on the data read from the underlying file stream. Used for reading compressed
        data from a rArchive file.
    */
    struct DecompressStream : public MtStream // sizeof = 0xA0
    {
        /* 0x08 */ MtFileStream *pFileStream;
        /* 0x10 */ BYTE _[0x38];
        /* 0x48 */ z_stream_s zStream;

        inline static DecompressStream * (__stdcall *_ctor)(DecompressStream *thisptr, MtFileStream *pFileStream) =
            GetModuleAddress<DecompressStream*(__stdcall*)(DecompressStream*, MtFileStream*)>(0x14064FD90);

        inline static DecompressStream * (__stdcall *_dtor)(DecompressStream *thisptr, bool bFreeMemory) =
            GetModuleAddress<DecompressStream*(__stdcall*)(DecompressStream*, bool)>(0x14064FE90);

        /*
            Description: Creates a new decompression stream from the file stream specified.

            Parameters:
                - pFileStream: File stream to read from when performing decompression operations.
        */
        DecompressStream(MtFileStream *pFileStream)
        {
            _ctor(this, pFileStream);
        }

        ~DecompressStream()
        {
            ThisPtrCallNoFixup<void, bool>(this->vtable[0], this, false);
        }
    };
    static_assert(sizeof(DecompressStream) == 0xA0, "rArchive::DecompressStream incorrect struct size");
};