
/*
dev_lines.vs

*/

#include <struct.h>
#include <const.h>

//##########################################################################

VSO_DevLines main(VSI_DevLines IN)
{
	VSO_DevLines OUT = (VSO_DevLines)0;
	
	float3 vPos = IN.vPosition.xyz;
	float3 vRight = normalize(cross(vPos - g_vObserverPosCam.xyz, IN.vDir));
	
	vPos += vRight * IN.vPosition.w * (IN.vTexUV.y - 0.5);
	
	OUT.vPosition = mul(float4(vPos, 1), g_mObserverVP);
	// OUT.vPosition = mul(OUT.vPosition, g_mObserverVP);
	OUT.vTexUV = IN.vTexUV;
	OUT.vColor = IN.vColor;
	return(OUT);
}
