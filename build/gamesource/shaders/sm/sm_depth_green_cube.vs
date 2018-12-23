
/*
sm_depth_green_cube.vs
Рендер глубины сцены (растительность) с позиции точечного источника света
*/

#include <../struct.h>
#include <../green.h>
#include <../shadow.h>

//##########################################################################

half4x4 g_mWVP;
half3 g_vLightPos;

#if defined(_GRASS_)

half2 g_vDistLessening	: register(GREEN_R_LESSENING);
half3 g_vViewPos		: register(GREEN_R_VIEWPOS);
half4 g_vBoundSphere	: register(GREEN_R_BSPHERE);
half3 g_vBoundMax		: register(GREEN_R_BBMAX);
half3 g_vBoundMin		: register(GREEN_R_BBMIN);

#endif

//##########################################################################

VSO_SceneCommon main( in VSI_Green IN) 
{
	VSO_SceneCommon OUT;
	
	half fMultiplier = 1.0;
	
	#if defined(_GRASS_)
	fMultiplier = GrassComMultiplier(IN.vInstPos,g_vViewPos,g_vDistLessening);
	#endif
	
	OUT.vPosition = GreenTransformPos(
						GreenComRotation(IN.vPosition,IN.vInstSinCosRot),
						IN.vInstTrans.x,
						fMultiplier,
						IN.vInstPos
					);
					
	OUT.vPos	= half4(g_vLightPos - OUT.vPosition,1);
	OUT.vPosition = mul(OUT.vPosition, g_mWVP);
	OUT.vNormal = IN.vNormal;
	OUT.vTexUV = IN.vTexUV;
	
	return OUT;
}