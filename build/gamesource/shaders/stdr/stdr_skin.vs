
/*
stdr_skin.vs
Обычный рендер анимационной модели
*/

#include "../struct.h"
#include "../skin.h"

//##########################################################################

half4x4	g_mWVP;
half4x4	g_mW;

//##########################################################################

VSO_SceneCommon main(VSI_Animation IN)
{
	VSO_SceneCommon OUT = SkinAllTransform(IN);
	
	OUT.vPos = mul(OUT.vPosition, g_mW);
	OUT.vPosition = mul(OUT.vPosition, g_mWVP);
	OUT.vNormal = mul(OUT.vNormal, g_mW);
	OUT.vTexUV = IN.vTexUV;
	
	return(OUT);
}
