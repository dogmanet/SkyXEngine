
#include <../struct.h>

float4x4 WorldViewProjection;
float2 DistBeginEndLessening;

void main( in vs_in_green IN, out vs_out_green OUT ) 
{
	float multiplier = 1.0 - clamp((mul(float4(IN.InstancePos, 1.f), WorldViewProjection).z - DistBeginEndLessening.x) / (DistBeginEndLessening.y - DistBeginEndLessening.x), 0.0, 1.0);
	
	float2 fSinCos = float2(sin(IN.InstanceTrans.y), cos(IN.InstanceTrans.y));
	OUT.Position.x = IN.Position.x * fSinCos.y - IN.Position.z * fSinCos.x;
	OUT.Position.z = IN.Position.x * fSinCos.x + IN.Position.z * fSinCos.y;
	OUT.Position.y = IN.Position.y;
	OUT.Position.w = 1.f;
	
	OUT.Normal.x = IN.Normal.x * fSinCos.y - IN.Normal.z * fSinCos.x;
	OUT.Normal.z = IN.Normal.x * fSinCos.x + IN.Normal.z * fSinCos.y;
	OUT.Normal.y = IN.Normal.y;
	
	OUT.Position.xyz *= IN.InstanceTrans.x * multiplier;

	
	OUT.Position.xyz += IN.InstancePos;
	OUT.Position = mul(OUT.Position, WorldViewProjection);
	
	OUT.Pos = OUT.Position;

	OUT.TexUV = IN.TexUV;
}