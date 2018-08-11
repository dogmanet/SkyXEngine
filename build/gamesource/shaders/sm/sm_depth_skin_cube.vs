
/*
sm_depth_skin_cube.vs
Рендер глубины сцены (анимационные модели) с позиции точечного источника света
*/

#include <../struct.h>
#include <../skin.h>
#include <../shadow.h>

//##########################################################################

half4x4	g_mWVP;
half4x4	g_mW;
half3 g_vLightPos;

//##########################################################################

VSO_SceneCommon main(VSI_Animation IN)
{
	VSO_SceneCommon OUT = SkinAllTransform(IN);
	
	OUT.vPos = half4(g_vLightPos - mul(OUT.vPosition, g_mW).xyz, 1);
	OUT.vPosition = mul(OUT.vPosition, g_mWVP);
	OUT.vTexUV = IN.vTexUV;
	
	return(OUT);
}
