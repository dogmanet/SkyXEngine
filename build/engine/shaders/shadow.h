// #ifndef __SHADOW_H
// #define __SHADOW_H

#define MAX_SHADOWS 4

SamplerComparisonState g_sComparisonLinearClamp: register(s1);

Texture2D g_atxDepthShadow2D[MAX_SHADOWS]: register(t4);
TextureCube g_atxDepthShadowCube[MAX_SHADOWS]: register(t4);

struct ShadowData
{
	float4x4 mMatrixTexture;
	float3 vPixelMapSizeBias;
};

cbuffer pssmData: register(b6)
{
	float4x4 g_mMatrixTextureV[6];
	float4x4 g_mMatrixTextureP[6];
	float4 g_vSizeBoundNearFarPSSM;
};

cbuffer perShadow: register(b7)
{
	ShadowData g_aShadowData[MAX_SHADOWS];
};

//##########################################################################

interface IShadow
{
	float genShadow(float3 vWorldPos, float3 vLightPos, uint uIdx, float fThickness);
};

//##########################################################################

class CShadowDirect: IShadow
{
	float genShadow(float3 vWorldPos, float3 vLightPos, uint uIdx, float fThickness)
	{
		float fShadow = 0.0f;
		float4 vTexCoord = mul(vWorldPos, g_aShadowData[uIdx].mMatrixTexture);
		vTexCoord.xyz = vTexCoord.xyz / vTexCoord.w;
		
		float3 vPixelMapSizeBias = g_aShadowData[uIdx].vPixelMapSizeBias;
		
		float fCompareTo = vTexCoord.z - (1.0f - fThickness) * 0.0005f;
		
		fShadow += g_atxDepthShadow2D[uIdx].SampleCmpLevelZero(g_sComparisonLinearClamp, vTexCoord.xy, fCompareTo);
		
		fShadow += g_atxDepthShadow2D[uIdx].SampleCmpLevelZero(g_sComparisonLinearClamp, vTexCoord.xy + float2(0, vPixelMapSizeBias.x * 2.0f), fCompareTo);
		fShadow += g_atxDepthShadow2D[uIdx].SampleCmpLevelZero(g_sComparisonLinearClamp, vTexCoord.xy + float2(vPixelMapSizeBias.x * 2.0f, 0), fCompareTo);
		fShadow += g_atxDepthShadow2D[uIdx].SampleCmpLevelZero(g_sComparisonLinearClamp, vTexCoord.xy + float2(vPixelMapSizeBias.x * 2.0f, vPixelMapSizeBias.x * 2.0f), fCompareTo);
#ifdef GSD_9
		fShadow += g_atxDepthShadow2D[uIdx].SampleCmpLevelZero(g_sComparisonLinearClamp, vTexCoord.xy + float2(-vPixelMapSizeBias.x * 2.0f, 0), fCompareTo);
		fShadow += g_atxDepthShadow2D[uIdx].SampleCmpLevelZero(g_sComparisonLinearClamp, vTexCoord.xy + float2(vPixelMapSizeBias.x * 2.0f, -vPixelMapSizeBias.x * 2.0f), fCompareTo);
		fShadow += g_atxDepthShadow2D[uIdx].SampleCmpLevelZero(g_sComparisonLinearClamp, vTexCoord.xy + float2(-vPixelMapSizeBias.x * 2.0f, vPixelMapSizeBias.x * 2.0f), fCompareTo);
		fShadow += g_atxDepthShadow2D[uIdx].SampleCmpLevelZero(g_sComparisonLinearClamp, vTexCoord.xy + float2(0, -vPixelMapSizeBias.x * 2.0f), fCompareTo);
		fShadow += g_atxDepthShadow2D[uIdx].SampleCmpLevelZero(g_sComparisonLinearClamp, vTexCoord.xy + float2(-vPixelMapSizeBias.x * 2.0f, -vPixelMapSizeBias.x * 2.0f), fCompareTo);

		fShadow *= 0.111;
#else
		fShadow *= 0.25;
#endif

		return(saturate(fShadow));
	}
};

//##########################################################################

class CShadowPoint: IShadow
{
	float genShadow(float3 vWorldPos, float3 vLightPos, uint uIdx, float fThickness)
	{
		float fShadow = 0.0;

		float3 vDir = vWorldPos - vLightPos;
		float fDistance = dot(vDir, vDir);
		
		vDir = normalize(vDir);
		float3 vTexUV = vDir;
		
		float fOffset = g_aShadowData[uIdx].vPixelMapSizeBias.x * 4;
		float fCompareTo = fDistance - (1.0f - fThickness) * 20.0f;

#ifdef GSC_6
		for(int x = 0; x <= 1; ++x)
		{
			for(int y = 0; y <= 1; ++y)
			{
				for(int z = 0; z <= 1; ++z)
				{
					vTexUV = vDir + float3(fOffset * x, fOffset * y, fOffset * z);
					fShadow += g_atxDepthShadowCube[uIdx].SampleCmpLevelZero(g_sComparisonLinearClamp, vTexUV, fCompareTo));
				}
			}
		}
		
		// fShadow *= 0.125;
		fShadow *= 1.0 / 6.0;
#else
		fShadow = g_atxDepthShadowCube[uIdx].SampleCmpLevelZero(g_sComparisonLinearClamp, vTexUV, fCompareTo);
#endif

		return(saturate(fShadow));
	}
};

//##########################################################################

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

#define NUM_SPLITS 6

class CShadowPSSM: IShadow
{
	float genShadow(float3 vWorldPos, float3 vLightPos, uint uIdx, float fThickness)
	{
		float fShadow = 1.0f;
		float4 vPixelPosWS = float4(vWorldPos, 1.f);
		
		float delta = 1.0f / g_vSizeBoundNearFarPSSM.x;
			
		[unroll]for(int split = 0; split < NUM_SPLITS; ++split)
		{
			float4 vPixelPosLS = mul(vPixelPosWS, g_mMatrixTextureV[split]);
			float4 vTexCoord = mul(vPixelPosLS, g_mMatrixTextureP[split]);
			
			vTexCoord.xyz = vTexCoord.xyz / vTexCoord.w;
			if(min(vTexCoord.x, vTexCoord.y) > delta && max(vTexCoord.x, vTexCoord.y) < 1.0f - delta)
			{
				float3 cubeCoord = offset[split] +
					mulX[split] * vTexCoord.x +
					mulY[split] * vTexCoord.y;
				
				
				fShadow = g_atxDepthShadowCube[uIdx].SampleCmpLevelZero(g_sComparisonLinearClamp, cubeCoord, vTexCoord.z - (1.0f - fThickness) * 0.00005f);
				break;
			}
		}
		// return(fShadow * 0.95 + 0.05);
		// return(fShadow * g_vLightPosShadow.w + (1.0f - g_vLightPosShadow.w));
		return(fShadow);
	}
};

// #endif
