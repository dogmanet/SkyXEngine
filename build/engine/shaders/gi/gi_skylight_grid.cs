
#include <lpv.h>

Texture2D<float4> g_txMapR :register(t0);
Texture2D<float4> g_txMapG :register(t1);
Texture2D<float4> g_txMapB :register(t2);

RWTexture3D<float4> g_outR :register(u0);
RWTexture3D<float4> g_outG :register(u1);
RWTexture3D<float4> g_outB :register(u2);

cbuffer b0 :register(b0)
{
	uint g_uOcclusionMap[16][16][4 / 2];
};

[numthreads(16, 16, 1)]
void main(uint3 dispatchThreadID: SV_DispatchThreadID, uint3 groupThreadID : SV_GroupThreadID)
{
	uint3 cellIndex = dispatchThreadID.xzy;

	uint uOcclusion = g_uOcclusionMap[cellIndex.x][cellIndex.y][cellIndex.z / 2];
	
	if(cellIndex.z % 2)
	{
		uOcclusion >>= 16;
	}
	
	float4 vR = (float4)0;
	float4 vG = (float4)0;
	float4 vB = (float4)0;
	
	[unroll]
	for(uint i = 0; i < 16; ++i)
	{
	//	if(uOcclusion & (1 << i))
		{
			vR += g_txMapR[uint2(i, 0)];
			vG += g_txMapG[uint2(i, 0)];
			vB += g_txMapB[uint2(i, 0)];
		}
	}

	g_outR[cellIndex] = vR;
	g_outG[cellIndex] = vG;
	g_outB[cellIndex] = vB;
}
