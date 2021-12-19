
/*
mtrlgreen_tree_base.ps
Базовый рендер деревьев/кустов
*/

#include <struct.h>
#include <green.h>

//##########################################################################

half4x4 g_mWVP;

//##########################################################################

VSO_SceneCommon main(in VSI_Green IN) 
{
	VSO_SceneCommon OUT;
	
	// расчет поворота нормали
	OUT.vNormal = GreenComRotation(normalize(IN.vNormal), IN.vInstSinCosRot);
	
	// расчет поворота позиции
	OUT.vPosition.xyz = GreenComRotation(IN.vPosition, IN.vInstSinCosRot);
	
	// расчет мировой позиции
	OUT.vPosition = GreenTransformPos(
						OUT.vPosition.xyz,
						IN.vInstTrans.x,
						1,
						IN.vInstPos
					);
	
	OUT.vPosition = mul(OUT.vPosition, g_mWVP);
	
	OUT.vPos = OUT.vPosition;
	
	OUT.vTexUV = IN.vTexUV;
	
	return OUT;
}