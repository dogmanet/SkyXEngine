
/*
terrax_handler.vs
Рендер крестиков объектов в 2d видах
*/

#include <struct.h>
#include <const.h>

//##########################################################################

VSO_TXColored main(in VSI_TXBound IN) 
{
	VSO_TXColored OUT;
	OUT.vPosition.xyz = IN.vPosition * IN.vInstSize + IN.vInstPos;
	OUT.vPosition.w = 1.0;
	OUT.vPosition = mul(OUT.vPosition, g_mObserverVP);
	
	return OUT;
}