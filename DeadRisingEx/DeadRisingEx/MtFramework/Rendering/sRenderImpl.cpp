/*

*/

#include "sRenderImpl.h"
#include <Misc/AsmHelpers.h>
#include <MtFramework/Game/sMain.h>
#include <MtFramework/Rendering/sRender.h>
#include <MtFramework/Rendering/sPrim.h>
#include <MtFramework/Rendering/sShader.h>
#include <stdio.h>
#include <string>
#include <detours.h>
#include "DeadRisingEx/Utilities/GuardedBuffer.h"
#include "renderdoc_app.h"
#include <assert.h>
#include "DeadRisingEx/MtFramework/Rendering/ImGui/ImGuiRenderer.h"

void *g_sRenderSingletonInst = (void*)0x141CF3268;

void *sRender_DrawFrame_Hook_Start_address = GetModuleAddress<void*>(0x140662362);
void *sRender_DrawFrame_Hook_End_address = GetModuleAddress<void*>(0x140663181);

bool g_CaptureFrame = false;
bool g_CaptureStarted = false;
RENDERDOC_API_1_4_1 *rdoc_api = NULL;

// Forward declarations for command functions.
__int64 PrintVertexDeclarations(WCHAR **argv, int argc);
__int64 CaptureFrame(WCHAR **argv, int argc);

sRender * __stdcall Hook_sRender_ctor(sRender *thisptr, DWORD interval, DWORD dwUnused1, DWORD dwGraphicsMemSize, DWORD dwUnused2);
void __stdcall Hook_sRender_Present(sRender *thisptr);
void __stdcall Hook_sRender_SystemCleanup(sRender *thisptr);

sPrim * __stdcall Hook_sPrim_ctor(sPrim *thisptr, DWORD entryCount);
//void __cdecl Hook_sRender_DrawFrame(sRender *thisptr);
sRender::Buffer * __stdcall Hook_sRender__Buffer_ctor(sRender::Buffer *thisptr, ID3D11DeviceContext *pDeviceContext, DWORD dwBufferSize, DWORD dwBufferType);
void * __stdcall Hook_sRender__Buffer_MapForWrite(sRender::Buffer *thisptr, DWORD dwSize);

// Table of commands for sRender objects.
const int g_sRenderCommandsLength = 2;
const ConsoleCommandInfo g_sRenderCommands[g_sRenderCommandsLength] =
{
    { L"list_vertex_decls", L"Lists all sRender vertex declarations", PrintVertexDeclarations },
    { L"capture_frame", L"If renderdoc is attached captures a frame from sRender only", CaptureFrame }
};

void sRenderImpl::RegisterTypeInfo()
{
    // Register commands:
    ImGuiConsole::Instance()->RegisterCommands(g_sRenderCommands, g_sRenderCommandsLength);
}

