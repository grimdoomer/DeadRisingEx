

#include "sShaderImpl.h"
#include <MtFramework/Rendering/sShader.h>
#include <stdio.h>
#include <string>

void *g_sShaderSingletonInst = (void*)0x141D179A0;

// Forward declarations for command functions.
__int64 PrintShaderParameters(WCHAR **argv, int argc);
__int64 PrintShaderTechniques(WCHAR **argv, int argc);

// Table of commands for sShader objects.
const int g_sShaderCommandsLength = 2;
const ConsoleCommandInfo g_sShaderCommands[g_sShaderCommandsLength] =
{
    { L"sShader.PrintShaderParameters", L"Lists all sShader parameter descriptors", PrintShaderParameters },
    { L"sShader.PrintShaderTechniques", L"Lists all sShader techniques", PrintShaderTechniques }
};

void sShaderImpl::RegisterTypeInfo()
{
    // Register commands.
    ImGuiConsole::Instance()->RegisterCommands(g_sShaderCommands, g_sShaderCommandsLength);
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
        //ImGuiConsole::Instance()->ConsolePrint(L"Parameter %d\n", i);
        //ImGuiConsole::Instance()->ConsolePrint(L"\tName: %S\n", pParameter[i].mName);
        //ImGuiConsole::Instance()->ConsolePrint(L"\tID: 0x%08x\tType: %d\tRegCount: %d\tIndex: %d\n", pParameter[i].mID, pParameter[i].Type, pParameter[i].RegCount, pParameter[i].SomethingIndex);
        //
        //// Check the value type and print accordingly.
        //switch (pParameter[i].Type)
        //{
        //case 0: ImGuiConsole::Instance()->ConsolePrint(L"\tValue: %d\n", pParameter[i].SomethingIndex); break;
        //case 1: ImGuiConsole::Instance()->ConsolePrint(L"\tValue: %s\n", pParameter[i].IntValue == 0 ? L"False" : L"True"); break;
        //case 2: ImGuiConsole::Instance()->ConsolePrint(L"\tValue: %d\n", pParameter[i].IntValue); break;
        //case 3: ImGuiConsole::Instance()->ConsolePrint(L"\tValue: %f\n", (float)pParameter[i].IntValue); break;
        //case 4: ImGuiConsole::Instance()->ConsolePrint(L"\tValue: %p\n", pParameter[i].pValue); break;
        //}

        //ImGuiConsole::Instance()->ConsolePrint(L"\n");
        if (pParameter[i].mName == nullptr)
            ImGuiConsole::Instance()->ConsolePrint(L"ShaderParameter_%08x = 0x%08x,\n", pParameter[i].mID, pParameter[i].mID);
        else
            ImGuiConsole::Instance()->ConsolePrint(L"%S = 0x%08x,\n", pParameter[i].mName, pParameter[i].mID);
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
        //ImGuiConsole::Instance()->ConsolePrint(L"Technique %d:\tID=0x%08x Name=%S\n", i, pTechniques[i].mID, pTechniques[i].mName);

        if (pTechniques[i].mName == nullptr)
            ImGuiConsole::Instance()->ConsolePrint(L"ShaderTechnique_%08x = 0x%08x,\n", pTechniques[i].mID, pTechniques[i].mID);
        else
            ImGuiConsole::Instance()->ConsolePrint(L"%S = 0x%08x,\n", pTechniques[i].mName, pTechniques[i].mID);
    }

    // Release the list lock.
    LeaveCriticalSection(GetModulePointer<__int64, CRITICAL_SECTION>(g_sShaderSingletonInst, 8));
    return 0;
}