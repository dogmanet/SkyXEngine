
#include <../struct.h>

half4x4	WorldViewProjection;
half4x4	World;

vs_out_gcommon main(vs_in_geom IN)
{
	vs_out_gcommon OUT;

	OUT.Position = mul(half4(IN.Position,1.f),WorldViewProjection);
	OUT.Normal = mul(IN.Normal,World);
	OUT.TexUV = IN.TexUV;
	OUT.Pos = OUT.Position;

	return OUT;
}