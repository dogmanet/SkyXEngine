
/*
terrax_colored.vs

*/

#include <struct.h>
#include <lpv.h>

//##########################################################################

cbuffer perFrame: register(b1)
{
	float4x4 g_mVP;
	float4x4 g_mViewInv;
	float2 g_vNearFar;
	float3 g_vParamProj;
};

//##########################################################################

VSO_GICubes main(VSI_GICubes IN)
{
	VSO_GICubes OUT = (VSO_GICubes)0;
	OUT.vPosition = float4(IN.vPosition.xyz * GetGridWorldSize(0) + GetGridCenter(0), 1.0);
	// OUT.vPosition = mul(half4(IN.vPosition.xyz,1), g_mVP);
	return(OUT);
}
