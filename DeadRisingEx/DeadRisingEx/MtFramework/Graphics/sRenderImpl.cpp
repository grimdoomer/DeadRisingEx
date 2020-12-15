/*

*/

#include "sRenderImpl.h"
#include <MtFramework/Rendering/cTrans.h>
#include <stdio.h>
#include <string>

void *g_sRenderSingletonInst = (void*)0x141CF3268;

// Forward declarations for command functions.
__int64 PrintVertexDeclarations(WCHAR **argv, int argc);

// Table of commands for sRender objects.
const int g_sRenderCommandsLength = 1;
const CommandEntry g_sRenderCommands[g_sRenderCommandsLength] =
{
    { L"list_vertex_decls", L"Lists all sRender vertex declarations", PrintVertexDeclarations }
};

void sRenderImpl::RegisterTypeInfo()
{
    // Register commands:
    RegisterCommands(g_sRenderCommands, g_sRenderCommandsLength);
}

__int64 PrintVertexDeclarations(WCHAR **argv, int argc)
{
    int VertexDeclsProcessed = 0;

    // Acquire the list lock.
    EnterCriticalSection((LPCRITICAL_SECTION)(GetModulePointer<__int64>(g_sRenderSingletonInst) + 0x2F468));

    // Setup the vertex decl loop pointer.
    cTrans::VertexDecl **ppDecl = (cTrans::VertexDecl**)(GetModulePointer<__int64>(g_sRenderSingletonInst) + 0x27460);

    // Loop for the number of registered vertex declaractions.
    DWORD VertexDeclCount = *(DWORD*)(GetModulePointer<__int64>(g_sRenderSingletonInst) + 0x2F460);
    for (int i = 0; i < VertexDeclCount; i++)
    {
        const char *psTypeName = (const char*)*(__int64*)GetModuleAddress<__int64*>((void*)0x141CF3738);
        MtDTI *pDTIInfo = ppDecl[i]->GetDTI();

        // Check the name of the DTI type is 'cTrans::VertexDecl'.
        if (pDTIInfo->pObjectName != psTypeName)
        {
            // Traverse the DTI list until we know this is a cTrans::VertexDecl.
            pDTIInfo = pDTIInfo->pParentObject;
            while (pDTIInfo != nullptr)
            {
                if (pDTIInfo->pObjectName == psTypeName)
                    break;

                pDTIInfo = pDTIInfo->pParentObject;
            }

            if (pDTIInfo == nullptr)
                continue;
        }

        // Print the vertex declaration info.
        wprintf(L"VertexDecl %d %p:\n", VertexDeclsProcessed++, ppDecl[i]);
        wprintf(L"\tUnk1: %p\n", ppDecl[i]->Unk1);
        wprintf(L"\tpElements: %p\n", ppDecl[i]->pElements);
        wprintf(L"\tmElementNum: %d\n", ppDecl[i]->mElementNum);
        wprintf(L"\tId: 0x%08x\n", ppDecl[i]->Id);

        // Loop and print each vertex element.
        for (int x = 0; x < ppDecl[i]->mElementNum; x++)
        {
            wprintf(L"\t\tElem %d: Slot=%d Offset=%d Format=%S SemanticName=%S SemanticIndex=%d\n", x, ppDecl[i]->pElements[x].Slot, ppDecl[i]->pElements[x].Offset,
                FormatName[ppDecl[i]->pElements[x].Format], SemanticNames[ppDecl[i]->pElements[x].SemanticNameIndex], ppDecl[i]->pElements[x].SemanticIndex);
        }
        wprintf(L"\n");
    }

    // Release the list lock.
    LeaveCriticalSection((LPCRITICAL_SECTION)(GetModulePointer<__int64>(g_sRenderSingletonInst) + 0x2F468));
    return 0;
}