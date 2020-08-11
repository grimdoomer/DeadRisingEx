# DeadRisingEx
An unofficial update and SDK for Dead Rising 1 on PC. The purpose of DeadRisingEx is to add in new features to the game to further support and expand modding capabilities. Modding the vanilla version of the game has a lot of limitations and drawbacks which this update aims to remove. 

One of the biggest features the update adds to the game is the mod manager, allowing you to keep all of your mods in separate from the original game files, and choose what mods to load and in what order to load them when starting the game. If you have ever played Skyrim or Fallout with mods, this feature works very similarly to the way they manage and load mods.

DeadRisingEx also doubles as a SDK for anyone who wants to make code modifications to the game. Using LibMtFramework, the C++ library that DeadRisingEx is built on, you can interact with different game objects and hook into various game systems to alter behavior. 

A good example of this is how DeadRisingEx overrides the game's file manager to load mod files from a custom directory and override the original game files. With LibMtFramework you will eventually be able to hook into different game objects like weapons, vehicles, npcs, and more.

### Current Features
- Mod management system that lets you load multiple mods at the same time without touching the original game files
- Mod manager allows you to choose what mods to load and in what order to load them
- Some basic object introspection is available through a console window, though it is highly experimental at this point in time

### Future Features
- Better object introspection and scripting functionality in the console window
- Create new file types for weapons/vehicles/items/etc. that move hard coded parameters out into modable files
- Support for interacting with more game objects and systems

## Installation
Installing the DeadRisingEx game update is as simple as copying the DeadRisingEx files into the root of your Dead Rising install folder. You will be asked if you want to merge files for the nativeWin64 folder, just click yes here. And that's it! The game update is now installed.
![](/Images/install_files.png)

## Running DeadRisingEx
When you want to run the game with the DeadRisingEx update you will need to use the DeadRisingLauncher.exe application in order to start the game. Steam must be also be running in the background before you start the game. If steam is not running when you launch the game it will run without the update, so when in doubt make sure steam is running. For quick access to the DeadRisingLauncher application you can create a desktop shortcut or add it as an external game in steam.

![](/Images/dead_rising_launcher.png)

All of your mod files can be placed into "Dead Rising\nativeWin64\Mods" and the settings dialog will scan this folder and all folders within it. By default new mods added are disabled and must be enabled manually.

Clicking on the settings button will display a window that lets you configure all of the settings for DeadRisingEx, including what mod files to load and in what order. The list will automatically scan the Mods directory for mod files to load. Checking the checkbox next to the mod file will enable it for loading, and using the up/down buttons will change the load order. 

The mod files are loaded in descending order, so which ever is at the top of the list loads first, then the next one, etc. Configuring load order is only required if two mod files override the same game file. In this case it may be desirable to load one mod file before the other depending on the mods.

![](/Images/mod_load_order.png)

DeadRisingEx comes with a test mod that you can use to verify the update installation was successful by enabling the "deadrisingex_test.arc" mod in the settings window. With this test mod enabled if you start the game and look at the whiteboard in the security room you will see "DEAD RISING EX" text on it.

![](/Images/install_test.png)

## Source Code and SDK
The code base is broken into 2 main areas:
- LibMtFramework: The SDK library, contains header files describing internal structures of the game and the systems that use them
- DeadRisingEx/DeadRisingLauncher: The unofficial update for the game that is based on LibMtFramework, and the launcher application for running the game with the update

### LibMtFramework
This is the SDK library for making code modifications to the game. It contains a bunch of header and source files that can be used to interact with various game objects and hook into different game components. For more information I will be creating a wiki on how to use LibMtFramework and interact with different game components. This is still a WIP!

### DeadRisingEx
This is the code for the DeadRisingEx update which uses LibMtFramework. Here is where you can see how I created all the features in the update and use it as a reference for writing your own code modifications. More information on this soon.

### DeadRisingLauncher
This is the code for the launcher application. It's main purpose to allow changing game settings and launching the game with the update.
