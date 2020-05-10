
/*
terrax_colored.vs

*/

#include <struct.h>
#include <const.h>

//##########################################################################

VSO_TXColored main(VSI_TXColored IN)
{
	VSO_TXColored OUT = (VSO_TXColored)0;
	OUT.vPosition = mul(float4(IN.vPosition.xyz, 1), g_mW);
	OUT.vPosition = mul(OUT.vPosition, g_mObserverVP);
	return(OUT);
}
