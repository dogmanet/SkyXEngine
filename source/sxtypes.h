#ifndef sxtypes_h
#define sxtypes_h

#include <stdint.h>
#include <string.h>
#include <memory.h>


typedef float float32_t;
static_assert(sizeof(float32_t) == 4, "Please check float32_t on your platform. The size should be exactly 4 bytes");

typedef unsigned long DWORD;
typedef unsigned short WORD;
typedef unsigned int size_t;
typedef int BOOL;
typedef unsigned char BYTE, byte;
typedef unsigned int UINT;
typedef wchar_t WCHAR;
typedef unsigned long ULONG;

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif

#if defined(_WINDOWS)
#define dbg_break _asm { int 3 }
#else
#define dbg_break asm("int $3");
#endif

#define mem_del(obj) delete obj;
#define mem_delete(obj) if(obj){delete obj;obj=0;}
#define mem_delete_a(obj) if(obj){delete[] obj;obj=0;}
#define mem_release(obj) if(obj)obj->Release();
#define mem_release_del(obj) obj->Release(); obj = 0;
#define mem_release_delete(obj) obj->Release();mem_delete(obj)
#define mem_free(a) free(a)
#define mem_alloc(a) malloc(a)

inline const char * basename(const char * str)
{
	const char * pos = str;
	while(*str)
	{
		if(*str == '/' || *str == '\\')
		{
			pos = str+1;
		}
		++str;
	}
	return(pos);
}

inline const char * dirname(char * str)
{
	char * pos = str, *ret = str;
	while(*str)
	{
		if(*str == '/' || *str == '\\')
		{
			pos = str + 1;
		}
		++str;
	}
	*pos = 0;
	return(ret);
}

inline const char * canonize_path(char * str)
{
	char * ret = str;
	while(*str)
	{
		if(*str == '\\')
		{
			*str = '/';
		}
		++str;
	}
	return(ret);
}

inline const char * strip_prefix(const char * str, const char * pref)
{
	int len = strlen(pref);
	if(!memcmp(str, pref, len))
	{
		return(str + len);
	}
	return(str);
}

#endif
