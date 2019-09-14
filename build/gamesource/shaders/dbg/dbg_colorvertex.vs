
/*
terrax_colored.vs

*/

#include <../struct.h>

//##########################################################################

cbuffer MVP :register(b4)
{
	half4x4 g_mVP;
};

//##########################################################################

VSO_DbgColorvertex main(VSI_DbgColorvertex IN)
{
	VSO_DbgColorvertex OUT = (VSO_DbgColorvertex)0;
	OUT.vPosition = mul(half4(IN.vPosition.xyz, 1), g_mVP);
	OUT.vColor = IN.vColor;
	return(OUT);
}
