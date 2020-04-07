
#include <lpv.h>

cbuffer perLight: register(b7)
{
	half4 g_vLightColorPower;
	half4 g_vLightPosShadow;
// #ifdef IS_SPOT
	half4 g_vLightSpotDirection;
	half2 g_vLightSpotInnerOuterAngles;
// #endif
};

cbuffer b6 :register(b6)
{
	float4x4 g_mInvVP;
};

cbuffer b8: register(b8)
{
	// float4x4 mV;
	float4x4 g_mVP;
	float4 g_vPosCam;
};

int3 getGridPos(float3 worldPos)
{
	return(((worldPos - floor(g_vPosCam.xyz)) / LPV_CELL_SIZE) + int3(LPV_DIMH, LPV_DIMH, LPV_DIMH));
}

struct VS_IN
{
	uint posIndex :SV_VertexID;
};

struct GS_IN
{
	float4 cellIndex :SV_POSITION;
	float3 normal :WORLD_NORMAL;
	float3 flux :LIGHT_FLUX;
};

Texture2D rsmWsPosMap :register(t0);
Texture2D rsmWsNorMap :register(t1);
Texture2D rsmFluxMap :register(t2);

struct RsmTexel
{
	float4 flux;
	float3 normalWS;
	float3 positionWS;
};

float Luminance(RsmTexel rsmTexel)
{
	return((rsmTexel.flux.r * 0.299f + rsmTexel.flux.g * 0.587f + rsmTexel.flux.b * 0.114f) + max(0.0f, dot(rsmTexel.normalWS, -g_vLightSpotDirection.xyz)));
}

RsmTexel GetRsmTexel(int2 coords, uint2 vTexSize)
{
	RsmTexel tx = (RsmTexel)0;
	
	float4 vScreenSpace = float4(((float2)coords + 0.5f) / (float2)vTexSize * 2.0 - 1.0, rsmWsPosMap.Load(int3(coords, 0)).x, 1.0);
	// float4 vScreenSpace = float4(((float2)coords + 0.5f) / (float2)vTexSize * 2.0 - 1.0, 0.0f, 1.0);
	
	tx.flux = rsmFluxMap.Load(int3(coords, 0)) * float4(g_vLightColorPower.xyz, 1.0);
	tx.normalWS = rsmWsNorMap.Load(int3(coords, 0)).xyz * 2.0 - 1.0;
	vScreenSpace.y *= -1.0;
	
	float4 vWS = mul(vScreenSpace, g_mInvVP);
	tx.positionWS = vWS.xyz / vWS.w;
	
	half3 vLigth  = normalize(g_vLightPosShadow.xyz - tx.positionWS);
	half fNdotD = dot(-vLigth, g_vLightSpotDirection.xyz);
	tx.flux *= saturate(fNdotD - g_vLightSpotInnerOuterAngles.y) / (g_vLightSpotInnerOuterAngles.x - g_vLightSpotInnerOuterAngles.y);
	
	half fDistance = distance(tx.positionWS, g_vLightPosShadow.xyz);
	half fInvDistance = 1.f - (fDistance/g_vLightColorPower.w);
	tx.flux *= fInvDistance * fInvDistance;
	
	tx.positionWS += (tx.normalWS * POSWS_BIAS_NORMAL);
	return(tx);
}

// #define KERNEL_SIZE 4
// #define STEP_SIZE 1

GS_IN main(VS_IN input)
{	
	const uint2 RSMsize = uint2(LPV_MAP_SIZE, LPV_MAP_SIZE);
	const uint2 RSMsizeNew = RSMsize / KERNEL_SIZE;
	int3 rsmCoords = int3(input.posIndex % RSMsizeNew.x, input.posIndex / RSMsizeNew.x, 0) * KERNEL_SIZE;
		
#ifdef _DEBUG
	RsmTexel rsmTexel = GetRsmTexel(rsmCoords, RSMsize);
	GS_IN output1 = (GS_IN)0;
	// rsmTexel.positionWS = float4(((float2)rsmCoords + 0.5f) / (float2)RSMsize * 2.0 - 1.0, 0.0f, 1.0);
	// rsmTexel.positionWS = float4(float3((float)(input.posIndex % 512), 0.0f, (float)(input.posIndex / 512)) / 512, 1.0);
	output1.cellIndex = mul(float4(rsmTexel.positionWS, 1.0), g_mVP);
	return(output1);
#endif
	
	// RsmTexel rsmTexel = GetRsmTexel(rsmCoords, RSMsize);
	// GS_IN output1 = (GS_IN)0;
	// output1.cellIndex = float4(rsmTexel.positionWS, 1.0);
	// output.normal = result.normalWS;
	// output.flux = result.flux.rgb;
	// return(output1);
	
	// Pick brightest cell in KERNEL_SIZExKERNEL_SIZE grid
	float3 brightestCellIndex = 0;
	float maxLuminance = 0;
	{
		for(uint y = 0; y < KERNEL_SIZE; y += STEP_SIZE)
		{
			for(uint x = 0; x < KERNEL_SIZE; x += STEP_SIZE)
			{
				int2 texIdx = rsmCoords.xy + int2(x, y);
				RsmTexel rsmTexel = GetRsmTexel(texIdx, RSMsize);
				
				float texLum = Luminance(rsmTexel);
				if(texLum > maxLuminance)
				{
					brightestCellIndex = getGridPos(rsmTexel.positionWS);
					maxLuminance = texLum;
				}
			}
		}
	}

	RsmTexel result = (RsmTexel)0;
	float numSamples = 0;
	for(uint y = 0; y < KERNEL_SIZE; y += STEP_SIZE)
	{
		for(uint x = 0; x < KERNEL_SIZE; x += STEP_SIZE)
		{
			int2 texIdx = rsmCoords.xy + int2(x, y);
			RsmTexel rsmTexel = GetRsmTexel(texIdx, RSMsize);
			
			int3 texelIndex = getGridPos(rsmTexel.positionWS);
			float3 deltaGrid = texelIndex - brightestCellIndex;
			if(dot(deltaGrid, deltaGrid) < 10) // If cell proximity is good enough
			{
				// Sample from texel
				result.flux += rsmTexel.flux;
				result.positionWS += rsmTexel.positionWS;
				result.normalWS += rsmTexel.normalWS;
				++numSamples;
			}
		}
	}

	//if (numSamples > 0) // This is always true due to picking a brightestCell, however, not all cells have light
	//{
	result.positionWS /= numSamples;
	result.normalWS /= numSamples;
	result.normalWS = normalize(result.normalWS);
	result.flux /= numSamples;

	//RsmTexel result = GetRsmTexel(rsmCoords.xy);

	GS_IN output;
	output.cellIndex = float4(getGridPos(result.positionWS), 1.0);
	// output.cellIndex = float4(result.positionWS, 1.0);
	output.normal = result.normalWS;
	output.flux = result.flux.rgb;

	return(output);
}
