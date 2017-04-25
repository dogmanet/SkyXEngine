
#include <../struct.h>
#include <../green.h>

half4x4 WorldViewProjection;

half2 DistBeginEndLessening : register(c62);
half3 ViewPos : register(c63);

void main(in vs_in_green IN, out vs_out_gcommon OUT) 
{
	OUT.Normal = GreenComRotation(IN.Normal,IN.InstanceSinCosRot);
	
	OUT.Position = GreenTransformPos(
						GreenComRotation(IN.Position,IN.InstanceSinCosRot),
						IN.InstanceTrans.x,
						GrassComMultiplier(IN.InstancePos,ViewPos,DistBeginEndLessening),
						IN.InstancePos
					);
		
	OUT.Position = mul(OUT.Position, WorldViewProjection);
	
	OUT.Pos = OUT.Position;

	OUT.TexUV = IN.TexUV;
}