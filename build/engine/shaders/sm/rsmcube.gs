
/*
terrax_colored.vs

*/

// #include "../struct.h"

//##########################################################################

cbuffer perCamera: register(b2)
{
	float4x4 g_mVP[6];
};

cbuffer perLight: register(b1)
{
	float4 g_vLightColorPower;
	float4 g_vLightPosShadow;
#ifdef IS_SPOT
	float4 g_vLightSpotDirection;
	float2 g_vLightSpotInnerOuterAngles;
#endif
};

//##########################################################################

struct XVSO_SceneCommon 
{
	float4 vPosition	:POSITION0;
	float4 vPos		:TEXCOORD2;
	XMAT_GS_STRUCT();
};

struct XGSO_SceneShadows
{
	float4 vPosition	:POSITION0;
	float4 vPos		:TEXCOORD2;
	XMAT_GS_STRUCT();
	
	uint uRTIndex   :SV_RenderTargetArrayIndex;
};

//##########################################################################

[maxvertexcount(18)]
void main(triangle XVSO_SceneCommon input[3], inout TriangleStream<XGSO_SceneShadows> OutputStream)
{
	[unroll]
	for(int f = 0; f < 6; ++f)
	{
		XGSO_SceneShadows OUT;
		OUT.uRTIndex = f;
				
		[unroll]
		for(int v = 0; v < 3; ++v)
		{
			OUT.vPosition = mul(float4(input[v].vPosition.xyz, 1.f), g_mVP[f]);
			OUT.vPos = float4(g_vLightPosShadow.xyz - input[v].vPosition.xyz, 1.0);
			
			XMAT_GS_PASS(OUT, input[v]);
			
			OutputStream.Append(OUT);
		}
		OutputStream.RestartStrip();
	}
	
}
