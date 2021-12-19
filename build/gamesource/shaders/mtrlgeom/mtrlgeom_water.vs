
/*
mtrlgeom_water.vs
Рендер воды (геометрии)
*/

#include <struct.h>
#include <mtrl.h>

//##########################################################################

half4x4 g_mWVP;
half2 g_vTimeDelta;
half4 g_vParam;
half4 g_vPosCam;
half4x4 g_mW;

//##########################################################################

VSO_WaterPlane main(VSI_Geometry IN)
{
	VSO_WaterPlane OUT;
	
	OUT.vPosition = mul(half4(IN.vPosition,1.f),g_mWVP);
	OUT.vTexUV = IN.vTexUV;
	
	half4 WorldPos = mul(IN.vPosition,g_mW);
    OUT.ViewDir = g_vPosCam - WorldPos;
	OUT.vPos = OUT.vPosition;
	
	OUT.vNormal = mul(IN.vNormal,g_mW);

	half2 vTexUV = IN.vTexUV * g_vParam.z * 100.f;
	OUT.vTexUV2.x = vTexUV.x + sin(g_vTimeDelta.x * g_vParam.w*0.01)*g_vParam.x;
	OUT.vTexUV2.y = vTexUV.y - cos(g_vTimeDelta.x * g_vParam.w*0.01)*g_vParam.y;
	OUT.vTexUV3.x = vTexUV.x - sin(g_vTimeDelta.x * g_vParam.w*0.01)*g_vParam.x;
	OUT.vTexUV3.y = vTexUV.y + cos(g_vTimeDelta.x * g_vParam.w*0.01)*g_vParam.y;
	
	return OUT;
}