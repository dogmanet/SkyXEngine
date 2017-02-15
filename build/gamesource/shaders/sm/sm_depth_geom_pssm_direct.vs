
float4x4 WorldViewProjection:register(c0);

struct VS_INPUT_SHADOW
{
	float4 Position	:POSITION;
	float2 TexUV		:TEXCOORD0;
};

struct VSOUTPUT_SHADOW
{
	float4 Position	:POSITION0;
	float4 Pos		:TEXCOORD0;
	float2 TexUV		:TEXCOORD1;
	//float  Depth		:DEPTH;
};

//-----------------------------------------------------------------------------------------------

VSOUTPUT_SHADOW main(VS_INPUT_SHADOW IN)
{
	VSOUTPUT_SHADOW OUT;
	//float4 tmp_pos = mul(float4(IN.Position.xyz,1.0),World);
	OUT.Position	= mul(float4(IN.Position.xyz,1.0),WorldViewProjection);
	//OUT.Position.y += 0.01;
	OUT.TexUV		= IN.TexUV;
	OUT.Pos		= OUT.Position;

	return OUT;
}