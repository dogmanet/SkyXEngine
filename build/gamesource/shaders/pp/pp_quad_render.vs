
/*
pp_quad_render.vs
Отрисовка полноэкранного квадрата (full screen quad)
*/

#include <../struct.h>

vs_out_pp main(vs_in_pp IN)
{
	vs_out_pp OUT;
	OUT.Position = IN.Position;
	OUT.TexUV = IN.TexUV;
	return OUT;
}