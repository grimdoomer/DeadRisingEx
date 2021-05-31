
#include "ImGuiRenderer.h"
#include "imgui.h"
#include <stdio.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <MtFramework/Utils/VTable.h>
#include <MtFramework/Game/sMain.h>
#include <MtFramework/Rendering/sRender.h>
#include "DeadRisingEx/MtFramework/Rendering/ImGui/ImGuiRenderer.h"
#include "DeadRisingEx/MtFramework/Debug/sSnatcherToolImpl.h"
#include <WinUser.h>

#pragma comment(lib, "d3dcompiler")

const char* ImGuiVertexShaderCode =
    "cbuffer vertexBuffer : register(b0) \
    {\
        float4x4 ProjectionMatrix; \
    };\
    struct VS_INPUT\
    {\
        float2 pos : POSITION;\
        float4 col : COLOR0;\
        float2 uv  : TEXCOORD0;\
    };\
    \
    struct PS_INPUT\
    {\
        float4 pos : SV_POSITION;\
        float4 col : COLOR0;\
        float2 uv  : TEXCOORD0;\
    };\
    \
    PS_INPUT main(VS_INPUT input)\
    {\
        PS_INPUT output;\
        output.pos = mul( ProjectionMatrix, float4(input.pos.xy, 0.f, 1.f));\
        output.col = input.col;\
        output.uv  = input.uv;\
        return output;\
    }";

const char* ImGuiPixelShaderCode =
    "struct PS_INPUT\
    {\
        float4 pos : SV_POSITION;\
        float4 col : COLOR0;\
        float2 uv  : TEXCOORD0;\
    };\
    sampler sampler0;\
    Texture2D texture0;\
    \
    float4 main(PS_INPUT input) : SV_Target\
    {\
        float4 out_col = input.col * texture0.Sample(sampler0, input.uv); \
        return out_col; \
    }";

struct VERTEX_CONSTANT_BUFFER
{
    float   mvp[4][4];
};


__int64 ShowImGuiDemo(WCHAR **argv, int argc);

// Table of commands for ImGuiRenderer.
const int g_ImGuiRendererCommandsLength = 1;
const ConsoleCommandInfo g_ImGuiRendererCommands[g_ImGuiRendererCommandsLength] =
{
    { L"show_imgui_demo", L"Displays the ImGui demo window", ShowImGuiDemo }
};



// Singleton instance:
ImGuiRenderer *g_pImGuiRendererInstance = nullptr;

void *ImGuiRenderer_vtable[4] = { nullptr };

bool g_ShowImGuiDemo = true;

// Fake destructor function to bypass not being able to take the address of a destructor:
void ImGuiRenderer_dtor(ImGuiRenderer *thisptr)
{
    thisptr->~ImGuiRenderer();
}

void ImGuiRenderer::RegisterTypeInfo()
{
    ImGuiConsole::Instance()->RegisterCommands(g_ImGuiRendererCommands, g_ImGuiRendererCommandsLength);
}

ImGuiRenderer * ImGuiRenderer::Instance()
{
    // If the singleton instance is null initialize it now.
    if (g_pImGuiRendererInstance == nullptr)
    {
        // Create the imgui renderer instance.
        g_pImGuiRendererInstance = new ImGuiRenderer();
    }

    // Return the singleton instance.
    return g_pImGuiRendererInstance;
}

ImGuiRenderer::ImGuiRenderer()
{
    // Call cSystem constructor.
    cSystem::_ctor(this);

    // Setup the vtable.
    BuildVtableLayout(ImGuiRenderer_vtable, ARRAYSIZE(ImGuiRenderer_vtable),
        ImGuiRenderer_dtor,
        &ImGuiRenderer::SystemCleanup,
        &ImGuiRenderer::SystemUpdate,
        &ImGuiRenderer::BuildSystemMenu);

    // Assign the vtable pointer.
    this->vtable = ImGuiRenderer_vtable;

    // Initialize the singleton instance.
    g_pImGuiRendererInstance = this;
}

ImGuiRenderer::~ImGuiRenderer()
{
    // Clear the singleton instance.
    g_pImGuiRendererInstance = nullptr;
}

