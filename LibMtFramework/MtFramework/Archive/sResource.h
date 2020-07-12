/*

*/

#pragma once
#include "DRDebugger.h"
#include "MtFramework/MtObject.h"
#include "cResource.h"
#include "MtFramework/System/MtThread.h"
#include "rArchive.h"

struct sResource;

extern sResource **g_sResourceInstance;

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

		static DecompressStream * ctor(DecompressStream *thisptr, DecompressStreamContext *context, DecodeFileRequest *pDecodeReq)
		{
			return (DecompressStream*)ThisPtrCall((void*)0x14063AB60, thisptr, context, pDecodeReq);
		}

		void dtor(bool bFreeMemory = false)
		{
			ThisPtrCallNoFixup(this->vtable[0], this, (void*)bFreeMemory);
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


#define RLF_SYNCHRONOUS		1
#define RLF_ASYNC			2
#define RLF_HDD_CACHE		4	// Load from flat file, do not use archives

	inline static sResource * (__stdcall *ctor)(void *thisptr) = GetModuleAddress<sResource*(__stdcall*)(void*)>((void*)0x14063AC10);

	inline static cResource * (__stdcall *LoadResourceFromArchive)(void *thisptr, rArchive::DecompressStream *pStream, MtDTI *pDTI, rArchiveFileEntry *pFileEntry) =
		GetModuleAddress<cResource*(__stdcall*)(void*, rArchive::DecompressStream*, MtDTI*, rArchiveFileEntry*)>((void*)0x14063B9E0);

	template<typename T> T* LoadGameResource(MtDTI *pObjectType, char *psFileName, DWORD flags)
	{
		return (T*)ThisPtrCall(0x14063BC60, this, pObjectType, psFileName, (void*)flags);
	}

	static ULONGLONG CalculateResourceId(MtDTI *pObjectType, char *psFileName)
	{
		return (ULONGLONG)ThisPtrCall((void*)0x14063DCB0, *g_sResourceInstance, pObjectType, psFileName);
	}
};
static_assert(sizeof(sResource) == 0x24458, "sResource incorrect struct size");

inline static void(__stdcall *sResource_ResourceDecoderProc)(int threadIndex) = GetModuleAddress<void(__stdcall*)(int)>((void*)0x14063C9B0);


class sResourceImpl
{
protected:

public:
	template<typename T> static T GetGameResourceAsType(void *pTypeDTI, char *psFileName, int dwFlags)
	{
		return (T)GetGameResourceAsType(pTypeDTI, psFileName, dwFlags);
	}

	static cResource* GetGameResourceAsType(void *pTypeDTI, char *psFileName, int dwFlags);

	static void IncrementResourceRefCount(cResource *pResource);

	static void InitializeTypeInfo();
};