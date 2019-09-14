
/*
pp_res_pos.vs
Восстановление позиции по глубине
*/

#include <../struct.h>

//##########################################################################

cbuffer perFrame: register(b1)
{
	half4x4 g_mVP;
	half4x4 g_mViewInv;
	half4 g_vNearFar;
	half3 g_vParamProj;
};

//##########################################################################

VSO_ResPos main(VSI_PP IN)
{
	VSO_ResPos OUT;
	OUT.vPosition = half4(IN.vPosition, 1.0);
	OUT.vTexUV = IN.vTexUV.xy;
	
	half fTanHalfFOV = tan(g_vParamProj.z * 0.5) ; 
	half aspectRatio = g_vParamProj.x / g_vParamProj.y; 
	half fFarY = fTanHalfFOV * g_vNearFar.y; 
	half fFarX = fFarY * aspectRatio; 
	
	OUT.vEyeRay = half3(sign(OUT.vPosition.x) * fFarX, sign(OUT.vPosition.y) * fFarY, g_vNearFar.y); 
	OUT.vWorldRay = mul(half4(OUT.vEyeRay, 0.0), g_mViewInv).xyz;
  
	return OUT;
}