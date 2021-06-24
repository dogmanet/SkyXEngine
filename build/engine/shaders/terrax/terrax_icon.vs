
/*
terrax_icon.vs
Рендер иконок в 3d
*/

#include <struct.h>
#include <const.h>

//##########################################################################

VSO_TXTextured main(in VSI_TXIcon IN) 
{
	VSO_TXTextured OUT;
	OUT.vPosition.xyz = IN.vPosition + IN.vInstPos;
	OUT.vPosition.w = 1.0;
	OUT.vPosition = mul(OUT.vPosition, g_mObserverVP);
	OUT.vTexUV = IN.vTexUV;
	
	return OUT;
}