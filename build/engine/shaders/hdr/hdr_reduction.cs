
#define THREAD_GROUP_DIM 16
#define TOTAL_THREADS (THREAD_GROUP_DIM * THREAD_GROUP_DIM)

Texture2D<float4> g_txInput: register(t0);

RWTexture2D<float4> g_txOutput: register(u0);

groupshared float4 g_vSharedR[TOTAL_THREADS];
groupshared float4 g_vSharedG[TOTAL_THREADS];
groupshared float4 g_vSharedB[TOTAL_THREADS];
groupshared float4 g_vSharedA[TOTAL_THREADS];

[numthreads(THREAD_GROUP_DIM, THREAD_GROUP_DIM, 1)]
void main(uint3 vGroupID: SV_GroupID, uint3 vGroupThreadID: SV_GroupThreadID)
{
	const uint uThreadID = vGroupThreadID.y * THREAD_GROUP_DIM + vGroupThreadID.x;

	const uint2 vSampleID = (vGroupID.xy * THREAD_GROUP_DIM + vGroupThreadID.xy) * 2;
	
	float4 vValR = 0.0f;
	float4 vValG = 0.0f;
	float4 vValB = 0.0f;
	float4 vValA = 0.0f;
	float4 vInput;
	
	vInput = g_txInput[vSampleID + uint2(0, 0)];
	vValR.x = vInput.x;
	vValG.x = vInput.y;
	vValB.x = vInput.z;
	vValA.x = vInput.w;
	
	vInput = g_txInput[vSampleID + uint2(1, 0)];
	vValR.y = vInput.x;
	vValG.y = vInput.y;
	vValB.y = vInput.z;
	vValA.y = vInput.w;
	
	vInput = g_txInput[vSampleID + uint2(0, 1)];
	vValR.z = vInput.x;
	vValG.z = vInput.y;
	vValB.z = vInput.z;
	vValA.z = vInput.w;
	
	vInput = g_txInput[vSampleID + uint2(1, 1)];
	vValR.w = vInput.x;
	vValG.w = vInput.y;
	vValB.w = vInput.z;
	vValA.w = vInput.w;

	g_vSharedR[uThreadID] = vValR;
	g_vSharedG[uThreadID] = vValG;
	g_vSharedB[uThreadID] = vValB;
	g_vSharedA[uThreadID] = vValA;
	
	GroupMemoryBarrierWithGroupSync();

	[unroll(TOTAL_THREADS)]
	for(uint s = TOTAL_THREADS / 2; s > 0; s >>= 1)
	{
		if(uThreadID < s)
		{
			g_vSharedR[uThreadID] = min(g_vSharedR[uThreadID], g_vSharedR[uThreadID + s]);
			g_vSharedG[uThreadID] = max(g_vSharedG[uThreadID], g_vSharedG[uThreadID + s]);
			g_vSharedB[uThreadID] += g_vSharedB[uThreadID + s];
			g_vSharedA[uThreadID] += g_vSharedA[uThreadID + s];
		}

		GroupMemoryBarrierWithGroupSync();
	}

	if(uThreadID == 0)
	{
		float4 vOutput;
		vOutput.x = min(min(g_vSharedR[0].x, g_vSharedR[0].y), min(g_vSharedR[0].z, g_vSharedR[0].w));
		vOutput.y = max(max(g_vSharedG[0].x, g_vSharedG[0].y), max(g_vSharedG[0].z, g_vSharedG[0].w));
		vOutput.z = dot(g_vSharedB[0], 1.0f);
		vOutput.w = dot(g_vSharedA[0], 1.0f);
		
		g_txOutput[vGroupID.xy] = vOutput;
		// g_txOutput[vGroupID.xy] = -10.0f;
	}
}
