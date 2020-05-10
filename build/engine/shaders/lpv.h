#define LPV_DIM 32
#define LPV_DIMH 16
// #define LPV_CELL_SIZE 1.0
// #define LPV_POINT_COUNT 256
// #define LPV_POINT_COUNT 32
// #define LPV_MAP_SIZE 1024
// #define LPV_MAP_SIZE 32
#define KERNEL_SIZE (LPV_MAP_SIZE / LPV_POINT_COUNT)
#ifdef IS_SUN
#define LPV_POINT_WEIGHT (256.0f * 256.0f * 2.0f / (float)(LPV_POINT_COUNT * LPV_POINT_COUNT))
#else
// #define LPV_POINT_WEIGHT (256.0f / (float)(LPV_POINT_COUNT * LPV_POINT_COUNT))
#define LPV_POINT_WEIGHT (1.0f / LPV_POINT_COUNT)
#endif

// #ifdef IS_SUN
// #define LPV_POINT_WEIGHT (256.0f * 256.0f * 10.0f / (float)(LPV_POINT_COUNT * LPV_POINT_COUNT))
// #else
// #define LPV_POINT_WEIGHT (256.0f * 256.0f / (float)(LPV_POINT_COUNT * LPV_POINT_COUNT))
// #endif

#define STEP_SIZE 1



cbuffer b9: register(b9)
{
	float4 g_vCenterSize[3];
};

cbuffer b10: register(b10)
{
	float4 g_vCurrentCascade;
};

// https://github.com/mafian89/Light-Propagation-Volumes/blob/master/shaders/lightInject.frag and
// https://github.com/djbozkosz/Light-Propagation-Volumes/blob/master/data/shaders/lpvInjection.cs seem
// to use the same coefficients, which differ from the RSM paper. Due to completeness of their code, I will stick to their solutions.
/*Spherical harmonics coefficients – precomputed*/
#define SH_C0 0.282094792f /* 1 / 2sqrt(pi) */
#define SH_C1 0.488602512f /* sqrt(3/pi) / 2 */

/*Cosine lobe coeff*/
#define SH_cosLobe_C0 0.886226925f /* sqrt(pi)/2 */
#define SH_cosLobe_C1 1.02332671f /* sqrt(pi/3) */
#define PI 3.1415926f

// #define POSWS_BIAS_NORMAL 2.0
#define POSWS_BIAS_NORMAL 0.2
#define POSWS_BIAS_LIGHT 1.0


#ifndef NO_LPV_TEXTURES
Texture3D g_txLightVolumeRed[3]:register(t4);
Texture3D g_txLightVolumeGreen[3]:register(t7);
Texture3D g_txLightVolumeBlue[3]:register(t10);
SamplerState g_sLinearClamp: register(s1);
#endif

float4 dirToSH(float3 dir)
{
	return float4(SH_C0, -SH_C1 * dir.y, SH_C1 * dir.z, -SH_C1 * dir.x);
}

float4 dirToCosineLobe(float3 dir)
{
	//dir = normalize(dir);
	return float4(SH_cosLobe_C0, -SH_cosLobe_C1 * dir.y, SH_cosLobe_C1 * dir.z, -SH_cosLobe_C1 * dir.x);
}

float3 GetGridCenter(uint uCascade)
{
	// return(floor(g_vCenterSize[uCascade].xyz) * g_vCenterSize[uCascade].w);
	return(g_vCenterSize[uCascade].xyz - g_vCenterSize[uCascade].xyz % g_vCenterSize[uCascade].w);
}

float GetGridWorldSize(uint uCascade)
{
	return(g_vCenterSize[uCascade].w);
}

int3 getGridPos(float3 worldPos, uint uCascade)
{
	return((worldPos - GetGridCenter(uCascade)) / GetGridWorldSize(uCascade) + int3(LPV_DIMH, LPV_DIMH, LPV_DIMH));
}

float3 GetGridTexCoord(float3 worldPos, uint uCascade)
{
	return((worldPos - GetGridCenter(uCascade)) / (GetGridWorldSize(uCascade) * LPV_DIM) + 0.5f + 1.0f / (LPV_DIM * 2.0f));
}

float3 GetPixelLight(float3 vPosition, float3 vNormalPixel)
{
	// float3 vNormalPixel = normalize(IN.vNormal);
	float4 SHintensity = dirToSH(-vNormalPixel);
	

	float4 vColorR = (float4)0;
    float4 vColorG = (float4)0;
    float4 vColorB = (float4)0;
	[unroll]for(uint i = 0; i < 3; ++i)
	{
		float3 vTexCoord = GetGridTexCoord(vPosition, i);
		vColorR += g_txLightVolumeRed[i].Sample(g_sLinearClamp, vTexCoord);
		vColorG += g_txLightVolumeGreen[i].Sample(g_sLinearClamp, vTexCoord);
		vColorB += g_txLightVolumeBlue[i].Sample(g_sLinearClamp, vTexCoord);
	}
	
	float r = max(0.0f, dot(SHintensity, vColorR));
	float g = max(0.0f, dot(SHintensity, vColorG));
	float b = max(0.0f, dot(SHintensity, vColorB));
	
	return(float3(r, g, b) / PI/*  * 6.0f */);
}





