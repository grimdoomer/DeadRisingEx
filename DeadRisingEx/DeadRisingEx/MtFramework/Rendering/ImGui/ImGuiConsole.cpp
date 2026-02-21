
#include "ImGuiConsole.h"
#include "imgui_internal.h"
#include <MtFramework/Rendering/sRender.h>
#include "DeadRisingEx/MtFramework/Debug/sSnatcherToolImpl.h"
#include "DeadRisingEx/Utilities.h"
#include "DeadRisingEx/ModConfig.h"
#include <varargs.h>
#include <shellapi.h>
#include <locale>
#include <codecvt>

__int64 PrintCommandHelp(WCHAR **argv, int argc);

// Scratch buffers for ConsolePrint functions.
char sConsolePrintBuffer[1024];
WCHAR sConsolePrintUnicBuffer[1024];

// Table of commands for sRender objects.
const ConsoleCommandInfo g_sRenderCommands[] =
{
    { L"help", L"Displays available commands with descriptions", PrintCommandHelp },
};

int TextEditCallback(ImGuiInputTextCallbackData* data)
{
    // Handle text input.
    return ((ImGuiConsole*)data->UserData)->TextEditHandler(data);
}

ImGuiConsole::ImGuiConsole()
{
    ClearConsole();
    memset(this->InputBuf, 0, sizeof(this->InputBuf));
    this->HistoryPos = -1;

    this->AutoCompletePos = -1;
    this->DrawAutoComplete = false;
    this->AutoCompleteSuppressed = false;

    this->AutoScroll = true;
    this->ScrollToBottom = false;

    // Reserve history buffers up front.
    this->Items.reserve(ModConfig::Instance()->ConsoleHistoryLimit);
    this->History.reserve(HistoryBufferMaxLength);

    // Register the help command.
    RegisterCommands(g_sRenderCommands, ARRAYSIZE(g_sRenderCommands));
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
    WCHAR* pBuffer = sConsolePrintUnicBuffer;

    // Setup the string converter.
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

    // Acquire the list lock which also guards access to the static string scratch buffer.
    std::lock_guard<std::mutex> listLock(this->ConsoleLogMutex);

    // Fast path: assume the buffer is less than 1024 characters and try to format it.
    va_start(args, format);
    int length = _vsnwprintf(pBuffer, ARRAYSIZE(sConsolePrintUnicBuffer) - 1, format, args);
    if (length >= 1024)
    {
        // Not enough space in the buffer to print, alloc a suitable buffer.
        pBuffer = (WCHAR*)malloc((length + 1) * sizeof(WCHAR));
        if (pBuffer != nullptr)
        {
            // Reset the var args pointer.
            va_end(args);
            va_start(args, format);

            // Format the string.
            _vsnwprintf(pBuffer, length, format, args);
        }
    }
    
    if (length > 0 && pBuffer != nullptr)
    {
        // Make sure the string is null terminated.
        pBuffer[length] = (WCHAR)0;

        // If the console log exceeded the max history limit remove the oldest item.
        if (this->Items.size() >= ModConfig::Instance()->ConsoleHistoryLimit)
        {
            // Remove the oldest item and free the string buffer.
            char* pOldest = this->Items[0];
            this->Items.erase(this->Items.begin());

            free(pOldest);
        }

        // Add the string to the console line list.
        this->Items.push_back(Strdup(converter.to_bytes(pBuffer).c_str()));
    }

    // If we needed to allocate a scratch buffer free it.
    if (length >= 1024 && pBuffer != nullptr)
        free(pBuffer);

    // Cleanup the args.
    va_end(args);
}

