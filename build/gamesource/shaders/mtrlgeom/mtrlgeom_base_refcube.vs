
#include <../struct.h>

half4x4	WorldViewProjection;
half4x4	WorldView;
half4x4	View;
half4x4	World;

vs_out_refcube main(vs_in_geom IN)
{
	vs_out_refcube OUT;

	OUT.Position = mul(half4(IN.Position,1), WorldViewProjection);
	OUT.Normal	= mul(IN.Normal, World);
	OUT.TexUV = IN.TexUV;
	OUT.Pos = OUT.Position;
	
	half3 viewVec= mul(IN.Position, WorldView);
	OUT.CubePPos = mul((half3x3)(View),reflect(viewVec, mul(OUT.Normal, (half3x3)(View))));
	
	return OUT;
}