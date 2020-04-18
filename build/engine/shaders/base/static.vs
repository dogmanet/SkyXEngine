
/*
mtrlgeom_base.vs
Базовый рендер геометрии
*/

#include <struct.h>
#include <const.h>

//##########################################################################

VSO_SceneCommon main(VSI_Geometry IN)
{
	VSO_SceneCommon OUT = (VSO_SceneCommon)0;

	OUT.vPosition = mul(float4(IN.vPosition, 1.0f), g_mW);
	OUT.vPosition = mul(OUT.vPosition, g_mVP);
	OUT.vNormal = mul(float4(IN.vNormal, 1.0f), g_mW).xyz;
	OUT.vTexUV = IN.vTexUV;
	OUT.vPos = OUT.vPosition;

	return OUT;
}
