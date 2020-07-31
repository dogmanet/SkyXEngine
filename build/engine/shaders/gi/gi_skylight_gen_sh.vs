
#include <lpv.h>
#include <const.h>

cbuffer b0 :register(b0)
{
	uint g_uSkyboxSize;
	float g_fWeight;
	float g_fTexOffset;
};

struct PS_IN
{
	float4 vPos :SV_POSITION;
	float3 vTex :TEXCOORD0;
};

static const float3 offset[6] = {   
	float3(0.5, 0.5, 0.5), 
	float3(-0.5, 0.5, -0.5),   
	float3(-0.5, 0.5, -0.5), 
	float3(-0.5, -0.5, 0.5),   
	float3(-0.5, 0.5, 0.5), 
	float3(0.5, 0.5, -0.5)
}; 
static const float3 mulX[6] = {   
	float3(0, 0, -1), 
	float3(0, 0, 1), 
	float3(1, 0, 0),   
	float3(1, 0, 0), 
	float3(1, 0, 0), 
	float3(-1, 0, 0)
}; 
static const float3 mulY[6] = {   
	float3(0, -1, 0), 
	float3(0, -1, 0), 
	float3(0, 0, 1),   
	float3(0, 0, -1), 
	float3(0, -1, 0), 
	float3(0, -1, 0)
};

PS_IN main(uint posIndex :SV_VertexID)
{
	PS_IN output = (PS_IN)0;
	
	output.vPos = float4(g_fTexOffset, 0.5, 0.0, 1.0);
		
	uint3 uvCoord = uint3((posIndex / 6) % g_uSkyboxSize, (posIndex / 6) / g_uSkyboxSize, posIndex % 6);
	
	float2 vTexCoord = (float2)uvCoord.xy / float2(g_uSkyboxSize.xx);
	
	output.vTex = offset[uvCoord.z] +
		mulX[uvCoord.z] * vTexCoord.x +
		mulY[uvCoord.z] * vTexCoord.y;
	
	return(output);
}

































