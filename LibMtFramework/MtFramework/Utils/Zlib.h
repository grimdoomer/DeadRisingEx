/*

*/

#pragma once
#include <Windows.h>
#include "Misc/AsmHelpers.h"

struct z_stream_s;

#define ZLIB_VERSION	"1.2.6"

static unsigned long(__stdcall *inflateInit)(z_stream_s *zstream, const char *psVersion, unsigned long streamsize) = 
    GetModuleAddress<unsigned long(__stdcall*)(z_stream_s*, const char*, unsigned long)>(0x140742C40);

static unsigned long(__stdcall *inflateEnd)(z_stream_s *zstream) = 
    GetModuleAddress<unsigned long(__stdcall*)(z_stream_s*)>(0x140742AF0);

// sizeof = 0x58
struct z_stream_s
{
	/* 0x00 */ BYTE		*next_in;
	/* 0x08 */ DWORD	avail_in;
	/* 0x0C */ DWORD	total_in;
	/* 0x10 */ BYTE		*next_out;
	/* 0x18 */ DWORD	avail_out;
	/* 0x1C */ DWORD	total_out;
	/* 0x20 */ const char *msg;
	/* 0x28 */ void		*state;
	/* 0x30 */ void		*zalloc;
	/* 0x38 */ void		*zfree;
	/* 0x40 */ void		*opaque;
	/* 0x48 */ DWORD	data_type;
	/* 0x4C */ DWORD	adler;
	/* 0x50 */ DWORD	reserved;
	/* 0x54 */ // padding
};
static_assert(sizeof(z_stream_s) == 0x58, "z_stream_s incorrect struct size");