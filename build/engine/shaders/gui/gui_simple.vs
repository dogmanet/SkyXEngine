
/*
gui_simple.vs

*/

#include <struct.h>

//##########################################################################

half4x4 g_mWVP1: register(b0);

//##########################################################################

VSO_GUISimple main(VSI_GUISimple IN)
{
	VSO_GUISimple OUT = (VSO_GUISimple)0;
	OUT.vPosition = mul(half4(IN.vPosition,1), g_mWVP1);
	return(OUT);
}
