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

    inline static MtDataReader * (__stdcall *_ctor)(MtDataReader *thisptr, MtStream* pStream, DWORD scratchBufferSize) =
        (MtDataReader*(__stdcall*)(MtDataReader*, MtStream*, DWORD))GetModuleAddress(0x14062C1F0);

    inline static MtDataReader * (__stdcall *_dtor)(MtDataReader *thisptr, bool bFreeMemory) =
        (MtDataReader*(__stdcall*)(MtDataReader*, bool))GetModuleAddress(0x14062C260);

    inline static WORD(__stdcall *_ReadUInt16)(MtDataReader *thisptr) =
        (WORD(__stdcall*)(MtDataReader*))GetModuleAddress(0x14062C4A0);

    inline static DWORD(__stdcall *_ReadUInt32)(MtDataReader *thisptr) =
        (DWORD(__stdcall*)(MtDataReader*))GetModuleAddress(0x14062C5F0);

    inline static ULONGLONG(__stdcall *_ReadUInt64)(MtDataReader *thisptr) =
        (ULONGLONG(__stdcall*)(MtDataReader*))GetModuleAddress(0x14062C630);

    inline static float(__stdcall *_ReadFloat)(MtDataReader *thisptr) =
        (float(__stdcall*)(MtDataReader*))GetModuleAddress(0x14062C400);

    inline static double(__stdcall *_ReadDouble)(MtDataReader *thisptr) =
        (double(__stdcall*)(MtDataReader*))GetModuleAddress(0x14062C450);

    inline static DWORD (__stdcall* _ReadData)(MtDataReader* thisptr, void* pBuffer, DWORD length) =
        (DWORD(__stdcall*)(MtDataReader*, void*, DWORD))GetModuleAddress(0x14062C330);


    MtDataReader(MtStream* pStream, DWORD scratchBufferSize)
    {
        _ctor(this, pStream, scratchBufferSize);
    }

    ~MtDataReader()
    {
        (void)ThisPtrCallNoFixup(this->vtable[0], this, false);
    }

    WORD ReadUInt16()
    {
        // TODO: use vtable pointer when we know which ones are signed/unsigned...
        return _ReadUInt16(this);
    }

    signed short ReadInt16()
    {
        // TODO: use vtable pointer when we know which ones are signed/unsigned...
        return (signed short)_ReadUInt16(this);
    }

    DWORD ReadUInt32()
    {
        // TODO: use vtable pointer when we know which ones are signed/unsigned...
        return _ReadUInt32(this);
    }

    signed int ReadInt32()
    {
        // TODO: use vtable pointer when we know which ones are signed/unsigned...
        return (signed int)_ReadUInt32(this);
    }

    ULONGLONG ReadUInt64()
    {
        // TODO: use vtable pointer when we know which ones are signed/unsigned...
        return _ReadUInt64(this);
    }

    signed long long ReadInt64()
    {
        // TODO: use vtable pointer when we know which ones are signed/unsigned...
        return (signed long long)_ReadUInt64(this);
    }

    float ReadFloat()
    {
        return (float)ThisPtrCallNoFixup(this->vtable[7], this);
    }

    double ReadDouble()
    {
        return (double)ThisPtrCallNoFixup(this->vtable[8], this);
    }

    DWORD ReadData(void* pBuffer, DWORD length)
    {
        return _ReadData(this, pBuffer, length);
    }
};