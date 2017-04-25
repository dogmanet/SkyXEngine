
#include <../struct.h>

half4x4 WorldViewProjection;
half4x4 World;
half3 LightPos;

vs_out_gcommon main(vs_in_geom IN)
{
	vs_out_gcommon OUT;
	
	OUT.Position= mul(half4(IN.Position.xyz,1.0),WorldViewProjection);
	OUT.TexUV	= IN.TexUV;
	OUT.Pos		= mul(half4(IN.Position.xyz,1.0),World);
	OUT.Pos		= half4(LightPos - OUT.Pos.xyz,1);

	return OUT;
}