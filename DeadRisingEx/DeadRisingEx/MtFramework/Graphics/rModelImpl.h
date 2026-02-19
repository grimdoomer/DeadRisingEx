/*

*/

#pragma once
#include "DeadRisingEx.h"
#include <MtFramework/Graphics/rModel.h>
#include "DeadRisingEx/MtFramework/Rendering/Utilities/ImGuiWindowBase.h"
#include <format>

class rModelEditorWindow : public ImGuiWindowBase
{
protected:
    rModel* pModel;

    int selectedPrimitiveIndex = 0;
    int selectedMaterialIndex = 0;

    void DrawWindowBody();

public:
    rModelEditorWindow(rModel* pModel) : ImGuiWindowBase(pModel->mPath, "")
    {
        this->pModel = pModel;

        // Use the object address as the window id.
        this->windowId = std::format("{0:p}", (void*)pModel);
    }
};

class rModelImpl
{
public:
    static void RegisterCommands();
};