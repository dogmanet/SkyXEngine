
#include <../struct.h>

half4x4 WorldViewProjection;

vs_out_skybox main(vs_in_skybox IN)
{
	vs_out_skybox OUT = (vs_out_skybox)0;
	OUT.Position = mul(half4(IN.Position.xyz,1),WorldViewProjection);
	OUT.TexUV = IN.TexUV;
	return OUT;
}