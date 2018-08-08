
/*
sky_clouds.vs
Рендер облаков (квадрат с натянутой текстурой облаков)
*/

#include <../struct.h>

//##########################################################################

half4x4 g_mWVP;

//##########################################################################

VSO_SkyClouds main(VSI_SkyClouds IN)
{
	VSO_SkyClouds OUT = (VSO_SkyClouds)0;
	OUT.vPosition = mul(half4(IN.vPosition.xyz,1), g_mWVP);
	OUT.vTexUV = IN.vTexUV;
	return OUT;
}