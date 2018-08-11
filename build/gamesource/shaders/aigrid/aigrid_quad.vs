
/*
aigrid_quad.vs
Рендер ai сетки
*/

#include <../struct.h>

//##########################################################################

half4x4 g_mWVP;

//##########################################################################

VSO_AIgrid main(in VSI_AIgrid IN) 
{
	VSO_AIgrid OUT;
	OUT.vPosition.xyz = IN.vPosition + IN.vInstPos;
	OUT.vPosition.w = 1.0;
	OUT.vPosition = mul(OUT.vPosition, g_mWVP);
	
	OUT.vPos = OUT.vPosition;
	OUT.vColor = IN.vInstColor;
	OUT.vTexUV = (IN.vTexUV * IN.vInstTex.xy)+IN.vInstTex.zw;
	
	return OUT;
}