bool sRenderImpl::InstallHooks()
{
    // Hook sRender functions needed for the imgui renderer.
    DetourAttach((void**)&sRender::_ctor, Hook_sRender_ctor);
    DetourAttach((void**)&sRender::_Present, Hook_sRender_Present);
    DetourAttach((void**)&sRender::_SystemCleanup, Hook_sRender_SystemCleanup);

    // Check if dynamic graphics memory is enabled and hook needed functions if so.
    if (ModConfig::Instance()->DynamicGraphicsMemory == true)
    {
        DetourAttach((void**)&sPrim::_ctor, Hook_sPrim_ctor);
        DetourAttach((void**)&sRender::Buffer::_ctor, Hook_sRender__Buffer_ctor);
        DetourAttach((void**)&sRender::Buffer::_MapForWrite, Hook_sRender__Buffer_MapForWrite);
    }

    //DetourAttach(&sRender_DrawFrame_Hook_Start_address, sRender_DrawFrame_Hook_Start);
    //DetourAttach(&sRender_DrawFrame_Hook_End_address, sRender_DrawFrame_Hook_End);

    //// Load the renderdoc dll into the process space.
    //HMODULE hRenderDoc = LoadLibraryA("C:\\Users\\rmiceli\\Downloads\\RenderDoc_1.11_64\\RenderDoc_1.11_64\\renderdoc.dll");
    //if (hRenderDoc != NULL)
    //{
    //    // Get the GetAPI function address.
    //    pRENDERDOC_GetAPI pGetAPI = (pRENDERDOC_GetAPI)GetProcAddress(hRenderDoc, "RENDERDOC_GetAPI");
    //    if (pGetAPI != nullptr)
    //    {
    //        // Call the GetAPI function to get renderdoc api info.
    //        int result = pGetAPI(eRENDERDOC_API_Version_1_4_1, (void**)&rdoc_api);
    //        if (result == 1)
    //            ImGuiConsole::Instance()->ConsolePrint(L"Renderdoc harness successfully initialized!\n");
    //        else
    //            ImGuiConsole::Instance()->ConsolePrint(L"Failed to initialize renderdoc harness %d!\n", result);
    //    }
    //}

    return true;
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
        ImGuiConsole::Instance()->ConsolePrint(L"VertexDecl %d %p:\n", VertexDeclsProcessed++, ppDecl[i]);
        ImGuiConsole::Instance()->ConsolePrint(L"\tUnk1: %p\n", ppDecl[i]->Unk1);
        ImGuiConsole::Instance()->ConsolePrint(L"\tpElements: %p\n", ppDecl[i]->pElements);
        ImGuiConsole::Instance()->ConsolePrint(L"\tmElementNum: %d\n", ppDecl[i]->mElementNum);
        ImGuiConsole::Instance()->ConsolePrint(L"\tId: 0x%08x\n", ppDecl[i]->Id);

        // Loop and print each vertex element.
        for (int x = 0; x < ppDecl[i]->mElementNum; x++)
        {
            ImGuiConsole::Instance()->ConsolePrint(L"\t\tElem %d: Slot=%d Offset=%d Format=%S SemanticName=%S SemanticIndex=%d\n", x, ppDecl[i]->pElements[x].Slot, ppDecl[i]->pElements[x].Offset,
                FormatName[ppDecl[i]->pElements[x].Format], SemanticNames[ppDecl[i]->pElements[x].SemanticNameIndex], ppDecl[i]->pElements[x].SemanticIndex);
        }
        ImGuiConsole::Instance()->ConsolePrint(L"\n");
    }

    // Release the list lock.
    LeaveCriticalSection((LPCRITICAL_SECTION)(GetModulePointer<__int64>(g_sRenderSingletonInst) + 0x2F468));
    return 0;
}

__int64 CaptureFrame(WCHAR **argv, int argc)
{
    // Flag that the next frame should be captured.
    g_CaptureFrame = true;
    return 0;
}

sRender * __stdcall Hook_sRender_ctor(sRender *thisptr, DWORD interval, DWORD dwUnused1, DWORD dwGraphicsMemSize, DWORD dwUnused2)
{
    // If we are running in dynamic graphics mode adjust the graphics memory size to avoid crashes.
    sRender *psRender = nullptr;
    if (ModConfig::Instance()->DynamicGraphicsMemory == true)
        psRender = sRender::_ctor(thisptr, interval, dwUnused1, 60 * 1024 * 1024, dwUnused2);
    else
        psRender = sRender::_ctor(thisptr, interval, dwUnused1, dwGraphicsMemSize, dwUnused2);

    // Initialize the imgui renderer.
    if (ImGuiRenderer::Instance()->Initialize() == false)
    {
        // Failed to initialize the imgui renderer.
        DbgPrint("### ERROR: Failed to initialize ImGuiRenderer!\n");
        DebugBreak();
    }

    // Call the first begin frame here.
    //ImGuiRenderer::Instance()->BeginFrame();

    // Return the sRender instnace.
    return psRender;
}

void __stdcall Hook_sRender_Present(sRender *thisptr)
{
    // Get all the variables we need for this function.
    BOOL VSync = *(BOOL*)((BYTE*)sRender::Instance() + 0x8404);
    IDXGISwapChain *pSwapChain = *(IDXGISwapChain**)((BYTE*)sRender::Instance() + 0x8618);
    bool *ShouldPostQuitMessage = (bool*)((BYTE*)sRender::Instance() + 0x8585);
    ID3D11DeviceContext **MainThreadDeferredContexts = (ID3D11DeviceContext**)((BYTE*)sRender::Instance() + 0x85E0);

    // Wait for the next frame to render.
    thisptr->RenderFrame();

    // Render the imgui frame.
    ImGuiRenderer::Instance()->SystemUpdate();

    // Present the final image.
    pSwapChain->Present(VSync == FALSE ? 0 : 1, 0);

    // Check if we should post a quit message (I don't think this is ever triggered).
    if (*ShouldPostQuitMessage != false)
    {
        // Flag sMain to quit and post the quit message to the main thread.
        *ShouldPostQuitMessage = false;
        sMain::Instance()->ShouldQuit = TRUE;
        SendMessageA(*sMain::GameWindowHandle, 2, NULL, NULL);
    }

    // Call some sShader function.
    ThisPtrCall((void*)0x140693E30, sShader::Instance());

    // Swap the deferred context pointers.
    ID3D11DeviceContext *pOld = MainThreadDeferredContexts[1];
    MainThreadDeferredContexts[1] = MainThreadDeferredContexts[0];
    MainThreadDeferredContexts[0] = pOld;

    ThisPtrCall((void*)0x140661B70, sRender::Instance());
}

