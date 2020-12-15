
#pragma once
#include "LibMtFramework.h"
#include "MtFramework/Game/cSystem.h"
#include "cTrans.h"
#include <d3d11.h>

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
    };
    static_assert(sizeof(Buffer) == 0x30, "sRender::Buffer incorrect struct size");

    /* 0x38 */

    /* 0x83D0 */ RECT       WindowRect;

    /* 0x8586 */ BOOL       mDisableRendering;
    /* 0x8587 */ BOOL       mParallelTrans;
    /* 0x8588 */ BOOL       mParallelRendering;

    /* 0x858A */ // Store Frame
    /* 0x858C */ float      mPersRate;
    /* 0x8590 */ ID3D11Device   *pD3dDevice;
    /* 0x8598 */ // mPresentRect (0x12 = RECT?)
    /* 0x85A8 */ ID3D11DeviceContext    *pDeviceContext;

    /* 0x8610 */ // DWORD index into 0x2F4C0
    /* 0x8618 */ IDXGISwapChain *pSwapChain;
    /* 0x8620 */ // SIZE window size?

    /* 0x8648 */ HANDLE     hRenderThread;

    /* 0x8658 */ HANDLE     hRenderEvent1;      // Signaled when a frame should be drawn
    /* 0x8660 */ HANDLE     hRenderEvent2;      // Signaled when the frame is complete

    /* 0x8680 */ float      mGammaMin;
    /* 0x8684 */ float      mGammaMax;

    /* 0x868D */ BOOL       mDynamicTrans;

    /* 0x8690 */ cTrans     mTrans[6];
    /* 0x24B90 */ // 
    /* 0x24B98 */ //

    /* 0x24BC0 */ //DWORD
    /* 0x24BC8 */ Pass      mPassProfile[12];
    /* 0x25288 */ TempTexture TempTextures[256];
    /* 0x27288 */ DWORD     TempTextureCount;

    /* 0x272A0 */ //cTrans::Texture mpScreenTexture

    /* 0x272B0 */ // mpScreenZTexture

    /* 0x272C0 */ // mpCacheFrameTexture

    /* 0x272C8 */ DWORD     Interval;
    /* 0x272CC */ DWORD     Threshold;

    /* 0x27418 */ DWORD     mQualityControl;
    /* 0x2741C */ DWORD     mQuality;
    /* 0x27420 */ ULONGLONG mRenderTimeOld;
    /* 0x27428 */ ULONGLONG mRenderTimeMargin;

    /* 0x27450 */ DWORD     mPSGPRCount;

    /* 0x27460 */ // Elements
    
    /* 0x2F460 */ // Elements count?
    /* 0x2F468 */ CRITICAL_SECTION  ElementsListLock;
    /* 0x2F490 */ // CRITICAL_SECTION
    /* 0x2F4C0 */ // Buffer buffer object[4]

    IMPLEMENT_MYDTI(sRender, 0x141CF32D0, 0x1400AF010, 0x140663EC0);

    IMPLEMENT_SINGLETON(sRender, 0x141CF3268);
};