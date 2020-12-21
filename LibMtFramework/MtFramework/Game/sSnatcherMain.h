
#pragma once
#include "MtFramework/MtObject.h"
#include "sMain.h"

// sizeof = 0x21110
struct sSnatcherMain : public sMain
{
    /* 0x20248 */
    /* 0x20250 */ void  *mpRender;
    /* 0x20258 */ void  *mpSnatcherTool;
    /* 0x20260 */ void  *mpUnit;
    /* 0x20268 */ void  *mpResource;
    /* 0x20270 */ void  *mpCamera;
    /* 0x20278 */ void  *mpPad;
    /* 0x20280 */ void  *mpPrim;

    /* 0x20290 */ void  *mpEffect;
    /* 0x20298 */ void  *mpCollision;
    /* 0x202A0 */ void  *mpSMManager;
    /* 0x202A8 */ void  *mpZombieManager;

    /* 0x202B8 */ void  *mpNpcFuncManager;
    /* 0x202C0 */ void  *mpNpcWordManager;
    /* 0x202C8 */ void  *mpNpcManager;

    /* 0x202D8 */ void  *mpHoldCtrl;
    /* 0x202E0 */ void  *mpItemCtrl;
    /* 0x202E8 */ void  *mpScore;
    /* 0x202F0 */ void  *mpTaskManager;
    /* 0x202F8 */ void  *mpAreaManager;

    /* 0x20308 */ void  *mpPathFind;
    /* 0x20310 */ void  *mpNpcMarker;

    /* 0x20320 */ void  *mpAreaHit;
    /* 0x20328 */ void  *mpPictureViewer;
    /* 0x20330 */ void  *mpLight;

    /* 0x20340 */ void  *mpFade;
    /* 0x20348 */ void  *mpShader;

    /* 0x20358 */ void  *mpCapture;

    /* 0x20370 */ void  *mpSCQManager;
    /* 0x20378 */ void  *mpLoadingEffect;
    /* 0x20380 */ void  *mpGpuParticle;

    /* 0x20398 */ void  *mpModelLayout;
    /* 0x203A0 */ void  *mpEffectVFR;

    IMPLEMENT_SINGLETON(sSnatcherMain, 0x141944DD8);
};