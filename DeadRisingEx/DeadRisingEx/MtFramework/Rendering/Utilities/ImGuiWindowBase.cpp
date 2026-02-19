
#include "ImGuiWindowBase.h"

ImGuiWindowBase::ImGuiWindowBase(std::string windowId, std::string title)
{
    this->windowId = windowId;
    this->title = title;
}

bool ImGuiWindowBase::DrawWindow()
{
    // If the window isn't open bail out.
    if (this->isOpen == false)
    {
        this->isFocused = false;
        return false;
    }

    // Calculate extra flags for UI settings.
    ImGuiWindowFlags extraFlags = ImGuiWindowFlags_None;
    if (this->ShowTitle == false)
        extraFlags |= ImGuiWindowFlags_NoTitleBar;
    if (this->IgnoreSavedSettings == true)
        extraFlags |= ImGuiWindowFlags_NoSavedSettings;
    if (this->Collapsible == false)
        extraFlags |= ImGuiWindowFlags_NoCollapse;
    if (this->AllowResize == false)
        extraFlags |= ImGuiWindowFlags_NoResize;
    if (this->AutoResize == true)
        extraFlags |= ImGuiWindowFlags_AlwaysAutoResize;
    if (this->MenuBar == true)
        extraFlags |= ImGuiWindowFlags_MenuBar;

    // If auto resize is disabled set the window size.
    if (this->AutoResize == false && this->size.x != 0 && this->size.y != 0)
        ImGui::SetNextWindowSize(this->size, ImGuiCond_Appearing);

    // Call the early draw function.
    DrawWindowEarly();

    // Draw the window.
    std::string windowTitleAndId = this->title + "###" + this->windowId;
    if (ImGui::Begin(windowTitleAndId.c_str(), &this->isOpen, extraFlags) == true)
    {
        // Update the size of the window.
        this->size = ImGui::GetWindowSize();

        // TODO: menu bar

        // TODO size and position

        // Draw the dialog body.
        DrawWindowBody();

        // Update window focused state.
        this->isFocused = ImGui::IsWindowFocused();
    }
    ImGui::End();

    return true;
}

bool ImGuiWindowBase::DrawWindowEarly()
{
    return true;
}