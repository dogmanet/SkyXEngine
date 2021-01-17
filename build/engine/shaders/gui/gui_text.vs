
/*
gui_main.vs

*/

#include <struct.h>
#include <const.h>

//##########################################################################

cbuffer CDataTramsform: register(b6)
{
	float2 g_vOffset;
};

//##########################################################################

VSO_GUITextured main(VSI_GUIText IN)
{
	VSO_GUITextured OUT = (VSO_GUITextured)0;
	OUT.vPosition = mul(float4(IN.vPosition.xy + g_vOffset, 0.0, 1.0), g_mVP);
	OUT.vTexUV = IN.vTexUV;
	return(OUT);
}
