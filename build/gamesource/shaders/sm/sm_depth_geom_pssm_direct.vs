
/*
sm_depth_geom_pssm_direct.vs
Рендер глубины сцены (геометрии) с позиции позиции направленного/глобального источника света
*/

#include <../struct.h>
#include <../shadow.h>

//##########################################################################

half4x4 g_mWVP:register(c0);
half3 g_vLigthPos;

//##########################################################################

VSO_SceneCommon main(VSI_Geometry IN)
{
	VSO_SceneCommon OUT;

	OUT.vPosition = mul(half4(IN.vPosition.xyz /*- vVertexToLigth * 1*/, 1.0),g_mWVP);
	OUT.vTexUV = IN.vTexUV;
	OUT.vPos = OUT.vPosition;

	return OUT;
}