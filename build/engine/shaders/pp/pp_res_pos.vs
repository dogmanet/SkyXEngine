
/*
pp_res_pos.vs
Восстановление позиции по глубине
*/

#include <struct.h>
#include <const.h>

//##########################################################################

VSO_ResPos main(VSI_PP IN)
{
	VSO_ResPos OUT;
	OUT.vPosition = float4(IN.vPosition, 1.0);
	OUT.vTexUV = IN.vTexUV.xy;
	
	float fTanHalfFOV = tan(g_vObserverParamProj.z * 0.5) ; 
	float aspectRatio = g_vObserverParamProj.x / g_vObserverParamProj.y; 
	float fFarY = fTanHalfFOV * g_vObserverNearFar.y; 
	float fFarX = fFarY * aspectRatio; 
	
	OUT.vEyeRay = float3(sign(OUT.vPosition.x) * fFarX, sign(OUT.vPosition.y) * fFarY, g_vObserverNearFar.y); 
	OUT.vWorldRay = mul(float4(OUT.vEyeRay, 0.0), g_mObserverInvV).xyz;
	
	return OUT;
}