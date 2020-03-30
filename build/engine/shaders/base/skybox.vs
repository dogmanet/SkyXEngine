
/*
skybox.vs
Скайбокс
*/

#include "../struct.h"
#include "../const.h"

//##########################################################################

VSO_SkyBox main(VSI_SkyBox IN)
{
	VSO_SkyBox OUT = (VSO_SkyBox)0;

	OUT.vPosition = mul(half4(IN.vPosition + g_vPosCam.xyz, 1.f), g_mVP);
	OUT.vTexUV = IN.vTexUV;

	return OUT;
}
