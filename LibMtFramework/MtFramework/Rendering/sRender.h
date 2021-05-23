
#pragma once
#include "LibMtFramework.h"
#include "MtFramework/System/cSystem.h"
#include "cTrans.h"
#include <d3d11.h>
#include <map>

// sizeof = 0x2F500
struct sRender : public cSystem
{
    // sizeof = 0x90
    struct Pass : public MtObject
    {
        /* 0x08 */ BOOL     mEnable;
        /* 0x10 */ char     *mName;
        /* 0x18 */ DWORD    mFillMode;
        /* 0x1C */ DWORD    mVSGPR;

        /* 0x58 */ DWORD    mCommand;
        /* 0x5C */ DWORD    mMesh;
        /* 0x60 */ DWORD    mMaterial;
        /* 0x64 */ DWORD    mVShader;
        /* 0x68 */ DWORD    mPShader;
        /* 0x6C */ DWORD    mVSConstant;
        /* 0x70 */ DWORD    mPSConstant;
        /* 0x74 */ DWORD    mDrawState;
        /* 0x78 */ DWORD    mBlendState;
        /* 0x7C */ DWORD    mSamplerState;
        /* 0x80 */ DWORD    mPolygon;
        /* 0x84 */ DWORD    mPrimitive;
        /* 0x88 */ DWORD    mClear;
        /* 0x8C */ DWORD    mResolve;
    };

    // sizeof = 0x20
    struct TempTexture : public MtObject
    {
        /* 0x08 */ char     *mPoolName;
        /* 0x10 */ char     *mName;
        /* 0x18 */ void     *mpTexture;
    };
    static_assert(sizeof(TempTexture) == 0x20, "sRender::TempTexture incorrect struct size");

    // sizeof = 0x30
    struct Buffer
    {
        #define BUFFER_TYPE_VERTEX      0   // Vertex buffer (D3D11_BIND_VERTEX_BUFFER)
        #define BUFFER_TYPE_INDEX       1   // Index buffer (D3D11_BIND_INDEX_BUFFER)

        /* 0x00 */ ID3D11Buffer         *pBuffer;
        /* 0x08 */ DWORD                CurrentPosition;    // Current position in the buffer to write data to
        /* 0x10 */ ULONGLONG            mMaxSize;           // Total size of the buffer in bytes
        /* 0x18 */ DWORD                PreviousPosition;
        /* 0x20 */ ID3D11DeviceContext  *pDeviceContext;    // Device context used to create the buffer
        /* 0x28 */ DWORD                BufferType;         // Type of buffer see BUFFER_TYPE_* above

        inline static Buffer * (__stdcall *_ctor)(Buffer *thisptr, ID3D11DeviceContext *pDeviceContext, DWORD dwBufferSize, DWORD dwBufferType) =
            GetModuleAddress<Buffer*(__stdcall*)(Buffer*, ID3D11DeviceContext*, DWORD, DWORD)>(0x14065A030);

        inline static void * (__stdcall *_MapForWrite)(Buffer *thisptr, DWORD dwSize) =
            GetModuleAddress<void*(__stdcall*)(Buffer*, DWORD)>(0x14065DEC0);

        /*
            Description: Maps the buffer for writing. If the current position in the buffer is 0 the contents are discarded on mapping,
                otherwise the contents are preseved. If the current position in the buffer + the requested size exceeds the capacity of
                the buffer an assert is triggered and the game will crash.

            Parameters:
                - dwSize: Requested size for writing, if the current position + dwSize exceeds the capacity of the buffer an assert is
                    triggered and the game will crash

            Returns: Pointer within the buffer available for writing
        */
        template<typename T> T* MapForWrite(DWORD dwSize)
        {
            return (T*)_MapForWrite(this, dwSize);
        }
    };
    static_assert(sizeof(Buffer) == 0x30, "sRender::Buffer incorrect struct size");

    // sizeof = 0x10
    struct DeferredContext
    {
        /* 0x00 */ ID3D11DeviceContext  *pDeviceContext;
        /* 0x08 */ DWORD                InUse;              // Indicates if the device context is currently being used by another thread
    };
    static_assert(sizeof(DeferredContext) == 0x10, "sRender::DeferredContext incorect struct size");

    /* 0x38 */ // array? elem size = 0x2040 (0x1406629B9)

    /* 0x8138 */ // DWORD index into array at 0x38?

    /* 0x83D0 */ RECT       WindowRect;

    /* 0x83F8 */ DWORD      RefreshRateNumerator;
    /* 0x83FC */ DWORD      RefreshRateDenominator;

    /* 0x8404 */ BOOL       VSync;
    /* 0x8408 */ DWORD      AntiAliasing;
    /* 0x840C */ DWORD      ShadowQuality;
    /* 0x8410 */ DWORD      TextureFiltering;
    /* 0x8414 */ DWORD      FullScreen;
    /* 0x8418 */ BOOL       MotionBlur;

    /* 0x8585 */ bool       ShouldPostQuitMessage;  // True if a quit message should be posted to the main game thread
    /* 0x8586 */ bool       mDisableRendering;
    /* 0x8587 */ bool       mParallelTrans;
    /* 0x8588 */ bool       mParallelRendering;     // True if rendering should be done on a seperate thread
    /* 0x8589 */ bool       RenderAsync;            // Same as above but set in the render loop based on mParallelRendering
    /* 0x858A */ // Store Frame
    /* 0x858C */ float      mPersRate;
    /* 0x8590 */ ID3D11Device   *pD3dDevice;
    /* 0x8598 */ // mPresentRect (0x12 = RECT?)
    /* 0x85A8 */ ID3D11DeviceContext    *pDeviceContext;
    /* 0x85B0 */ DeferredContext        DeferredContexts[3];
    /* 0x85E0 */ ID3D11DeviceContext    *MainThreadDeferredContexts[2];     // Deferred contexts for the main thread, swapped at the end of each frame
    /* 0x85F0 */ // DeferredContext[2]
    /* 0x8610 */ DWORD                  ActiveBufferIndex;                  // Index into pVertexBuffers/pIndexBuffers buffers
    /* 0x8618 */ IDXGISwapChain         *pSwapChain;
    /* 0x8620 */ SIZE                   WindowSize;

