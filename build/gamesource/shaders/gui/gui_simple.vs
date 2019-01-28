
/*
gui_simple.vs

*/

#include <../struct.h>

//##########################################################################

half4x4 g_mWVP;

//##########################################################################

VSO_GUISimple main(VSI_GUISimple IN)
{
	VSO_GUISimple OUT = (VSO_GUISimple)0;
	OUT.vPosition = mul(half4(IN.vPosition.xyz,1), g_mWVP);
	return(OUT);
}
