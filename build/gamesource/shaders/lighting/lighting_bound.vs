
/*
lighting_bound.vs
Шейдер для отрисовки объемов света
*/

#include <../struct.h>

//##########################################################################

half4x4 g_mWVP;

//##########################################################################

VSO_LightBound main(VSI_LightBound IN)
{
	VSO_LightBound OUT = (VSO_LightBound)0;
	OUT.vPosition = mul(half4(IN.vPosition.xyz,1), g_mWVP);
	return(OUT);
}
