
#pragma once
#include "MtFramework/Object/uCoord.h"
#include "MtFramework/Graphics/rModel.h"
#include "MtFramework/Graphics/rMotionList.h"

// sizeof = 0xD50
struct uModel : public uCoord
{
	// sizeof = 0xA0
	struct Joint : public MtObject
	{
		/* 0x08 */ BYTE			mAttr;
		/* 0x09 */ BYTE			mParent;
		/* 0x0A */ BYTE			mType;
		/* 0x0B */ BYTE			mNo;
		/* 0x0C */ float		mLength;

		/* 0x?? */ //float ? mpConstraint;

		/* 0x20 */ Vector3Aligned	mOffset;		// starting translation?
		/* 0x30 */ Vector4			mQuat;			// XMQuaternionRotationMatrix(joint data 2 SRT matrix)
		/* 0x40 */ Vector4			mScale;			// Set to Vector3(1.0f, 1.0f, 1.0f) at creation
		/* 0x50 */ Vector4			mTrans;			// Current translation
		/* 0x60 */ Vector4			SRTMatrix[4];	// joint SRT matrix from joint data 2
	};

	struct KeyFrameDescriptor
	{

	};

	// sizeof = 0x20
	struct KeyFrameBlendInfo
	{
		/* 0x00 */ KeyFrameDescriptor*	pKeyFrameDescriptor;
		/* 0x08 */ float				PreviousKeyFrameStart;	// Indicates what frame number the last key frame started at
		/* 0x10 */ void*				pKeyFrameData;			// Gets set to KeyFrameDescriptor->DataOffset			
		/* 0x18 */ float				BlendWeight;			// I think this is blend weight
	};
	static_assert(sizeof(KeyFrameBlendInfo) == 0x20, "uModel::KeyFrameBlendInfo incorrect struct size");

	// sizeof = 0x90
	struct Constraint	// I'm pretty sure this is the wrong name for this object
	{
		/* 0x00 */ KeyFrameBlendInfo	BlendInfo[3];	// 0 = rotation, 1 = translation, 2 = scale
		/* 0x60 */ Vector4				Quat;
		/* 0x70 */ Vector4				Offset;
		/* 0x80 */ Vector4				Scale;
	};
	static_assert(sizeof(Constraint) == 0x90, "uModel::Constraint incorrect struct size");

	// sizeof = 0x150
	struct Motion : public MtObject
	{
		/* 0x08 */ DWORD		mMotionNo;
		/* 0x0C */ WORD			mState;
		/* 0x0E */ WORD			mAttr;
		/* 0x10 */ float		mInterCount;
		/* 0x14 */ signed short	mInterFrame;
		/* 0x18 */ float		mBlend;
		/* 0x1C */ DWORD		mFrame;
		/* 0x20 */ DWORD		mFrameMax;			// Set to AnimationDescriptor->FrameCount
		/* 0x24 */ float		mSpeed;
		/* 0x28 */ //DWORD?
		/* 0x2C */ // mInterCurve
		
		/* 0x40 */ //Vector4		CurrentTranslation?		// 0x1406B39DC 	Interpolate(&uModel->mMotion[motionIndex].BlendInfo[1], uModel->mMotion[motionIndex].mFrame, 			uModel->mMotion[motionIndex].mFrameMax)
		/* 0x50 */ //Vector4		// 0x1406B39DC 							Interpolate(&uModel->mMotion[motionIndex].BlendInfo[1], uModel->mMotion[motionIndex].mFrameMax - 1.0f, 	uModel->mMotion[motionIndex].mFrameMax)
		/* 0x60 */ //Vector4		// 0x1406BC4D0 							Interpolate(&uModel->mMotion[i].BlendInfo[1], 			uModel->mMotion[i].mFrame, 						uModel->mMotion[i].mFrameMax)
		/* 0x70 */ Vector4 		mPrevTrans;		// 						Interpolate(&uModel->mMotion[motionIndex].BlendInfo[0], uModel->mMotion[motionIndex].mFrame, 			uModel->mMotion[motionIndex].mFrameMax)
		/* 0x80 */ Vector4 		mBaseTrans;
		/* 0x90 */ //Vector4		// 0x1406BC4D0 							Interpolate(&uModel->mMotion[i].BlendInfo[0], 			uModel->mMotion[i].mFrame, 						uModel->mMotion[i].mFrameMax)
		/* 0xA0 */ KeyFrameBlendInfo	BlendInfo[2];
		/* 0xE0 */ //void* // points to AnimationDescriptor+0x30
		/* 0xE8 */ //void* // points to AnimationDescriptor+0x80		
		/* 0xF0 */ DWORD mSequence;
		/* 0xF4 */
		/* 0xF8 */ DWORD mPrevSequence;
		/* 0xFC */
		/* 0x100 */ Vector4		mBasePlane;
		/* 0x110 */ float		mWaistOffset;
		/* 0x114 */ float		mWaistAdjustFactor;
		/* 0x118 */ float		mWaistAdjust;
		/* 0x11C */ //float?		mWaistAdjustCurve;
		
