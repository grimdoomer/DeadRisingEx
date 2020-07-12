/*

*/

#include "sShader.h"
#include "Misc/AsmHelpers.h"
#include <stdio.h>
#include <string>

void *g_sShaderSingletonInst = (void*)0x141D179A0;

// Forward declarations for command functions.
__int64 PrintShaderParameters(WCHAR **argv, int argc);
__int64 PrintShaderTechniques(WCHAR **argv, int argc);

// Table of commands for sShader objects.
const int g_sShaderCommandsLength = 2;
const CommandEntry g_sShaderCommands[g_sShaderCommandsLength] =
{
	{ L"sShader.PrintShaderParameters", L"Lists all sShader parameter descriptors", PrintShaderParameters },
	{ L"sShader.PrintShaderTechniques", L"Lists all sShader techniques", PrintShaderTechniques }
};

void sShaderImpl::RegisterTypeInfo()
{
	// Register commands.
	RegisterCommands(g_sShaderCommands, g_sShaderCommandsLength);
}

__int64 PrintShaderParameters(WCHAR **argv, int argc)
{
	// Acquire the list lock.
	EnterCriticalSection(GetModulePointer<__int64, CRITICAL_SECTION>(g_sShaderSingletonInst, 8));

	// Get a pointer to the parameter array.
	sShader::ParameterDesc *pParameter = GetModulePointer<__int64, sShader::ParameterDesc>(g_sShaderSingletonInst, 0x298);
	DWORD ParameterCount = *GetModulePointer<__int64, DWORD>(g_sShaderSingletonInst, 0xB298);

	// Loop and print all of the parameters.
	for (int i = 0; i < ParameterCount; i++)
	{
		//// Print the parameter description.
		//wprintf(L"Parameter %d\n", i);
		//wprintf(L"\tName: %S\n", pParameter[i].mName);
		//wprintf(L"\tID: 0x%08x\tType: %d\tRegCount: %d\tIndex: %d\n", pParameter[i].mID, pParameter[i].Type, pParameter[i].RegCount, pParameter[i].SomethingIndex);
		//
		//// Check the value type and print accordingly.
		//switch (pParameter[i].Type)
		//{
		//case 0: wprintf(L"\tValue: %d\n", pParameter[i].SomethingIndex); break;
		//case 1: wprintf(L"\tValue: %s\n", pParameter[i].IntValue == 0 ? L"False" : L"True"); break;
		//case 2: wprintf(L"\tValue: %d\n", pParameter[i].IntValue); break;
		//case 3: wprintf(L"\tValue: %f\n", (float)pParameter[i].IntValue); break;
		//case 4: wprintf(L"\tValue: %p\n", pParameter[i].pValue); break;
		//}

		//wprintf(L"\n");
		if (pParameter[i].mName == nullptr)
			wprintf(L"ShaderParameter_%08x = 0x%08x,\n", pParameter[i].mID, pParameter[i].mID);
		else
			wprintf(L"%S = 0x%08x,\n", pParameter[i].mName, pParameter[i].mID);
	}

	// Release the list lock.
	LeaveCriticalSection(GetModulePointer<__int64, CRITICAL_SECTION>(g_sShaderSingletonInst, 8));
	return 0;
}

__int64 PrintShaderTechniques(WCHAR **argv, int argc)
{
	// Acquire the list lock.
	EnterCriticalSection(GetModulePointer<__int64, CRITICAL_SECTION>(g_sShaderSingletonInst, 8));

	// Get a pointer to the technique array.
	sShader::TechniqueDesc *pTechniques = GetModulePointer<__int64, sShader::TechniqueDesc>(g_sShaderSingletonInst, 0x6298);
	DWORD TechniqueCount = *GetModulePointer<__int64, DWORD>(g_sShaderSingletonInst, 0xB29C);

	// Loop and print all of the techniques.
	for (int i = 0; i < TechniqueCount; i++)
	{
		// Print the technique description.
		//wprintf(L"Technique %d:\tID=0x%08x Name=%S\n", i, pTechniques[i].mID, pTechniques[i].mName);

		if (pTechniques[i].mName == nullptr)
			wprintf(L"ShaderTechnique_%08x = 0x%08x,\n", pTechniques[i].mID, pTechniques[i].mID);
		else
			wprintf(L"%S = 0x%08x,\n", pTechniques[i].mName, pTechniques[i].mID);
	}

	// Release the list lock.
	LeaveCriticalSection(GetModulePointer<__int64, CRITICAL_SECTION>(g_sShaderSingletonInst, 8));
	return 0;
}