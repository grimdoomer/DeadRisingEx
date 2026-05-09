
#pragma once
#include "MtFramework/Object/Model/uModel.h"

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

	inline static uHavokModel* (* _ctor)(uHavokModel* thisptr) =
		(uHavokModel * (*)(uHavokModel*))GetModuleAddress(0x14010E4A0);

	inline static void* (* _scalar_deleting_dtor)(uHavokModel* thisptr, unsigned int flags) =
		(void * (*)(uHavokModel*, unsigned int))GetModuleAddress(0x14010E520);

    IMPLEMENT_MYDTI(uHavokModel, 0x141948C60, 0x1400AF010, 0x14010E5D0);

	SHIM_API uHavokModel() SHIM_BODY(0x14010E4A0)

	SHIM_API ~uHavokModel() SHIM_BODY_DTOR_VCALL()

	IMPLEMENT_OPERATOR_NEW_DELETE(g_pUnitHeapAllocator, 32)
};