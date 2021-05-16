/*

*/

#pragma once
#include <LibMtFramework.h>
#include <MtFramework/MtObject.h>

// sizeof = 0x148
struct MtFile : public MtObject
{
    /*
        VTable:
            0x28 bool OpenFile(const char *psFileName, DWORD flags);
            0x30 void CloseFile();
            0x38 DWORD ReadFile(void *pBuffer, DWORD dwNumberOfBytes);
            0x40 DWORD ReadFileAsync(void *pBuffer, DWORD dwNumberOfBytes);  // This might actually be a read file with buffer function, both seem to be synchronous
            0x48 void WaitForCompletion();
            0x50 DWORD WriteFile(void *pBuffer, DWORD dwNumberOfBytes);
            0x58 DWORD Seek(DWORD dwOffset, int seekOrigin);
            0x60 DWORD GetCurrentPosition();
            0x68 DWORD GetFileSize();
            0x70 DWORD SetFileFile(DWORD dwLength);
            0x78 bool CanRead();
            0x80 bool CanWrite();
            0x88 bool IsAsync();

    */

    /* 0x08 */ HANDLE hFile;
    /* 0x10 */ DWORD Flags;     // See FOF_* flags

    /* 0x1C */

    /* 0x140 */ // void *ScratchBuffer;

    BYTE _[0x134];

    inline static MtFile * (__stdcall *_ctor)(MtFile *thisptr, const char *psFileName, DWORD flags) =
        GetModuleAddress<MtFile*(__stdcall*)(MtFile*, const char*, DWORD)>(0x140618ED0);

    inline static MtFile * (__stdcall *_dtor)(MtFile *thisptr, bool bFreeMemory) =
        GetModuleAddress<MtFile*(__stdcall*)(MtFile*, bool)>(0x140619130);

    inline static MtDTI * (__stdcall *_GetDTI)(MtFile *thisptr) =
        GetModuleAddress<MtDTI*(__stdcall*)(MtFile*)>(0x140619780);

    inline static bool(__stdcall *_OpenFile)(MtFile *thisptr, const char *psFileName, DWORD flags) =
        GetModuleAddress<bool(__stdcall*)(MtFile*, const char*, DWORD)>(0x140619940);

    inline static void(__stdcall *_CloseFile)(MtFile *thisptr) =
        GetModuleAddress<void(__stdcall*)(MtFile*)>(0x140619160);

    inline static DWORD(__stdcall *_ReadFile)(MtFile *thisptr, void *pBuffer, DWORD dwNumberOfBytes) =
        GetModuleAddress<DWORD(__stdcall*)(MtFile*, void*, DWORD)>(0x140619B20);

    inline static DWORD(__stdcall *_ReadFileAsync)(MtFile *thisptr, void *pBuffer, DWORD dwNumberOfBytes) =
        GetModuleAddress<DWORD(__stdcall*)(MtFile*, void*, DWORD)>(0x140619E40);

    inline static void(__stdcall *_WaitForCompletion)(MtFile *thisptr) =
        GetModuleAddress<void(__stdcall*)(MtFile*)>(0x14061A050);

    inline static DWORD(__stdcall *_WriteFile)(MtFile *thisptr, void *pBuffer, DWORD dwNumberOfBytes) =
        GetModuleAddress<DWORD(__stdcall*)(MtFile*, void*, DWORD)>(0x14061A1D0);

    inline static DWORD(__stdcall* _Seek)(MtFile *thisptr, DWORD dwOffset, int seekOrigin) =
        GetModuleAddress<DWORD(__stdcall*)(MtFile*, DWORD, int)>(0x14061A0A0);

    inline static DWORD(__stdcall *_GetCurrentPosition)(MtFile *thisptr) =
        GetModuleAddress<DWORD(__stdcall*)(MtFile*)>(0x1406197F0);

    inline static DWORD(__stdcall *_GetFileSize)(MtFile *thisptr) =
        GetModuleAddress<DWORD(__stdcall*)(MtFile*)>(0x140619860);

    inline static DWORD(__stdcall *_SetFileSize)(MtFile *thisptr, DWORD dwFileSize) =
        GetModuleAddress<DWORD(__stdcall*)(MtFile*, DWORD)>(0x14061A170);

    inline static bool(__stdcall *_CanRead)(MtFile *thisptr) =
        GetModuleAddress<bool(__stdcall*)(MtFile*)>(0x140619840);

