
#include "sSnatcherToolImpl.h"
#include <MtFramework/Game/sSnatcherMain.h>
#include <MtFramework/Debug/sToolBase.h>
#include <MtFramework/Rendering/sRender.h>
#include "DeadRisingEx/MtFramework/Rendering/ImGui/imgui.h"

#define FIELD_NAME_WIDTH 300.0f

void **g_sToolBaseInstance = (void**)GetModuleAddress(0x141CF3230);

// Forward declarations for command functions.
__int64 OpenDebugMenu(WCHAR **argv, int argc);

// Table of commands.
const int g_sSnatcherToolCommandsLength = 1;
const ConsoleCommandInfo g_sSnatcherToolCommands[g_sSnatcherToolCommandsLength] =
{
    { L"open_debug_menu", L"Opens the in-game debug menu", OpenDebugMenu },
};

void sSnatcherToolImpl::RegisterTypeInfo()
{
    // Register commands:
    ImGuiConsole::Instance()->RegisterCommands(g_sSnatcherToolCommands, g_sSnatcherToolCommandsLength);
}

template<typename T> T GetFieldValue(MtPropertyListEntry *pEntry)
{
    // Check if the field uses a callback function or not.
    T value;
    if ((pEntry->Flags & 0x80000) != 0)
    {
        // TODO: callback function
    }
    else
    {
        // If the address is not null read the value.
        if (pEntry->pFieldValue != nullptr)
            value = *(T*)pEntry->pFieldValue;
    }

    // Return the field value.
    return value;
}

