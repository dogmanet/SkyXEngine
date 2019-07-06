
/*
terrax_colored.vs

*/

#include "../struct.h"

//##########################################################################

cbuffer perCamera: register(b2)
{
	half4x4 g_mVP[6];
};

cbuffer perLight: register(b1)
{
	half4 g_vLightColorPower;
	half4 g_vLightPosShadow;
#ifdef IS_SPOT
	half4 g_vLightSpotDirection;
	half2 g_vLightSpotInnerOuterAngles;
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
			OUT.VSO.vPosition = mul(half4(input[v].vPosition.xyz, 1.f), g_mVP[f]);
			OUT.VSO.vNormal = input[v].vNormal;
			OUT.VSO.vTexUV = input[v].vTexUV;
			// OUT.VSO.vPos = OUT.VSO.vPosition;
			OUT.VSO.vPos = half4(g_vLightPosShadow.xyz - input[v].vPosition.xyz, 1.0);
			
			OutputStream.Append(OUT);
		}
		OutputStream.RestartStrip();
	}
	
}
