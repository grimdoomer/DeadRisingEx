
#pragma once
#include "DeadRisingEx.h"
#include <MtFramework/System/cSystem.h>
#include <d3d11.h>

struct ImGuiRenderer : public cSystem
{
protected:

    // Indicates if the UI layer is visible or not.
    bool isVisible = false;

    // Vertex buffer input layout.
    ID3D11InputLayout *pInputLayout = nullptr;

    // Vertex and index buffers.
    ID3D11Buffer *pVertexBuffer = nullptr;
    ID3D11Buffer *pIndexBuffer = nullptr;

    // Current size of the vertex and index buffers.
    int vertexBufferSize = 0;
    int indexBufferSize = 0;

    // Vertex and pixel shader data.
    ID3D11VertexShader *pVertexShader = nullptr;
    ID3D11PixelShader *pPixelShader = nullptr;
    ID3D11Buffer *pShaderConstantBuffer = nullptr;
    ID3D11ShaderResourceView *pShaderResourceView = nullptr;

    // Raster states.
    ID3D11SamplerState *pSamplerState = nullptr;
    ID3D11RasterizerState *pRasterState = nullptr;
    ID3D11BlendState *pBlendState = nullptr;
    ID3D11DepthStencilState *pDepthStencilState = nullptr;

    bool ResizeVertexBuffers(int vertexBufferSize, int indexBufferSize);

public:

    static ImGuiRenderer * Instance();

    ImGuiRenderer();
    ~ImGuiRenderer();

    bool Initialize();

    void BeginFrame();

    void SystemUpdate();
    void SystemCleanup();
    void BuildSystemMenu(MtPropertyList *pPropertyList);

    bool IsVisible()
    {
        return this->isVisible;
    }

    bool SetVisible(bool isVisible)
    {
        this->isVisible = isVisible;
    }
};