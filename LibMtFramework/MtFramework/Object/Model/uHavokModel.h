
#pragma once
#include "uModel.h"

// sizeof = 0xD60
struct uHavokModel : public uModel
{
	/* 0xD50 */ //void*

	/*
		VTable:
			0x90
			0x98
			0xA0
			0xA8
			0xB0
			0xB8
			0xC0
			0xC8
			0xD0
			0xD8
	*/

	inline static uHavokModel* (__stdcall* _ctor)(uHavokModel* thisptr) =
		(uHavokModel * (__stdcall*)(uHavokModel*))GetModuleAddress(0x14010E4A0);

	inline static uHavokModel* (__stdcall* _dtor)(uHavokModel* thisptr, bool bFreeMemory) =
		(uHavokModel * (__stdcall*)(uHavokModel*, bool))GetModuleAddress(0x14010E520);

    IMPLEMENT_MYDTI(uHavokModel, 0x141948C60, 0x1400AF010, 0x14010E5D0);

	uHavokModel()
	{
		_ctor(this);
	}

	~uHavokModel()
	{
		(void)ThisPtrCallNoFixup(this->vtable[0], this, false);
	}
};