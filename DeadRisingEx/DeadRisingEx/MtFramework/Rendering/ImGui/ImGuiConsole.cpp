
#include "ImGuiConsole.h"
#include <MtFramework/Rendering/sRender.h>
#include "DeadRisingEx/Utilities.h"
#include <varargs.h>
#include <shellapi.h>
#include <locale>
#include <codecvt>

__int64 PrintCommandHelp(WCHAR **argv, int argc);

// Table of commands for sRender objects.
const ConsoleCommandInfo g_sRenderCommands[] =
{
    { L"help", L"Displays available commands with descriptions", PrintCommandHelp },
};

int TextEditCallback(ImGuiInputTextCallbackData* data)
{
    // Handle text input.
    return ((ImGuiConsole*)data)->TextEditHandler(data);
}

ImGuiConsole::ImGuiConsole()
{
    ClearConsole();
    memset(this->InputBuf, 0, sizeof(this->InputBuf));
    this->HistoryPos = -1;

    this->AutoScroll = true;
    this->ScrollToBottom = false;
}

ImGuiConsole::~ImGuiConsole()
{
    // Clear the console line buffers.
    ClearConsole();

    // Loop and free all the history string buffers.
    for (int i = 0; i < this->History.size(); i++)
        free(this->History[i]);

    // Clear the history string list.
    this->History.clear();
}

void ImGuiConsole::ConsolePrint(const WCHAR *format, ...)
{
    va_list args;
    WCHAR Buffer[1024];

    // Setup the string converter.
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

    // Fast path: assume the buffer is less than 2014 characters and try to format it.
    va_start(args, format);
    int length = _vsnwprintf(Buffer, ARRAYSIZE(Buffer) - 1, format, args);
    if (length >= 1024)
    {
        // Not enough space in the buffer to print, alloc a suitable buffer.
        WCHAR *pBuffer = (WCHAR*)malloc((length + 1) * sizeof(WCHAR));
        if (pBuffer != nullptr)
        {
            // Reset the var args pointer.
            va_end(args);
            va_start(args, format);

            // Format the string.
            _vsnwprintf(pBuffer, length, format, args);

            // Add the string to the console line list.
            std::lock_guard<std::mutex> listLock(this->ConsoleLogMutex);
            this->Items.push_back(Strdup(converter.to_bytes(pBuffer).c_str()));
        }
    }
    else if (length > 0)
    {
        // Add the string to the console line list.
        std::lock_guard<std::mutex> listLock(this->ConsoleLogMutex);
        Buffer[length] = (WCHAR)0;
        this->Items.push_back(Strdup(converter.to_bytes(Buffer).c_str()));
    }

    // Cleanup the args.
    va_end(args);
}

void ImGuiConsole::ConsolePrint(const char *format, ...)
{
    va_list args;
    char Buffer[1024];

    // Fast path: assume the buffer is less than 2014 characters and try to format it.
    va_start(args, format);
    int length = vsnprintf(Buffer, sizeof(Buffer) - 1, format, args);
    if (length >= 1024)
    {
        // Not enough space in the buffer to print, alloc a suitable buffer.
        char *pBuffer = (char*)malloc(length + 1);
        if (pBuffer != nullptr)
        {
            // Reset the var args pointer.
            va_end(args);
            va_start(args, format);

            // Format the string.
            vsnprintf(pBuffer, length, format, args);

            // Add the string to the console line list.
            std::lock_guard<std::mutex> listLock(this->ConsoleLogMutex);
            this->Items.push_back(pBuffer);
        }
    }
    else if (length > 0)
    {
        // Add the string to the console line list.
        std::lock_guard<std::mutex> listLock(this->ConsoleLogMutex);
        Buffer[length] = (char)0;
        this->Items.push_back(Strdup(Buffer));
    }

    // Cleanup the args.
    va_end(args);
}

void ImGuiConsole::ClearConsole()
{
    // Loop and free all console line buffers.
    std::lock_guard<std::mutex> listLock(this->ConsoleLogMutex);
    for (int i = 0; i < this->Items.size(); i++)
        free(this->Items[i]);

    // Clear the list.
    this->Items.clear();
}

