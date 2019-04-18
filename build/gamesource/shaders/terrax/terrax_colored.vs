
/*
terrax_colored.vs

*/

#include <../struct.h>

//##########################################################################

cbuffer perFrame: register(b4)
{
	half4x4 g_mWVP;
};

//##########################################################################

VSO_TXColored main(VSI_TXColored IN)
{
	VSO_TXColored OUT = (VSO_TXColored)0;
	OUT.vPosition = mul(half4(IN.vPosition.xyz,1), g_mWVP);
	return(OUT);
}
