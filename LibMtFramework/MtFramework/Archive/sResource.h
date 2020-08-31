/*

*/

#pragma once
#include "LibMtFramework.h"
#include "MtFramework/MtObject.h"
#include "cResource.h"
#include "MtFramework/System/MtThread.h"
#include "rArchive.h"

struct sResource;

// sizeof = 0x24458
struct sResource
{
	// sizeof = 0x20
	struct DecodeFileRequest
	{
		/* 0x00 */ cResource		*pResource;
		/* 0x08 */ DWORD			OffsetFromHeader;	// Offset of the compressed data relative to end of file entries
		/* 0x0C */ DWORD			DecompressedSize;
		/* 0x10 */ DWORD			CompressedSize;
		/* 0x14 */ DWORD			BytesRead;			// Number of bytes read from the archive stream
		/* 0x18 */ DWORD			Result;				// 1 if the operation was successful
	};
	static_assert(sizeof(DecodeFileRequest) == 0x20, "sResource::DecodeFileRequest incorrect struct size");

	// sizeof = 0x44?
	struct DecompressStreamContext
	{
		/* 0x00 */ MtFileStream			*pArchiveStream;
		/* 0x08 */ CRITICAL_SECTION		AsyncDecodeLock;
		/* 0x30 */ void					*pScratchBuffer;
		/* 0x38 */ DWORD				ScratchBufferSize;
		/* 0x3C */ DWORD				CurrentArchiveOffset;			// Current position in the archive for async decoding
		/* 0x40 */ DWORD				AsyncBytesRead;					// Number of bytes read from the archive during async decoding
	};

	// sizeof = 0x78
	struct DecompressStream : public MtStream
	{
		/* 0x08 */ DecompressStreamContext	*pContext;
		/* 0x10 */ DecodeFileRequest	*pDecodeRequest;
		/* 0x18 */ z_stream_s			zStream;
		/* 0x70 */ DWORD				CurrentPosition;	// Current position in the decompressed data
		/* 0x74 */ DWORD				DecompressedSize;	// Basically the length of the stream (in a decompressed state)

        inline static DecompressStream * (__stdcall *_ctor)(DecompressStream *thisptr, DecompressStreamContext *context, DecodeFileRequest *pDecodeReq) =
            GetModuleAddress<DecompressStream*(__stdcall*)(DecompressStream*, DecompressStreamContext*, DecodeFileRequest*)>(0x14063AB60);

        inline static DecompressStream * (__stdcall *_dtor)(DecompressStream *thisptr, bool bFreeMemory) =
            GetModuleAddress<DecompressStream*(__stdcall*)(DecompressStream*, bool)>(0x14063B2C0);

        /*
            Parameters:
                - context: Decompression context to read compressed data from
                - pDecodeReq: Decode request for the file
        */
		DecompressStream(DecompressStreamContext *context, DecodeFileRequest *pDecodeReq)
		{
			_ctor(this, context, pDecodeReq);
		}

		~DecompressStream()
		{
            ThisPtrCallNoFixup<void, bool>(this->vtable[0], this, false);
		}
	};
	static_assert(sizeof(DecompressStream) == 0x78, "sResource::DecompressStream incorrect struct size");

	// sizeof = 0x20
	struct TypeInfo : public MtObject
	{
		/* 0x08 */ __int64	mAttr;		// I think this is actually a DWORD + padding
		/* 0x10 */ char		mExt[4];	// File extension
		/* 0x18 */ MtDTI	*pTypeInfo;
	};
	static_assert(sizeof(TypeInfo) == 0x20, "sResource::TypeInfo incorrect struct size");


