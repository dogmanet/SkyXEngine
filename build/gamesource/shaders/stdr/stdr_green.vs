
/*
stdr_green.vs
Обычный рендер растительности
*/

#include <../struct.h>
#include <../green.h>

half4x4 WorldViewProjection;
half4x4 World;

#if defined(_GRASS_)
half2 DistBeginEndLessening : register(GREEN_R_LESSENING);
half3 ViewPos : register(GREEN_R_VIEWPOS);
half4 BSphere : register(GREEN_R_BSPHERE);
half3 BBMax : register(GREEN_R_BBMAX);
half3 BBMin : register(GREEN_R_BBMIN);
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