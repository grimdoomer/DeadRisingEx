# DeadRisingEx
An unofficial update and mod SDK for Dead Rising 1 on PC. The purpose of DeadRisingEx is to add new features to the game, expand modding capabilities, and fix game crashes and stability issues in the vanilla version of the game. Modding the vanilla version of the game has a lot of limitations and drawbacks which this update aims to remove.

DeadRisingEx also doubles as a SDK for anyone who wants to make code modifications to the game. Using LibMtFramework, the C++ library that DeadRisingEx is built on, you can interact with different game objects and hook into various game systems to alter behavior.

### Current Features
- Mod management system that lets you load multiple mods at the same time without touching the original game files
- Customizable load order for mods
- Adds in-game console for spawning items, loading levels, and other miscellaneous game tweaks
- Fixes DirectX related crashes and game stability issues, see the [dynamic graphics memory](https://github.com/grimdoomer/DeadRisingEx/wiki/Settings#dynamic-graphics-memory) option for more info on direct game fixes
- Re-implements the game's debug menu which allows you to inspect and change certain object states, see the [open_debug_menu](https://github.com/grimdoomer/DeadRisingEx/wiki/Developer-Console#open-debug-menu) console command for more info
- Auto update system that will inform you of new updates automatically

### Future Features
- Better object introspection and scripting functionality in the console window
- Create new file types for weapons/vehicles/items/etc. that move hard coded parameters out of the game code and into moddable files
- Support for interacting with more game objects and systems
- Lua scripting system

## Installation
Download the latest update [here](https://github.com/grimdoomer/DeadRisingEx/releases). Installing the DeadRisingEx game update is as simple as copying the DeadRisingEx files into the root of your Dead Rising install folder. You will be asked if you want to merge files for the nativeWin64 folder, just click yes here. And that's it! The game update is now installed. Any time there is an update for DeadRisingEx the launcher will notify you and allow you to install it automatically.

![](/Images/install_files.gif)

## Running DeadRisingEx
When you want to run the game with the DeadRisingEx update you will need to use the DeadRisingLauncher.exe application in order to start the game. For quick access to the DeadRisingLauncher application you can create a desktop shortcut or add it as an external game in steam.

Steam MUST be running in the background before you start the game. If steam is not running when you launch the game it will run without the update, so when in doubt make sure steam is running. If you encounter issues where steam gets stuck in a loop showing a "Preparing to launch Dead Rising" message, you will need to force kill steam from task manager and restart it, this is a known issue with how steam launches games.

![](/Images/launcher.gif)

All of your mod files can be placed into "Dead Rising\nativeWin64\Mods" and the settings dialog will scan this folder and all folders within it. By default new mods added are disabled and must be enabled manually.

Clicking on the settings button will display a window that lets you configure all of the settings for DeadRisingEx, including what mod files to load and in what order. The list will automatically scan the Mods directory for mod files to load. Checking the checkbox next to the mod file will enable it for loading, and using the up/down buttons will change the load order. For a complete list of settings and game tweaks DeadRisingEx has see the [Settings](https://github.com/grimdoomer/DeadRisingEx/wiki/Settings) page in the wiki.

The mod files are loaded in descending order, so which ever is at the top of the list loads first, then the next one, etc. Configuring load order is only required if two mod files override the same game file. In this case it may be desirable to load one mod file before the other depending on the mods.

![](/Images/mod_load_order.png)

DeadRisingEx comes with a test mod that you can use to verify the update installation was successful by enabling the "deadrisingex_test.arc" mod in the settings window. With this test mod enabled if you start the game and look at the whiteboard in the security room you will see "DEAD RISING EX" text on it.

![](/Images/install_test.png)

## In-game Console
DeadRisingEx adds an in-game developer console into the game which can be opened by pressing the \` (tilde) key. From there you can use various console commands to spawn items, change levels, open the game's debug menu, and more. The console itself features command suggestions, auto complete, and command history. For more information see the [developer console](https://github.com/grimdoomer/DeadRisingEx/wiki/Developer-Console) wiki page.

![](/Images/console.gif)

## Bug fixes and stability patches
DeadRisingEx fixes a number of game crashing bugs in the vanilla version of the game. You may have encountered a "directx assert" error while playing the vanilla version of the game, probably when trying to blow up too much stuff at once. This is due to a bug in the rendering system that attempts to use more VRAM than the game has access to. 

DeadRisingEx fixes these issues by removing the VRAM limitations on the game and increasing the amount of RAM available to it for rendering. Now you can play the game and blow up as much stuff as you want without the game crashing. To enable the rendering fixes see the [dynamic graphics memory](https://github.com/grimdoomer/DeadRisingEx/wiki/Settings#dynamic-graphics-memory) option in the settings menu.

![](/Images/directx_assert.png)

There are a number of other stability bug fixes implemented as well which are always on by default such as:
- Increased memory to help support mods and eliminate out of memory game crashes
- Spawning more than 2048 zombies no longer crashes the game
- Fixed various memory corruption bugs when game objects are destroyed
- Fixed bugs that caused game crashes when area hoping too often

## Source Code and SDK
The code base is broken into 2 main areas:
- LibMtFramework: The SDK library, contains header files describing internal structures of the game and the systems that use them
- DeadRisingEx/DeadRisingLauncher: The unofficial update for the game that is based on LibMtFramework, and the launcher application for running the game with the update

### LibMtFramework
This is the SDK library for making code modifications to the game. It contains a bunch of header and source files that can be used to interact with various game objects and hook into different game components. For more information I will be creating a wiki on how to use LibMtFramework and interact with different game components. This is still a WIP! Also full disclosure the library is not thoroughly documented or cleaned up for the first release of this.

### DeadRisingEx
This is the code for the DeadRisingEx update which uses LibMtFramework. Here is where you can see how I created all the features in the update and use it as a reference for writing your own code modifications. More information on this soon.

### DeadRisingLauncher
This is the code for the launcher application. It's main purpose to allow changing game settings and launching the game with the update.
