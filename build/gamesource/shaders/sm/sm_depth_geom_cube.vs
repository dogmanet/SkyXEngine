
float4x4 WorldViewProjection;
float3 LightPos;

struct VS_INPUT_SHADOW
{
	float4 Position	:POSITION;
	float2 TexUV		:TEXCOORD0;
};

struct VSOUTPUT_SHADOW
{
	float4 Position	:POSITION0;
	float3 Pos		:TEXCOORD0;
	float2 TexUV	:TEXCOORD1;
};

//-----------------------------------------------------------------------------------------------

VSOUTPUT_SHADOW main(VS_INPUT_SHADOW IN)
{
	VSOUTPUT_SHADOW OUT;
	
	OUT.Position= mul(float4(IN.Position.xyz,1.0),WorldViewProjection);
	OUT.TexUV	= IN.TexUV;
	OUT.Pos		= LightPos - IN.Position.xyz;

	return OUT;
}