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


	char GameDirectory[MAX_PATH];					// Game directory folder path

	std::vector<std::string> OverlayArchives;		// List of archives to overlay the game files
};