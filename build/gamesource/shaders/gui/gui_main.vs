
/*
gui_main.vs

*/

#include <../struct.h>

//##########################################################################

half4x4 g_mWVP: register(b0);

//##########################################################################

VSO_GUITextured main(VSI_GUITextured IN)
{
	VSO_GUITextured OUT = (VSO_GUITextured)0;
	OUT.vPosition = mul(half4(IN.vPosition.xyz,1), g_mWVP);
	OUT.vTexUV = IN.vTexUV;
	return(OUT);
}