bool ImGuiRenderer::Initialize()
{
    HRESULT hr = S_OK;

    // Create the imgui context for our dll.
    ImGuiContext *pContext = ImGui::CreateContext();
    ImGui::SetCurrentContext(pContext);

    // Use default style colors.
    ImGui::StyleColorsClassic();

    // Setup rendering capability flags.
    ImGuiIO& io = ImGui::GetIO();
    io.BackendRendererName = "imgui_dr_dx11";
    io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;

    // Setup keyboard key map
    io.KeyMap[ImGuiKey_Tab] = VK_TAB;
    io.KeyMap[ImGuiKey_LeftArrow] = VK_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = VK_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = VK_UP;
    io.KeyMap[ImGuiKey_DownArrow] = VK_DOWN;
    io.KeyMap[ImGuiKey_PageUp] = VK_PRIOR;
    io.KeyMap[ImGuiKey_PageDown] = VK_NEXT;
    io.KeyMap[ImGuiKey_Home] = VK_HOME;
    io.KeyMap[ImGuiKey_End] = VK_END;
    io.KeyMap[ImGuiKey_Insert] = VK_INSERT;
    io.KeyMap[ImGuiKey_Delete] = VK_DELETE;
    io.KeyMap[ImGuiKey_Backspace] = VK_BACK;
    io.KeyMap[ImGuiKey_Space] = VK_SPACE;
    io.KeyMap[ImGuiKey_Enter] = VK_RETURN;
    io.KeyMap[ImGuiKey_Escape] = VK_ESCAPE;
    //io.KeyMap[ImGuiKey_KeyPadEnter] = (int)Keys.Return;
    io.KeyMap[ImGuiKey_A] = 'A';
    io.KeyMap[ImGuiKey_C] = 'C';
    io.KeyMap[ImGuiKey_V] = 'V';
    io.KeyMap[ImGuiKey_X] = 'X';
    io.KeyMap[ImGuiKey_Y] = 'Y';
    io.KeyMap[ImGuiKey_Z] = 'Z';

    // Initialize fonts.
    ImFont *pFont = io.Fonts->AddFontDefault();
    bool result = io.Fonts->Build();
    assert(pFont != nullptr && result == true);

    // Get the d3d device pointer from sRender instance.
    ID3D11Device *pD3dDevice = *(ID3D11Device**)(((BYTE*)sRender::Instance()) + 0x8590);

    // Create the vertex and index buffers.
    if (this->ResizeVertexBuffers(5000, 10000) == false)
    {
        // Failed to create the vertex or index buffer.
        return false;
    }

    // Compile the vertex shader.
    ID3DBlob *pShaderCode = nullptr;
    if ((hr = D3DCompile(ImGuiVertexShaderCode, strlen(ImGuiVertexShaderCode), nullptr, nullptr, nullptr, "main", "vs_4_0", 0, 0, &pShaderCode, nullptr)) != S_OK)
    {
        // Failed to compile the vertex shader.
        DbgPrint("ImGuiRenderer::Initialize(): failed to compile vertex shader 0x%08x\n", hr);
        return false;
    }

    // Create the vertex shader.
    if ((hr = pD3dDevice->CreateVertexShader(pShaderCode->GetBufferPointer(), pShaderCode->GetBufferSize(), nullptr, &this->pVertexShader)) != S_OK)
    {
        // Failed to create the vertex shader.
        DbgPrint("ImGuiRenderer::Initialize(): failed to create vertex shader 0x%08x!\n", hr);
        return false;
    }

    // Input layout description:
    D3D11_INPUT_ELEMENT_DESC vertexLayout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT,   0, (UINT)IM_OFFSETOF(ImDrawVert, pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,   0, (UINT)IM_OFFSETOF(ImDrawVert, uv),  D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, (UINT)IM_OFFSETOF(ImDrawVert, col), D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    // Create the input layout.
    if ((hr = pD3dDevice->CreateInputLayout(vertexLayout, 3, pShaderCode->GetBufferPointer(), pShaderCode->GetBufferSize(), &this->pInputLayout)) != S_OK)
    {
        // Failed to create the input layout.
        DbgPrint("ImGuiRenderer::Initialize(): failed to create input layout 0x%08x\n", hr);
        return false;
    }

    // Release the shader code.
    pShaderCode->Release();

    // Compile the pixel shader.
    if ((hr = D3DCompile(ImGuiPixelShaderCode, strlen(ImGuiPixelShaderCode), nullptr, nullptr, nullptr, "main", "ps_4_0", 0, 0, &pShaderCode, nullptr)) != S_OK)
    {
        // Failed to compile the pixel shader.
        DbgPrint("ImGuiRenderer::Initialize(): failed to compile pixel shader 0x%08x\n", hr);
        return false;
    }

    // Create the pixel shader
    if ((hr = pD3dDevice->CreatePixelShader(pShaderCode->GetBufferPointer(), pShaderCode->GetBufferSize(), nullptr, &this->pPixelShader)) != S_OK)
    {
        // Failed to create the pixel shader.
        DbgPrint("ImGuiRenderer::Initialize(): failed to create pixel shader 0x%08x\n", hr);
        return false;
    }

    // Release the shader code.
    pShaderCode->Release();

    // Create the shader constant buffer.
    D3D11_BUFFER_DESC BufferDesc = { 0 };
    BufferDesc.ByteWidth = sizeof(VERTEX_CONSTANT_BUFFER);
    BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    BufferDesc.MiscFlags = 0;
    if ((hr = pD3dDevice->CreateBuffer(&BufferDesc, nullptr, &this->pShaderConstantBuffer)) != S_OK)
    {
        // Failed to create the shader constant buffer.
        DbgPrint("ImGuiRenderer::Initialize(): failed to create shader constant buffer 0x%08x\n", hr);
        return false;
    }

    // Setup the blend state.
    D3D11_BLEND_DESC BlendDesc = { 0 };
    BlendDesc.AlphaToCoverageEnable = false;
    BlendDesc.RenderTarget[0].BlendEnable = true;
    BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
    BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    if ((hr = pD3dDevice->CreateBlendState(&BlendDesc, &this->pBlendState)) != S_OK)
    {
        // Failed to create the blend state.
        DbgPrint("ImGuiRenderer::Initialize(): failed to create blend state 0x%08x\n", hr);
        return false;
    }

    // Setup the rasterizer state.
    D3D11_RASTERIZER_DESC RasterDesc;
    memset(&RasterDesc, 0, sizeof(RasterDesc));
    RasterDesc.FillMode = D3D11_FILL_SOLID;
    RasterDesc.CullMode = D3D11_CULL_NONE;
    RasterDesc.ScissorEnable = true;
    RasterDesc.DepthClipEnable = true;
    if ((hr = pD3dDevice->CreateRasterizerState(&RasterDesc, &this->pRasterState)) != S_OK)
    {
        // Failed to create the rasterizer state.
        DbgPrint("ImGuiRenderer::Initialize(): failed to create rasterizer state 0x%08x\n", hr);
        return false;
    }

    // Setup the depth stencil state.
    D3D11_DEPTH_STENCIL_DESC DepthDesc = { 0 };
    DepthDesc.DepthEnable = false;
    DepthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    DepthDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
    DepthDesc.StencilEnable = false;
    DepthDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    DepthDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    DepthDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    DepthDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    DepthDesc.BackFace = DepthDesc.FrontFace;
    if ((hr = pD3dDevice->CreateDepthStencilState(&DepthDesc, &this->pDepthStencilState)) != S_OK)
    {
        // Failed to create depth stencil state.
        DbgPrint("ImGuiRenderer::Initialize(): failed to create depth stencil state 0x%08\n", hr);
        return false;
    }

    // Get the texture info for the font texture.
    unsigned char *pFontPixelData;
    int fontWidth, fontHeight;
    io.Fonts->GetTexDataAsRGBA32(&pFontPixelData, &fontWidth, &fontHeight);

    // Setup the font texture.
    D3D11_TEXTURE2D_DESC TextureDesc = { 0 };
    TextureDesc.Width = fontWidth;
    TextureDesc.Height = fontHeight;
    TextureDesc.MipLevels = 1;
    TextureDesc.ArraySize = 1;
    TextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    TextureDesc.SampleDesc.Count = 1;
    TextureDesc.Usage = D3D11_USAGE_DEFAULT;
    TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    TextureDesc.CPUAccessFlags = 0;

    ID3D11Texture2D *pFontTexture = nullptr;
    D3D11_SUBRESOURCE_DATA subResource = { 0 };
    subResource.pSysMem = pFontPixelData;
    subResource.SysMemPitch = fontWidth * 4;
    subResource.SysMemSlicePitch = 0;
    if ((hr = pD3dDevice->CreateTexture2D(&TextureDesc, &subResource, &pFontTexture)) != S_OK)
    {
        // Failed to create the font texture.
        DbgPrint("ImGuiRenderer::Initialize(): failed to create font texture 0x%08x\n", hr);
        return false;
    }

    // Create the shader resource view for the font texture.
    D3D11_SHADER_RESOURCE_VIEW_DESC ResourceDesc;
    memset(&ResourceDesc, 0, sizeof(ResourceDesc));
    ResourceDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    ResourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    ResourceDesc.Texture2D.MipLevels = 1;
    ResourceDesc.Texture2D.MostDetailedMip = 0;
    if ((hr = pD3dDevice->CreateShaderResourceView(pFontTexture, &ResourceDesc, &this->pShaderResourceView)) != S_OK)
    {
        // Failed to create the shader resource view.
        printf("ImGuiRenderer::Initialize(): failed to create shader resource view 0x%08x\n", hr);
        return false;
    }

    // Release the font texture.
    pFontTexture->Release();

    // Save the shader resource view as the imgui texture id.
    io.Fonts->SetTexID((ImTextureID)this->pShaderResourceView);

    // Setup the texture sampler.
    D3D11_SAMPLER_DESC SamplerDesc;
    memset(&SamplerDesc, 0, sizeof(SamplerDesc));
    SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    SamplerDesc.MipLODBias = 0.0f;
    SamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    SamplerDesc.MinLOD = 0.0f;
    SamplerDesc.MaxLOD = 0.0f;
    if ((hr = pD3dDevice->CreateSamplerState(&SamplerDesc, &this->pSamplerState)) != S_OK)
    {
        // Failed to create the sampler state.
        DbgPrint("ImGuiRenderer::Initialize(): failed to create sampler state 0x%08x\n", hr);
        return false;
    }

    // Successfully initialized directx resources.
    DbgPrint("### INFO: ImGui renderer initialized successfully\n");
    this->initialized = true;
    return true;
}

bool ImGuiRenderer::ResizeVertexBuffers(int vertexBufferSize, int indexBufferSize)
{
    HRESULT hr = S_OK;

    // Get the d3d device pointer from sRender instance.
    ID3D11Device *pD3dDevice = *(ID3D11Device**)(((BYTE*)sRender::Instance()) + 0x8590);

    // Check if we need to resize the vertex buffer.
    if (this->vertexBufferSize < vertexBufferSize)
    {
        // Free the old vertex buffer.
        if (this->pVertexBuffer != nullptr)
        {
            this->pVertexBuffer->Release();
            this->pVertexBuffer = nullptr;
        }

        // Update the vertex buffer size.
        this->vertexBufferSize = vertexBufferSize;

        // Setup the vertex buffer.
        D3D11_BUFFER_DESC BufferDesc = { 0 };
        BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        BufferDesc.ByteWidth = this->vertexBufferSize * sizeof(ImDrawVert);
        BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        BufferDesc.MiscFlags = 0;
        if ((hr = pD3dDevice->CreateBuffer(&BufferDesc, nullptr, &this->pVertexBuffer)) != S_OK)
        {
            // Failed to create the vertex buffer.
            DbgPrint("ImGuiRenderer::ResizeVertexBuffers(): failed to create vertex buffer 0x%08x\n", hr);
            return false;
        }
    }

    // Check if we need to resize the index buffer.
    if (this->indexBufferSize < indexBufferSize)
    {
        // Free the old index buffer.
        if (this->pIndexBuffer != nullptr)
        {
            this->pIndexBuffer->Release();
            this->pIndexBuffer = nullptr;
        }

        // Update the index buffer size.
        this->indexBufferSize = indexBufferSize;

        // Setup the index buffer.
        D3D11_BUFFER_DESC BufferDesc = { 0 };
        BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        BufferDesc.ByteWidth = this->indexBufferSize * sizeof(ImDrawIdx);
        BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        BufferDesc.MiscFlags = 0;
        if ((hr = pD3dDevice->CreateBuffer(&BufferDesc, nullptr, &this->pIndexBuffer)) != S_OK)
        {
            // Failed to create the index buffer.
            DbgPrint("ImGuiRenderer::ResizeVertexBuffers(): failed to create index buffer 0x%08x\n", hr);
            return false;
        }
    }

    // Buffers successfully resized.
    return true;
}

void ImGuiRenderer::BeginFrame()
{
    // Get the IO structure.
    ImGuiIO& io = ImGui::GetIO();

    // Get the window size from sRender.
    SIZE windowSize = *(SIZE*)(((BYTE*)sRender::Instance()) + 0x8620);

    // Update display size and time delta.
    io.DisplaySize = ImVec2((float)windowSize.cx, (float)windowSize.cy);
    io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
    io.DeltaTime = sMain::Instance()->GameTime.DeltaTime1InSeconds;

    // Start a new imgui frame.
    ImGui::NewFrame();

    // If the imgui UI is not visible bail out.
    if (this->isVisible == false)
        return;

    // Check if we should draw the imgui demo window.
    //if (g_ShowImGuiDemo == true)
    //ImGui::ShowDemoWindow(&g_ShowImGuiDemo);

    // Draw the console window.
    ImGuiConsole::Instance()->Draw();

    // Draw the debug menu.
    sSnatcherToolImpl::Instance()->Draw();
}

void ImGuiRenderer::SystemUpdate()
{
    HRESULT hr = S_OK;

    // Finish the imgui frame.
    ImGui::Render();

    // If the UI layer is not visible bail out.
    if (this->isVisible == false)
        return;

    // Get the d3d device context from sRender instance.
    ID3D11DeviceContext *pDeviceContext = *(ID3D11DeviceContext**)(((BYTE*)sRender::Instance()) + 0x85A8);

    // Get the render target view and depth stencil.
    ID3D11RenderTargetView *pRenderTargetView = *(ID3D11RenderTargetView**)(((BYTE*)sRender::Instance()) + 0x272D0);
    ID3D11DepthStencilView *pDepthStencilView = *(ID3D11DepthStencilView**)(((BYTE*)sRender::Instance()) + 0x272D8);

    // Get the imgui draw data structure.
    ImDrawData *pDrawData = ImGui::GetDrawData();

    // Check if we need to resize the vertex buffer.
    if (this->vertexBufferSize < pDrawData->TotalVtxCount)
    {
        // Resize the vertex buffer.
        if (this->ResizeVertexBuffers(pDrawData->TotalVtxCount + 1000, 0) == false)
        {
            // Failed to resize the vertex buffer.
            DbgPrint("### FATAL: failed to resize vertex buffer during ImGui render pass!\n");
            DebugBreak();
            return;
        }
    }

    // Check if we need to resize the index buffer.
    if (this->indexBufferSize < pDrawData->TotalIdxCount)
    {
        // Resize the index buffer.
        if (this->ResizeVertexBuffers(0, pDrawData->TotalIdxCount + 1000) == false)
        {
            // Failed to resize the index buffer.
            DbgPrint("### FATAL: failed to resize index buffer during ImGui render pass!\n");
            DebugBreak();
            return;
        }
    }

    // Map the vertex and index buffers for cpu access.
    D3D11_MAPPED_SUBRESOURCE vertexResource, indexResource;
    if ((hr = pDeviceContext->Map(this->pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &vertexResource)) != S_OK)
    {
        // Failed to map the vertex buffer for cpu access.
        DbgPrint("ImGuiRenderer::SystemUpdate(): failed to map vertex buffer for cpu access 0x%08x\n", hr);
        return;
    }

    if ((hr = pDeviceContext->Map(this->pIndexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &indexResource)) != S_OK)
    {
        // Failed to map the index buffer for cpu access.
        DbgPrint("ImGuiRenderer::SystemUpdate(): failed to map index buffer for cpu access 0x%08x\n", hr);
        return;
    }

    // Cast the mapped buffer pointers to ImGui types for easy access.
    ImDrawVert *pVertexDst = (ImDrawVert*)vertexResource.pData;
    ImDrawIdx *pIndexDst = (ImDrawIdx*)indexResource.pData;

    // Loop and copy all the imgui draw data into the buffers.
    for (int i = 0; i < pDrawData->CmdListsCount; i++)
    {
        // Copy vertex and index data for the current command list.
        memcpy(pVertexDst, pDrawData->CmdLists[i]->VtxBuffer.Data, pDrawData->CmdLists[i]->VtxBuffer.Size * sizeof(ImDrawVert));
        memcpy(pIndexDst, pDrawData->CmdLists[i]->IdxBuffer.Data, pDrawData->CmdLists[i]->IdxBuffer.Size * sizeof(ImDrawIdx));

        pVertexDst += pDrawData->CmdLists[i]->VtxBuffer.Size;
        pIndexDst += pDrawData->CmdLists[i]->IdxBuffer.Size;
    }

    // Unmap the buffers.
    pDeviceContext->Unmap(this->pVertexBuffer, 0);
    pDeviceContext->Unmap(this->pIndexBuffer, 0);

    // Map the shader constant buffer for cpu access.
    D3D11_MAPPED_SUBRESOURCE shaderConstantResource;
    if ((hr = pDeviceContext->Map(this->pShaderConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &shaderConstantResource)) != S_OK)
    {
        // Failed to map shader constant buffer for cpu access.
        DbgPrint("ImGuiRenderer::SystemUpdate(): failed to map shader constant buffer for cpu access 0x%08x\n", hr);
        return;
    }

    // Update the orthographic projection matrix.
    DirectX::XMMATRIX projection = DirectX::XMMatrixOrthographicOffCenterLH(
        pDrawData->DisplayPos.x,
        pDrawData->DisplayPos.x + pDrawData->DisplaySize.x,
        pDrawData->DisplayPos.y + pDrawData->DisplaySize.y,
        pDrawData->DisplayPos.y,
        1.0f,
        -1.0f);
    memcpy(shaderConstantResource.pData, &projection, sizeof(DirectX::XMMATRIX));

    // Unmap the shader constant buffer.
    pDeviceContext->Unmap(this->pShaderConstantBuffer, 0);

    // Setup the directx rendering state.
    pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);

    pDeviceContext->IASetInputLayout(this->pInputLayout);
    pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    UINT vrtxStrides = sizeof(ImDrawVert), vrtxOffsets = 0;
    pDeviceContext->IASetVertexBuffers(0, 1, &this->pVertexBuffer, &vrtxStrides, &vrtxOffsets);
    pDeviceContext->IASetIndexBuffer(this->pIndexBuffer, sizeof(ImDrawIdx) == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT, 0);

    pDeviceContext->VSSetShader(this->pVertexShader, nullptr, 0);
    pDeviceContext->PSSetShader(this->pPixelShader, nullptr, 0);

    pDeviceContext->VSSetConstantBuffers(0, 1, &this->pShaderConstantBuffer);
    pDeviceContext->PSSetConstantBuffers(0, 1, &this->pShaderConstantBuffer);
    pDeviceContext->PSSetSamplers(0, 1, &this->pSamplerState);

    const float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    pDeviceContext->OMSetBlendState(this->pBlendState, blendFactor, 0xFFFFFFFF);
    pDeviceContext->RSSetState(this->pRasterState);

    // Set the clipping rectangle scale.

    // Loop through all the command lists and render each one.
    unsigned int vertexOffset = 0, indexOffset = 0;
    for (int i = 0; i < pDrawData->CmdListsCount; i++)
    {
        // Get the command list info.
        ImDrawList *pCmdList = pDrawData->CmdLists[i];

        // Loop through all the commands in the list.
        for (int x = 0; x < pCmdList->CmdBuffer.Size; x++)
        {
            // Check if there is a user callback for this command.
            if (pCmdList->CmdBuffer[x].UserCallback != nullptr)
            {
                // TODO: Special case
            }
            else
            {
                // Apply scissor/clipping rectangle.
                RECT scissorRect = 
                {
                    (int)(pCmdList->CmdBuffer[x].ClipRect.x - pDrawData->DisplayPos.x),
                    (int)(pCmdList->CmdBuffer[x].ClipRect.y - pDrawData->DisplayPos.y),
                    (int)(pCmdList->CmdBuffer[x].ClipRect.z - pDrawData->DisplayPos.x),
                    (int)(pCmdList->CmdBuffer[x].ClipRect.w - pDrawData->DisplayPos.y)
                };
                pDeviceContext->RSSetScissorRects(1, &scissorRect);

                // Check if this mesh is being rendered with a texture or not.
                if (pCmdList->CmdBuffer[x].TextureId != nullptr)
                {
                    // Bind the texture.
                    ID3D11ShaderResourceView *pShaderResource = (ID3D11ShaderResourceView*)pCmdList->CmdBuffer[x].TextureId;
                    pDeviceContext->PSSetShaderResources(0, 1, &pShaderResource);
                }

                // Draw primitives.
                pDeviceContext->DrawIndexed(pCmdList->CmdBuffer[x].ElemCount, pCmdList->CmdBuffer[x].IdxOffset + indexOffset, pCmdList->CmdBuffer[x].VtxOffset + vertexOffset);
            }
        }

        // Update the vertex and index offsets.
        vertexOffset += pCmdList->VtxBuffer.Size;
        indexOffset += pCmdList->IdxBuffer.Size;
    }
}

void ImGuiRenderer::SystemCleanup()
{

}

void ImGuiRenderer::BuildSystemMenu(MtPropertyList *pPropertyList)
{

}

__int64 ShowImGuiDemo(WCHAR **argv, int argc)
{
    g_ShowImGuiDemo = true;

    return 0;
}