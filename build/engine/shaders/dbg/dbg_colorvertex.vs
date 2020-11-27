
/*
terrax_colored.vs

*/

#include <struct.h>
#include <const.h>

//##########################################################################

VSO_DbgColorvertex main(VSI_DbgColorvertex IN)
{
	VSO_DbgColorvertex OUT = (VSO_DbgColorvertex)0;
	OUT.vPosition = mul(float4(IN.vPosition.xyz, 1), g_mObserverVP);
	OUT.vColor = IN.vColor;
	return(OUT);
}
