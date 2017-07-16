
/*
sm_depth_geom_pssm_direct.vs
Рендер глубины сцены (геометрии) с позиции позиции направленного/глобального источника света
*/

#include <../struct.h>

half4x4 WorldViewProjection:register(c0);

vs_out_gcommon main(vs_in_geom IN)
{
	vs_out_gcommon OUT;
	OUT.Position = mul(half4(IN.Position.xyz,1.0),WorldViewProjection);
	OUT.TexUV = IN.TexUV;
	OUT.Pos = OUT.Position;

	return OUT;
}