/*

*/

#include "rModelImpl.h"
#include <codecvt>
#include <MtFramework/Archive/sResource.h>

// Default object instances:
Primitive defaultPrim = { 0 };
Material defaultMaterial = { 0 };

const char* MaterialShaderTypes[] =
{
    "Skinned 4-weight",
    "Skinned 8-weight",
    "Level geometry"
};

std::string GetMaterialDisplayName(rModel* pModel, int materialIndex)
{
    // Check for null material index.
    if (materialIndex == 0 || materialIndex - 1 > pModel->MaterialCount)
        return "(null)";

    // Check if the texture has a basemap set.
    if (pModel->pMaterials[materialIndex - 1].BaseMapTexture != nullptr)
    {
        // Try to get the basemap texture file name.
        cResource* pTexture = (cResource*)pModel->pMaterials[materialIndex - 1].BaseMapTexture;
        return std::format("{:d}: {:s}", materialIndex - 1, pTexture->mPath);
    }

    // Return just the material index.
    return std::format("{:d}", materialIndex - 1);
}

void rModelEditorWindow::DrawWindowBody()
{
    // Primitives:
    if (ImGui::CollapsingHeader("Primitives", ImGuiTreeNodeFlags_DefaultOpen) == true)
    {
        Primitive* pPrim = this->selectedPrimitiveIndex < this->pModel->PrimitiveCount ? &this->pModel->pPrimitiveData[this->selectedPrimitiveIndex] : &defaultPrim;
        //if (this->selectedPrimitiveIndex >= this->pModel->PrimitiveCount)
        //    ImGui::BeginDisabled(true);

        int groupId = pPrim->GroupID >> 5;
        int partId = pPrim->GroupID & 0x1F;

        std::string primitiveDisplayName = std::format("Primitive[{:d}] {:d}:{:d}", this->selectedPrimitiveIndex, groupId, partId);
        if (ImGui::BeginCombo("##primitive_selection", primitiveDisplayName.c_str()) == true)
        {
            for (int i = 0; i < this->pModel->PrimitiveCount; i++)
            {
                primitiveDisplayName = std::format("Primitive[{:d}] {:d}:{:d}##{:d}", i, this->pModel->pPrimitiveData[i].GroupID >> 5, this->pModel->pPrimitiveData[i].GroupID & 0x1F, i);
                if (ImGui::Selectable(primitiveDisplayName.c_str(), this->selectedPrimitiveIndex == i) == true)
                {
                    this->selectedPrimitiveIndex = i;
                }

                // Set active focus to the selected item.
                if (this->selectedPrimitiveIndex == i)
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }
        ImGui::NewLine();

        ImGui::InputScalar("Group ID", ImGuiDataType_U32, &groupId, nullptr, nullptr, nullptr, ImGuiInputTextFlags_ReadOnly);
        ImGui::InputScalar("Part ID", ImGuiDataType_U32, &partId, nullptr, nullptr, nullptr, ImGuiInputTextFlags_ReadOnly);

        std::string materialDisplayName = GetMaterialDisplayName(this->pModel, pPrim->MaterialIndex);
        if (ImGui::BeginCombo("Material", materialDisplayName.c_str()) == true)
        {
            // Loop and draw material options.
            for (int i = 0; i < this->pModel->MaterialCount + 1; i++)
            {
                materialDisplayName = GetMaterialDisplayName(this->pModel, i);
                if (ImGui::Selectable(materialDisplayName.c_str(), pPrim->MaterialIndex == i) == true)
                {
                    pPrim->MaterialIndex = i;
                }

                // Set active focus to the selected item.
                if (pPrim->MaterialIndex == i)
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }

        bool bEnabled = pPrim->Enabled != 0;
        if (ImGui::Checkbox("Enabled", &bEnabled) == true)
            pPrim->Enabled = bEnabled == true ? 1 : 0;

        ImGui::InputScalar("Unk3", ImGuiDataType_U8, &pPrim->Unk3);
        ImGui::InputScalar("Unk11", ImGuiDataType_U8, &pPrim->Unk11);
        ImGui::InputScalar("Unk12", ImGuiDataType_U8, &pPrim->Unk12);
        ImGui::InputScalar("Vertex Stride 1", ImGuiDataType_U8, &pPrim->VertexStride1, nullptr, nullptr, nullptr, ImGuiInputTextFlags_ReadOnly);
        ImGui::InputScalar("Vertex Stride 2", ImGuiDataType_U8, &pPrim->VertexStride2, nullptr, nullptr, nullptr, ImGuiInputTextFlags_ReadOnly);
        ImGui::InputScalar("Unk13", ImGuiDataType_U8, &pPrim->Unk13);
        ImGui::InputScalar("Vertex Count", ImGuiDataType_U32, &pPrim->VertexCount, nullptr, nullptr, nullptr, ImGuiInputTextFlags_ReadOnly);
        ImGui::InputScalar("Starting Vertex", ImGuiDataType_U32, &pPrim->StartingVertex, nullptr, nullptr, nullptr, ImGuiInputTextFlags_ReadOnly);
        ImGui::InputScalar("Vertex Stream 1 Offset", ImGuiDataType_U32, &pPrim->VertexStream1Offset, nullptr, nullptr, nullptr, ImGuiInputTextFlags_ReadOnly);
        ImGui::InputScalar("Vertex Stream 2 Offset", ImGuiDataType_U32, &pPrim->VertexStream2Offset, nullptr, nullptr, nullptr, ImGuiInputTextFlags_ReadOnly);
        ImGui::InputScalar("Starting Index Location", ImGuiDataType_U32, &pPrim->StartingIndexLocation, nullptr, nullptr, nullptr, ImGuiInputTextFlags_ReadOnly);
        ImGui::InputScalar("Index Count", ImGuiDataType_U32, &pPrim->IndexCount, nullptr, nullptr, nullptr, ImGuiInputTextFlags_ReadOnly);
        ImGui::InputScalar("Base Vertex Location", ImGuiDataType_U32, &pPrim->BaseVertexLocation, nullptr, nullptr, nullptr, ImGuiInputTextFlags_ReadOnly);
        ImGui::InputFloat4("Bounding Box Min", (float*)&pPrim->BoundingBoxMin);
        ImGui::InputFloat4("Bounding Box Max", (float*)&pPrim->BoundingBoxMax);
    }

    // Materials:
    if (ImGui::CollapsingHeader("Materials", ImGuiTreeNodeFlags_DefaultOpen) == true)
    {
        Material* pMat = this->selectedMaterialIndex < this->pModel->MaterialCount ? &this->pModel->pMaterials[this->selectedMaterialIndex] : &defaultMaterial;

        std::string materialDisplayName = GetMaterialDisplayName(this->pModel, this->selectedMaterialIndex + 1);
        if (ImGui::BeginCombo("##material_selection", materialDisplayName.c_str()) == true)
        {
            for (int i = 0; i < this->pModel->MaterialCount; i++)
            {
                materialDisplayName = GetMaterialDisplayName(this->pModel, i + 1);
                if (ImGui::Selectable(materialDisplayName.c_str(), this->selectedMaterialIndex == i) == true)
                {
                    this->selectedMaterialIndex = i;
                }

                // Set active focus to the selected item.
                if (this->selectedMaterialIndex == i)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        ImGui::InputScalar("Flags", ImGuiDataType_U32, &pMat->Flags, nullptr, nullptr, "%x", ImGuiInputTextFlags_ReadOnly);
        ImGui::InputText("Vertex Type", (char*)MaterialShaderTypes[(pMat->Flags >> 27) & 3], 0, ImGuiInputTextFlags_ReadOnly);

        bool alphaTestEnabled = (pMat->Flags & 0x40) != 0;
        if (ImGui::Checkbox("Alpha test enabled", &alphaTestEnabled) == true)
            pMat->Flags = (pMat->Flags & ~0x40) | (alphaTestEnabled == true ? 0x40 : 0);

        ImGui::InputScalar("Unk4", ImGuiDataType_U32, &pMat->Unk4, nullptr, nullptr, "%x");
        // TODO: shader technique
        ImGui::InputScalar("Unk5", ImGuiDataType_U32, &pMat->Unk5, nullptr, nullptr, nullptr, ImGuiInputTextFlags_ReadOnly);
        ImGui::InputScalar("Unk11", ImGuiDataType_U32, &pMat->Unk11);

        ImGui::DragFloat("Transparency", &pMat->Transparency, 0.1);
        ImGui::DragFloat("Fresnel Factor", &pMat->FresnelFactor, 0.1);
        ImGui::DragFloat("Fresnel Bias", &pMat->FresnelBias, 0.1);
        ImGui::DragFloat("Specular Power", &pMat->SpecularPow, 0.1);
        ImGui::DragFloat("Environment Map Power", &pMat->EnvmapPower, 0.1);
        ImGui::DragFloat4("Lightmap Scale", (float*)&pMat->LightMapScale, 0.1);
        ImGui::DragFloat("Detail Factor", &pMat->DetailFactor, 0.1);
        ImGui::DragFloat("Detail Wrap", &pMat->DetailWrap, 0.1);
        ImGui::DragFloat("Unk22", &pMat->Unk22, 0.1);
        ImGui::DragFloat("Unk23", &pMat->Unk23, 0.1);
        ImGui::DragFloat4("Transmit", (float*)&pMat->Transmit, 0.1);
        ImGui::DragFloat4("Parallax", (float*)&pMat->Parallax, 0.1);
        ImGui::DragFloat("Unk32", &pMat->Unk32, 0.1);
        ImGui::DragFloat("Unk33", &pMat->Unk33, 0.1);
        ImGui::DragFloat("Unk34", &pMat->Unk34, 0.1);
        ImGui::DragFloat("Unk35", &pMat->Unk35, 0.1);
    }
}

__int64 OpenModelEditor(WCHAR** argv, int argc)
{
    // Make sure at least one argument was provided.
    if (argc == 0)
    {
        // Invalid syntax.
        ImGuiConsole::Instance()->ConsolePrint(L"Invalid command syntax!\n");
        return 0;
    }

    // Setup the unicode converter and convert the model path to ascii.
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> unicConvert;
    std::string sModelPath = unicConvert.to_bytes(argv[0]);

    // Get the model instance.
    ULONGLONG modelResourceId = sResource::Instance()->CalculateResourceId(rModel::DebugTypeInfo, (char*)sModelPath.c_str());
    rModel* pModel = sResource::Instance()->FindResourceById<rModel>(modelResourceId);
    if (pModel == nullptr)
    {
        // Failed to load the model.
        ImGuiConsole::Instance()->ConsolePrint(L"Model '%S' is not currently loaded\n", sModelPath.c_str());
        return 0;
    }

    // Create a new editor window.
    rModelEditorWindow* pEditorWindow = new rModelEditorWindow(pModel);
    if (pEditorWindow == nullptr)
    {
        ImGuiConsole::Instance()->ConsolePrint(L"Failed to create model editor window\n");
        return 0;
    }

    pEditorWindow->SetSize(ImVec2(500, 700));
    ImGuiConsole::Instance()->OpenWindow(pEditorWindow);

    return 0;
}

// Table of commands for sShader objects.
const ConsoleCommandInfo g_rModelCommands[] =
{
    { L"edit_rmodel", L"Lists all sShader parameter descriptors", OpenModelEditor }
};

void rModelImpl::RegisterCommands()
{
    // Register commands.
    ImGuiConsole::Instance()->RegisterCommands(g_rModelCommands, _countof(g_rModelCommands));
}