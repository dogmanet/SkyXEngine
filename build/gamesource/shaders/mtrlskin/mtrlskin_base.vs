
/*
mtrlskin_base.vs
Базовый рендер анимационных моделей
*/

#include <../struct.h>
#include <../skin.h>
#include <../const.h>

//##########################################################################

VSO_SceneCommon main(VSI_Animation IN)
{
	// трансформация позиции и нормали вершины
	VSO_SceneCommon OUT = SkinAllTransform(IN);
	
	OUT.vPosition = mul(OUT.vPosition, g_mWVP);
	OUT.vNormal = mul(OUT.vNormal, g_mW);
	OUT.vTexUV = IN.vTexUV;
	
	OUT.vPos = OUT.vPosition;
    
	return(OUT);
}
