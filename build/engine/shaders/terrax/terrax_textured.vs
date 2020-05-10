
/*
terrax_textured.vs

*/

#include <struct.h>
#include <const.h>

//##########################################################################

VSO_TXTextured main(VSI_TXTextured IN)
{
	VSO_TXTextured OUT = (VSO_TXTextured)0;
	OUT.vPosition = mul(float4(IN.vPosition.xyz, 1), g_mObserverVP);
	// OUT.vPosition = mul(OUT.vPosition, g_mObserverVP);
	OUT.vTexUV = IN.vTexUV;
	return(OUT);
}
