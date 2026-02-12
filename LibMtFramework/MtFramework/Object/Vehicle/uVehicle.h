
#pragma once
#include "MtFramework/Object/Model/uSnatcherModel.h"
#include "MtFramework/Object/Explosion/cExplodeParts.h"
#include "MtFramework/Physics/Vehicle/rHavokVehicleData.h"
#include "MtFramework/Physics/rHavokVertexLayout.h"

enum eVehicleVital
{
    VITALFRONTRIGHT = 0,
    VITALFRONTLEFT,
    VITALBACKRIGHT,
    VITALBACKLEFT,
    VITALDOORRIGHT,
    VITALDOORLEFT,
    VITALFRONTGLASS,

    VITALMAX
};

// sizeof = 0x32A0
struct uVehicle : public uSnatcherModel
{
    /* 0x1390 */ //cExplodeParts
    /* 0x2DC0 */ 

    /* 0x2DD0 */ //rSoundWed*
    /* 0x2DD8 */ //rSoundRrd*

    /* 0x2DF0 */ Vector3            mPlOffset;

    /* 0x2E00 */ Vector3Aligned     mCenterOffset;
    /* 0x2E10 */ Vector3Aligned     mOffset1;
    /* 0x2E20 */ Vector3Aligned     mOffset2;
    /* 0x2E30 */ Vector3Aligned     mDamageHitOffs1[3];
    /* 0x2E60 */ Vector3Aligned     mDamageHitOffs2[3];
    /* 0x2E90 */ Vector3            mRideOffs;

    /* 0x2ED0 */ Vector3Aligned     mCameraTargetOffs;
    /* 0x2EE0 */ Vector3Aligned     mCameraPosOffs;
    /* 0x2EF0 */ Vector3Aligned     mCameraTargetOffsGetIn;
    /* 0x2F00 */ Vector3Aligned     mCameraPosOffsGetIn;

    /* 0x2F20 */ rHavokVertexLayout*    pHavokVertexLayout;

    /* 0x2F30 */ rHavokVehicleData* pVehicleHavokData;

    /* 0x2F7C */ int 		        mVheicleVital[VITALMAX];
    /* 0x2F98 */

    /* 0x3050 */ DWORD 		        mFrontRightDamageType;
    /* 0x3054 */ DWORD 		        mFrontLeftDamageType;
    /* 0x3058 */ DWORD 		        mBackRightDamageType;
    /* 0x305C */ DWORD 		        mBackLeftDamageType;
    /* 0x3060 */ DWORD 		        mFrontDoorRightDamageType;
    /* 0x3064 */ DWORD 		        mFrontDoorLeftDamageType;

    /* 0x3078 */ DWORD 		        mFrontGlassDamageType;

    /* 0x3094 */ DWORD 		        mPartsNo;
    /* 0x3098 */ float              mDamageHitRadius[3];

    /* 0x30A4 */ float              LinearDamping;
    /* 0x30A8 */ float              AgularDamping;
    /* 0x30AC */ float              Friction;
    /* 0x30B0 */ float              Rest;

    /* 0x30B8 */ int                mRideAngleY;
    /* 0x30BC */ float              mRideRadius;

    /* 0x3104 */ float 		        mHitObjSmallTimer;
    /* 0x3108 */ float 		        mHitObjSmallRate;
    /* 0x310C */ int                mRideOnEnableLength;
    /* 0x3110 */ float              mRideOnEnableScrHitSize;
    /* 0x3120 */ Vector3Aligned     mPosOffset;
    /* 0x3130 */ Vector3Aligned     mTargetOffset;
    /* 0x3140 */ float              mStart;
    /* 0x3144 */ float              mEnd;
    /* 0x3148 */ float              mCone;
    /* 0x314C */ float              mSpread;
    /* 0x3150 */ float              mColorRate;

    /*
        VTable:
            0x1B0 sets up havok state?

            0x228 sets which parts of the model are displayed
    */

    inline static uVehicle* (__stdcall* _ctor)(uVehicle* thisptr) =
        (uVehicle * (__stdcall*)(uVehicle*))GetModuleAddress(0x1401BD720);

    inline static uVehicle* (__stdcall* _dtor)(uVehicle* thisptr, bool bFreeMemory) =
        (uVehicle * (__stdcall*)(uVehicle*, bool))GetModuleAddress(0x1401BDAB0);

    inline static void (__stdcall* _LoadUnitResources)(uVehicle* thisptr) =
        (void(__stdcall*)(uVehicle*))GetModuleAddress(0x1401C49D0);

    IMPLEMENT_MYDTI(uVehicle, 0x141950838, 0x1400AF010, 0x1401E94B0);

    uVehicle()
    {
        _ctor(this);
    }
};