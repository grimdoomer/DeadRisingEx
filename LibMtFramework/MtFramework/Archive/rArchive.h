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
ASSERT_STRUCT_SIZE(rArchiveHeader, 0x8);

// sizeof = 0x50
struct rArchiveFileEntry
{
    /* 0x00 */ char FileName[64];
    /* 0x40 */ DWORD FileType;
    /* 0x44 */ DWORD CompressedSize;
    /* 0x48 */ DWORD DecompressedSize;
    /* 0x4C */ DWORD DataOffset;
};
ASSERT_STRUCT_SIZE(rArchiveFileEntry, 0x50);

// sizeof = 0x78
struct rArchive : public cResource
{
    /* 0x60 */ cResource    **pResources;           // Array of resource instances loaded from the archive
    /* 0x68 */ DWORD        ResourceNum;            // Number of resources in the pResources array
    /* 0x6C */ DWORD        DecompressedSize;       // Size of all decompressed files
    /* 0x70 */ DWORD        _unk;

    IMPLEMENT_MYDTI(rArchive, 0x141CF30F8, 0x1400AF010, 0x140650980);

    /*
        Performs ZLib decompression on the data read from the underlying file stream. Used for reading compressed
        data from a rArchive file.
    */
    struct DecompressStream : public MtStream
    {
        /* 0x08 */ MtFileStream *pFileStream;
        /* 0x10 */ BYTE _[0x8];
        /* 0x18 */ DWORD CompressedSize;
        /* 0x1C */ DWORD DecompressedSize;
        /* 0x20 */ BYTE __[0x20];
        /* 0x40 */ DWORD DataOffset;
        /* 0x44 */ BYTE ___[4];
        /* 0x48 */ z_stream_s zStream;

        inline static DecompressStream * (*_ctor)(DecompressStream *thisptr, MtFileStream *pFileStream) =
            (DecompressStream*(*)(DecompressStream*, MtFileStream*))GetModuleAddress(0x14064FD90);

        inline static void * (*_scalar_deleting_dtor)(DecompressStream *thisptr, unsigned int flags) =
            (void*(*)(DecompressStream*, unsigned int))GetModuleAddress(0x14064FE90);

        /*
            Description: Creates a new decompression stream from the file stream specified.

            Parameters:
                - pFileStream: File stream to read from when performing decompression operations.
        */
        SHIM_API DecompressStream(MtFileStream *pFileStream) SHIM_BODY(0x14064FD90)

        SHIM_API ~DecompressStream() SHIM_BODY_DTOR_VCALL()

        IMPLEMENT_OPERATOR_NEW_DELETE(g_pResourceHeapAllocator2, 16)
    };
    ASSERT_STRUCT_SIZE(DecompressStream, 0xA0);


    inline static void** _vtable = (void**)GetModuleAddress(0x14103EA50);

    inline static void* (*_scalar_deleting_dtor)(rArchive* thisptr, unsigned int flags) =
        (void * (*)(rArchive*, unsigned int))GetModuleAddress(0x14064FF10);

    rArchive()
    {
        // rArchive doesn't have a non-inlined default constructor so implement it ourself.
        this->vtable = rArchive::_vtable;
        this->mAttr = 0x10;
        this->pResources = nullptr;
        this->ResourceNum = 0;
        this->DecompressedSize = 0;
        this->_unk = 0;
    }

    SHIM_API ~rArchive() SHIM_BODY_DTOR_VCALL()

    IMPLEMENT_OPERATOR_NEW_DELETE(g_pResourceHeapAllocator, 16)
};
ASSERT_STRUCT_SIZE(rArchive, 0x78);