void ImGuiConsole::Draw()
{
    // Draw the console window.
    if (ImGui::Begin("ConsoleWindow", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar) == true)
    {
        // Set the size and position of the console window.
        SIZE windowSize = *(SIZE*)(((BYTE*)sRender::Instance()) + 0x8620);
        ImGui::SetWindowSize(ImVec2((float)windowSize.cx, (float)windowSize.cy / 5.0f));
        ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));

        // Reserve enough space for the line seperator and command textbox.
        float footerSpace = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
        ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footerSpace), false);

        // Reduce line spacing between items.
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

        {
            // Acquire the list lock.
            std::lock_guard<std::mutex> listLock(this->ConsoleLogMutex);

            // Loop and print all the console lines.
            for (int i = 0; i < this->Items.size(); i++)
            {
                // TODO: Filtering

                // TODO: Per-line coloring

                ImGui::TextUnformatted(this->Items[i]);
            }
        }

        // Check if we should scroll to the bottom of the log.
        if (this->ScrollToBottom == true || (this->AutoScroll == true && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
            ImGui::SetScrollHereY(1.0f);
        this->ScrollToBottom = false;

        // End the console log region.
        ImGui::PopStyleVar();
        ImGui::EndChild();
        ImGui::Separator();

        // Command line input:
        if (ImGui::InputText("CommandLine", this->InputBuf, IM_ARRAYSIZE(this->InputBuf),
            ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory, TextEditCallback, this))
        {
            // Cleanup the input buffer and if it's valid handle the command.
            Strtrim(this->InputBuf);
            if (this->InputBuf[0])
                ExecuteCommand(this->InputBuf);

            // Clear the input buffer.
            this->InputBuf[0] = (char)0;
        }
    }
    ImGui::End();
}

void ImGuiConsole::ExecuteCommand(const char *psCommand)
{
    // Add the command to the console log.
    ConsolePrint(psCommand);

    // If the history buffer index is valid, push that command to the buttom of the history buffer, otherwise add the command to the end of the history buffer.
    if (this->HistoryPos != -1)
    {
        // Remove the old entry.
        char *pOldCommand = this->History[this->HistoryPos];
        this->History.erase(this->History.begin() + this->HistoryPos);
        this->History.push_back(pOldCommand);
    }
    else
    {
        // Add the new command to the history buffer.
        this->History.push_back(Strdup(psCommand));
    }
    this->HistoryPos = -1;

    // Setup the string converter.
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring unicCommandLine = converter.from_bytes(psCommand);

    // Parse the command line arguments.
    int argc = 0;
    WCHAR **argv = CommandLineToArgvW(unicCommandLine.c_str(), &argc);

    // Find the command info structure for the command.
    if (this->CommandInfo.find(argv[0]) == this->CommandInfo.end())
    {
        // Unknown command.
        ConsolePrint("Unknown command '%S'", argv[0]);
    }
    else
    {
        // Call the command handler.
        const ConsoleCommandInfo *pCommandInfo = this->CommandInfo[argv[0]];
        pCommandInfo->pHandlerFunction(&argv[1], argc - 1);
    }
}

int ImGuiConsole::TextEditHandler(ImGuiInputTextCallbackData* data)
{
    return 0;
}

void ImGuiConsole::RegisterCommand(const ConsoleCommandInfo *pCommand)
{
    ImGuiConsole::Instance()->RegisterCommands(pCommand, 1);
}

void ImGuiConsole::RegisterCommands(const ConsoleCommandInfo *pCommands, int count)
{
    // Setup the string converter.
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

    // Loop and add each command to the commands list.
    for (int i = 0; i < count; i++)
    {
        // Check if a command with the same name has already been registered.
        if (this->CommandInfo.find(pCommands[i].psName) != this->CommandInfo.end())
        {
            // A command with this name has already been registered.
            DbgPrint("A command with the name '%S' has already been registered!\n", pCommands[i].psName);
            continue;
        }

        // Add the command to the commands list.
        this->CommandInfo.emplace(pCommands[i].psName, &pCommands[i]);
        this->Commands.push_back(Strdup(converter.to_bytes(pCommands[i].psName).c_str()));
    }
}

void ImGuiConsole::PrintCommandHelp(const char *psCommandName)
{
    // If no command is provided print descriptions for all commands.
    if (psCommandName == nullptr)
    {
        // Loop and print info for all commands.
        for (auto iter = this->CommandInfo.begin(); iter != this->CommandInfo.end(); iter++)
        {
            // Print the command info.
            int length = min(0, 32 - lstrlenW(iter->first.c_str()));
            ConsolePrint("\t%S %*S\n", iter->first, length, iter->second->psHelpMessage);
        }
    }
    else
    {
        // TODO: Print help specific to this command.
    }
}

__int64 PrintCommandHelp(WCHAR **argv, int argc)
{
    // Setup the string converter.
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

    // If an argument was provided convert it to ascii.
    if (argc > 0)
    {
        // Convert the argument to ascii.
        std::string sHelpArg = converter.to_bytes(argv[0]);
        ImGuiConsole::Instance()->PrintCommandHelp(sHelpArg.c_str());
    }
    else
    {
        // Print help for all commands.
        ImGuiConsole::Instance()->PrintCommandHelp();
    }

    return 0;
}