
#include <../struct.h>

float4x4 WorldViewProjection;

vs_out_skyclouds main(vs_in_skyclouds IN)
{
	vs_out_skyclouds OUT = (vs_out_skyclouds)0;
	OUT.Position = mul(float4(IN.Position.xyz,1),WorldViewProjection);
	OUT.TexUV = IN.TexUV;
	return OUT;
}