void ImGuiConsole::ConsolePrint(const char *format, ...)
{
    va_list args;
    char* pBuffer = sConsolePrintBuffer;

    // Acquire the list lock which also guards access to the static string scratch buffer.
    std::lock_guard<std::mutex> listLock(this->ConsoleLogMutex);

    // Fast path: assume the buffer is less than 1024 characters and try to format it.
    va_start(args, format);
    int length = vsnprintf(pBuffer, sizeof(sConsolePrintBuffer) - 1, format, args);
    if (length >= 1024)
    {
        // Not enough space in the buffer to print, alloc a suitable buffer.
        pBuffer = (char*)malloc(length + 1);
        if (pBuffer != nullptr)
        {
            // Reset the var args pointer.
            va_end(args);
            va_start(args, format);

            // Format the string.
            vsnprintf(pBuffer, length, format, args);
        }
    }
    
    if (length > 0 && pBuffer != nullptr)
    {
        // Make sure the string is null terminated.
        pBuffer[length] = (char)0;

        // If the console log exceeded the max history limit remove the oldest item.
        if (this->Items.size() >= ModConfig::Instance()->ConsoleHistoryLimit)
        {
            // Remove the oldest item and free the string buffer.
            char* pOldest = this->Items[0];
            this->Items.erase(this->Items.begin());

            free(pOldest);
        }

        // Add the string to the console line list.
        this->Items.push_back(Strdup(pBuffer));
    }

    // If we needed to allocate a scratch buffer free it.
    if (length >= 1024 && pBuffer != nullptr)
        free(pBuffer);

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
    float cmdPosition;
    ImVec2 consoleSize;

    // Draw the console window.
    if (ImGui::Begin("ConsoleWindow", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar) == true)
    {
        // Set the size and position of the console window.
        SIZE windowSize = *(SIZE*)(((BYTE*)sRender::Instance()) + 0x8620);
        consoleSize = ImVec2((float)windowSize.cx, (float)windowSize.cy / 5.0f);
        ImGui::SetWindowSize(consoleSize);
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

        // Give the command line textbox focus if the window is opening.
        if (ImGui::IsWindowAppearing() && sSnatcherToolImpl::Instance()->IsVisible() == false)
            ImGui::SetKeyboardFocusHere(0);

        // Command line input:
        bool reclaimFocus = false;
        if (ImGui::InputText("##CommandLine", this->InputBuf, IM_ARRAYSIZE(this->InputBuf),
            ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory | ImGuiInputTextFlags_CallbackAlways | ImGuiInputTextFlags_IgnoreEscapeKey, TextEditCallback, this))
        {
            // Cleanup the input buffer and if it's valid handle the command.
            Strtrim(this->InputBuf);
            if (this->InputBuf[0])
                ExecuteCommand(this->InputBuf);

            // Clear the input buffer.
            this->InputBuf[0] = (char)0;

            // Flag to reclaim focus.
            reclaimFocus = true;
        }

        // If the command line box has focus and the user pressed the escape key, suppress the auto complete box.
        if (ImGui::IsItemActive() && this->DrawAutoComplete == true && ImGui::IsKeyPressed(VK_ESCAPE) == true)
            this->AutoCompleteSuppressed = true;

        // Check if we need to give the command box focus.
        if (reclaimFocus == true && sSnatcherToolImpl::Instance()->IsVisible() == false)
            ImGui::SetKeyboardFocusHere(-1);

        // Check if we should draw the auto complete window.
        if (this->DrawAutoComplete == true)
        {
            // Get the position and size of the command line input.
            cmdPosition = ImGui::GetCursorPosX();
        }
    }
    ImGui::End();

    // Check if we need to draw the auto complete window.
    if (this->DrawAutoComplete == true)
    {
        // Set the size and color of the auto complete window.
        ImGui::SetNextWindowBgAlpha(ImGui::GetStyleColorVec4(ImGuiCol_PopupBg).w * 0.60f);
        ImGui::SetNextWindowPos(ImVec2(cmdPosition, consoleSize.y));

        // Draw the auto complete window.
        ImGui::Begin("##AutoComplete", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_AlwaysAutoResize);
        {
            // Loop and draw all auto complete items.
            for (int i = 0; i < this->AutoCompleteCommands.size(); i++)
            {
                // Auto complete option:
                if (ImGui::Selectable(this->AutoCompleteCommands[i], this->AutoCompletePos == i) == true)
                    this->AutoCompletePos = i;
            }
        }
        ImGui::End();
    }
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
        // If the history buffer has exceeded the max number of items to hold remove the oldest item.
        if (this->History.size() >= this->HistoryBufferMaxLength)
        {
            // Remove the oldest element and free the string buffer.
            char* pOldest = this->History[0];
            this->History.erase(this->History.begin());

            free(pOldest);
        }

        // Add the new command to the history buffer.
        this->History.push_back(Strdup(psCommand));
    }
    this->HistoryPos = -1;

    // Close the auto complete window.
    this->DrawAutoComplete = false;
    this->AutoCompleteSuppressed = false;

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

void ImGuiConsole::InsertAutoCompleteOption(const char *pItem)
{
    // Setup our high and low pointers.
    int low = 0;
    int high = this->AutoCompleteCommands.size() - 1;

    // If there are no items in the list just add the item and bail out.
    if (this->AutoCompleteCommands.size() == 0)
    {
        // Add the item and return.
        this->AutoCompleteCommands.push_back(pItem);
        return;
    }

    // Get the length of the string to insert.
    int length = strlen(pItem);

    // Loop until we find a place to insert the command.
    while (true)
    {
        // If our search points are the same determine where to add the item.
        if (low == high)
        {
            // Check if we should insert before or after the current position.
            if (strncmp(pItem, this->AutoCompleteCommands[low], length) <= 0)
                this->AutoCompleteCommands.insert(&this->AutoCompleteCommands[low], pItem);
            else if (low == this->AutoCompleteCommands.size() - 1)
                this->AutoCompleteCommands.push_back(pItem);
            else
                this->AutoCompleteCommands.insert(&this->AutoCompleteCommands[low + 1], pItem);

            return;
        }
        else if (high == low + 1)
        {
            // Determine if we are inserting at high or low.
            if (strncmp(pItem, this->AutoCompleteCommands[low], length) <= 0)
                this->AutoCompleteCommands.insert(&this->AutoCompleteCommands[low], pItem);
            else if (strncmp(pItem, this->AutoCompleteCommands[high], length) <= 0)
                this->AutoCompleteCommands.insert(&this->AutoCompleteCommands[high], pItem);
            else if (high == this->AutoCompleteCommands.size() - 1)
                this->AutoCompleteCommands.push_back(pItem);
            else
                this->AutoCompleteCommands.insert(&this->AutoCompleteCommands[high + 1], pItem);

            return;
        }

        // Binary search between [low, high].
        int middle = low + ((high - low) / 2);

        // Check the string at the pivot point compared to the item to be inserted.
        if (strncmp(pItem, this->AutoCompleteCommands[middle], length) <= 0)
            high = middle;
        else
            low = middle;
    }
}

