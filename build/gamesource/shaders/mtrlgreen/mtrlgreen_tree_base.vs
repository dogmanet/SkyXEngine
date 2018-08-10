
/*
mtrlgreen_tree_base.ps
Базовый рендер деревьев/кустов
*/

#include <../struct.h>
#include <../green.h>

//##########################################################################

half4x4 g_mWVP;

//##########################################################################

void main(in VSI_Green IN, out VSO_SceneCommon OUT) 
{
	OUT.vNormal = (GreenComRotation(normalize(IN.vNormal),IN.vInstSinCosRot));
	
	OUT.vPosition = GreenTransformPos(
						GreenComRotation(IN.vPosition,IN.vInstSinCosRot),
						IN.vInstTrans.x,
						1,
						IN.vInstPos
					);
	
	OUT.vPosition = mul(OUT.vPosition, g_mWVP);
	
	OUT.vPos = OUT.vPosition;
	
	OUT.vTexUV = IN.vTexUV;
}