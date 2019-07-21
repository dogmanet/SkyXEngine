
#include "../lpv.h"

struct GS_IN
{
	float4 cellIndex :SV_POSITION;
	float3 normal :WORLD_NORMAL;
	float3 flux :LIGHT_FLUX;
};
struct PS_IN 
{
	float4 screenPos :SV_POSITION;
	float3 normal :WORLD_NORMAL;
	float3 flux :LIGHT_FLUX;
	uint depthIndex :SV_RenderTargetArrayIndex;
};

/* cbuffer test: register(b8)
{
	float4x4 g_mVP;
}; */

[maxvertexcount(1)]
void main(point GS_IN input[1], inout PointStream<PS_IN> OutputStream)
{
	PS_IN output;

	if(input[0].cellIndex.z < 0 || input[0].cellIndex.z >= LPV_DIM)
	{
		return;
	}
	output.screenPos.xy = (float2(input[0].cellIndex.xy) + 0.5) / float2(LPV_DIM, LPV_DIM) * 2.0 - 1.0;
	output.depthIndex = input[0].cellIndex.z;

	// output.depthIndex = 10;
	// invert y direction because y points downwards in the viewport?
	output.screenPos.y = -output.screenPos.y;
	output.screenPos.zw = float2(0, 1);
	
	// output.screenPos = mul(input[0].cellIndex, g_mVP);

	output.normal = input[0].normal;
	output.flux = input[0].flux;

	OutputStream.Append(output);
}