void __stdcall Hook_sRender_SystemCleanup(sRender *thisptr)
{
    // Cleanup the imgui renderer.
    ImGuiRenderer::Instance()->SystemCleanup();

    // Call the trampoline.
    sRender::_SystemCleanup(thisptr);
}

sPrim * __stdcall Hook_sPrim_ctor(sPrim *thisptr, DWORD entryCount)
{
    // Increase the number of command entries to avoid crashes.
    return sPrim::_ctor(thisptr, entryCount * 10);
}

//void __stdcall Hook_sRender_DrawFrame(sRender *thisptr)
//{
//    bool doFrameCapture = false;
//
//    // Check if we should capture a frame.
//    if (g_CaptureFrame == true)
//    {
//        // Check if renderdoc is loaded.
//        if (rdoc_api != nullptr)
//        {
//            // Flag that we will need to complete the frame capture.
//            doFrameCapture = true;
//
//            // Start the frame capture.
//            rdoc_api->StartFrameCapture(NULL, NULL);
//        }
//        else
//        {
//            // Renderdoc is not currently loaded.
//            g_CaptureFrame = false;
//            ImGuiConsole::Instance()->ConsolePrint(L"Renderdoc harness is not loaded!\n");
//        }
//    }
//
//    // Call the trampoline.
//    sRender::_DrawFrame(thisptr);
//
//    // If we captured a frame reset our frame capture flag.
//    if (doFrameCapture == true)
//    {
//        // End the frame capture.
//        rdoc_api->EndFrameCapture(NULL, NULL);
//        ImGuiConsole::Instance()->ConsolePrint(L"Frame captured\n");
//
//        // Reset the capture frame flag.
//        g_CaptureFrame = false;
//    }
//}

sRender::Buffer * __stdcall Hook_sRender__Buffer_ctor(sRender::Buffer *thisptr, ID3D11DeviceContext *pDeviceContext, DWORD dwBufferSize, DWORD dwBufferType)
{
    // Check the buffer type and increase the size to avoid crashes.
    if (dwBufferType == BUFFER_TYPE_VERTEX)
        dwBufferSize = 15 * 1024 * 1024;    // 15MB for vertex buffers
    else
        dwBufferSize = 5 * 1024 * 1024;     // 5MB for index buffers

    // Initialize fields.
    thisptr->pBuffer = nullptr;
    thisptr->pDeviceContext = pDeviceContext;
    thisptr->BufferType = dwBufferType;
    thisptr->CurrentPosition = 0;
    thisptr->PreviousPosition = 0;
    thisptr->mMaxSize = dwBufferSize;

    // Make sure the device context is valid.
    if (pDeviceContext == nullptr)
    {
        // Fatal error: device context is null.
        ImGuiConsole::Instance()->ConsolePrint(L"FATAL: sRender::Buffer::Buffer() was passed a null d3d device context!\n");
        assert(pDeviceContext);
    }

    // Setup the buffer descriptor.
    D3D11_BUFFER_DESC bufferDesc = { 0 };
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.ByteWidth = dwBufferSize;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bufferDesc.BindFlags = thisptr->BufferType == BUFFER_TYPE_VERTEX ? D3D11_BIND_VERTEX_BUFFER : D3D11_BIND_INDEX_BUFFER;

    // Get the d3d device pointer from sRender instance.
    ID3D11Device *pD3dDevice = *(ID3D11Device**)(((BYTE*)sRender::Instance()) + 0x8590);

    // Create the buffer.
    HRESULT hr = pD3dDevice->CreateBuffer(&bufferDesc, nullptr, &thisptr->pBuffer);
    if (hr != S_OK)
    {
        // Failed to create buffer.
        ImGuiConsole::Instance()->ConsolePrint(L"FATAL: sRender::Buffer::Buffer() failed to create ID3D11Buffer hr=0x%08x!\n", hr);
        assert(hr == S_OK);
    }

    return thisptr;
}

