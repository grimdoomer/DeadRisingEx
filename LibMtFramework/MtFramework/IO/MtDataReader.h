/*

*/

#pragma once
#include "MtStream.h"

// sizeof = 0x28?
struct MtDataReader
{
    /* 0x00 */ void         **vtable;
    /* 0x08 */ MtStream     *pStream;
    /* 0x10 */ void         *pScratchBuffer;            // Scratch buffer to store decompressed data
    /* 0x18 */ DWORD        ScratchBufferOffset;        // Current offset in the scratch buffer
    /* 0x1C */ DWORD        ScratchBufferAvailable;     // Number of bytes available in the scratch buffer (number of bytes decompressed into it)
    /* 0x20 */ DWORD        ScratchBufferSize;          // Size of the scratch buffer allocation

    /*
        VTable:
            0x00 ~MtDataReader(bool bFreeMemory)
            0x08 WORD ReadUInt16()
            0x10 DWORD ReadUInt32()
            0x18 QWORD ReadUInt64()
            0x20 WORD ReadUInt16()      // One of these sets should be signed but the vtable pointers are the same...
            0x28 DWORD ReadUInt32()
            0x30 QWORD ReadUInt64()
            0x38 float ReadFloat()
            0x40 double ReadDouble()
    */

    inline static MtDataReader * (__stdcall *_ctor)(MtDataReader *thisptr) =
        GetModuleAddress<MtDataReader*(__stdcall*)(MtDataReader*)>(0x14062C1F0);

    inline static MtDataReader * (__stdcall *_dtor)(MtDataReader *thisptr, bool bFreeMemory) =
        GetModuleAddress<MtDataReader*(__stdcall*)(MtDataReader*, bool)>(0x14062C260);

    inline static WORD(__stdcall *_ReadUInt16)(MtDataReader *thisptr) =
        GetModuleAddress<WORD(__stdcall*)(MtDataReader*)>(0x14062C4A0);

    inline static DWORD(__stdcall *_ReadUInt32)(MtDataReader *thisptr) =
        GetModuleAddress<DWORD(__stdcall*)(MtDataReader*)>(0x14062C5F0);

    inline static ULONGLONG(__stdcall *_ReadUInt64)(MtDataReader *thisptr) =
        GetModuleAddress<ULONGLONG(__stdcall*)(MtDataReader*)>(0x14062C630);

    inline static float(__stdcall *_ReadFloat)(MtDataReader *thisptr) =
        GetModuleAddress<float(__stdcall*)(MtDataReader*)>(0x14062C400);

    inline static double(__stdcall *_ReadDouble)(MtDataReader *thisptr) =
        GetModuleAddress<double(__stdcall*)(MtDataReader*)>(0x14062C450);
};