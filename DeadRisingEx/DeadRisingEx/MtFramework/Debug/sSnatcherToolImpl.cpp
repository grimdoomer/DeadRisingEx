
#include "sSnatcherToolImpl.h"
#include <MtFramework/Archive/cResource.h>
#include <MtFramework/Area/sAreaManager.h>
#include <MtFramework/Game/sSnatcherMain.h>
#include <MtFramework/Debug/sToolBase.h>
#include <MtFramework/Rendering/sRender.h>
#include "DeadRisingEx/MtFramework/Rendering/ImGui/imgui.h"
#include "DeadRisingEx/MtFramework/Rendering/ImGui/imgui_internal.h"

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

template<typename T> void SetFieldValue(MtPropertyListEntry *pEntry, T value)
{
    // Check if this is an array or not and handle accordingly.
    if ((pEntry->Flags & MT_PROP_FLAG_IS_ARRAY) != 0)
    {
        // Check if the property uses pointers for accessing the value.
        if ((pEntry->Flags & MT_PROP_FLAG_USE_FUNCTION_POINTERS) != 0)
        {
            // Is this used for non-bool values?
            if (pEntry->PropertyType != MT_PROP_TYPE_BOOL)
                DebugBreak();

            // Use the set field value callback.
            pEntry->pSetFieldValue(pEntry->pOwnerObject, (__int64)pEntry->pUnkFunc3);
        }
        else
        {
            // Set the value directly.
            *((T*)pEntry->pFieldValue + (int)pEntry->pUnkFunc3) = value;
        }
    }
    else
    {
        // Check if the property uses pointers for accessing the value.
        if ((pEntry->Flags & MT_PROP_FLAG_USE_FUNCTION_POINTERS) != 0)
        {
            // Use the set field value callback.
            pEntry->pSetFieldValue(pEntry->pOwnerObject, (__int64)value);
        }
        else
        {
            // Set the value directly.
            *(T*)pEntry->pFieldValue = value;
        }
    }
}

void SetVectorValue(MtPropertyListEntry *pEntry, float *pValue, int count)
{
    // Check if this is an array or not and handle accordingly.
    if ((pEntry->Flags & MT_PROP_FLAG_IS_ARRAY) != 0)
    {
        // Check if the property uses pointers for accessing the value.
        if ((pEntry->Flags & MT_PROP_FLAG_USE_FUNCTION_POINTERS) != 0)
        {
            // Use the set field value callback.
            DebugBreak();
            pEntry->pSetFieldValue(pEntry->pOwnerObject, (__int64)pEntry->pUnkFunc3);
        }
        else
        {
            // Set the value directly.
            for (int i = 0; i < count; i++)
                *((float*)pEntry->pFieldValue + (int)pEntry->pUnkFunc3 + i) = pValue[i];
        }
    }
    else
    {
        // Check if the property uses pointers for accessing the value.
        if ((pEntry->Flags & MT_PROP_FLAG_USE_FUNCTION_POINTERS) != 0)
        {
            // Use the set field value callback.
            pEntry->pSetFieldValue(pEntry->pOwnerObject, (__int64)pValue);
        }
        else
        {
            // Set the value directly.
            for (int i = 0; i < count; i++)
                *((float*)pEntry->pFieldValue + i) = pValue[i];
        }
    }
}

