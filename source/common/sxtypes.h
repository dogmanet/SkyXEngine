
#ifndef sxtypes_h
#define sxtypes_h

#include <stdint.h>

typedef float float32_t;
static_assert(sizeof(float32_t) == 4, "Please check float32_t on your platform. The size should be exactly 4 bytes");

typedef int32_t ID;
typedef unsigned long DWORD;
typedef unsigned short WORD;
typedef unsigned int size_t;
typedef int BOOL;
typedef unsigned char BYTE;
typedef unsigned int UINT;
typedef wchar_t WCHAR;

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

#define mem_del(obj) if(obj){delete obj;}
#define mem_delete(obj) if(obj){delete obj;obj=0;}
#define mem_delete_a(obj) if(obj){delete[] obj;obj=0;}
#define mem_release(obj) if(obj){obj->Release(); obj = 0;}
#define mem_release_del(obj) if(obj){obj->Release(); obj = 0;}
#define mem_release_delete(obj)if(obj){ obj->Release();mem_delete(obj)}
#define mem_free(a) free(a)
#define mem_alloc(a) malloc(a)

#endif
