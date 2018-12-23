#ifndef _GUIbase_H_
#define _GUIbase_H_

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <gcore/sxgcore.h>

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d9.lib")
#if defined(_DEBUG) && 0
#	pragma comment(lib, "d3dx9d.lib")
#else
#	pragma comment(lib, "d3dx9.lib")
#endif

#if defined(_DEBUG)
#	pragma comment(lib, "sxgcore_d.lib")
#else
#	pragma comment(lib, "sxgcore.lib")
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

#include <common/SXMath.h>
#include <gdefines.h>
#include <common/array.h>
#include <common/MemAlloc.h>
#include <common/stack.h>
#include <common/assotiativearray.h>
#include <common/string.h>

#include "SXGUI.h"

#include <cstdio>

#pragma comment(lib, "Vfw32.lib")

#define TEX_WHITE L"/dev/white.png"

namespace gui
{
	class IDesktop;
};

#include "Texture.h"

#endif
