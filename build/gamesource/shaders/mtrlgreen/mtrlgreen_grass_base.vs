
/*
mtrlgreen_grass_base.ps
Базовый рендер травы
*/

#include <../struct.h>
#include <../green.h>

//##########################################################################

half4x4 g_mWVP;

half2 g_vDistLessening	: register(GREEN_R_LESSENING);
half3 g_vViewPos		: register(GREEN_R_VIEWPOS);
half4 g_vBoundSphere	: register(GREEN_R_BSPHERE);
half3 g_vBoundMax		: register(GREEN_R_BBMAX);
half3 g_vBoundMin		: register(GREEN_R_BBMIN);

//##########################################################################

void main(in VSI_Green IN, out VSO_SceneCommon OUT) 
{
	OUT.vNormal = GreenComRotation(normalize(IN.vNormal), IN.vInstSinCosRot);
	
	OUT.vPosition = GreenTransformPos(
						GreenComRotation(IN.vPosition, IN.vInstSinCosRot),
						IN.vInstTrans.x,
						GrassComMultiplier(IN.vInstPos, g_vViewPos, g_vDistLessening),
						IN.vInstPos
					);
		
	OUT.vPosition = mul(OUT.vPosition, g_mWVP);
	
	OUT.vPos = OUT.vPosition;
	OUT.vPos.w = length(g_vBoundSphere.xyz - IN.vPosition.xyz) / g_vBoundSphere.w;
	OUT.vPos.w *= OUT.vPos.w;
	OUT.vTexUV = IN.vTexUV;
}