void sSnatcherToolImpl::Draw()
{
    // If the menu is hidden bail out.
    if (this->sysMenuOpen == false)
        return;

    // Draw the sysmenu window.
    if (ImGui::Begin("SysMenu", &this->sysMenuOpen))
    {
        // TODO: Set window position and size

        // Get the starting link pointer.
        MtPropertyListEntry *pEntry = this->propertyList.GetFirstNode();

        // Loop and draw all the menu options.
        bool inSection = false;
        while (pEntry != nullptr)
        {
            // Check the property type and handle accordingly.
            switch (pEntry->PropertyType)
            {
            case MT_PROP_TYPE_SUB_SECTION_START:
            {
                // Check if we need to close the child group for a previously open section.
                if (inSection == true)
                {
                    // End the sub section.
                    ImGui::PopID();
                    ImGui::EndGroup();
                    inSection = false;
                }

                // Create a collapsable group for the section.
                if (ImGui::CollapsingHeader(pEntry->pPropertyName) == true)
                {
                    // Create a child group for the sub section.
                    ImGui::BeginGroup();
                    ImGui::PushID(pEntry);
                    inSection = true;
                }
                else
                {
                    // The header is closed, skip all inner elements.
                    pEntry = pEntry->pBLink;
                    while (pEntry != nullptr && pEntry->PropertyType != MT_PROP_TYPE_SUB_SECTION_END && pEntry->PropertyType != MT_PROP_TYPE_SUB_SECTION_START)
                        pEntry = pEntry->pBLink;

                    // If we hit a start section element don't advance the link pointer.
                    if (pEntry != nullptr && pEntry->PropertyType == MT_PROP_TYPE_SUB_SECTION_START)
                        continue;
                }
                break;
            }
            case MT_PROP_TYPE_SUB_SECTION_END:
            {
                // If the section header is open end the section.
                if (inSection == true)
                {
                    // End the sub section.
                    ImGui::PopID();
                    ImGui::EndGroup();
                    inSection = false;
                }
                break;
            }

            case MT_PROP_TYPE_BOOL:
            {
                // Draw the field name.
                ImGui::Text(pEntry->pPropertyName);
                ImGui::SameLine(FIELD_NAME_WIDTH);

                // Get the field value and display it.
                bool value = (bool)pEntry->GetValueByte();
                ImGui::PushID(pEntry);
                if (ImGui::Checkbox("", &value) == true)
                {
                    // TODO:
                }
                ImGui::PopID();
                break;
            }
            case MT_PROP_TYPE_BYTE:
            case MT_PROP_TYPE_WORD:
            case MT_PROP_TYPE_DWORD:
            case MT_PROP_TYPE_QWORD:
            case MT_PROP_TYPE_SBYTE:
            case MT_PROP_TYPE_SWORD:
            case MT_PROP_TYPE_SINT:
            case MT_PROP_TYPE_LONGLONG:
            case MT_PROP_TYPE_FLOAT:
            case MT_PROP_TYPE_DOUBLE:
            {
                // Draw the field name.
                ImGui::Text(pEntry->GetDisplayName());
                if (ImGui::IsItemHovered() == true)
                {
                    // Display the tool tip dialog.
                    ImGui::BeginTooltip();
                    ImGui::Text(pEntry->pPropertyName);
                    ImGui::EndTooltip();
                }
                ImGui::SameLine(FIELD_NAME_WIDTH);

                // Determine the imgui field type.
                ImGuiDataType dataType;
                switch (pEntry->PropertyType)
                {
                case MT_PROP_TYPE_BYTE: dataType = ImGuiDataType_U8; break;
                case MT_PROP_TYPE_WORD: dataType = ImGuiDataType_U16; break;
                case MT_PROP_TYPE_DWORD: dataType = ImGuiDataType_U32; break;
                case MT_PROP_TYPE_QWORD: dataType = ImGuiDataType_U64; break;
                case MT_PROP_TYPE_SBYTE: dataType = ImGuiDataType_S8; break;
                case MT_PROP_TYPE_SWORD: dataType = ImGuiDataType_S16; break;
                case MT_PROP_TYPE_SINT: dataType = ImGuiDataType_S32; break;
                case MT_PROP_TYPE_LONGLONG: dataType = ImGuiDataType_S64; break;
                case MT_PROP_TYPE_FLOAT: dataType = ImGuiDataType_Float; break;
                case MT_PROP_TYPE_DOUBLE: dataType = ImGuiDataType_Double; break;
                default: DebugBreak(); break;
                }

                // TODO: Handle array fields

                // Check if the field has a getter callback.
                if ((pEntry->Flags & MT_PROP_FLAG_USE_FUNCTION_POINTERS) != 0)
                {
                    // TODO:
                }
                else
                {
                    // Set the width of the input field.
                    ImGui::SetNextItemWidth(150.0f);

                    // Input field:
                    ImGui::PushID(pEntry);
                    ImGui::InputScalar("", dataType, pEntry->pFieldValue);
                    ImGui::PopID();
                }
                break;
            }
            default:
            {
                // If the property has a name draw it.
                if (pEntry->pPropertyName != nullptr)
                    ImGui::Text(pEntry->pPropertyName);
            }
            }

            // Next element.
            if (pEntry != nullptr)
                pEntry = pEntry->pBLink;
        }

        // Check if we need to close a previously open section.
        if (inSection == true)
        {
            // End the sub section.
            ImGui::PopID();
            ImGui::EndGroup();
            inSection = false;
        }
    }
    ImGui::End();
}

__int64 OpenDebugMenu(WCHAR **argv, int argc)
{
    // Get the sSnatcherToolImpl instance.
    sSnatcherToolImpl *pSnatcherTool = sSnatcherToolImpl::Instance();

    // Build the root of the system menu.
    //sSnatcherMain::Instance()->RegisterDebugOptions(&pSnatcherTool->propertyList);
    sRender::Instance()->RegisterDebugOptions(&pSnatcherTool->propertyList);

    // Loop and print info on all elements in the property list.
    /*MtPropertyListEntry *pEntry = pSnatcherTool->propertyList.GetFirstNode();
    for (; pEntry != nullptr; pEntry = pEntry->pBLink)
    {
        DbgPrint("0x%02x 0x%02x %s\n", pEntry->PropertyType, pEntry->Flags, pEntry->pPropertyName);
    }*/

    // Display the debug menu.
    pSnatcherTool->sysMenuOpen = true;

    return 0;
}