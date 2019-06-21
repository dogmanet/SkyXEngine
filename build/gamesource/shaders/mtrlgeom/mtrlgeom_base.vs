
/*
mtrlgeom_base.vs
Базовый рендер геометрии
*/

#include <../struct.h>
#include <../const.h>

//##########################################################################

VSO_SceneCommon main(VSI_Geometry IN)
{
	VSO_SceneCommon OUT = (VSO_SceneCommon)0;

	// OUT.vPosition = mul(half4(IN.vPosition, 1.f), g_mWVP);
	OUT.vPosition = mul(half4(IN.vPosition, 1.f), g_mW);
	OUT.vPosition = mul(OUT.vPosition, g_mVP);
	OUT.vNormal = mul(IN.vNormal,g_mW);
	OUT.vTexUV = IN.vTexUV;
	OUT.vPos = OUT.vPosition;

	return OUT;
}