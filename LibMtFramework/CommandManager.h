/*

*/

#pragma once
#include <string>
#include <Windows.h>

typedef __int64(__stdcall *CommandHandlerFunc)(WCHAR **argv, int argc);

struct CommandEntry
{
	const WCHAR *psName;
	const WCHAR *psHelpMessage;
	CommandHandlerFunc pHandlerFunction;
};

enum VariableType
{
	VariableType_Number,
	VariableType_String,
	VariableType_Pointer,
};

struct LocalVariable
{
	std::wstring	Name;		// Name of the variable
	VariableType	Type;		// Type of variable
	std::wstring	TypeName;	// Name of the underlying field type
	void*			Value;		// Value or pointer to the value of the local variable
};

bool RegisterCommand(const CommandEntry *pEntry);
bool RegisterCommands(const CommandEntry *pCommands, int Count);

bool FindCommand(std::wstring sCommandName, CommandEntry *pCommandInfo);

bool CreateLocalVar(std::wstring name, VariableType type, std::wstring typeName, void *value);
void DeleteLocalVar(std::wstring name);
const LocalVariable* GetLocalVar(std::wstring name);

__int64 GetLocalVarValue(std::wstring name);

template<typename T> T GetLocalVarValue(std::wstring name)
{
	return (T)GetLocalVarValue(name);
}

// Table of commands for the command manager.
const int g_CommandManagerCommandsLength = 3;
extern const CommandEntry g_CommandManagerCommands[];