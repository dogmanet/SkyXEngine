
/*
sm_depth_skin_direct.vs
Рендер глубины сцены (анимационные модели) с позиции направленного источника света
*/

#include <../struct.h>
#include <../skin.h>
#include <../shadow.h>

//##########################################################################

half4x4	g_mWVP;

//##########################################################################

VSO_SceneCommon main(VSI_Animation IN)
{
	VSO_SceneCommon OUT = SkinAllTransform(IN);
	
	OUT.vPosition = mul(OUT.vPosition, g_mWVP);
	OUT.vTexUV = IN.vTexUV;
	
	OUT.vPos = OUT.vPosition;
    
	return(OUT);
}
