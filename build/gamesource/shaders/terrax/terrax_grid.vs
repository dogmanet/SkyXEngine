
/*
terrax_grid.vs

*/

#include <../struct.h>

//##########################################################################

half4x4 g_mWVP;

//##########################################################################

VSO_TXGrid main(VSI_TXGrid IN)
{
	VSO_TXGrid OUT = (VSO_TXGrid)0;
	OUT.vPosition = mul(half4(IN.vPosition.xyz,1), g_mWVP);
	return(OUT);
}
