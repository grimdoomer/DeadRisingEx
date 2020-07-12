/*

*/

#include "ArchiveOverlay.h"
#include <algorithm>
#include "detours.h"
#include <MtFramework/Archive/sResource.h>
#include <MtFramework/IO/MtFileStream.h>
#include <MtFramework/Utils/Utilities.h>
#include <MtFramework/Memory/MtHeapAllocator.h>

sResource * __stdcall Hook_sResource_ctor(void *thisptr);
void __stdcall Hook_sResource_ResourceDecoderProc(int threadIndex);
cResource * __stdcall Hook_sResource_LoadResourceFromArchive(void *thisptr, rArchive::DecompressStream *pStream, MtDTI *pDTI, rArchiveFileEntry *pFileEntry);

ArchiveOverlay::ArchiveOverlay()
{

}

ArchiveOverlay::~ArchiveOverlay()
{

}

bool ArchiveOverlay::Initialize()
{
	// Install hooks.
	DetourAttach((void**)&sResource::ctor, Hook_sResource_ctor);
	DetourAttach((void**)&sResource_ResourceDecoderProc, Hook_sResource_ResourceDecoderProc);
	DetourAttach((void**)&sResource::LoadResourceFromArchive, Hook_sResource_LoadResourceFromArchive);

	// Successfully initialized.
	return true;
}

std::vector<std::string> ArchiveOverlay::GetModDirectoryFiles()
{
	std::vector<std::string> vModFiles;
	WIN32_FIND_DATAA FindFileInfo = { 0 };

	// Initialize the mod list with any overlay arc files from the mod config.
	for (int i = 0; i < ModConfig::Instance()->OverlayArchives.size(); i++)
	{
		// Add the overlay arc file to the list of mods to load.
		vModFiles.push_back(ModConfig::Instance()->OverlayArchives.at(i));
	}

	// Create our search path string.
	std::string sGameDir = ModConfig::Instance()->GameDirectory;
	std::string sSearchPath = sGameDir + "\\nativeWin64\\Mods\\*.arc";

	// Initiate the search.
	HANDLE hSearch = FindFirstFile(sSearchPath.c_str(), &FindFileInfo);
	if (hSearch == INVALID_HANDLE_VALUE)
	{
		// Failed to initiate search.
		DbgPrint("ArchiveOverlay::GetModDirectoryFiles(): failed to initiate mod file search %d!\n", GetLastError());
		return vModFiles;
	}

	// Loop and process all files in the directory.
	do
	{
		// Make sure we don't already have this file in the list.
		if (std::find(ModConfig::Instance()->OverlayArchives.begin(), ModConfig::Instance()->OverlayArchives.end(),
			std::string(FindFileInfo.cFileName)) != ModConfig::Instance()->OverlayArchives.end())
		{
			// Skip adding the file.
			continue;
		}

		// Add the file to the list.
		vModFiles.push_back(FindFileInfo.cFileName);

	} while (FindNextFile(hSearch, &FindFileInfo) != 0);

	// Close the search handle.
	FindClose(hSearch);

	// Return the list of mod files to load.
	return vModFiles;
}

