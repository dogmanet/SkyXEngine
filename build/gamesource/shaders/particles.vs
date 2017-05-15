
float4x4 ViewProjection;
float4x4 World;
float4x4 WorldMat;

float4x4 MatRot;
float4x4 MatPos;

float3 PosCam;

float3 CamUp;
float3 CamRight;

struct INPUT 
{
	float3 Position :POSITION0;
	float2 TexUV : TEXCOORD0;
	
	float3 InstancePos :TEXCOORD1;
	float4 InstanceTex :TEXCOORD2;
	float InstanceSize :TEXCOORD3;
	float InstanceAlpha :TEXCOORD4;
	
	float2 InstanceRot1 :TEXCOORD5;
	float InstanceLight :TEXCOORD6;
};

struct OUTPUT 
{
	float4 Position :POSITION0;
	float2 TexUV 	:TEXCOORD0;
	float Alpha		:TEXCOORD1;
	float4 Pos		:TEXCOORD2;
	float Light	:TEXCOORD3;
	float2 TexUV2	:TEXCOORD4;
};


void main( in INPUT IN, out OUTPUT OUT ) 
{
	half2 tvec;
	tvec.x = IN.Position.x * IN.InstanceRot1.y - IN.Position.y * IN.InstanceRot1.x;
	tvec.y = IN.Position.x * IN.InstanceRot1.x + IN.Position.y * IN.InstanceRot1.y;
	IN.Position.xy = tvec;
	
	IN.Position.xyz *= IN.InstanceSize;
	
	OUT.Position.xyz = IN.Position + IN.InstancePos;
	OUT.Position.w = 1.f;
	
	OUT.Position = mul(OUT.Position, MatRot);
	OUT.Position = mul(OUT.Position, MatPos);
	
	OUT.Alpha = IN.InstanceAlpha;
	OUT.Light = IN.InstanceLight;
	
	OUT.Position = mul(OUT.Position, ViewProjection);
	OUT.Pos = OUT.Position;
	OUT.TexUV2 = IN.TexUV;
	OUT.TexUV = (IN.TexUV * IN.InstanceTex.xy)+IN.InstanceTex.zw;
}