/*

*/

#pragma once
#include <Windows.h>
#include "Misc/AsmHelpers.h"

struct cResource;

// sizeof = 0x38
struct MtDTI
{
	/*
		VTable:
			0x00 ~dtor()
			0x08 cResource * CreateInstance()
	*/

	/* 0x00 */ void		**vtable;
	/* 0x08 */ char		*pObjectName;
	/* 0x10 */ MtDTI	*pFLink;
	/* 0x18 */ MtDTI	*pBLink;
	/* 0x20 */ MtDTI	*pParentObject;

	union
	{
		/* 0x28 */ DWORD	ObjectSize : 24;		// Upper 8 bits are flags, lower 24 bits are object size
		/* 0x2B */ DWORD	Flags : 8;
	};

	/* 0x2C */ DWORD	FileTypeId;
	/* 0x30 */ void		*pUnknown3;

	/*
		Creates a new instance of this object type.
	*/
	cResource * CreateInstance()
	{
		return ThisPtrCallNoFixup<cResource*>(this->vtable[1], this);
	}

	/*
		Creates a new instance of this object type and casts it to type T.
	*/
	template<typename T> T* CreateInstance()
	{
		return (T*)CreateInstance();
	}
};
static_assert(sizeof(MtDTI) == 0x38, "MtDTI struct has incorrect size");

inline MtDTI *g_cResourceDTI = GetModuleAddress<MtDTI*>((void*)0x141CF2698);

inline MtDTI * (__stdcall *GetDTIForFileType)(DWORD dwFileType, MtDTI *pRoot) = GetModuleAddress<MtDTI*(__stdcall*)(DWORD, MtDTI*)>((void*)0x140618590);

// sizeof = 0x8
struct MtObject
{
	/*
		VTable:
			0x00 void ~dtor();
			0x08
			0x10 bool
			0x18 void RegisterDebugOptions(MtPropertyList *pPropList)
			0x20 MtDTI * GetDTI();
	*/

	/* 0x00 */ void **vtable;

	/*
		Gets the MtDTI object information struct for this object.
	*/
	MtDTI * GetDTI()
	{
		return ThisPtrCallNoFixup<MtDTI*>(this->vtable[4], this);
	}
};
static_assert(sizeof(MtObject) == 8, "MtObject incorrect struct size");

class MtDTIImpl
{
public:
	// MtDTI(this, char *psObjectName, MtDTI *pParentObject, DWORD dwObjectSize);
};