bool ArchiveOverlay::LoadArcFile(std::string sFilePath)
{
	bool result = false;
	DWORD BytesRead = 0;
	rArchiveHeader ArcHeader = { 0 };
	rArchiveFileEntry *pFileEntries = nullptr;
	ArcOverlayFile OverlayInfo;

	// Open the arc file for reading.
	HANDLE hFile = CreateFile(sFilePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		// Failed to open the mod file for reading.
		DbgPrint("ArchiveOverlay::LoadArcFile(): failed to open arc file: %s\n", sFilePath.c_str());
		return false;
	}

	// Read the arc file header.
	if (ReadFile(hFile, &ArcHeader, sizeof(ArcHeader), &BytesRead, NULL) == FALSE || BytesRead != sizeof(ArcHeader))
	{
		// Failed to read archive header.
		DbgPrint("ArchiveOverlay::LoadArcFile(): failed to read arc file: %s\n", sFilePath.c_str());
		goto Cleanup;
	}

	// Check the header magic and version are correct.
	if (ArcHeader.Magic != ARC_HEADER_MAGIC || ArcHeader.Version != ARC_HEADER_VERSION)
	{
		// Header is invalid.
		DbgPrint("ArchiveOverlay::LoadArcFile(): arc file has invalid header: %s\n", sFilePath.c_str());
		goto Cleanup;
	}

	// Allocate memory for the file entries.
	pFileEntries = (rArchiveFileEntry*)malloc(sizeof(rArchiveFileEntry) * ArcHeader.NumberOfFiles);
	if (pFileEntries == nullptr)
	{
		// Failed to allocate memory for file entry array.
		DbgPrint("ArchiveOverlay::LoadArcFile(): failed to allocate memory for file entries: %s\n", sFilePath.c_str());
		goto Cleanup;
	}

	// Read the file entries from the arc file.
	if (ReadFile(hFile, pFileEntries, sizeof(rArchiveFileEntry) * ArcHeader.NumberOfFiles, &BytesRead, NULL) == FALSE ||
		BytesRead != sizeof(rArchiveFileEntry) * ArcHeader.NumberOfFiles)
	{
		// Failed to read file entry data.
		DbgPrint("ArchiveOverlay::LoadArcFile(): failed to read file entry data: %s\n", sFilePath.c_str());
		goto Cleanup;
	}

	// Add the overlay file info to the overlay list.
	OverlayInfo.sFilePath = sFilePath;
	OverlayInfo.pFileEntries = pFileEntries;
	OverlayInfo.FileEntryCount = ArcHeader.NumberOfFiles;
	this->vArcFiles.push_back(OverlayInfo);

	// Loop through the list of file entries and add each one to the overlay map.
	for (int i = 0; i < ArcHeader.NumberOfFiles; i++)
	{
		// Get the DTI info for the file type.
		MtDTI *pDTI = GetDTIForFileType(pFileEntries[i].FileType, g_cResourceDTI);

		// Calculate the id for this resource.
		ULONGLONG resourceId = sResource::CalculateResourceId(pDTI, pFileEntries[i].FileName);

		// Add an entry to the overlay map.
		DWORD arcFileId = (DWORD)(((this->vArcFiles.size() - 1) << 16) & 0xFFFF0000) | (i & 0xFFFF);
		this->mFileOverlayMap.emplace(resourceId, arcFileId);

		DbgPrint("\tOverlay: 0x%llu -> %s\n", resourceId, pFileEntries[i].FileName);
	}

	// Successfully read the arc file.
	result = true;

Cleanup:
	// Free file entry data if the operation failed.
	if (result == false)
		free(pFileEntries);

	// Close the file handle.
	CloseHandle(hFile);

	// Return the result.
	return result;
}

sResource * __stdcall Hook_sResource_ctor(void *thisptr)
{
	// Call the trampoline.
	sResource *ret = sResource::ctor(thisptr);

	// Build a list of mod files to load, honoring the load order in the mod config file.
	std::vector<std::string> vModFiles = ArchiveOverlay::Instance()->GetModDirectoryFiles();
	std::reverse(vModFiles.begin(), vModFiles.end());

	// Loop through the list in reverse so we honor the load order.
	DbgPrint("Loading mods...\n");
	for (auto iter = vModFiles.begin(); iter != vModFiles.end(); iter++)
	{
		// Format the file path to the arc file.
		std::string sGameDir = ModConfig::Instance()->GameDirectory;
		std::string sModFile = sGameDir + "\\nativeWin64\\Mods\\" + iter->c_str();

		// Load the archive file.
		if (ArchiveOverlay::Instance()->LoadArcFile(sModFile) == false)
		{
			// Failed to load the archive.
			continue;
		}

		// Successfully loaded the archive.
		DbgPrint("Loaded: %s\n", iter->c_str());
	}

	// Return class instance pointer.
	return ret;
}

