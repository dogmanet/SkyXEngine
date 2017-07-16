
/*
sm_depth_green_cube.vs
Рендер глубины сцены (растительность) с позиции точечного источника света
*/

#include <../struct.h>
#include <../green.h>

half4x4 WorldViewProjection;
half3 LightPos;

#if defined(_GRASS_)
half2 DistBeginEndLessening : register(GREEN_R_LESSENING);
half3 ViewPos : register(GREEN_R_VIEWPOS);
half4 BSphere : register(GREEN_R_BSPHERE);
half3 BBMax : register(GREEN_R_BBMAX);
half3 BBMin : register(GREEN_R_BBMIN);
#endif

void main( in vs_in_green IN, out vs_out_gcommon OUT ) 
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
					
	OUT.Pos	= half4(LightPos - OUT.Position,1);
	OUT.Position = mul(OUT.Position, WorldViewProjection);
	OUT.Normal = IN.Normal;
	OUT.TexUV = IN.TexUV;
}