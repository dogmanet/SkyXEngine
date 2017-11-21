
/*
particles_main.vs
Рендер партиклов
*/

#include <../struct.h>

half4x4 ViewProjection;
half4x4 WorldViewProjection;
half4x4 CamRot;

half3 PosCam;

void main(in vs_in_particles IN, out vs_out_particles OUT) 
{
	half2 tvec;
	tvec.x = IN.Position.x * IN.InstanceRot1.y + IN.Position.y * IN.InstanceRot1.x;
	tvec.y = -IN.Position.x * IN.InstanceRot1.x + IN.Position.y * IN.InstanceRot1.y;
	IN.Position.xy = tvec;
	
	IN.Position.xyz *= IN.InstanceSize;
	
	IN.Position = mul(IN.Position, CamRot);
	
	OUT.Position.xyz = IN.Position + IN.InstancePos;
	OUT.Position.w = 1.f;
	
	OUT.Alpha = IN.InstanceAlpha;
	OUT.Light = IN.InstanceLight;
	
	OUT.Position = mul(OUT.Position, WorldViewProjection);
	OUT.Pos = OUT.Position;
	OUT.TexUV2 = IN.TexUV;
	OUT.TexUV = (IN.TexUV * IN.InstanceTex.xy)+IN.InstanceTex.zw;
}