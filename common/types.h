#pragma once


typedef unsigned char byte;
typedef int		sound_t;

typedef byte rgba_t[4];
typedef byte rgb_t[3];


#undef true
#undef false

#ifndef __cplusplus
typedef enum { false, true }	qboolean;
#else
typedef int qboolean;
#endif

#if _MSC_VER == 1200
typedef __int64 integer64; //msvc6
#elif defined (XASH_SDL) && !defined(REF_DLL)
typedef Uint64 integer64;
#else
typedef unsigned long long integer64;
#endif
typedef integer64 longtime_t;

typedef unsigned int	dword;
typedef unsigned int	uint;