    inline static bool(__stdcall *_CanWrite)(MtFile *thisptr) =
        GetModuleAddress<bool(__stdcall*)(MtFile*)>(0x140619850);

    inline static bool(__stdcall *_IsAsync)(MtFile *thisptr) =
        GetModuleAddress<bool(__stdcall*)(MtFile*)>(0x140619810);

    IMPLEMENT_MYDTI(MtFile, 0x141CE5E28, 0x1400AF010, 0x1406198D0);

    // Flags for opening files:
    #define FOF_READ_ONLY       1   // Opens existing file for reading
    #define FOF_WRITE_ONLY      2   // Creates a new file with write access
    #define FOF_READ_WRITE      FOF_READ_ONLY | FOF_WRITE_ONLY   // Creates a new file with read and write access
    #define FOF_ASYNC           4   // File operations are performed asynchronously
    
    /*
        Description: Constructs a new MtFile object and opens the file.

        Parameters:
            - psFileName: Full file path of the file to open
            - flags: Flags for how to open the file, see FOF_* above
    */
    MtFile(const char *psFileName, DWORD flags)
    {
        _ctor(this, psFileName, flags);
    }

    ~MtFile()
    {
        ThisPtrCallNoFixup<void, bool>(this->vtable[0], this, false);
    }

    /*
        Description: Opens the specified file

        Parameters:
            - psFileName: Full file path of the file to open
            - flags: Flags for how to open the file, see FOF_* above

        Returns: True if the file was opened successfully, false otherwise.
    */
    bool OpenFile(const char *psFileName, DWORD flags)
    {
        return ThisPtrCallNoFixup<bool, const char*, DWORD>(this->vtable[5], this, psFileName, flags);
    }

    /*
        Closes the file and releases the file handle.
    */
    void CloseFile()
    {
        ThisPtrCallNoFixup<void>(this->vtable[6], this);
    }

    // ReadFile

    // ReadFileAsync

    void WaitForCompletion()
    {
        ThisPtrCallNoFixup<void>(this->vtable[9], this);
    }

    /*
        Description: Writes the specified buffer to file.

        Parameters:
            - pBuffer: Buffer to write
            - dwNumberOfBytes: Number of bytes to write

        Returns: The number of bytes written to file.
    */
    DWORD WriteFile(void *pBuffer, DWORD dwNumberOfBytes)
    {
        return ThisPtrCallNoFixup<DWORD, void*, DWORD>(this->vtable[10], this, pBuffer, dwNumberOfBytes);
    }

    // Seek origin:
    #define SEEK_BEGIN      FILE_BEGIN + 1
    #define SEEK_CURRENT    FILE_CURRENT + 1
    #define SEEK_END        FILE_END + 1

    /*
        Description: Seeks to the specified offset from the specified origin.

        Parameters:
            - dwOffset: Offset to seek to
            - seekOrigin: Origin to seek from, see SEEK_* above

        Returns: The new file position
    */
    DWORD Seek(DWORD dwOffset, int seekOrigin)
    {
        return ThisPtrCallNoFixup<DWORD, DWORD, int>(this->vtable[11], this, dwOffset, seekOrigin);
    }

    /*
        Gets the current position in the file.
    */
    DWORD GetCurrentPosition()
    {
        return ThisPtrCallNoFixup<DWORD>(this->vtable[12], this);
    }

    /*
        Gets the size of the file.
    */
    DWORD GetFileSize()
    {
        return ThisPtrCallNoFixup<DWORD>(this->vtable[13], this);
    }

    /*
        Description: Sets the size of the file.

        Parameters:
            - dwLength: New file size

        Returns: Nonzero value on success, 0 if the operation failed.
    */
    DWORD SetFileSize(DWORD dwLength)
    {
        return ThisPtrCallNoFixup<DWORD, DWORD>(this->vtable[14], this, dwLength);
    }

    /*
        Returns: True if the file was open for reading, false otherwise.
    */
    bool CanRead()
    {
        return ThisPtrCallNoFixup<bool>(this->vtable[15], this);
    }

    /*
        Returns: True if the file was open for writing, false otherwise.
    */
    bool CanWrite()
    {
        return ThisPtrCallNoFixup<bool>(this->vtable[16], this);
    }

    /*
        Returns: True if file operations are performed asynchronously.
    */
    bool IsAsync()
    {
        return ThisPtrCallNoFixup<bool>(this->vtable[17], this);
    }
};
static_assert(sizeof(MtFile) == 0x148, "MtFile incorrect struct size");