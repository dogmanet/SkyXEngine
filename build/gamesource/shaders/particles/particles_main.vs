
#include <../struct.h>

float4x4 ViewProjection;
// float4x4 World;
float4x4 WorldViewProjection;
float4x4 CamRot;
//float4x4 WorldMat;

// float4x4 MatRot;
// float4x4 MatPos;

float3 PosCam;

void main( in vs_in_particles IN, out vs_out_particles OUT ) 
{
	half2 tvec;
	tvec.x = IN.Position.x * IN.InstanceRot1.y + IN.Position.y * IN.InstanceRot1.x;
	tvec.y = -IN.Position.x * IN.InstanceRot1.x + IN.Position.y * IN.InstanceRot1.y;
	IN.Position.xy = tvec;
	
	IN.Position.xyz *= IN.InstanceSize;
	
	IN.Position = mul(IN.Position, CamRot);
	
	OUT.Position.xyz = IN.Position + IN.InstancePos;
	OUT.Position.w = 1.f;
	
	// OUT.Position = mul(OUT.Position, MatRot);
	// OUT.Position = mul(OUT.Position, MatPos);
	// OUT.Position = mul(OUT.Position, World);
	
	
	OUT.Alpha = IN.InstanceAlpha;
	OUT.Light = IN.InstanceLight;
	
	OUT.Position = mul(OUT.Position, WorldViewProjection);
	OUT.Pos = OUT.Position;
	OUT.TexUV2 = IN.TexUV;
	OUT.TexUV = (IN.TexUV * IN.InstanceTex.xy)+IN.InstanceTex.zw;
}