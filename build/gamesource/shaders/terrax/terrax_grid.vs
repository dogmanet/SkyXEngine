
/*
terrax_grid.vs

*/

#include <../struct.h>

//##########################################################################

half4x4 g_mWVP: register(b0);

//##########################################################################

VSO_TXColored main(VSI_TXGrid IN)
{
	VSO_TXColored OUT = (VSO_TXColored)0;
	OUT.vPosition = mul(half4(IN.vPosition.xyz,1), g_mWVP);
	return(OUT);
}