		/* 0x124 */ BOOL		mAnkleAdjust;
		/* 0x125 */ BOOL		mFootAdjust;
		
		/* 0x128 */ float 		mRotInverseWeight;
		/* 0x12C */ float		mWaistAdjustSpeed;
		/* 0x130 */ float		mHeelHeight;
		
		/* 0x138 */ Constraint* pConstraints;		// This is really the interpolated skeleton
		/* 0x140 */ //DWORD?
		/* 0x144 */ //DWORD?
		/* 0x148 */ //float
		/* 0x14C */ //DWORD?

		BYTE _padding[0x150 - 0xE0];
	};
	static_assert(sizeof(Motion) == 0x150, "uMode::Motion incorrect struct size");

	// sizeof = 0x50
	struct RenderInfo : public MtObject
	{
		/* 0x08 */ DWORD 		mRenderMode;
		/* 0x0C */ DWORD 		mCullMode;
		/* 0x10 */ int 			mZPrepassDist;
		/* 0x14 */ int 			mMotionBlurDist;
		/* 0x18 */ float 		mTransparency;
		/* 0x1C */ int 			mPriorityBias;

		/* 0x30 */ Vector3		mUVScroll;
	};

	/* 0xF0 */ DWORD		mBlendNum;
	/* 0xF4 */ DWORD		JointCount;
	/* 0xF8 */ Joint*		mJoint;			// Array of uModel::Joint objects
	/* 0x100 */ Motion 		mMotion[8];		// Sized for mBlendNum, not sure what array length is

	/* 0xBC0 */ DWORD		mLightGroup;
	/* 0xBC4 */ //DWORD 		PartsDisp[? ];	// Used to determine what groupIDs to display for primitives (0x1406B8E76)

	/* 0xC04 */ //DWORD ?

	/* 0xC10 */ //RenderInfo
	/* 0xC60 */ // 0xD0 on rModel, BB sphere?
	/* 0xC70 */ //Vector4
	/* 0xC80 */ //Vector4
	/* 0xC90 */ // byte?

	/* 0xC98 */ rModel* mpModel;		// Gets set to rModel instance after creation

	/* 0xCA8 */ rMotionList* mpMotionList[16];
	/* 0xD28 */
	/* 0xD30 */ Vector3Aligned		mLightMapScale;
	/* 0xD40 */ BOOL		mShowBoundingBox;
	/* 0xD41 */ BOOL		mShowBoundingSphere;
	/* 0xD42 */ BOOL		mShowJoint;

	/*
		VTable:
			0x70 AssignModel
			0x78
			0x80
			0x88
	*/

	inline static uModel* (__stdcall* _ctor)(uModel* thisptr) =
		(uModel * (__stdcall*)(uModel*))GetModuleAddress(0x1406A18E0);

	inline static uModel* (__stdcall* _dtor)(uModel* thisptr, bool bFreeMemory) =
		(uModel * (__stdcall*)(uModel*, bool))GetModuleAddress(0x1406A1D40);

	inline static void (__stdcall* _AssignModel)(uModel* thisptr, rModel* pModel) =
		(void(__stdcall*)(uModel*, rModel*))GetModuleAddress(0x1406B2730);

	inline static void (__stdcall* _AssignAnimation)(uModel* thisptr, rMotionList* pAnimation, int Slot) =
		(void(__stdcall*)(uModel*, rMotionList*, int))GetModuleAddress(0x1406B2E30);

    IMPLEMENT_MYDTI(uModel, 0x141D17FE8, 0x1400AF010, 0x1406B1DB0);

	uModel()
	{
		_ctor(this);
	}

	void AssignModel(rModel* pModel)
	{
		(void)ThisPtrCallNoFixup(this->vtable[14], this, pModel);
	}

	void AssignAnimation(rMotionList* pAnimation, int Slot)
	{
		_AssignAnimation(this, pAnimation, Slot);
	}
};