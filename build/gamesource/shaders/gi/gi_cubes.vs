
/*
terrax_colored.vs

*/

#include <../struct.h>

//##########################################################################

cbuffer perFrame: register(b1)
{
	half4x4 g_mVP;
	half4x4 g_mViewInv;
	half2 g_vNearFar;
	half3 g_vParamProj;
};

//##########################################################################

VSO_GICubes main(VSI_GICubes IN)
{
	VSO_GICubes OUT = (VSO_GICubes)0;
	OUT.vPosition = float4(IN.vPosition.xyz,1);
	// OUT.vPosition = mul(half4(IN.vPosition.xyz,1), g_mVP);
	return(OUT);
}
