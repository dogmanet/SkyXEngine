
#include <../struct.h>

float4x4	WorldViewProjection;
float4x4	World;

vs_out_green main(vs_in_geom IN)
{
	vs_out_green OUT;

	OUT.Position = mul(float4(IN.Position,1.f),WorldViewProjection);
	OUT.Normal	= mul(IN.Normal,World);
	OUT.TexUV = IN.TexUV;
	OUT.Pos = OUT.Position;
	
	return OUT;
}