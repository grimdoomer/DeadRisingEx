/*
	DRDebugger - Open source SDK/debugger for Dead Rising PC

	rModel.h - rModel object definition

	Author - Grimdoomer
*/

#pragma once
#include "LibMtFramework.h"
#include "MtFramework/Archive/cResource.h"

// sizeof = 0x18
struct Joint
{
	/* 0x00 */ BYTE Index;
	/* 0x01 */ BYTE ParentIndex;
	/* 0x02 */ BYTE pad[6];   // 6 bytes of padding to align floats
	/* 0x08 */ float Length;
	/* 0x0C */ Vector3 Offset;
};
static_assert(sizeof(Joint) == 0x18, "Joint incorrect struct size");

// sizeof = 0x50
struct Primitive
{
	/* 0x00 */ WORD		GroupID;				// Lower 5 bits = sub part number, upper 11 bits = part number (see uModel->PartsDisp)
	/* 0x02 */ WORD		MaterialIndex;
	/* 0x04 */ BYTE		Enabled;
	/* 0x05 */ BYTE		Unk3;					// Something to do with min/max draw distance clipping
	/* 0x06 */ BYTE		Unk11;
	/* 0x07 */ BYTE		Unk12;
	/* 0x08 */ BYTE		VertexStride1;
	/* 0x09 */ BYTE		VertexStride2;
	/* 0x0A */ BYTE		Unk13;
	/* 0x0B */ // padding
	/* 0x0C */ DWORD	VertexCount;
	/* 0x10 */ DWORD	StartingVertex;
	/* 0x14 */ DWORD	VertexStream1Offset;      // Passed to CDeviceContext::IASetVertexBuffers
	/* 0x18 */ DWORD	VertexStream2Offset;	  // Passed to CDeviceContext::IASetVertexBuffers
	/* 0x1C */ DWORD	StartingIndexLocation;    // Passed to CDeviceContext::DrawIndexed
	/* 0x20 */ DWORD	IndexCount;               // Passed to CDeviceContext::DrawIndexed
	/* 0x24 */ DWORD	BaseVertexLocation;       // Passed to CDeviceContext::DrawIndexed
	/* 0x28 */ // padding to align vectors
	/* 0x30 */ Vector4	BoundingBoxMin;
	/* 0x40 */ Vector4	BoundingBoxMax;
};
static_assert(sizeof(Primitive) == 0x50, "Primitive incorrect struct size");

enum ShaderId : DWORD
{
	tXfScreenClear = 0x17796f56,
	tXfScreenCopy = 0xcab2a170,
	tXfYUY2Copy = 0xfe9a5edf,
	tXfRGBICopy = 0x3e5389ee,
	tXfRGBICubeCopy = 0x26461f2a,
	tXfSubPixelCopy = 0x48e2d4bb,
	tXfReductionZCopy = 0x96f834af,
	tXfMaterialDebug = 0x3b810b10,
	tXfMaterialZPass = 0xaa626ffb,
	tXfMaterialVelocity = 0x97471581,
	tXfMaterialShadowReceiver = 0x53ae7416,
	tXfMaterialShadowCaster = 0x30b6bdb8,
	tXfMaterialStandard = 0x88367c19,
	tXfFilterStandard = 0x640e8a05,
	tXfFilterBloom = 0x2d5767f8,
	tXfFilterDOF = 0x185168f1,
	tXfFilterTVNoise = 0x2e1ad607,
	tXfFilterVolumeNoise = 0xaf9cb5e1,
	tXfFilterRadialBlur = 0x2bb2716a,
	tXfFilterFeedbackBlur = 0xf4db0ad3,
	tXfFilterToneMap = 0xe8b68a03,
	tXfFilterGaussianBlur = 0xf7c48941,
	tXfFilterMotionBlur = 0x5ce1a976,
	tXfFilterMerge = 0x9d1670d5,
	tXfFilterImagePlane = 0x414e40d3,
	tXfFilterColorCorrect = 0x2b6ec81b,
	tXfFilterFXAA = 0x35bf832f,
	tXfResolveDepth = 0xd5b6d6b3,
	//null = 0xe41f5dd1
	//null = 0xcc11f30d
	//null = 0x493a954c
	//null = 0x1fe78683
	tXfPrimStandard = 0xed2827cf,
	tXfEnvmapCubicBlur = 0x312cb4e1,
	tXfEnvmapBlend = 0xe5f39d43,
	//null = 0xfb4f06ed
	tXfMaterialSky = 0x399a88f9,
	tXfAdhesionPart = 0x5eecea3d,
	//null = 0x23e98e1c
	//null = 0x49c0b237
	//null = 0x5168f29
	//null = 0x5fa8066f
	//null = 0xfbc055e4
	//null = 0x6cd7aba0
	//null = 0xae243cee
	//null = 0x83614817
	//null = 0xc4dd56de
	//null = 0xef36423a
	//null = 0x264bf16
	//null = 0xfbb2f636
	//null = 0x5a2a6823
	tXfPrimGpuParticleBatch = 0x2752e134,
	//null = 0x81f59164
};

