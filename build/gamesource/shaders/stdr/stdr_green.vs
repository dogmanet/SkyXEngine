
/*
stdr_green.vs
Обычный рендер растительности
*/

#include <../struct.h>
#include <../green.h>

//##########################################################################

half4x4 g_mWVP;
half4x4 g_mW;

#if defined(_GRASS_)

half2 g_vDistLessening	: register(GREEN_R_LESSENING);
half3 g_vViewPos		: register(GREEN_R_VIEWPOS);
half4 g_vBoundSphere	: register(GREEN_R_BSPHERE);
half3 g_vBoundMax		: register(GREEN_R_BBMAX);
half3 g_vBoundMin		: register(GREEN_R_BBMIN);

#endif

//##########################################################################

VSO_SceneCommon main(in VSI_Green IN) 
{
	VSO_SceneCommon OUT;

	half multiplier = 1;
	
	#if defined(_GRASS_)
	multiplier = GrassComMultiplier(IN.vInstPos, g_vViewPos, g_vDistLessening);
	#endif
	
	OUT.vPosition = GreenTransformPos(
						GreenComRotation(IN.vPosition, IN.vInstSinCosRot),
						IN.vInstTrans.x,
						multiplier,
						IN.vInstPos
					);
					
	OUT.vPos = mul(OUT.vPosition, g_mW);
	OUT.vPosition = mul(OUT.vPosition, g_mWVP);

	OUT.vTexUV = IN.vTexUV;
	
	return OUT;
}