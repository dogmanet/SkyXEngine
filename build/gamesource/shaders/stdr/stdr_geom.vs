
/*
stdr_geom.vs
Обычный рендер статической геометрии
*/

#include <struct.h>

//##########################################################################

half4x4	g_mWVP;
half4x4	g_mW;

//##########################################################################

VSO_SceneCommon main(VSI_Geometry IN)
{
	VSO_SceneCommon OUT;

	OUT.vPosition = mul(half4(IN.vPosition,1.f), g_mWVP);
	OUT.vNormal = IN.vNormal;
	OUT.vTexUV = IN.vTexUV;
	OUT.vPos = mul(half4(IN.vPosition,1.f), g_mW);

	return OUT;
}