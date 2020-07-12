/*

*/

#include "ModConfig.h"
#include "DeadRisingEx/Utilities.h"

ModConfig::ModConfig()
{
	// Initialize config with default values.
	GetCurrentDirectory(sizeof(this->GameDirectory), this->GameDirectory);
}

bool ModConfig::LoadConfigFile(std::string sConfigFile)
{
#define SCRATCH_BUFFER_SIZE		0x10000

	// Allocate a scratch buffer for string space.
	char *psBuffer = (char*)malloc(SCRATCH_BUFFER_SIZE);
	if (psBuffer == nullptr)
	{
		// Failed to allocate scratch buffer for config file data.
		DbgPrint("ModConfig::LoadConfigFile(): failed to allocate memory for scratch buffer!\n");
		return false;
	}

	// Get the game directory folder path.
	GetCurrentDirectory(sizeof(this->GameDirectory), this->GameDirectory);

	// Get a list of all keys under the load order section.
	int length = GetPrivateProfileString("ModLoadOrder", nullptr, nullptr, psBuffer, SCRATCH_BUFFER_SIZE, sConfigFile.c_str());
	if (length > 0)
	{
		// Loop and read each file name.
		char *pKeyName = psBuffer;
		while (pKeyName < psBuffer + SCRATCH_BUFFER_SIZE)
		{
			char sFileName[MAX_PATH];

			// Check if this is the end of the list.
			if (*pKeyName == 0)
				break;

			// Get the file name for the current key.
			GetPrivateProfileString("ModLoadOrder", pKeyName, nullptr, sFileName, sizeof(sFileName), sConfigFile.c_str());
			this->OverlayArchives.push_back(sFileName);

			// Next key name.
			pKeyName += lstrlen(pKeyName) + 1;
		}
	}

	// Config file successfully parsed.
	return true;
}