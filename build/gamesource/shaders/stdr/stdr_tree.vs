
#include <../struct.h>

half4x4 WorldViewProjection;
half4x4 World;

void main(in vs_in_green IN, out vs_out_gcommon OUT) 
{
	OUT.Position.x = IN.Position.x * IN.InstanceSinCosRot.y - IN.Position.z * IN.InstanceSinCosRot.x;
	OUT.Position.z = IN.Position.x * IN.InstanceSinCosRot.x + IN.Position.z * IN.InstanceSinCosRot.y;
	OUT.Position.y = IN.Position.y;
	OUT.Position.w = 1.f;
	
	/*OUT.Normal.x = IN.Normal.x * IN.InstanceSinCosRot.y - IN.Normal.z * IN.InstanceSinCosRot.x;
	OUT.Normal.z = IN.Normal.x * IN.InstanceSinCosRot.x + IN.Normal.z * IN.InstanceSinCosRot.y;
	OUT.Normal.y = IN.Normal.y;*/
	
	OUT.Position.xyz *= IN.InstanceTrans.x;
	OUT.Position.xyz += IN.InstancePos;
	
	OUT.Position = mul(OUT.Position, WorldViewProjection);
	OUT.Pos = mul(OUT.Position, World);

	OUT.TexUV = IN.TexUV;
}