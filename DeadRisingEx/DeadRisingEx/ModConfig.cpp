/*

*/

#include "ModConfig.h"
#include <cstring>
#include "DeadRisingEx/Utilities.h"

bool GetPrivateProfileBool(LPCSTR lpAppName, LPCSTR lpKeyName, LPCSTR lpDefault, LPCSTR lpFileName)
{
    CHAR stringValue[16] = { 0 };

    // Get the string value from the config file.
    GetPrivateProfileString(lpAppName, lpKeyName, lpDefault, stringValue, sizeof(stringValue) - 1, lpFileName);

    // Check if the string is 'true' or not.
    return _stricmp(stringValue, "true") == 0;
}

ModConfig::ModConfig()
{
    // Initialize config with default values.
    GetCurrentDirectory(sizeof(this->GameDirectory), this->GameDirectory);
}

bool ModConfig::LoadConfigFile(std::string sConfigFile)
{
#define SCRATCH_BUFFER_SIZE        0x10000

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

    // Format the file path for the config file.
    std::string sConfigFilePath = this->GameDirectory;
    sConfigFilePath += "\\" + sConfigFile;

    // Load game settings.
    this->DebugLog = GetPrivateProfileBool("GameSettings", "DebugLog", "false", sConfigFilePath.c_str());
    this->RecursiveGrenade = GetPrivateProfileBool("GameSettings", "RecursiveGrenade", "false", sConfigFilePath.c_str());

    // Load graphics settings.
    this->DynamicGraphicsMemory = GetPrivateProfileBool("GraphicsSettings", "DynamicGraphicsMemory", "true", sConfigFilePath.c_str());

    // Get a list of all keys under the load order section.
    int length = GetPrivateProfileString("ModLoadOrder", nullptr, nullptr, psBuffer, SCRATCH_BUFFER_SIZE, sConfigFilePath.c_str());
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
            GetPrivateProfileString("ModLoadOrder", pKeyName, nullptr, sFileName, sizeof(sFileName), sConfigFilePath.c_str());
            this->OverlayArchives.push_back(sFileName);

            // Next key name.
            pKeyName += lstrlen(pKeyName) + 1;
        }
    }

    // Free the scratch buffer.
    free(psBuffer);

    // Config file successfully parsed.
    return true;
}