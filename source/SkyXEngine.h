
/*
//#define SKYXENGINE_IN_SOURCE	
//если SKYXENGINE_IN_SOURCE объявлен то грузим исходники всех либ, иначе будут загружены либы
#ifdef SKYXENGINE_IN_SOURCE
#define SXCORE_IN_SOURCE
#define SXINPUT_IN_SOURCE
#define SXGCORE_IN_SOURCE
#define SXGEOM_IN_SOURCE
#define SXMTLLIGHT_IN_SOURCE
#define SXPP_IN_SOURCE
#endif*/


#if defined(_DEBUG)
#pragma comment(lib, "sxcore_d.lib")
#else
#pragma comment(lib, "sxcore.lib")
#endif
#include <core\\sxcore.h>

#if defined(_DEBUG)
#pragma comment(lib, "sxinput_d.lib")
#else
#pragma comment(lib, "sxinput.lib")
#endif
#include <input\\sxinput.h>

#if defined(_DEBUG)
#pragma comment(lib, "sxgcore_d.lib")
#else
#pragma comment(lib, "sxgcore.lib")
#endif
#include <gcore\\sxgcore.h>

#if defined(_DEBUG)
#pragma comment(lib, "sxgeom_d.lib")
#else
#pragma comment(lib, "sxgeom.lib")
#endif
#include <geom\\sxgeom.h>


#if defined(_DEBUG)
#pragma comment(lib, "sxmtllight_d.lib")
#else
#pragma comment(lib, "sxmtllight.lib")
#endif
#include <mtllight\\sxmtllight.h>

#if defined(_DEBUG)
#pragma comment(lib, "sxpp_d.lib")
#else
#pragma comment(lib, "sxpp.lib")
#endif
#include <pp\\sxpp.h>
