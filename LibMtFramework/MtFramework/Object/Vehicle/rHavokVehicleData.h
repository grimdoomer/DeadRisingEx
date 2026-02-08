
#pragma once
#include "MtFramework/Archive/cResource.h"

// sizeof = 0x1B0
struct rHavokVehicleData : public cResource
{
    // sizeof = 0x80
    struct TireData : public MtObject
    {
        /* 0x08 */ float 	radius;
        /* 0x0C */ float 	mass;
        /* 0x10 */ float 	width;
        /* 0x14 */ float 	friction;
        /* 0x18 */ float 	viscosityFriction;
        /* 0x1C */ float 	maxFriction;
        /* 0x20 */ float 	forceFeedbackMultiplier;
        /* 0x24 */ float 	maxContactBodyAcceleration;
        /* 0x28 */ BYTE 	axle;
        /* 0x29 */ BYTE 	jointno;
        /* 0x2A */ BYTE 	parentno;
        /* 0x2B */ BOOL 	doesWheelSteer;
        /* 0x2C */ float 	torqueRatio;
        /* 0x30 */ float 	maxBreakingTorque;
        /* 0x34 */ float 	minPedalInputToBlock;
        /* 0x38 */ BOOL 	isConnectedToHandbrake;
        /* 0x3C */ float 	strength;
        /* 0x40 */ float 	dampingCompression;
        /* 0x44 */ float 	dampingRelaxation;
        /* 0x50 */ Vector3Aligned 	hardpointCs;
        /* 0x60 */ Vector3Aligned 	directionCs;
        /* 0x70 */ float 	length;
        /* 0x74 */ float    _unused[3];
    };
    static_assert(sizeof(TireData) == 0x80, "rHavokVehicleData::TireData incorrect struct size");

    /* 0x60 */ BOOL 	mBalanceSystem;
    /* 0x64 */ float 	mWeight;
    /* 0x70 */ Vector3Aligned 	mGravity;
    /* 0x80 */ float 	mTorqueRollFactor;
    /* 0x84 */ float 	mTorquePitchFactor;
    /* 0x88 */ float 	mTorqueYawFactor;
    /* 0x8C */ float 	mExtraTorqueFactor;
    /* 0x90 */ float 	mMaxVelocityForPositionalFriction;
    /* 0x94 */ float 	mChassisUnitInertiaYaw;
    /* 0x98 */ float 	mChassisUnitInertiaRoll;
    /* 0x9C */ float 	mChassisUnitInertiaPitch;
    /* 0xA0 */ float 	mFrictionEqualizer;
    /* 0xA4 */ DWORD 	mNumWheels;
    /* 0xA8 */ float 	mSlopeChangePointX;
    /* 0xAC */ float 	mInitialSlope;
    /* 0xB0 */ float 	mDeadZone;
    /* 0xB4 */ BOOL 	mAutoReverse;
    /* 0xB8 */ float 	mMaxSteeringAngle;
    /* 0xBC */ float 	mMaxSpeedFullSteeringAngle;
    /* 0xC0 */ float 	mMinRPM;
    /* 0xC4 */ float 	mOptRPM;
    /* 0xC8 */ float 	mMaxRPM;
    /* 0xCC */ float 	mMaxTorque;
    /* 0xD0 */ float 	mTorqueFactorAtMinRPM;
    /* 0xD4 */ float 	mTorqueFactorAtMaxRPM;
    /* 0xD8 */ float 	mResistanceFactorAtMinRPM;
    /* 0xDC */ float 	mResistanceFactorAtOptRPM;
    /* 0xE0 */ float 	mResistanceFactorAtMaxRPM;
    /* 0xE4 */ DWORD 	mNumGears;
    /* 0xE8 */ float 	mTopSpeed;
    /* 0xEC */ float 	mDownshiftRPM;
    /* 0xF0 */ float 	mUpshiftRPM;
    /* 0xF4 */ float 	mClutchDelayTime;
    /* 0xF8 */ float 	mReverseGearRatio;
    /* 0xFC */ float 	mGearsRatio[6];
    /* 0x114 */ float 	mWheelsMinTimeToBlock;
    /* 0x118 */ float 	mAirDensity;
    /* 0x11C */ float 	mFrontalArea;
    /* 0x120 */ float 	mDragCoefficient;
    /* 0x124 */ float 	mLiftCoefficient;
    /* 0x130 */ Vector3Aligned 	mExtraGravityws;
    /* 0x140 */ float 	mNormalSpinDamping;
    /* 0x144 */ float 	mCollisionSpinDamping;
    /* 0x148 */ float 	mCollisionThreshold;
    /* 0x14C */ float 	mPositionX;
    /* 0x150 */ float 	mPositionY;
    /* 0x154 */ BOOL 	mHandbrakeButtonPressed;
    /* 0x155 */ BOOL 	mReverseButtonPressed;
    /* 0x158 */ TireData*   mpTireWk[10];

    inline static rHavokVehicleData* (__stdcall* _ctor)(rHavokVehicleData* thisptr) =
        (rHavokVehicleData * (__stdcall*)(rHavokVehicleData*))GetModuleAddress(0x1400C6E90);

    inline static rHavokVehicleData* (__stdcall* _dtor)(rHavokVehicleData* thisptr, bool bFreeMemory) =
        (rHavokVehicleData * (__stdcall*)(rHavokVehicleData*, bool))GetModuleAddress(0x1400C72E0);

    IMPLEMENT_MYDTI(rHavokVehicleData, 0x141947488, 0x1400AF010, 0x1400C98A0);

    rHavokVehicleData()
    {
        _ctor(this);
    }

    ~rHavokVehicleData()
    {
        (void)ThisPtrCallNoFixup(this->vtable[0], this, false);
    }
};
static_assert(sizeof(rHavokVehicleData) == 0x1B0, "rHavokVehicleData incorrect struct size");