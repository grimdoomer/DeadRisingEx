
#pragma once
#include "imgui.h"
#include <Windows.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unordered_map>
#include <mutex>

// Function prototype for command handlers:
typedef __int64(__stdcall *ConsoleCommandHandlerFunc)(WCHAR **argv, int argc);

// Console command structure:
struct ConsoleCommandInfo
{
    const WCHAR *psName;
    const WCHAR *psHelpMessage;
    ConsoleCommandHandlerFunc pHandlerFunction;
};

class ImGuiConsole
{
    friend int TextEditCallback(ImGuiInputTextCallbackData* data);

protected:

    char                    InputBuf[256];
    ImVector<char*>         Items;

    ImVector<const char*>   Commands;
    std::unordered_map<std::wstring, const ConsoleCommandInfo*> CommandInfo;

    ImVector<char*>         History;
    int                     HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.

    ImVector<const char*>   AutoCompleteCommands;
    int                     AutoCompletePos;
    bool                    DrawAutoComplete;

    ImGuiTextFilter         Filter;
    bool                    AutoScroll;
    bool                    ScrollToBottom;
    std::mutex              ConsoleLogMutex;

    int TextEditHandler(ImGuiInputTextCallbackData* data);

    void ExecuteCommand(const char *psCommand);

    void InsertAutoCompleteOption(const char *pItem);

public:

    static ImGuiConsole * Instance()
    {
        static ImGuiConsole *pInstance = nullptr;

        // If the singleton has not been initialized create it now.
        if (pInstance == nullptr)
        {
            pInstance = new ImGuiConsole();
        }

        // Return the singleton instance.
        return pInstance;
    }

    ImGuiConsole();
    ~ImGuiConsole();

    void ConsolePrint(const WCHAR *format, ...);
    void ConsolePrint(const char *format, ...);
    void ClearConsole();

    void PrintCommandHelp(const char *psCommandName = nullptr);

    void Draw();

    void RegisterCommand(const ConsoleCommandInfo *pCommand);
    void RegisterCommands(const ConsoleCommandInfo *pCommands, int count);

    // Portable helpers
    static int   Stricmp(const char* s1, const char* s2) { int d; while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; } return d; }
    static int   Strnicmp(const char* s1, const char* s2, int n) { int d = 0; while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; n--; } return d; }
    static char* Strdup(const char* s) { IM_ASSERT(s); size_t len = strlen(s) + 1; void* buf = malloc(len); IM_ASSERT(buf); return (char*)memcpy(buf, (const void*)s, len); }
    static void  Strtrim(char* s) { char* str_end = s + strlen(s); while (str_end > s && str_end[-1] == ' ') str_end--; *str_end = 0; }
};