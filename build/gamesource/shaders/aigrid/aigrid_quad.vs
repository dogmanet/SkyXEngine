
/*
aigrid_quad.vs
Рендер ai сетки
*/

#include <../struct.h>

half4x4 WorldViewProjection;

void main(in vs_input_aigrid IN, out vs_output_aigrid OUT) 
{
	OUT.Position.xyz = IN.Position + IN.InstancePos;
	OUT.Position.w = 1.f;
	OUT.Position = mul(OUT.Position, WorldViewProjection);
	
	OUT.Pos = OUT.Position;
	OUT.Color = IN.InstanceCol;
	OUT.TexUV = (IN.TexUV * IN.InstanceTex.xy)+IN.InstanceTex.zw;
}