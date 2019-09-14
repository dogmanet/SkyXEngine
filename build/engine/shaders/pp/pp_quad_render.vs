
/*
pp_quad_render.vs
Отрисовка полноэкранного квадрата (full screen quad)
*/

#include "../struct.h"

//##########################################################################

VSO_PP main(VSI_PP IN)
{
	VSO_PP OUT;
	OUT.vPosition = float4(IN.vPosition, 1.0);
	OUT.vTexUV = IN.vTexUV;
	return OUT;
}
