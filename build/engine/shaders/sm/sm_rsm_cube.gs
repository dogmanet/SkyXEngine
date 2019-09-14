
/*
terrax_colored.vs

*/

#include "../struct.h"

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

[maxvertexcount(18)]
void main(triangle VSO_SceneCommon input[3], inout TriangleStream<GSO_SceneShadows> OutputStream)
{
	[unroll]
	for(int f = 0; f < 6; ++f)
	{
		GSO_SceneShadows OUT;
		OUT.uRTIndex = f;
				
		[unroll]
		for(int v = 0; v < 3; ++v)
		{
			OUT.VSO.vPosition = mul(float4(input[v].vPosition.xyz, 1.f), g_mVP[f]);
			OUT.VSO.vNormal = input[v].vNormal;
			OUT.VSO.vTexUV = input[v].vTexUV;
			// OUT.VSO.vPos = OUT.VSO.vPosition;
			OUT.VSO.vPos = float4(g_vLightPosShadow.xyz - input[v].vPosition.xyz, 1.0);
			// OUT.VSO.vPos = float4(float3(-0.13445021, -0.55678467, -0.0801237256), 1.0);
			// OUT.VSO.vPos = float4(float3(-4.89091921, 2.32815933, -0.0218197256) - float3(-4.756469, 2.884944, 0.058304), 1.0);
			
			OutputStream.Append(OUT);
		}
		OutputStream.RestartStrip();
	}
	
}
