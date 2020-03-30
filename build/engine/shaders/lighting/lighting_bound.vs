
/*
lighting_bound.vs
Шейдер для отрисовки объемов света
*/

#include <struct.h>

//##########################################################################

// half4x4 g_mWVP;

cbuffer perLight: register(b0)
{
	float4x4 g_mW;
};

cbuffer perFrame: register(b1)
{
	float4x4 g_mVP;
	float4x4 g_mViewInv;
	float2 g_vNearFar;
	float3 g_vParamProj;
};

//##########################################################################

VSO_LightBound main(VSI_LightBound IN)
{
	VSO_LightBound OUT = (VSO_LightBound)0;
	OUT.vPosition = mul(float4(IN.vPosition, 1.0), mul(g_mW, g_mVP));
	return(OUT);
}