void __stdcall Hook_sResource_ResourceDecoderProc(int threadIndex)
{
	// Get the global sResource instance.
	sResource *thisptr = *g_sResourceInstance;

	// Loop until we are told to quit.
	while (thisptr->WorkerThreadsShouldExit == false)
	{
		// Wait until there is work to do.
		while (WaitForEventHelper(&thisptr->DecoderWorkAvailableEvent[threadIndex], INFINITE) == false);

		// Acquire the decode list lock.
		EnterCriticalSection(&thisptr->AsyncDecodeLock);

		// Check if the decoding work is complete.
		while (thisptr->Unknown1 == 0)
		{
			// Check if there are more decode requests and get the next available one if so.
			sResource::DecodeFileRequest *pDecodeReq = nullptr;
			if (thisptr->AsyncLoadCount < thisptr->AsyncDecodeCount)
			{
				// Get the next decode request.
				pDecodeReq = &thisptr->AsyncDecodeQueue[thisptr->AsyncLoadCount];

				// If the resource has not been loaded yet or we don't have the buffered data for it, bail out.
				if ((pDecodeReq->pResource->mState & RESS_RESOURCE_LOADED) == 0 && pDecodeReq->OffsetFromHeader >= thisptr->AsyncBytesRead)
				{
					// Not enough data has buffered to load the next file.
					goto NextIteration;
				}

				// Increase the number of files loaded.
				thisptr->AsyncLoadCount++;
			}

			// Release the list lock.
			LeaveCriticalSection(&thisptr->AsyncDecodeLock);

			// If we didn't find a request to process break the processing loop and sleep.
			if (pDecodeReq == nullptr)
				break;

			// Check if the resource has been loaded yet.
			if ((pDecodeReq->pResource->mState & RESS_RESOURCE_LOADED) == 0)
			{
				sResource::DecompressStream stream;
				MtFile arcFile = { 0 };
				MtFileStream fileStream = { 0 };
				sResource::DecodeFileRequest overlayReq = { 0 };
				sResource::DecompressStreamContext context = { 0 };
				BYTE CrapPad[0x30] = { 0x69 };

				// Check if we have an overlay entry for this resource id.
				if (ArchiveOverlay::Instance()->mFileOverlayMap.find(pDecodeReq->pResource->mID) != ArchiveOverlay::Instance()->mFileOverlayMap.end())
				{
					// Get the arc and file indices from the map.
					DWORD arcFileId = ArchiveOverlay::Instance()->mFileOverlayMap[pDecodeReq->pResource->mID];
					DWORD archiveIndex = (arcFileId >> 16) & 0xFFFF;
					DWORD fileIndex = arcFileId & 0xFFFF;

					// Create a file stream for the arc file.
					MtFile::ctor(&arcFile, ArchiveOverlay::Instance()->vArcFiles[archiveIndex].sFilePath.c_str(), 1);
					MtFileStream::ctor(&fileStream, &arcFile);

					// Open the file for reading.
					if (fileStream.Open() == false)
					{
						// Failed to open arc file.
						DbgPrint("Failed to open arc file: %s\n", ArchiveOverlay::Instance()->vArcFiles[archiveIndex].sFilePath.c_str());
						goto LoadNormal;
					}

					// Create a fake decode request using info from the overlay arc file.
					overlayReq.pResource = pDecodeReq->pResource;
					overlayReq.CompressedSize = ArchiveOverlay::Instance()->vArcFiles[archiveIndex].pFileEntries[fileIndex].CompressedSize;
					overlayReq.DecompressedSize = ArchiveOverlay::Instance()->vArcFiles[archiveIndex].pFileEntries[fileIndex].DecompressedSize;
					overlayReq.OffsetFromHeader = 0; // ArchiveOverlay::Instance()->vArcFiles[archiveIndex].pFileEntries[fileIndex].DataOffset -
						//ArchiveOverlay::Instance()->vArcFiles[archiveIndex].pFileEntries[0].DataOffset;

					// Update the size of the resource.
					overlayReq.pResource->mSize = overlayReq.DecompressedSize;

					// Create the fake decompression context struct.
					context.pArchiveStream = &fileStream;
					context.ScratchBufferSize = overlayReq.DecompressedSize;
					context.pScratchBuffer = (*g_pTempHeapAllocator)->Alloc(context.ScratchBufferSize, 0x10);
					context.CurrentArchiveOffset = ArchiveOverlay::Instance()->vArcFiles[archiveIndex].pFileEntries[fileIndex].DataOffset;
					context.AsyncBytesRead = ArchiveOverlay::Instance()->vArcFiles[archiveIndex].pFileEntries[fileIndex].CompressedSize;
					InitializeCriticalSection(&context.AsyncDecodeLock);
					
					if (context.pScratchBuffer == nullptr)
					{
						DbgPrint("Scratch buffer allocation is null!\n");
						DebugBreak();
					}

					// Seek to the start of the compressed data.
					fileStream.Seek(ArchiveOverlay::Instance()->vArcFiles[archiveIndex].pFileEntries[fileIndex].DataOffset, 1);

					// Read the compressed data into the scratch buffer.
					fileStream.ReadData(context.pScratchBuffer, context.ScratchBufferSize);

					// Load from the overlay archive.
					DbgPrint("Loading from overlay archive: %s\n", overlayReq.pResource->mPath);
					sResource::DecompressStream::ctor(&stream, &context, &overlayReq);
				}
				else
				{
				LoadNormal:
					// No overlay, load the stock file.
					sResource::DecompressStream::ctor(&stream, (sResource::DecompressStreamContext*)&thisptr->pArchiveStream, pDecodeReq);
				}

				// Load the resource.
				if (pDecodeReq->pResource->LoadResource(&stream) == false)
				{
					// Failed to load the resource.
					pDecodeReq->pResource->mState |= RESS_RESOURCE_LOAD_FAILED;
					DbgPrint("Load failed. %s\n", pDecodeReq->pResource->mPath);
				}
				else
				{
					// Resource loaded successfully.
					pDecodeReq->pResource->mState |= RESS_RESOURCE_LOADED;
					DbgPrint("Load success. %s\n", pDecodeReq->pResource->mPath);
				}

				// Cleanup the stream.
				stream.dtor();

				// If we loaded from the overlay file close the archive.
				if (overlayReq.pResource != nullptr)
				{
					// Close the archive.
					fileStream.dtor();
					arcFile.dtor();

					(*g_pTempHeapAllocator)->Free(context.pScratchBuffer);
				}
			}

			// Acquire the list lock so we can update the decode request.
			EnterCriticalSection(&thisptr->AsyncDecodeLock);

			// Update the decode request to be complete.
			pDecodeReq->Result = 1;
			pDecodeReq->BytesRead = pDecodeReq->CompressedSize;

		NextIteration:
			// Release the lock so the loader thread has a chance to buffer data.
			LeaveCriticalSection(&thisptr->AsyncDecodeLock);

			// Times up! Get the lock back before we query Unknonw1.
			EnterCriticalSection(&thisptr->AsyncDecodeLock);
		}

		// No work to do yet.
		LeaveCriticalSection(&thisptr->AsyncDecodeLock);
		SetEventHelper(&thisptr->DecoderWorkCompletedEvent[threadIndex]);
		continue;
	}
}

