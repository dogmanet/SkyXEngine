
/*
sm_depth_geom_cube.vs
Рендер глубины сцены (геометрия) с позиции точечного источника света
*/

#include <../struct.h>
#include <../shadow.h>

//##########################################################################

half4x4 g_mWVP;
half4x4 g_mW;
//half3 g_vLightPos;

//##########################################################################

VSO_SceneCommon main(VSI_Geometry IN)
{
	VSO_SceneCommon OUT;
	
	OUT.vPosition= mul(half4(IN.vPosition.xyz, 1.0),g_mWVP);
	OUT.vTexUV = IN.vTexUV;
	/*OUT.vPos.xyz = ShadowPosScopeBias(mul(half4(IN.vPosition.xyz, 1.0),g_mW), IN.vNormal);
	OUT.vPos.w = 1;*/
	
	OUT.vPos = mul(half4(IN.vPosition.xyz, 1.0),g_mW);

	return OUT;
}