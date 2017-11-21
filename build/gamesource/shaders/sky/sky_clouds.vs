
/*
sky_clouds.vs
Рендер облаков (квадрат с натянутой текстурой облаков)
*/

#include <../struct.h>

half4x4 WorldViewProjection;

vs_out_skyclouds main(vs_in_skyclouds IN)
{
	vs_out_skyclouds OUT = (vs_out_skyclouds)0;
	OUT.Position = mul(half4(IN.Position.xyz,1),WorldViewProjection);
	OUT.TexUV = IN.TexUV;
	return OUT;
}