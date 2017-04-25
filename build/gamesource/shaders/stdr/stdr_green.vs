
#include <../struct.h>
#include <../green.h>

half4x4 WorldViewProjection;
half4x4 World;

#if defined(_GRASS_)
half2 DistBeginEndLessening : register(c62);
half3 ViewPos : register(c63);
#endif

void main(in vs_in_green IN, out vs_out_gcommon OUT) 
{
	half multiplier = 1;
	
	#if defined(_GRASS_)
	multiplier = GrassComMultiplier(IN.InstancePos,ViewPos,DistBeginEndLessening);
	#endif
	
	OUT.Position = GreenTransformPos(
						GreenComRotation(IN.Position,IN.InstanceSinCosRot),
						IN.InstanceTrans.x,
						multiplier,
						IN.InstancePos
					);
					
	OUT.Pos = mul(OUT.Position, World);
	OUT.Position = mul(OUT.Position, WorldViewProjection);

	OUT.TexUV = IN.TexUV;
}