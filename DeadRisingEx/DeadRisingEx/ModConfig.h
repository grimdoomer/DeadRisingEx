/*

*/

#pragma once
#include <Windows.h>
#include <string>
#include <vector>

class ModConfig
{
protected:
    ModConfig();

public:
    static ModConfig * Instance()
    {
        static ModConfig *pSingleton = new ModConfig();
        return pSingleton;
    }

    bool LoadConfigFile(std::string sConfigFile);

    bool ItemRandomizerEnabled;     // Determines if items should be randomized

    bool DebugLog;                  // Determines if debug output should be written to a log file or not
    int ConsoleHistoryLimit;        // Maximum number of items to keep in the console log history buffer

    bool RecursiveGrenade;          // Enables the recursive grenade mod

    // Graphics options:
    bool DynamicGraphicsMemory;     // Determines if graphics memory should be dynamically allocated/resized as needed

    char GameDirectory[MAX_PATH];                    // Game directory folder path

    std::vector<std::string> OverlayArchives;        // List of archives to overlay the game files
};