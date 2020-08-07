/*

*/

#pragma once
#include "DeadRisingEx.h"
#include <MtFramework/MtObject.h>
#include <MtFramework/Archive/rArchive.h>
#include <MtFramework/Archive/sResource.h>
#include <map>

struct ArcOverlayFile
{
	std::string			sFilePath;			// File path to the arc file

	rArchiveFileEntry	*pFileEntries;		// Array of file entries
	DWORD				FileEntryCount;		// Number of file entries
};

class ArchiveOverlay
{
protected:
	ArchiveOverlay();
	~ArchiveOverlay();

	std::vector<ArcOverlayFile> vArcFiles;
	std::map<ULONGLONG, DWORD> mFileOverlayMap;

	//std::vector<std::string> GetModDirectoryFiles();

	bool LoadArcFile(std::string sFilePath);

	friend sResource * __stdcall Hook_sResource_ctor(void *thisptr);
	friend void __stdcall Hook_sResource_ResourceDecoderProc(int threadIndex);
	friend cResource * __stdcall Hook_sResource_LoadResourceFromArchive(void *thisptr, rArchive::DecompressStream *pStream, MtDTI *pDTI, rArchiveFileEntry *pFileEntry);

public:
	static ArchiveOverlay * Instance()
	{
		static ArchiveOverlay *pSingleton = new ArchiveOverlay();
		return pSingleton;
	}

	bool Initialize();
};