    /* 0x8648 */ HANDLE     hRenderThread;

    /* 0x8658 */ HANDLE     hRenderEvent1;      // Signaled when a frame should be drawn
    /* 0x8660 */ HANDLE     hRenderEvent2;      // Signaled when the frame is complete

    /* 0x8680 */ float      mGammaMin;
    /* 0x8684 */ float      mGammaMax;
    /* 0x8688 */ float      Gamma;

    /* 0x868D */ bool       mDynamicTrans;

    /* 0x8690 */ cTrans                         mTrans[6];
    /* 0x24B90 */ DWORD                         RenderBufferIndex;          // Index into pRenderCommandBuffer/pRenderCommands/RenderCommandCount arrays below
    /* 0x24B98 */ void                          *pRenderCommandBuffer[2];   // Buffer backing all cTrans->pCommandBuffer's
    /* 0x24BA8 */ cTrans::RenderCommandInfo     *pRenderCommands[2];        // Sorted render commands from all mTrans command buffers alloc size = 1MB
    /* 0x24BB8 */ DWORD                         RenderCommandCount;         // Number of render commands in the pRenderCommands buffer
    /* 0x24BC0 */ DWORD                         RenderDataMemorySize;       // Size of memory allocated for render command data (pRenderCommandBuffer), 14MB
    /* 0x24BC8 */ Pass          mPassProfile[12];
    /* 0x25288 */ TempTexture   TempTextures[256];
    /* 0x27288 */ DWORD         TempTextureCount;

    /* 0x272A0 */ //cTrans::Texture mpScreenTexture

    /* 0x272B0 */ // mpScreenZTexture

    /* 0x272C0 */ // mpCacheFrameTexture

    /* 0x272C8 */ DWORD     Interval;
    /* 0x272CC */ DWORD     Threshold;
    /* 0x272D0 */ ID3D11RenderTargetView        *pRenderTargetView;
    /* 0x272D8 */ ID3D11DepthStencilView        *pDepthStencilView;

    /* 0x27348 */ std::map<DWORD, ID3D11InputLayout>    mInputLayouts;

    /* 0x27398 */ // ID3D11Buffer * array of vertex buffers for something

    /* 0x27408 */ ULONGLONG StartFrameTime;     // Time at the start of the frame
    /* 0x27410 */ ULONGLONG DeltaFrameTime;     // How long it took to do frame operations
    /* 0x27418 */ DWORD     mQualityControl;
    /* 0x2741C */ DWORD     mQuality;
    /* 0x27420 */ ULONGLONG mRenderTimeOld;
    /* 0x27428 */ ULONGLONG mRenderTimeMargin;

    /* 0x27450 */ DWORD     mPSGPRCount;

    /* 0x27460 */ cTrans::Element   *ElementList[4096];
    
    /* 0x2F460 */ DWORD             ElementCount;
    /* 0x2F468 */ CRITICAL_SECTION  ElementListLock;
    /* 0x2F490 */ CRITICAL_SECTION  DeferredContextListLock;
    /* 0x2F4C0 */ Buffer *pVertexBuffers[2];
    /* 0x2F4D0 */ Buffer *pIndexBuffers[2];

    IMPLEMENT_MYDTI(sRender, 0x141CF32D0, 0x1400AF010, 0x140663EC0);

    IMPLEMENT_SINGLETON(sRender, 0x141CF3268);

    static inline sRender* (__stdcall *_ctor)(sRender *thisptr, DWORD interval, DWORD dwUnused1, DWORD dwGraphicsMemSize, DWORD dwUnused2) =
        GetModuleAddress<sRender*(__stdcall*)(sRender*, DWORD, DWORD, DWORD, DWORD)>(0x14065A140);

    static inline void(__stdcall *_SystemUpdate)(sRender *thisptr) =
        GetModuleAddress<void(__stdcall*)(sRender*)>(0x1406638E0);

    static inline void(__stdcall *_SystemCleanup)(sRender *thisptr) =
        GetModuleAddress<void(__stdcall*)(sRender*)>(0x1406642B0);

    static inline void(__stdcall *_RenderFrame)(sRender *thisptr) =
        GetModuleAddress<void(__stdcall*)(sRender*)>(0x140666860);

    static inline void(__stdcall *_BeginFrame)(sRender *thisptr) =
        GetModuleAddress<void(__stdcall*)(sRender*)>(0x14065ED90);

    static inline void(__stdcall *_DrawFrame)(sRender *thisptr) =
        GetModuleAddress<void(__stdcall*)(sRender*)>(0x140662200);

    static inline void(__stdcall *_EndFrame)(sRender *thisptr) =
        GetModuleAddress<void(__stdcall*)(sRender*)>(0x140661D30);

    static inline void(__stdcall *_Present)(sRender *thisptr) =
        GetModuleAddress<void(__stdcall*)(sRender*)>(0x140661AD0);

    /*
        Description: If asynchronous rendering is disabled draws the next frame, otherwise waits for the rendering thread
            to draw the next frame (BeginFrame, DrawFrame, EndFrame).
    */
    void RenderFrame()
    {
        _RenderFrame(this);
    }
};