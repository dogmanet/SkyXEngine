
/*
sky_box.vs
Рендер SkyBox
*/

#include <struct.h>

//##########################################################################

half4x4 g_mWVP;

//##########################################################################

VSO_SkyBox main(VSI_SkyBox IN)
{
	VSO_SkyBox OUT = (VSO_SkyBox)0;
	OUT.vPosition = mul(half4(IN.vPosition.xyz,1), g_mWVP);
	OUT.vTexUV = IN.vTexUV;
	return OUT;
}