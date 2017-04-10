
#include <../struct.h>

float4x4 WorldViewProjection;
#if defined(_GRASS_)
half2 DistBeginEndLessening : register(c62);
half3 ViewPos : register(c63);
#endif

void main(in vs_in_green IN, out vs_out_gcommon OUT) 
{
	#if defined(_GRASS_)
	half multiplier = 1.0 - saturate((distance(ViewPos,IN.InstancePos) - DistBeginEndLessening.x) / (DistBeginEndLessening.y - DistBeginEndLessening.x));
	#endif
	
	OUT.Position.x = IN.Position.x * IN.InstanceSinCosRot.y - IN.Position.z * IN.InstanceSinCosRot.x;
	OUT.Position.z = IN.Position.x * IN.InstanceSinCosRot.x + IN.Position.z * IN.InstanceSinCosRot.y;
	OUT.Position.y = IN.Position.y;
	OUT.Position.w = 1.f;
	
	OUT.Position.xyz *= IN.InstanceTrans.x;
	
	#if defined(_GRASS_)
	OUT.Position.xyz *= multiplier;
	#endif

	OUT.Position.xyz += IN.InstancePos;
	OUT.Position = mul(OUT.Position, WorldViewProjection);
	
	OUT.Normal = IN.Normal;
	OUT.Pos = OUT.Position;

	OUT.TexUV = IN.TexUV;
}