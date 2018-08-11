
/*
mtrlskin_base.vs
Базовый рендер анимационных моделей
*/

#include <../struct.h>
#include <../skin.h>

//##########################################################################

half4x4	g_mWVP;
half4x4	g_mW;

//##########################################################################

VSO_SceneCommon main(VSI_Animation IN)
{
	VSO_SceneCommon OUT = SkinAllTransform(IN);
	
	OUT.vPosition = mul(OUT.vPosition, g_mWVP);
	OUT.vNormal = mul(OUT.vNormal, g_mW);
	OUT.vTexUV = IN.vTexUV;
	
	OUT.vPos = OUT.vPosition;
    
	return(OUT);
}
