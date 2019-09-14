
/*
terrax_textured.vs

*/

#include <../struct.h>

//##########################################################################

cbuffer perFrame: register(b4)
{
	half4x4 g_mWVP;
};

//##########################################################################

VSO_TXTextured main(VSI_TXTextured IN)
{
	VSO_TXTextured OUT = (VSO_TXTextured)0;
	OUT.vPosition = mul(half4(IN.vPosition.xyz,1), g_mWVP);
	OUT.vTexUV = IN.vTexUV;
	return(OUT);
}