// sizeof = 0xD0
struct Material
{
	/* 0x00 */ DWORD	Flags;				// Upper 5 bits are some sort of shader group id (0x14064F550)
	/* 0x04 */ DWORD	Unk4;				// Flags for what bitmaps are used/how they are used (0x1406B2167)
	/* 0x08 */ ShaderId ShaderTechnique;
	/* 0x0C */ DWORD	Unk5;				// Never read, set on init
	/* 0x10 */ DWORD	Unk6;				// Never read, set to cTrans::VertexDecl pointer on init
	/* 0x14 */ DWORD	Unk7;
	/* 0x18 */ DWORD	Unk8;				// Checked to be non-zero, then set to a cTrans::VertexDecl pointer
	/* 0x20 */ void		*BaseMapTexture;	// texture index, subtract 1 (0 indicates null?)
	/* 0x28 */ void		*NormalMapTexture;	// texture index, subtract 1 (0 indicates null?)
	/* 0x30 */ void		*MaskMapTexture;	// texture index, subtract 1 (0 indicates null?)
	/* 0x38 */ void		*LightmapTexture;	// texture index, subtract 1 (0 indicates null?)
	/* 0x40 */ void		*TextureIndex5;	// texture index, subtract 1 (0 indicates null?)
	/* 0x48 */ void		*TextureIndex6;	// texture index, subtract 1 (0 indicates null?)
	/* 0x50 */ void		*TextureIndex7;	// texture index, subtract 1 (0 indicates null?)
	/* 0x58 */ void		*TextureIndex8;	// texture index, subtract 1 (0 indicates null?)
	/* 0x60 */ void		*TextureIndex9;	// texture index, subtract 1 (0 indicates null?)
	/* 0x68 */ float	Transparency;
	/* 0x6C */ DWORD	Unk11;
	/* 0x70 */ float	FresnelFactor;
	/* 0x74 */ float	FresnelBias;
	/* 0x78 */ float	SpecularPow;
	/* 0x7C */ float	EnvmapPower;    // not sure where I found this name...
	/* 0x80 */ Vector4	LightMapScale;
	/* 0x90 */ float	DetailFactor;
	/* 0x94 */ float	DetailWrap;
	/* 0x98 */ float	Unk22;
	/* 0x9C */ float	Unk23;
	/* 0xA0 */ Vector4	Transmit;
	/* 0xB0 */ Vector4	Parallax;
	/* 0xC0 */ float	Unk32;
	/* 0xC4 */ float	Unk33;
	/* 0xC8 */ float	Unk34;
	/* 0xCC */ float	Unk35;
};
static_assert(sizeof(Material) == 0xD0, "Material incorrect struct size");

// sizeof = 0x130
struct rModel : cResource
{
	/* 0x60 */ Joint		*pJointData1;
	/* 0x68 */ DWORD		JointCount;
	/* 0x70 */ Matrix4x4	*pJointData2;
	/* 0x78 */ Matrix4x4	*pJointData3;
	/* 0x80 */ Primitive	*pPrimitiveData;
	/* 0x88 */ DWORD		PrimitiveCount;
	/* 0x90 */ Material		*pMaterials;
	/* 0x98 */ DWORD		MaterialCount;
	/* 0x9C */ DWORD		PolygonCount;
	/* 0xA0 */ DWORD		VertexCount;
	/* 0xA4 */ DWORD		IndiceCount;
	/* 0xA8 */ DWORD		Count1;
	/* 0xAC */ DWORD		Count2;
	/* 0xB0 */ void			*pIndiceManager;	// 0x000000014064F8B4
	/* 0xB8 */ void			*pVertexManager1;	// 0x000000014064F7BB
	/* 0xC0 */ void			*pVertexManager2;	// same as above
	/* 0xD0 */ // bb sphere?
	BYTE pad[0x18];
	/* 0xE0 */ Vector4		BoundingBoxMin;
	/* 0xF0 */ Vector4		BoundingBoxMax;
	/* 0x100 */ DWORD		MidDist;
	/* 0x104 */ DWORD		LowDist;
	/* 0x108 */ DWORD		LightGroup;
	/* 0x110 */ Vector4		Scale; // BBMax - BBMin
	/* 0x120 */ Vector4		Unk; // gets set to BBMin at load
};
static_assert(sizeof(rModel) == 0x130, "rModel incorrect struct size");