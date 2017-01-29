
#ifndef __sxmlight
#define __sxmlight

#include <gdefines.h>

#include <d3d9.h>
#include <d3dx9.h>
#include <sxmath.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "DxErr9.lib")
#pragma comment(lib, "d3dx9.lib")

#define LIGHTS_TYPE_NONE -1
#define LIGHTS_TYPE_GLOBAL 0
#define LIGHTS_TYPE_POINT 1
#define LIGHTS_TYPE_DIRECTION 2
#define LIGHTS_MAX_LEN_NAME 64
#define LIGHTS_LOCAL_STD_NEAR 0.1

SX_LIB_API long SML_0GetVersion();
SX_LIB_API void SML_Dbg_Set(report_func rf);
SX_LIB_API void SML_0Create(const char* name, IDirect3DDevice9* device, float2_t* winsize, float projfov, DWORD id_depth, DWORD id_depth_d2, DWORD id_depth_d4, bool is_unic = true);

SX_LIB_API void SML_OnLostDevice();
SX_LIB_API void SML_OnResetDevice(int width, int heigth, float projfov);

SX_LIB_API void SML_LigthsSettLCoefSizeDepth(float coef);
SX_LIB_API void SML_LigthsSettLCoefSizeShadow(float coef);
SX_LIB_API void SML_LigthsSettGCoefSizeDepth(float coef);
SX_LIB_API void SML_LigthsSettGCoefSizeShadow(float coef);

SX_LIB_API void SML_LigthsUpdate(float3* const_cam_pos,float4x4* camview);

SX_LIB_API long SML_LigthsGetCount();

SX_LIB_API long SML_LigthsCreatePoint(float4* center, float3* color, bool isglobal, bool is_shadowed/*, const char* bound_volume*/);
SX_LIB_API long SML_LigthsCreateDirection(float4* pos, float3* color, float3* dir, float top_radius, float angle, bool is_shadow/*, const char* bound_volume*/);

SX_LIB_API void SML_LigthsRender(long id, DWORD timeDelta);

SX_LIB_API void SML_LigthsGetColor(long id, float3* color);
SX_LIB_API void SML_LigthsSetColor(long id, float3* vec);
SX_LIB_API void SML_LigthsGetPosW(long id, float4* pos);
SX_LIB_API void SML_LigthsGetPos(long id, float3* vec);
SX_LIB_API void SML_LigthsSetPos(long id, float3* vec);

SX_LIB_API void SML_LigthsGetRot(long id, float3* vec);
SX_LIB_API void SML_LigthsSetRot(long id, float3* vec);
SX_LIB_API void SML_LigthsGetDir(long id, float3* vec);
SX_LIB_API void SML_LigthsSetDir(long id, float3* vec);

SX_LIB_API int SML_LigthsGetType(long id);
SX_LIB_API int SML_LigthsComVisibleForFrustum(long id, ISXFrustum* frustum);
SX_LIB_API bool SML_LigthsGetVisibleForFrustum(long id, ISXFrustum* frustum);
SX_LIB_API float SML_LigthsComDistFor(long id, float3* vec);
SX_LIB_API float SML_LigthsGetDistFor(long id);
SX_LIB_API void SML_LigthsComVisibleFrustumDistFor(ISXFrustum* frustum, float3* vec);

SX_LIB_API int SML_LigthsIsEnable(long id);
SX_LIB_API void SML_LigthsSetEnable(long id, bool val);
SX_LIB_API int SML_LigthsIsShadow(long id);

SX_LIB_API IDirect3DTexture9* SML_LigthsGetShadow(long id);

SX_LIB_API void SML_LigthsInRenderBegin(long id);
SX_LIB_API void SML_LigthsInRenderEnd(long id);
SX_LIB_API void SML_LigthsInRenderPre(long id,int cube);
SX_LIB_API void SML_LigthsInRenderPost(long id,int cube);

SX_LIB_API ISXFrustum* SML_LigthsGetFrustum(long id);
SX_LIB_API ISXFrustum* SML_LigthsGetFrustumG(long id,int split);
SX_LIB_API void SML_LigthsUpdateG(long id, int split,float3* pos, float3* dir);

SX_LIB_API void SML_LigthsGenShadow(long id);
SX_LIB_API void SML_LigthsSoftShadow(long id, bool randomsam, float size);


SX_LIB_API long SML_LigthsCreateCopy(long id);
SX_LIB_API void SML_LigthsDeleteLight(long id);
SX_LIB_API char* SML_LigthsGetName(long id);
SX_LIB_API void SML_LigthsSetName(long id, const char* name);


SX_LIB_API void SML_LigthsChangeAngle(long id, float angle, bool is_create);
SX_LIB_API void SML_LigthsChangeTopRadius(long id, float top_radius);
SX_LIB_API void SML_LigthsChangeRadiusHeight(long id, float radius_height, bool is_create);
SX_LIB_API void SML_LigthsChangeShadow(long id, bool is_shadow);

SX_LIB_API void SML_LigthsSetBlurPixel(long id, float blur_pixel);
SX_LIB_API float SML_LigthsGetBlurPixel(long id);
SX_LIB_API void SML_LigthsSetShadowLocalNear(long id, float slnear);
SX_LIB_API float SML_LigthsGetShadowLocalNear(long id);
SX_LIB_API void SML_LigthsSetShadowLocalFar(long id, float slfar);
SX_LIB_API float SML_LigthsGetShadowLocalFar(long id);
SX_LIB_API void SML_LigthsSetEnableCubeEdge(long id, int edge, bool enable);
SX_LIB_API bool SML_LigthsGetEnableCubeEdge(long id, int edge);

#endif