
#include <../struct.h>
#include <../green.h>

half4x4 WorldViewProjection;

half2 DistBeginEndLessening : register(GREEN_R_LESSENING);
half3 ViewPos : register(GREEN_R_VIEWPOS);
half4 BSphere : register(GREEN_R_BSPHERE);
half3 BBMax : register(GREEN_R_BBMAX);
half3 BBMin : register(GREEN_R_BBMIN);

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
	OUT.Pos.w = length(BSphere.xyz - IN.Position.xyz)/BSphere.w;
	OUT.Pos.w *= OUT.Pos.w;
	OUT.TexUV = IN.TexUV;
}