	/* 0x00 */ void					**vtable;
	/* 0x08 */ CRITICAL_SECTION		ListLock;
	/* 0x30 */ bool					LockOnAccess;		// Determines if the lock is held on read operations?
	/* 0x38 */ TypeInfo				mTypeInfo[256]; // pointer or not?
	/* 0x2038 */ DWORD				TypeInfoCount;
	/* 0x203C */ char				RootPath[MAX_PATH];		// GameDir + "\\resource"
	/* 0x2140 */ char				NativePath[MAX_PATH];	// GameDir + "\\nativeWin64"
	/* 0x2244 */ // padding?
	/* 0x2248 */ cResource			*pResourceEntries[8192];
	/* 0x12248 */ void**			ppFunctionTableThing;	// Function pointer array for something, have not seen it used yet...
	/* 0x12250 */ char*				mPCName;			// Remote PC name for arc loading
	/* 0x12258 */ char*				mPCFolderName;		// Remote folder path for arc loading
	/* 0x12260 */ bool				EnableRemote;
	/* 0x12261 */ bool				mOptimizeEnable;
	/* 0x12262 */ bool				mBuildComplete;
	/* 0x12263 */ bool				mForceHDDCache;
	// padding?
	/* 0x12268 */ MtThread			*pLoaderThread;		// Thread worker for loading resource asynchronously
	/* 0x12270 */ DWORD				Unknown2;
	/* 0x12274 */ DWORD				mAsyncPriority;
	/* 0x12278 */ bool				WorkerThreadsShouldExit;	// Flags the loader and decoder threads to exit
	/* 0x12279 */ bool				mAutoUpdate;
	/* 0x1227A */ bool				LoadFromHDDCache;			// True if files should be loaded from cache instead of archives
	// padding?
	/* 0x12280 */ cResource			*AsyncLoadQueue[1024];		// Queue of files to be loaded asynchronously
	/* 0x14280 */ DWORD				AsycLoadQueueCount;			// Number of files in the queue
	/* 0x14284 */ char				OpenArchiveFilePath[MAX_PATH];		// File path of the archive that is currently open for decoding
	/* 0x14388 */ //DecompressStreamContext		DecompressContext;
		/* 0x14388 */ MtFileStream		*pArchiveStream;
		/* 0x14390 */ CRITICAL_SECTION	AsyncDecodeLock;
		/* 0x143B8 */ void				*pScratchBuffer;
		/* 0x143C0 */ DWORD				ScratchBufferSize;
		/* 0x143C4 */ DWORD				CurrentArchiveOffset;			// Current position in the archive for async decoding
		/* 0x143C8 */ DWORD				AsyncBytesRead;					// Number of bytes read from the archive during async decoding
	/* 0x143CC */ DWORD				AsyncDecodeCount;				// Number decode requests submitted for this archive
	/* 0x143D0 */ DWORD				AsyncLoadCount;					// Number of resources loaded (pResource->LoadResource()) for this archive
	/* 0x143D4 */ DWORD				AsyncDecodeRequestsProcessed;	// Number of async decoding requests processed?
	/* 0x143D8 */ DWORD				Unknown1;						// Some sort of completion signal, AsyncDecodeLock must be held while accessing it
	// padding?
	/* 0x143E0 */ DecodeFileRequest	AsyncDecodeQueue[2048];		// array element size = 0x20, count = 2048
	/* 0x243E0 */ void				*_pUnused;					// Doesn't seem to be used
	/* 0x243E8 */ MtThread			*pDecoderThreads[6];		// Performs zlib decompression for arc files (only 4 threads are used in game)
	/* 0x24418 */ HANDLE			DecoderWorkAvailableEvent[4];	// Events signaled when there is work for a decoder thread to do
	/* 0x24438 */ HANDLE			DecoderWorkCompletedEvent[4];	// Events signaled when the decoding work for the thread is done


    // sResource singleton instance:
    inline static sResource **_Instance = GetModuleAddress<sResource**>(0x141CF27F8);

	inline static sResource * (__stdcall *_ctor)(sResource *thisptr) = 
        GetModuleAddress<sResource*(__stdcall*)(sResource*)>(0x14063AC10);

    inline static ULONGLONG(__stdcall *_CalculateResourceId)(sResource *thisptr, MtDTI *pObjectType, char *psFileName) =
        GetModuleAddress<ULONGLONG(__stdcall*)(sResource*, MtDTI*, char*)>(0x14063DCB0);

    inline static void(__stdcall *_EmplaceResource)(sResource *thisptr, cResource *pResource, int bitIndex) =
        GetModuleAddress<void(__stdcall*)(sResource*, cResource*, int)>(0x14063E240);

    inline static cResource * (__stdcall *_FindResourceById)(sResource *thisptr, ULONGLONG resourceId) =
        GetModuleAddress<cResource*(__stdcall*)(sResource*, ULONGLONG)>(0x14063BDB0);

