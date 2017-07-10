
half4x4 WorldViewProjection;

struct INPUT 
{
	half3 Position	:POSITION0;
	half2 TexUV 	:TEXCOORD0;
	
	half3 InstancePos	:TEXCOORD1;
	half4 InstanceTex	:TEXCOORD2;
	half4 InstanceCol	:COLOR0;
};

struct OUTPUT 
{
	half4 Position	:POSITION0;
	half2 TexUV		:TEXCOORD0;
	half4 Pos		:TEXCOORD1;
	half4 Color		:TEXCOORD2;
};

void main( in INPUT IN, out OUTPUT OUT ) 
{
	OUT.Position.xyz = IN.Position + IN.InstancePos;
	OUT.Position.w = 1.f;
	OUT.Position = mul(OUT.Position, WorldViewProjection);
	
	OUT.Pos = OUT.Position;
	OUT.Color = IN.InstanceCol;
	OUT.TexUV = (IN.TexUV * IN.InstanceTex.xy)+IN.InstanceTex.zw;
}