void * __stdcall Hook_sRender__Buffer_MapForWrite(sRender::Buffer *thisptr, DWORD dwSize)
{
    // Check if the size requested will exceed the capacity of the buffer.
    if (thisptr->CurrentPosition + dwSize <= thisptr->mMaxSize)
    {
        // Requested size is okay, call the trampoline.
        return sRender::Buffer::_MapForWrite(thisptr, dwSize);
    }

    // Calculate the new buffer size needed and round up to the nearest megabyte.
    DWORD newBufferSize = ((thisptr->CurrentPosition + dwSize) + 0x100000) & ~(0x100000 - 1);
    ImGuiConsole::Instance()->ConsolePrint(L"WARNING: sRender::Buffer out of memory, resizing [%s] buffer %.2FMB -> %.2fMB\n", (thisptr->BufferType == BUFFER_TYPE_VERTEX ? L"Vertex" : L"Index"),
        (float)thisptr->mMaxSize / (float)(1024 * 1024), (float)newBufferSize / (float)(1024 * 1024));

    // Setup the buffer descriptor for the new buffer.
    D3D11_BUFFER_DESC bufferDesc = { 0 };
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.ByteWidth = newBufferSize;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bufferDesc.BindFlags = thisptr->BufferType == BUFFER_TYPE_VERTEX ? D3D11_BIND_VERTEX_BUFFER : D3D11_BIND_INDEX_BUFFER;

    // Get the d3d device pointer from sRender instance.
    ID3D11Device *pD3dDevice = *(ID3D11Device**)(((BYTE*)sRender::Instance()) + 0x8590);

    // Create the buffer.
    ID3D11Buffer *pNewBuffer = nullptr;
    HRESULT hr = pD3dDevice->CreateBuffer(&bufferDesc, nullptr, &pNewBuffer);
    if (hr != S_OK)
    {
        // Failed to create buffer.
        ImGuiConsole::Instance()->ConsolePrint(L"FATAL: sRender::Buffer::MapForWrite() failed to create new ID3D11Buffer hr=0x%08x!\n", hr);
        assert(hr == S_OK);
    }

    // Setup the copy box for copying the contents of the old buffer.
    D3D11_BOX srcBox = { 0 };
    srcBox.left = 0;
    srcBox.right = thisptr->CurrentPosition;
    srcBox.top = 0;
    srcBox.bottom = 1;
    srcBox.front = 0;
    srcBox.back = 1;

    // Copy the contents of the old buffer into the new one.
    thisptr->pDeviceContext->CopySubresourceRegion(pNewBuffer, 0, 0, 0, 0, thisptr->pBuffer, 0, &srcBox);

    // TODO: CopySubresourceRegion works asynchronously since the GPU is the one doing the copy. Because of that we
    // can't immediate destroy the old buffer (I think). So we will need to implement a deferred destruction of it
    // in a subsequent frame. For now we just eat it.

    ID3D11Buffer *pOldBuffer = thisptr->pBuffer;

    // Update the buffer properties.
    thisptr->mMaxSize = newBufferSize;
    thisptr->pBuffer = pNewBuffer;

    // Map the buffer for write access.
    D3D11_MAPPED_SUBRESOURCE mappedResource = { 0 };
    hr = thisptr->pDeviceContext->Map(thisptr->pBuffer, 0, thisptr->CurrentPosition == 0 ? D3D11_MAP_WRITE_DISCARD : D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mappedResource);
    if (hr != S_OK || mappedResource.pData == nullptr)
    {
        // Failed to map the buffer for writing.
        ImGuiConsole::Instance()->ConsolePrint(L"FATAL: sRender::Buffer::MapForWrite() failed to map buffer for writing!\n");
        assert(mappedResource.pData);
    }

    // Update the current and previous positions.
    thisptr->PreviousPosition = thisptr->CurrentPosition;
    thisptr->CurrentPosition += dwSize;

    // Return a pointer to the data for writing.
    return (BYTE*)mappedResource.pData + thisptr->CurrentPosition;
}

__int64 sRender_CaptureFrame_Start()
{
    // Check if we should capture a frame.
    if (g_CaptureFrame == true)
    {
        // Check if renderdoc is loaded.
        if (rdoc_api != nullptr)
        {
            // Flag that we will need to complete the frame capture.
            g_CaptureStarted = true;

            // Start the frame capture.
            rdoc_api->StartFrameCapture(NULL, NULL);
        }
        else
        {
            // Renderdoc is not currently loaded.
            g_CaptureFrame = false;
            ImGuiConsole::Instance()->ConsolePrint(L"Renderdoc harness is not loaded!\n");
        }
    }

    // Return the trampoline address.
    return (__int64)sRender_DrawFrame_Hook_Start_address;
}

__int64 sRender_CaptureFrame_End()
{
    // If we started a frame capture end it.
    if (g_CaptureStarted == true)
    {
        // End the frame capture.
        rdoc_api->EndFrameCapture(NULL, NULL);
        ImGuiConsole::Instance()->ConsolePrint(L"Frame captured\n");

        // Reset the capture frame flag.
        g_CaptureFrame = false;
        g_CaptureStarted = false;
    }

    // Return the trampoline address.
    return (__int64)sRender_DrawFrame_Hook_End_address;
}