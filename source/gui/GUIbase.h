#ifndef _GUIbase_H_
#define _GUIbase_H_

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d9.lib")
#if defined(_DEBUG) && 0
#	pragma comment(lib, "d3dx9d.lib")
#else
#	pragma comment(lib, "d3dx9.lib")
#endif

#ifndef SX_SAFE_DELETE_A
#define SX_SAFE_DELETE_A(p) if(p)\
	{\
		/*DS_DBG_DELETE(p);*/\
		delete [] (p);\
		(p) = NULL;\
	}
#endif

#ifndef SX_SAFE_DELETE
#define SX_SAFE_DELETE(p) if(p)\
	{\
		/*DS_DBG_DELETE(p);*/\
		delete (p);\
		(p) = NULL;\
	}
#endif

#ifndef SX_SAFE_RELEASE
#define SX_SAFE_RELEASE(p) if(p)\
	{\
		(p)->Release();\
		(p) = NULL;\
	}
#endif

#if defined(_WINDOWS)
#pragma warning(disable:4244)
#endif

//#include <gdefines.h>
#include <new>

__forceinline int _round(double value)
{
   return((int)floor(value + 0.5));
}

__forceinline int _log2(int n)
{
	int log = 0;
	while(n > 1)
	{
		n /= 2;
		log++;
	}
	return(log);
}

#include <SXMath.h>
#include <gdefines.h>
#include <core/array.h>
#include <core/memalloc.h>
#include <core/stack.h>
#include <core/assotiativearray.h>
#include <string/string.h>



#include "SXGUI.h"



#include <cstdio>

#pragma comment(lib, "Vfw32.lib")

#define TEX_WHITE L"/dev/white.png"

#define DECLARE_CLASS(cls, basecls)		\
	typedef cls ThisClass;				\
	typedef basecls BaseClass;


namespace GUI
{
	class IDesktop;
};

#include "ITexture.h"

#endif
