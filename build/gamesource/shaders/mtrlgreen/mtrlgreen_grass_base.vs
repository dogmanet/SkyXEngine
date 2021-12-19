
/*
mtrlgreen_grass_base.ps
Базовый рендер травы
*/

#include <struct.h>
#include <green.h>

//##########################################################################

half4x4 g_mWVP;

half2 g_vDistLessening	: register(GREEN_R_LESSENING);
half3 g_vViewPos		: register(GREEN_R_VIEWPOS);
half4 g_vBoundSphere	: register(GREEN_R_BSPHERE);
half3 g_vBoundMax		: register(GREEN_R_BBMAX);
half3 g_vBoundMin		: register(GREEN_R_BBMIN);

//##########################################################################

VSO_SceneCommon main(in VSI_Green IN) 
{
	VSO_SceneCommon OUT;
	
	// расчет поворота нормали
	OUT.vNormal = GreenComRotation(normalize(IN.vNormal), IN.vInstSinCosRot);
	
	// расчет поворота позиции
	OUT.vPosition.xyz = GreenComRotation(IN.vPosition, IN.vInstSinCosRot);
	
	// расчет коэффициента уменьшения растительности по объему
	half fMultiplier = GrassComMultiplier(IN.vInstPos, g_vViewPos, g_vDistLessening);
	
	// расчет мировой позиции
	OUT.vPosition = GreenTransformPos(
						OUT.vPosition.xyz,
						IN.vInstTrans.x,
						fMultiplier,
						IN.vInstPos
					);
	
	// расчет затенености вершины, на основании отдаленности от центра ограничивающей сферы
	half fShading = GreenComShadingBySphere(g_vBoundSphere, IN.vPosition.xyz);
		
	OUT.vPosition = mul(OUT.vPosition, g_mWVP);
	
	OUT.vPos = OUT.vPosition;
	
	OUT.vPos.w = fShading;
	
	OUT.vTexUV = IN.vTexUV;
	
	return OUT;
}