	inline static cResource * (__stdcall *_LoadResourceFromArchive)(sResource *thisptr, rArchive::DecompressStream *pStream, MtDTI *pDTI, rArchiveFileEntry *pFileEntry) =
		GetModuleAddress<cResource*(__stdcall*)(sResource*, rArchive::DecompressStream*, MtDTI*, rArchiveFileEntry*)>(0x14063B9E0);

	inline static void * (__stdcall *_LoadGameResource)(sResource *thisptr, MtDTI *pObjectType, char *psFileName, DWORD flags) =
		GetModuleAddress<void*(__stdcall*)(sResource*, MtDTI*, char*, DWORD)>(0x14063BC60);

    inline static cResource * (__stdcall * _LoadGameResourceSynchronous)(sResource *thisptr, MtDTI *pObjectType, char *psFileName, ULONGLONG resourceId, DWORD flags) =
        GetModuleAddress<cResource*(__stdcall*)(sResource*, MtDTI*, char*, ULONGLONG, DWORD)>(0x14063D520);

	inline static void(__stdcall *_ResourceDecoderProc)(int threadIndex) = 
        GetModuleAddress<void(__stdcall*)(int)>(0x14063C9B0);


    /*
        Description: Gets the sResource singleton instance
    */
    inline static sResource * Instance()
    {
        return *sResource::_Instance;
    }

    /*
        Description: sResource constructor
    */
    sResource()
    {
        _ctor(this);
    }

    // Resource load flags:
    #define RLF_SYNCHRONOUS		    1	    // File is loaded synchronously
    #define RLF_ASYNC			    2	    // A load file request is placed into the AsyncLoadQueue queue, file is loaded asynchronously
    #define RLF_HDD_CACHE		    4	    // Load from flat file, do not use archives
    #define RLF_LOAD_AS_ARCHIVE     0x10    // Converts file path into archive file name for loading, e.x.: arc\rom\om\om0001\om0001 -> arcromomom0001om0001

    /*
        Description: Loads and decompresses the specified resource and emplaces it into the pResourceEntries table. Loading
            can happen synchronously or asynchronously depending on the flags specified.

        Parameters:
            - pObjectType: Pointer to the MtDTI type info for the file type
            - psFileName: File path for the file to be loaded
            - flags: Loading flags, see RLF_* above

        Returns: A pointer to the object instance if loading was successful, or nullptr if loading failed
    */
	template<typename T> T* LoadGameResource(MtDTI *pObjectType, char *psFileName, DWORD flags)
	{
		return (T*)_LoadGameResource(this, pObjectType, psFileName, flags);
	}

    /*
        Description: Loads the specified resource synchronously

        Parameters:
            - pObjectType: Pointer to the MtDTI type info for the file type
            - psFileName: File path for the file to be loaded
            - resourceId: Resource id for the file being loaded
            - flags: Loading flags, see RLF_* above

        Returns: A pointer to the object instance if loading was successful, or nullptr if loading failed
    */
    template<typename T> T* LoadGameResourceSynchronous(MtDTI *pObjectType, char *psFileName, ULONGLONG resourceId, DWORD flags)
    {
        return (T*)_LoadGameResourceSynchronous(this, pObjectType, psFileName, resourceId, flags);
    }

    /*
        Description: Calculates the resource if for a file

        Parameters:
            - pObjectType: Pointer to the MtDTI type info for the file type
            - psFileName: File path for the file

        Returns: Resource id for the specified resource
    */
	ULONGLONG CalculateResourceId(MtDTI *pObjectType, char *psFileName)
	{
        return _CalculateResourceId(this, pObjectType, psFileName);
	}

    /*
        Description: Adds the specified resource to the pResourceEntries list at the next available index.

        Parameters:
            - pResource: Resource instance to add
            - bitIndex: Starting bit index for resource id collision algorithm
    */
    void EmplaceResource(cResource *pResource, int bitIndex = 0)
    {
        _EmplaceResource(this, pResource, bitIndex);
    }

    /*
        Description: Searches for a loaded resource with the specified resource id.

        Parameters:
            - resourceId: unique resource id to search for

        Returns: The resource instance if it was found, or nullptr otherwise. If the resource was found
            the reference count is incremented before returning.
    */
    template<typename T> T* FindResourceById(ULONGLONG resourceId)
    {
        return (T*)_FindResourceById(this, resourceId);
    }
};
static_assert(sizeof(sResource) == 0x24458, "sResource incorrect struct size");