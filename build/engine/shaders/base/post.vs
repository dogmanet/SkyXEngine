
/*
pp_res_pos.vs
Восстановление позиции по глубине
*/

#include <struct.h>

//##########################################################################

cbuffer perFrame: register(b1)
{
	float4x4 g_mVP;
	float4x4 g_mViewInv;
	float4 g_vNearFar;
	float3 g_vParamProj;
};

//##########################################################################

VSO_ResPos main(VSI_PP IN)
{
	VSO_ResPos OUT;
	OUT.vPosition = float4(IN.vPosition, 1.0);
	OUT.vTexUV = IN.vTexUV.xy;
	
	float fTanHalfFOV = tan(g_vParamProj.z * 0.5) ; 
	float aspectRatio = g_vParamProj.x / g_vParamProj.y; 
	float fFarY = fTanHalfFOV * g_vNearFar.y; 
	float fFarX = fFarY * aspectRatio; 
	
	OUT.vEyeRay = float3(sign(OUT.vPosition.x) * fFarX, sign(OUT.vPosition.y) * fFarY, g_vNearFar.y); 
	OUT.vWorldRay = mul(half4(OUT.vEyeRay, 0.0), g_mViewInv).xyz;
  
	return OUT;
}