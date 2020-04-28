
/*
terrax_colored.vs

*/

#include <struct.h>
#include <lpv.h>

//##########################################################################

VSO_GICubes main(VSI_GICubes IN)
{
	uint uCurrentCascade = g_vCurrentCascade.x;
	
	VSO_GICubes OUT = (VSO_GICubes)0;
	OUT.vPosition = float4(IN.vPosition.xyz * GetGridWorldSize(uCurrentCascade) + GetGridCenter(uCurrentCascade), 1.0);
	// OUT.vPosition = mul(half4(IN.vPosition.xyz,1), g_mVP);
	return(OUT);
}
