
#pragma once
#include "DeadRisingEx.h"
#include <string>

class ImGuiWindowBase
{
protected:

    std::string title;                      // Title of the window
    std::string windowId;                   // Unique window id (so changing the window title doesn't create a new window)
    bool isOpen = true;                     // Indicates if the window is currently open or not
    bool isFocused;                         // Indicates if the window currently has focus
    ImVec2 size = ImVec2(-1, -1);           // Size of the window

    virtual bool DrawWindowEarly();
    virtual void DrawWindowBody() = 0;

public:

    ImGuiWindowBase(std::string windowId, std::string title);

    virtual bool DrawWindow();

    bool ShowTitle = true;                  // True if the title bar should be drawn or false to hide it
    bool AllowResize = true;                // True if the window can be resized or false otherwise
    bool AutoResize = false;                // True if the window should auto resize to fit the contents or false otherwise
    bool IgnoreSavedSettings = true;        // True if saved window settings should be ignored or false to use them
    bool Collapsible = true;                // True if the window can be collapsed, false otherwise
    bool MenuBar = false;                   // True if the window should render a menu bar, false otherwise

    std::string GetTitle()                  { return this->title; }
    std::string GetWindowId()               { return this->windowId; }
    bool GetIsOpen()                        { return this->isOpen; }
    bool GetIsFocused()                     { return this->isFocused; }

    ImVec2 GetSize()                        { return this->size; }
    void SetSize(ImVec2 size)               { this->size = size; this->AutoResize = false; }
};