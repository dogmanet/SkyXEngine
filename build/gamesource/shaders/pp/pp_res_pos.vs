
/*
pp_res_pos.vs
Восстановление позиции по глубине
*/

#include <../struct.h>

//##########################################################################

half4x4 g_mViewInv;
half2 g_vNearFar;
half3 g_vParamProj;

//##########################################################################

VSO_ResPos main(VSO_PP IN)
{
	VSO_ResPos OUT;
	OUT.vPosition = IN.vPosition;
	OUT.vTexUV = IN.vTexUV.xy;
	
	half fTanHalfFOV = tan(g_vParamProj.z * 0.5) ; 
	half aspectRatio = g_vParamProj.x / g_vParamProj.y; 
	half fFarY = fTanHalfFOV * g_vNearFar.y; 
	half fFarX = fFarY * aspectRatio; 
	
	OUT.vEyeRay = half3(sign(OUT.vPosition.x) * fFarX, sign(OUT.vPosition.y) * fFarY, g_vNearFar.y); 
	OUT.vWorldRay = mul(half4(OUT.vEyeRay, 0.0), g_mViewInv).xyz;
  
	return OUT;
}