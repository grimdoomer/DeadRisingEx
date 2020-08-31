/*

*/

#pragma once
#include "MtFramework/MtObject.h"

// sizeof = 0x8
struct MtStream : public MtObject
{
    /*
        VTable:
            0x28 bool CanRead();
            0x30 bool CanWrite();
            0x38 bool
            0x40 bool
            0x48 DWORD GetCurrentPosition();
            0x50 void Close();
            0x58 
            0x60 DWORD ReadData(void *pBuffer, DWORD dwNumberOfBytes);
            0x68 DWORD ReadDataAsync(void *pBuffer, DWORD dwNumberOfBytes); // I think this one is async or something
            0x70 void WaitForCompletion();
            0x78 DWORD WriteData(void *pBuffer, DWORD dwNumberOfBytes);
            0x80
            0x88 DWORD GetLength()
            0x90 void Seek(DWORD dwOffset, int seekOrigin);
    */

    inline static MtStream * (__stdcall* _dtor)(MtStream *thisptr, bool bFreeMemory) =
        GetModuleAddress<MtStream*(__stdcall*)(MtStream*, bool)>(0x1400C72B0);

    inline static MtDTI * (__stdcall *_GetDTI)(MtStream *thisptr) =
        GetModuleAddress<MtDTI*(__stdcall*)(MtStream*)>(0x14062CC70);

    IMPLEMENT_MYDTI(MtStream, 0x141CF2568, 0x1400AF010, 0x14062CEB0);

    /*
        Returns: True if the stream was open for reading, false otherwise.
    */
    bool CanRead()
    {
        return ThisPtrCallNoFixup<bool>(this->vtable[5], this);
    }

    /*
        Returns: True if the stream was open for writing, false otherwise.
    */
    bool CanWrite()
    {
        return ThisPtrCallNoFixup<bool>(this->vtable[6], this);
    }

    //

    //

    /*
        Gets the current position in the stream.
    */
    DWORD GetCurrentPosition()
    {
        return ThisPtrCallNoFixup<DWORD>(this->vtable[9], this);
    }

    /*
        Closes the stream and released any resources held.
    */
    void Close()
    {
        ThisPtrCallNoFixup<void>(this->vtable[10], this);
    }

    //

    /*
        Description: Reads the specified number of bytes from the stream.

        Parameters:
            - pBuffer: Buffer to read into
            - dwNumberOfBytes: Number of bytes to read from the stream

        Returns: The number of bytes read from the stream or 0 if an error occurred.
    */
    DWORD ReadData(void *pBuffer, DWORD dwNumberOfBytes)
    {
        return ThisPtrCallNoFixup<DWORD, void*, DWORD>(this->vtable[12], this, pBuffer, dwNumberOfBytes);
    }

    //

    /*
        Waits for async stream operations to complete.
    */
    void WaitForCompletion()
    {
        ThisPtrCallNoFixup<void>(this->vtable[14], this);
    }

    /*
        Description: Writes the specified buffer to the stream.

        Parameters:
            - pBuffer: Buffer to write
            - dwNumberOfBytes: Number of bytes to write

        Returns: The number of bytes written.
    */
    DWORD WriteData(void *pBuffer, DWORD dwNumberOfBytes)
    {
        return ThisPtrCallNoFixup<DWORD, void*, DWORD>(this->vtable[15], this, pBuffer, dwNumberOfBytes);
    }

    //

    /*
        Gets the length of the stream in bytes.
    */
    DWORD GetLength()
    {
        return ThisPtrCallNoFixup<DWORD>(this->vtable[17], this);
    }

    /*
        Description: Seeks to the specified offset from the specified origin.

        Parameters:
            - dwOffset: Offset to seek to
            - seekOrigin: Origin to seek from (FILE_BEGIN, FILE_CURRENT, or FILE_END)

        Returns: The new stream position
    */
    DWORD Seek(DWORD dwOffset, int seekOrigin)
    {
        return ThisPtrCallNoFixup<DWORD, DWORD, int>(this->vtable[18], this, dwOffset, seekOrigin);
    }
};