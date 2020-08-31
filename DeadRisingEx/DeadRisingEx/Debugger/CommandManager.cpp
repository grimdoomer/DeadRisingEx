/*

*/

#include "CommandManager.h"
#include <map>

// Forward declarations for commands:
__int64 ListCommands(WCHAR **argv, int argc);
__int64 ListLocalVariables(WCHAR **argv, int argc);
__int64 CreateLocalVariable(WCHAR **argv, int argc);

// Table of commands for the command manager.
const CommandEntry g_CommandManagerCommands[g_CommandManagerCommandsLength] =
{
	{ L"help", L"Displays a list of commands and their descriptions", ListCommands },
	{ L"list_vars", L"Displays a list of local variables", ListLocalVariables },
	{ L"let", L"Creates a local variable and assigns the specified type and value", CreateLocalVariable }
};

std::map<std::wstring, CommandEntry> g_CommandTable;
std::map<std::wstring, LocalVariable> g_LocalVariables;

bool RegisterCommand(const CommandEntry *pEntry)
{
	// Check to see if there is already a command with this name.
	if (g_CommandTable.find(std::wstring(pEntry->psName)) != g_CommandTable.end())
	{
		// A command with this name already exists.
		return false;
	}

	// Insert the command entry into the command table.
	g_CommandTable.emplace(std::wstring(pEntry->psName), *pEntry);
	return true;
}

bool RegisterCommands(const CommandEntry *pCommands, int Count)
{
	// Loop and add all of the commands to the command table.
	for (int i = 0; i < Count; i++)
	{
		// Add the command.
		if (RegisterCommand(&pCommands[i]) == false)
			return false;
	}

	// Successfully added all the commands.
	return true;
}

bool FindCommand(std::wstring sCommandName, CommandEntry *pCommandInfo)
{
	// Check if the command table has an entry for the specified command.
	if (g_CommandTable.find(sCommandName) == g_CommandTable.end())
		return false;

	// Get the command info from the command table.
	*pCommandInfo = g_CommandTable.at(sCommandName);
	return true;
}

bool CreateLocalVar(std::wstring name, VariableType type, std::wstring typeName, void *value)
{
	// Check to make sure a local variable with the same name doesn't already exist.
	if (g_LocalVariables.find(name) != g_LocalVariables.end())
	{
		// Remove the local variable.
		g_LocalVariables.erase(name);
	}

	// Create a new local variable struct to add to the dictionary.
	LocalVariable local = { name, type, typeName, value };
	g_LocalVariables.emplace(name, local);
	return true;
}

void DeleteLocalVar(std::wstring name)
{
	// Check to make sure a local with the specified name exists.
	if (g_LocalVariables.find(name) != g_LocalVariables.end())
	{
		// Remove the local variable.
		g_LocalVariables.erase(name);
	}
}

const LocalVariable* GetLocalVar(std::wstring name)
{
	// If the specified local variable exists return it.
	if (g_LocalVariables.find(name) != g_LocalVariables.end())
	{
		// Return the local variable info.
		return &g_LocalVariables.at(name);
	}

	// A local variable with the specified name was not found.
	return nullptr;
}

__int64 GetLocalVarValue(std::wstring name)
{
	// Check if a local variable with the specified name exists.
	if (g_LocalVariables.find(name) == g_LocalVariables.end())
	{
		// A local variable with the specified name does not exist.
		return 0;
	}

	// Get the local variable and return its value.
	const LocalVariable local = g_LocalVariables.at(name);
	return (__int64)local.Value;
}

__int64 ListCommands(WCHAR **argv, int argc)
{
	// Loop through all of the commands and print each one.
	for (auto iter = g_CommandTable.begin(); iter != g_CommandTable.end(); iter++)
	{
		// Format the command text.
		int length = min(0, 32 - lstrlenW(iter->second.psName));
		wprintf(L"\t%s %*s\n", iter->second.psName, length, iter->second.psHelpMessage);
	}

	return 0;
}

__int64 ListLocalVariables(WCHAR **argv, int argc)
{
	// Loop through all the local variables and print each one.
	for (auto iter = g_LocalVariables.begin(); iter != g_LocalVariables.end(); iter++)
	{
		WCHAR sLocalText[MAX_PATH] = { 0 };

		// Check the variable type and handle accordingly.
		switch (iter->second.Type)
		{
		case VariableType_Number: _snwprintf(sLocalText, sizeof(sLocalText), L"%s %s = %llu\n", iter->second.TypeName.c_str(), iter->second.Name.c_str(), (__int64)iter->second.Value); break;
		case VariableType_String: _snwprintf(sLocalText, sizeof(sLocalText), L"%s %s = %s\n", iter->second.TypeName.c_str(), iter->second.Name.c_str(), (wchar_t*)iter->second.Value); break;
		case VariableType_Pointer: _snwprintf(sLocalText, sizeof(sLocalText), L"%s %s = %p\n", iter->second.TypeName.c_str(), iter->second.Name.c_str(), iter->second.Value); break;
		}

		// Print the local variable.
		wprintf(sLocalText);
	}

	return 0;
}

__int64 CreateLocalVariable(WCHAR **argv, int argc)
{
	CommandEntry valCommand;
	void *variableValue = nullptr;

	// Make sure there are at least 4 arguments for the command.
	if (argc < 4)
	{
		// Command is invalid.
		wprintf(L"Invalid command syntax");
		return 0;
	}

	// Get the command components.
	std::wstring sVarType = argv[0];
	std::wstring sVarName = argv[1];
	std::wstring sOp = argv[2];
	std::wstring sVarValue = argv[3];

	// Check the value and see if there is a command by that name.
	if (FindCommand(sVarValue, &valCommand) == true)
	{
		// Execute the command to get the variable value.
		variableValue = (void*)valCommand.pHandlerFunction(&argv[4], argc - 4);
	}
	else
	{
		// Unsupported value type.
		wprintf(L"Unsupported value type\n");
		return 0;
	}

	// Create or update the local variable.
	CreateLocalVar(sVarName, VariableType_Pointer, sVarType, variableValue);

	return 0;
}