void sSnatcherToolImpl::DrawSection(MtPropertyListEntry *pEntry)
{
    // Loop and draw all the menu options.
    bool inSection = false;
    while (pEntry != nullptr)
    {
        // If we are inside a collapsable section indent the nested items.
        if (inSection == true)
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 6.0f);

        // Check the property type and handle accordingly.
        switch (pEntry->PropertyType)
        {
        case MT_PROP_TYPE_SUB_SECTION_START:
        case MT_PROP_TYPE_SUB_SECTION_START2:
        {
            // Check if we need to close the child group for a previously open section.
            if (inSection == true)
            {
                // End the sub section.
                ImGui::EndGroup();
                inSection = false;
            }

            // Create a collapsable group for the section.
            ImGui::PushID(pEntry);
            bool expanded = ImGui::CollapsingHeader(pEntry->pPropertyName);
            ImGui::PopID();

            // Check if the section is expanded or not.
            if (expanded == true)
            {
                // Create a child group for the sub section.
                ImGui::BeginGroup();
                inSection = true;
            }
            else
            {
                // The header is closed, skip all inner elements.
                pEntry = pEntry->pBLink;
                while (pEntry != nullptr && pEntry->PropertyType != MT_PROP_TYPE_SUB_SECTION_END && pEntry->PropertyType != MT_PROP_TYPE_SUB_SECTION_END2 && 
                    pEntry->PropertyType != MT_PROP_TYPE_SUB_SECTION_START && pEntry->PropertyType != MT_PROP_TYPE_SUB_SECTION_START2)
                    pEntry = pEntry->pBLink;

                // If we hit a start section element don't advance the link pointer.
                if (pEntry != nullptr && (pEntry->PropertyType == MT_PROP_TYPE_SUB_SECTION_START || pEntry->PropertyType == MT_PROP_TYPE_SUB_SECTION_START2))
                    continue;
            }
            break;
        }
        case MT_PROP_TYPE_SUB_SECTION_END:
        case MT_PROP_TYPE_SUB_SECTION_END2:
        {
            // If the section header is open end the section.
            if (inSection == true)
            {
                // End the sub section.
                ImGui::EndGroup();
                inSection = false;
            }
            break;
        }

        case MT_PROP_TYPE_POINTER1:
        case MT_PROP_TYPE_POINTER2:
        {
            // Check if the pointer is an array or not.
            if ((pEntry->Flags & MT_PROP_FLAG_IS_ARRAY) != 0)
            {
                // Draw the field label.
                ImGui::Text(MtPropertyListEntry::GetDisplayName(pEntry->pPropertyName));

                // Save the cursor position.
                ImVec2 cursorBegin = ImGui::GetCursorPos();

                // Create a group for the combo box.
                float startPos = ImGui::GetCursorPosX() + 6.0f;
                ImGui::PushID(pEntry);
                ImGui::BeginGroup();
                {
                    ImGui::Dummy(ImVec2(0.0f, 5.0f));

                    // Get the length of the array.
                    int length = pEntry->GetArrayLength();
                    if (length > 0)
                    {
                        // Get the object value for the selected index.
                        MtObject *pObject = pEntry->GetValueMtObject();

                        // Format the combo box display label.
                        CHAR sComboPreview[128];
                        if (pObject != nullptr)
                            snprintf(sComboPreview, sizeof(sComboPreview), "[%d] %s", pEntry->pUnkFunc3, MtPropertyListEntry::GetDisplayName(pObject->GetDTI()->pObjectName));
                        else
                            snprintf(sComboPreview, sizeof(sComboPreview), "[%d] null", pEntry->pUnkFunc3);

                        // Draw the combo box:
                        ImGui::SetCursorPosX(startPos);
                        ImGui::SetNextItemWidth(200.0f);
                        if (ImGui::BeginCombo("##combobox", sComboPreview) == true)
                        {
                            // Save the old array index.
                            int oldIndex = (int)pEntry->pUnkFunc3;

                            // Loop and draw all elements in the array.
                            for (int i = 0; i < length; i++)
                            {
                                // Get the object for this entry.
                                pEntry->pUnkFunc3 = (void*)i;
                                MtObject *pArrayEntry = pEntry->GetValueMtObject();
                                
                                // Format the display name for this entry.
                                CHAR sOptionName[128];
                                if (pArrayEntry != nullptr)
                                    snprintf(sOptionName, sizeof(sOptionName), "[%d] %s", i, MtPropertyListEntry::GetDisplayName(pArrayEntry->GetDTI()->pObjectName));
                                else
                                    snprintf(sOptionName, sizeof(sOptionName), "[%d] null", i);

                                // Create the selectable:
                                if (ImGui::Selectable(sOptionName, i == oldIndex) == true)
                                {
                                    // If this is a different option than what was previously selected save the index.
                                    // Note: we don't update pObject because it's not possible to change the combo index and hit the jump button in the same frame.
                                    if (i != oldIndex)
                                        oldIndex = i;
                                }
                            }

                            // Restore the array index.
                            pEntry->pUnkFunc3 = (void*)oldIndex;

                            ImGui::EndCombo();
                        }

                        // If the object is not null draw the jump button.
                        if (pObject != nullptr)
                        {
                            // Draw the jump button:
                            ImGui::SameLine(FIELD_NAME_WIDTH);
                            if (ImGui::Button(MtPropertyListEntry::GetDisplayName(pObject->GetDTI()->pObjectName)) == true)
                            {
                                // Format the nav string.
                                CHAR sNavString[128];
                                snprintf(sNavString, sizeof(sNavString), "%s [%d]", MtPropertyListEntry::GetDisplayName(pEntry->pPropertyName), pEntry->pUnkFunc3);

                                // Add an entry to the nav list.
                                this->sysMenuNavList.push_back(sNavString);

                                // Build the menu options list for this object.
                                MtPropertyList *pMenuOptions = (MtPropertyList*)malloc(sizeof(MtPropertyList));
                                MtPropertyList::_ctor(pMenuOptions);
                                pObject->RegisterDebugOptions(pMenuOptions);

                                // Add the options to the properties list.
                                this->sysMenuPropertyList.push_back(pMenuOptions);
                                this->sysMenuIndex++;
                            }
                        }
                    }
                    else
                    {
                        // Draw a disabled combo box.
                        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
                        ImGui::SetCursorPosX(startPos);
                        ImGui::SetNextItemWidth(200.0f);
                        ImGui::BeginCombo("##combobox", "empty array");
                        ImGui::PopItemFlag();
                    }

                    ImGui::Dummy(ImVec2(0.0f, 5.0f));
                }
                ImGui::EndGroup();
                ImGui::PopID();

                // Get the group size so we can draw the group box border.
                ImVec2 itemMin = ImGui::GetItemRectMin();
                ImVec2 itemMax = ImGui::GetItemRectMax();
                float frameHeight = ImGui::GetFrameHeight();

                // Adjust box width.
                itemMax.x += 6.0f;

                // Draw a rectangle for the group box border with rounded corners.
                ImVec2 halfFrame = ImVec2((frameHeight * 0.25f) * 0.5f, frameHeight * 0.5f);
                ImGui::GetWindowDrawList()->AddRect(itemMin, itemMax, ImGui::GetColorU32(ImGuiCol_Border), halfFrame.x);
            }
            else
            {
                // Draw the field name.
                ImGui::Text(MtPropertyListEntry::GetDisplayName(pEntry->pPropertyName));
                ImGui::SameLine(FIELD_NAME_WIDTH);

                // Get the object value and display it accordingly.
                MtObject *pObject = pEntry->GetValueMtObject();
                if (pObject == nullptr)
                {
                    ImGui::Text("null");
                }
                else
                {
                    // Nav button for object:
                    ImGui::PushID(pEntry);
                    if (ImGui::Button(MtPropertyListEntry::GetDisplayName(pObject->GetDTI()->pObjectName)) == true)
                    {
                        // Add an entry to the nav list.
                        this->sysMenuNavList.push_back(MtPropertyListEntry::GetDisplayName(pEntry->pPropertyName));

                        // Build the menu options list for this object.
                        MtPropertyList *pMenuOptions = (MtPropertyList*)malloc(sizeof(MtPropertyList));
                        MtPropertyList::_ctor(pMenuOptions);
                        pObject->RegisterDebugOptions(pMenuOptions);

                        // Add the options to the properties list.
                        this->sysMenuPropertyList.push_back(pMenuOptions);
                        this->sysMenuIndex++;
                    }
                    ImGui::PopID();
                }
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
                // Update the field value.
                SetFieldValue<bool>(pEntry, value);
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
            ImGui::Text(MtPropertyListEntry::GetDisplayName(pEntry->pPropertyName));
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

            // Get the field value.
            __int64 value = pEntry->GetPrimitiveFieldValue();

            // Set the width of the input field.
            ImGui::SetNextItemWidth(150.0f);

            // Input field:
            ImGui::PushID(pEntry);
            if (ImGui::InputScalar("", dataType, &value) == true)
            {
                // Check the field type and update accordingly.
                switch (pEntry->PropertyType)
                {
                case MT_PROP_TYPE_BYTE: SetFieldValue<BYTE>(pEntry, (BYTE)value); break;
                case MT_PROP_TYPE_WORD: SetFieldValue<WORD>(pEntry, (WORD)value); break;
                case MT_PROP_TYPE_DWORD: SetFieldValue<DWORD>(pEntry, (DWORD)value); break;
                case MT_PROP_TYPE_QWORD: SetFieldValue<__int64>(pEntry, value); break;
                case MT_PROP_TYPE_SBYTE: SetFieldValue<INT8>(pEntry, (INT8)value); break;
                case MT_PROP_TYPE_SWORD: SetFieldValue<INT16>(pEntry, (INT16)value); break;
                case MT_PROP_TYPE_SINT: SetFieldValue<INT32>(pEntry, (INT32)value); break;
                case MT_PROP_TYPE_LONGLONG: SetFieldValue<INT64>(pEntry, (INT64)value); break;
                case MT_PROP_TYPE_FLOAT: SetFieldValue<float>(pEntry, (float)value); break;
                case MT_PROP_TYPE_DOUBLE: SetFieldValue<double>(pEntry, (double)value); break;
                default: DebugBreak(); break;
                }
            }
            ImGui::PopID();
            break;
        }
        case MT_PROP_TYPE_MTSTRING:
        case MT_PROP_TYPE_STRING_PTR:
        {
            // Draw the field name.
            ImGui::Text(MtPropertyListEntry::GetDisplayName(pEntry->pPropertyName));
            if (ImGui::IsItemHovered() == true)
            {
                // Display the tool tip dialog.
                ImGui::BeginTooltip();
                ImGui::Text(pEntry->pPropertyName);
                ImGui::EndTooltip();
            }
            ImGui::SameLine(FIELD_NAME_WIDTH);

            // Get the string value.
            const char *pStringValue = pEntry->GetValueString();
            ImGui::Text(pStringValue);
            break;
        }
        //case 0x10:
        case 0x22:  // Vector2/POINT?
        case MT_PROP_TYPE_VECTOR3:
        case 0x23:  // 3 floats?
        case MT_PROP_TYPE_VECTOR4:
        case MT_PROP_TYPE_QUATERNION:
        case 0x24:  // 4 floats?
        {
            // Draw the field name.
            ImGui::Text(MtPropertyListEntry::GetDisplayName(pEntry->pPropertyName));
            if (ImGui::IsItemHovered() == true)
            {
                // Display the tool tip dialog.
                ImGui::BeginTooltip();
                ImGui::Text(pEntry->pPropertyName);
                ImGui::EndTooltip();
            }
            ImGui::SameLine(FIELD_NAME_WIDTH);

            // Get the field value.
            Vector4 vector;
            pEntry->GetValueVector(&vector);

            // Vector input:
            ImGui::PushID(pEntry);
            bool result;
            switch (pEntry->PropertyType)
            {
            case 0x10:
            case 0x22: result = ImGui::InputFloat2("", (float*)&vector); break;
            case MT_PROP_TYPE_VECTOR3:
            case 0x23: result = ImGui::InputFloat3("", (float*)&vector); break;
            case MT_PROP_TYPE_VECTOR4:
            case MT_PROP_TYPE_QUATERNION:
            case 0x24: result = ImGui::InputFloat4("", (float*)&vector); break;
            }
            ImGui::PopID();

            // Check if we need to update the field value.
            if (result == true)
            {
                // Check the field type and handle accordingly.
                switch (pEntry->PropertyType)
                {
                case 0x10:
                case 0x22: SetVectorValue(pEntry, (float*)&vector, 2); break;
                case MT_PROP_TYPE_VECTOR3:
                case 0x23: SetVectorValue(pEntry, (float*)&vector, 3); break;
                case MT_PROP_TYPE_VECTOR4:
                case MT_PROP_TYPE_QUATERNION:
                case 0x24: SetVectorValue(pEntry, (float*)&vector, 4); break;
                }
            }
            break;
        }
        case MT_PROP_TYPE_ACTION:
        {
            // Draw the action button:
            ImGui::PushID(pEntry);
            if (ImGui::Button(MtPropertyListEntry::GetDisplayName(pEntry->pPropertyName)) == true)
            {
                // If the action callback is valid call it.
                if (pEntry->pFieldValue != nullptr)
                    pEntry->pFieldValue(pEntry->pOwnerObject, 0);
            }
            ImGui::PopID();
            break;
        }
        case MT_PROP_TYPE_CRESOURCE:
        {
            // Draw the field name.
            ImGui::Text(MtPropertyListEntry::GetDisplayName(pEntry->pPropertyName));
            if (ImGui::IsItemHovered() == true)
            {
                // Display the tool tip dialog.
                ImGui::BeginTooltip();
                ImGui::Text(pEntry->pPropertyName);
                ImGui::EndTooltip();
            }
            ImGui::SameLine(FIELD_NAME_WIDTH);

            // Display the field value:
            cResource *pResource = (cResource*)pEntry->GetValueMtObject();
            ImGui::Text("%s", pResource != nullptr ? pResource->mPath : "None");
            break;
        }
        default:
        {
            // If the property has a name draw it.
            if (pEntry->pPropertyName != nullptr)
                ImGui::Text(pEntry->pPropertyName);

            // Handle tooltip:
            if (ImGui::IsItemHovered() == true)
            {
                // Display the tool tip dialog.
                ImGui::BeginTooltip();
                ImGui::Text("Unsupported field type: %d", pEntry->PropertyType);
                ImGui::EndTooltip();
            }
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
        ImGui::EndGroup();
        inSection = false;
    }
}

void sSnatcherToolImpl::Draw()
{
    // If the menu is hidden bail out.
    if (this->sysMenuOpen == false)
        return;

    // Check if the loaded area has changed since it was last opened.
    sAreaManager *pAreaManager = sAreaManager::Instance();
    if (*(DWORD*)((BYTE*)pAreaManager + 0x40) != this->lastAreaId)
    {
        // Pop all child menus until we reach the root menu entry.
        while (this->sysMenuPropertyList.size() > 1)
        {
            this->sysMenuPropertyList.pop_back();
            this->sysMenuNavList.pop_back();
        }

        // Set the menu index to the root.
        this->sysMenuIndex = 0;

        // Update the last area id.
        this->lastAreaId = *(DWORD*)((BYTE*)pAreaManager + 0x40);
    }

    // Setup the window title.
    std::string menuTitle = this->sysMenuNavList[0];
    for (int i = 1; i < this->sysMenuNavList.size(); i++)
        menuTitle += " -> " + this->sysMenuNavList[i];

    // Add the menu id to the title string.
    menuTitle += "###sysMenu";

    // If the menu is open set the size and position.
    if (this->sysMenuOpen == true)
    {
        ImGui::SetNextWindowSize(ImVec2(700.0f, 0.0f), ImGuiCond_Once);
    }

    // Draw the sysmenu window.
    if (ImGui::Begin(menuTitle.c_str(), &this->sysMenuOpen, ImGuiWindowFlags_NoSavedSettings))
    {
        // Check if we need to disable the nav back button.
        bool popItemFlag = this->sysMenuIndex == 0;
        if (this->sysMenuIndex == 0)
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);

        // Nav back button:
        if (ImGui::Button(" <-- ") == true)
        {
            // Update the property set index.
            this->sysMenuIndex--;

            // Remove the property list from the list.
            this->sysMenuNavList.pop_back();
            this->sysMenuPropertyList.pop_back();
        }

        // Check if we need to pop item flags.
        if (popItemFlag == true)
            ImGui::PopItemFlag();

        // Draw the options.
        MtPropertyListEntry *pEntry = this->sysMenuPropertyList[this->sysMenuIndex]->GetFirstNode();
        DrawSection(pEntry);
    }
    ImGui::End();
}

__int64 OpenDebugMenu(WCHAR **argv, int argc)
{
    // Get the sSnatcherToolImpl instance.
    sSnatcherToolImpl *pSnatcherToolImpl = sSnatcherToolImpl::Instance();

    // Check if the sysmenu root has been setup yet or not.
    if (pSnatcherToolImpl->sysMenuPropertyList.size() == 0)
    {
        // Build the root of the system menu.
        MtPropertyList *pSysMenuRoot = (MtPropertyList*)malloc(sizeof(MtPropertyList));
        MtPropertyList::_ctor(pSysMenuRoot);

        sSnatcherTool *pSnatcherTool = *(sSnatcherTool**)(((BYTE*)sSnatcherMain::Instance()) + 0x20258);
        pSnatcherTool->RegisterDebugOptions(pSysMenuRoot);
        sSnatcherMain::Instance()->RegisterDebugOptions(pSysMenuRoot);

        // Add the root entry to the nav list.
        pSnatcherToolImpl->sysMenuNavList.push_back("SysMenu");
        pSnatcherToolImpl->sysMenuPropertyList.push_back(pSysMenuRoot);
        pSnatcherToolImpl->sysMenuIndex = 0;

        // Set the last area id loaded.
        sAreaManager *pAreaManager = sAreaManager::Instance();
        pSnatcherToolImpl->lastAreaId = *(DWORD*)((BYTE*)pAreaManager + 0x40);
    }

    // Display the debug menu.
    pSnatcherToolImpl->sysMenuOpen = true;

    return 0;
}