int ImGuiConsole::TextEditHandler(ImGuiInputTextCallbackData* data)
{
    // Check the event type and handle accordingly.
    switch (data->EventFlag)
    {
    case ImGuiInputTextFlags_CallbackAlways:
    {
        // Check if the text has changed since the last iteration.
        if (strcmp(this->LastCommandString.c_str(), data->Buf) != 0)
        {
            // The line has been modified so reset the history buffer position.
            this->HistoryPos = -1;
            this->LastCommandString = data->Buf;
        }

        // Clear the auto complete items list.
        this->AutoCompleteCommands.clear();
        this->DrawAutoComplete = false;

        // If the length of the input string is 0 don't process auto complete.
        if (data->BufTextLen == 0)
        {
            this->AutoCompletePos = -1;
            this->AutoCompleteSuppressed = false;
            return 0;
        }

        // Check if auto complete is suppressed and update suggestion list if not.
        if (this->AutoCompleteSuppressed == false)
        {
            // Loop and add any commands that match the entered text to the auto complete list.
            for (int i = 0; i < this->Commands.size(); i++)
            {
                // Check if the entered text matches the start of the command.
                if (strncmp(data->Buf, this->Commands[i], data->BufTextLen) == 0)
                {
                    // Insert the command into the auto complete list in a sorted manor.
                    InsertAutoCompleteOption(this->Commands[i]);
                    this->DrawAutoComplete = true;
                }
            }

            // Check if we need to reset the auto complete position.
            if (this->DrawAutoComplete == true && this->AutoCompletePos == -1)
                this->AutoCompletePos = 0;
        }
        break;
    }
    case ImGuiInputTextFlags_CallbackCompletion:
    {
        // If the auto complete index is valid auto complete the input.
        if (this->AutoCompletePos != -1)
        {
            // Auto complete the input.
            data->DeleteChars(0, data->BufTextLen);
            data->InsertChars(0, this->AutoCompleteCommands[this->AutoCompletePos]);

            // Reset the auto complete state.
            this->DrawAutoComplete = false;
            this->AutoCompletePos = -1;
        }
        break;
    }
    case ImGuiInputTextFlags_CallbackHistory:
    {
        // Check if we should update the history buffer or auto complete.
        if (this->DrawAutoComplete == true)
        {
            // Check the key pressed and handle accordingly.
            if (data->EventKey == ImGuiKey_UpArrow)
            {
                // If possible move up in the auto complete list.
                if (this->AutoCompletePos > 0)
                    this->AutoCompletePos--;
            }
            else if (data->EventKey == ImGuiKey_DownArrow)
            {
                // If possible move down in the auto complete list.
                if (this->AutoCompletePos < this->AutoCompleteCommands.size() - 1)
                    this->AutoCompletePos++;
            }
        }
        else
        {
            // If the history buffer position is not set, set it to the end of the history buffer.
            if (this->HistoryPos == -1)
            {
                // If the history buffer is not empty and the up key was pressed set the position to the end of the history buffer.
                if (this->History.size() > 0 && data->EventKey == ImGuiKey_UpArrow)
                    this->HistoryPos = this->History.size() - 1;
                else
                    return 0;
            }
            else
            {
                // Check the key pressed and handle accordingly.
                if (data->EventKey == ImGuiKey_UpArrow)
                {
                    // If possible move up in the history buffer.
                    if (this->HistoryPos > 0)
                        this->HistoryPos--;
                }
                else if (data->EventKey == ImGuiKey_DownArrow)
                {
                    // If possible move down in the history buffer.
                    if (this->History.size() > 0 && this->HistoryPos < this->History.size() - 1)
                        this->HistoryPos++;
                }
            }

            // Replace the current line with the command in the history buffer.
            data->DeleteChars(0, data->BufTextLen);
            data->InsertChars(0, this->History[this->HistoryPos]);

            // Reset the last command string so the history buffer position does not get reset.
            this->LastCommandString = this->History[this->HistoryPos];
        }
        break;
    }
    }

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
        // Loop through all of the commands and print each one.
        for (auto iter = this->CommandInfo.begin(); iter != CommandInfo.end(); iter++)
        {
            // Format the command text.
            int length = max(0, 32 - lstrlenW(iter->second->psName));
            ConsolePrint(L"\t%s %*s\n", iter->second->psName, length, iter->second->psHelpMessage);
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