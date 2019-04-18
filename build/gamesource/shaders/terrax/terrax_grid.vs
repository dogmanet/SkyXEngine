
/*
terrax_grid.vs

*/

#include <../struct.h>

//##########################################################################

cbuffer MVP : register(b0)
{
	half4x4 g_mWVP1;
};

//##########################################################################

VSO_TXColored main(VSI_TXGrid IN)
{
	VSO_TXColored OUT = (VSO_TXColored)0;
	OUT.vPosition = mul(half4(IN.vPosition.xyz,1), g_mWVP1);
	return(OUT);
}
