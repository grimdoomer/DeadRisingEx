/*

*/

#pragma once
#include "DeadRisingEx.h"
#include <MtFramework/Graphics/rModel.h>

TYPE_INFO_BEGIN(Joint,)
	FIELD_NUMBER(Joint, Index),
	FIELD_NUMBER(Joint, ParentIndex),
	FIELD_FLOAT(Joint, Length),
	FIELD_VECTOR3(Joint, Offset),
	FIELD_TERM()
TYPE_INFO_END()

TYPE_INFO_BEGIN(Primitive,)
	FIELD_NUMBER(Primitive, GroupID),
	FIELD_NUMBER(Primitive, MaterialIndex),
	FIELD_NUMBER(Primitive, Enabled),
	FIELD_NUMBER(Primitive, Unk3),
	FIELD_NUMBER(Primitive, Unk11),
	FIELD_NUMBER(Primitive, Unk12),
	FIELD_NUMBER(Primitive, VertexStride1),
	FIELD_NUMBER(Primitive, VertexStride2),
	FIELD_NUMBER(Primitive, Unk13),
	FIELD_NUMBER(Primitive, VertexCount),
	FIELD_NUMBER(Primitive, StartingVertex),
	FIELD_NUMBER(Primitive, VertexStream1Offset),
	FIELD_NUMBER(Primitive, VertexStream2Offset),
	FIELD_NUMBER(Primitive, StartingIndexLocation),
	FIELD_NUMBER(Primitive, IndexCount),
	FIELD_NUMBER(Primitive, BaseVertexLocation),
	FIELD_VECTOR4(Primitive, BoundingBoxMin),
	FIELD_VECTOR4(Primitive, BoundingBoxMax),
	FIELD_TERM()
TYPE_INFO_END()

const EnumTypeInfo ShaderIdTypeInfo =
{
	"ShaderId", sizeof(ShaderId),
	{
		{ "tXfScreenClear", 0x17796f56 },
		// TODO
		{ "tXfMaterialStandard", 0x88367c19 },
		{ nullptr, 0 }
	}
};

TYPE_INFO_BEGIN(Material,)
	FIELD_NUMBER(Material, Flags),
	FIELD_ENUM(Material, ShaderTechnique, ShaderIdTypeInfo),
	FIELD_NUMBER(Material, Unk5),
	FIELD_NUMBER(Material, Unk6),
	FIELD_NUMBER(Material, Unk7),
	FIELD_NUMBER(Material, Unk8),
	FIELD_NUMBER(Material, BaseMapTexture),
	FIELD_NUMBER(Material, NormalMapTexture),
	FIELD_NUMBER(Material, MaskMapTexture),
	FIELD_NUMBER(Material, LightmapTexture),
	FIELD_NUMBER(Material, TextureIndex5),
	FIELD_NUMBER(Material, TextureIndex6),
	FIELD_NUMBER(Material, TextureIndex7),
	FIELD_NUMBER(Material, TextureIndex8),
	FIELD_NUMBER(Material, TextureIndex9),
	FIELD_FLOAT(Material, Transparency),
	FIELD_NUMBER(Material, Unk11),
	FIELD_FLOAT(Material, FresnelFactor),
	FIELD_FLOAT(Material, FresnelBias),
	FIELD_FLOAT(Material, SpecularPow),
	FIELD_FLOAT(Material, EnvmapPower),
	FIELD_VECTOR4(Material, LightMapScale),
	FIELD_FLOAT(Material, DetailFactor),
	FIELD_FLOAT(Material, DetailWrap),
	FIELD_FLOAT(Material, Unk22),
	FIELD_FLOAT(Material, Unk23),
	FIELD_VECTOR4(Material, Transmit),
	FIELD_VECTOR4(Material, Parallax),
	FIELD_FLOAT(Material, Unk32),
	FIELD_FLOAT(Material, Unk33),
	FIELD_FLOAT(Material, Unk34),
	FIELD_FLOAT(Material, Unk35),
	FIELD_TERM()
TYPE_INFO_END()

ARRAY_DEFINITION_PTR(rModel, pJointData1, Joint, 0, JointCount)
ARRAY_DEFINITION_PTR(rModel, pJointData2, Matrix4x4, 0, JointCount)
ARRAY_DEFINITION_PTR(rModel, pJointData3, Matrix4x4, 0, JointCount)
ARRAY_DEFINITION_PTR(rModel, pPrimitiveData, Primitive, 0, PrimitiveCount)
ARRAY_DEFINITION_PTR(rModel, pMaterials, Material, 0, MaterialCount)

TYPE_INFO_BEGIN(rModel, cResource)
	FIELD_ARRAY(rModel, pJointData1),
	FIELD_NUMBER(rModel, JointCount),
	FIELD_ARRAY(rModel, pJointData2),
	FIELD_ARRAY(rModel, pJointData3),
	FIELD_ARRAY(rModel, pPrimitiveData),
	FIELD_NUMBER(rModel, PrimitiveCount),
	FIELD_ARRAY(rModel, pMaterials),
	FIELD_NUMBER(rModel, MaterialCount),
	FIELD_NUMBER(rModel, PolygonCount),
	FIELD_NUMBER(rModel, VertexCount),
	FIELD_NUMBER(rModel, IndiceCount),
	FIELD_NUMBER(rModel, Count1),
	FIELD_NUMBER(rModel, Count2),
	FIELD_NUMBER(rModel, pIndiceManager),
	FIELD_NUMBER(rModel, pVertexManager1),
	FIELD_NUMBER(rModel, pVertexManager2),
	FIELD_VECTOR4(rModel, BoundingBoxMin),
	FIELD_VECTOR4(rModel, BoundingBoxMax),
	FIELD_NUMBER(rModel, MidDist),
	FIELD_NUMBER(rModel, LowDist),
	FIELD_NUMBER(rModel, LightGroup),
	FIELD_VECTOR4(rModel, Scale),
	FIELD_VECTOR4(rModel, Unk),
	FIELD_TERM()
TYPE_INFO_END()

class rModelImpl
{
public:
    static void InitializeTypeInfo();
};