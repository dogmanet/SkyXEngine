
/*
terrax_colored.vs

*/

#include <struct.h>

//##########################################################################

cbuffer perFrame: register(b1)
{
	float4x4 g_mVP;
	float4x4 g_mViewInv;
	float2 g_vNearFar;
	float3 g_vParamProj;
};

cbuffer b8: register(b8)
{
	// float4x4 mV;
	float4x4 g_mVP1;
	float4 g_vPosCam;
};

//##########################################################################

VSO_GICubes main(VSI_GICubes IN)
{
	VSO_GICubes OUT = (VSO_GICubes)0;
	OUT.vPosition = float4(IN.vPosition.xyz + floor(g_vPosCam).xyz,1);
	// OUT.vPosition = mul(half4(IN.vPosition.xyz,1), g_mVP);
	return(OUT);
}
