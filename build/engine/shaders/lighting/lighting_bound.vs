
/*
lighting_bound.vs
Шейдер для отрисовки объемов света
*/

#include <struct.h>
#include <const.h>

//##########################################################################

VSO_LightBound main(VSI_LightBound IN)
{
	VSO_LightBound OUT = (VSO_LightBound)0;
	OUT.vPosition = mul(float4(IN.vPosition, 1.0), mul(g_mW, g_mObserverVP));
	return(OUT);
}