cResource * __stdcall Hook_sResource_LoadResourceFromArchive(void *thisptr, rArchive::DecompressStream *pStream, MtDTI *pDTI, rArchiveFileEntry *pFileEntry)
{
	MtFile arcFile = { 0 };
	MtFileStream fileStream = { 0 };
	rArchive::DecompressStream arcStream = { 0 };
	cResource *pResource = nullptr;

	// Calculate the resource id for this file.
	ULONGLONG resourceId = sResource::CalculateResourceId(pDTI, pFileEntry->FileName);

	// Check to see if we have an overlay file for this id.
	DbgPrint("Load 0x%llu -> %s\n", resourceId, pFileEntry->FileName);
	if (ArchiveOverlay::Instance()->mFileOverlayMap.find(resourceId) == ArchiveOverlay::Instance()->mFileOverlayMap.end())
	{
		// No overlay file found, call the trampoline.
		return sResource::LoadResourceFromArchive(thisptr, pStream, pDTI, pFileEntry);
	}

	// WARNING: This is all untested code as it seems this code path is rarely hit.
	DbgPrint("WARNING: Hook_sResource_LoadResourceFromArchive untested code!!!\n");

	// Get the arc and file indices from the map.
	DWORD arcFileId = ArchiveOverlay::Instance()->mFileOverlayMap[resourceId];
	DWORD archiveIndex = (arcFileId >> 16) & 0xFFFF;
	DWORD fileIndex = arcFileId & 0xFFFF;

	// Create a file stream for the arc file.
	MtFile::ctor(&arcFile, ArchiveOverlay::Instance()->vArcFiles[archiveIndex].sFilePath.c_str(), 1);
	MtFileStream::ctor(&fileStream, &arcFile);

	// Open the file for reading.
	if (fileStream.Open() == false)
	{
		// Failed to open arc file.
		DbgPrint("Failed to open arc file: %s\n", ArchiveOverlay::Instance()->vArcFiles[archiveIndex].sFilePath.c_str());
		goto Cleanup;
	}

	// Make sure there is data.
	if (MtFile::GetFileSize(&arcFile) == 0)
	{
		// File is empty.
		DbgPrint("File is empty: %s\n", ArchiveOverlay::Instance()->vArcFiles[archiveIndex].sFilePath.c_str());
		goto Cleanup;
	}

	// Create a new archive decompression stream capable of decompressing data.
	rArchive::DecompressStream::ctor(&arcStream, &fileStream);
	if (inflateInit(&arcStream.zStream, ZLIB_VERSION, sizeof(z_stream_s)) != 0)
	{
		// Print the zlib error.
		DbgPrint("inflateInit: %s\n", arcStream.zStream.msg != nullptr ? arcStream.zStream.msg : "???");
	}

	// Decompress the arc file entry and create a new resource for it.
	pResource = sResource::LoadResourceFromArchive(thisptr, &arcStream, pDTI, &ArchiveOverlay::Instance()->vArcFiles[archiveIndex].pFileEntries[fileIndex]);

	// Cleanup the zlib stream.
	inflateEnd(&arcStream.zStream);
	arcStream.dtor();

Cleanup:
	// Call the dtors for the file and file stream.
	fileStream.dtor();
	arcFile.dtor();

	// Return the resource pointer.
	return pResource;
}