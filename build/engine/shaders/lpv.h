#define LPV_DIM 32
#define LPV_DIMH 16
#define LPV_CELL_SIZE 1.0
// #define LPV_POINT_COUNT 256
// #define LPV_POINT_COUNT 32
// #define LPV_MAP_SIZE 1024
// #define LPV_MAP_SIZE 32
#define KERNEL_SIZE (LPV_MAP_SIZE / LPV_POINT_COUNT)
#define LPV_POINT_WEIGHT (256.0f * 256.0f / (float)(LPV_POINT_COUNT * LPV_POINT_COUNT))
#define STEP_SIZE 1


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

float4 dirToSH(float3 dir) {
	return float4(SH_C0, -SH_C1 * dir.y, SH_C1 * dir.z, -SH_C1 * dir.x);
}

float4 dirToCosineLobe(float3 dir)
{
	//dir = normalize(dir);
	return float4(SH_cosLobe_C0, -SH_cosLobe_C1 * dir.y, SH_cosLobe_C1 * dir.z, -SH_cosLobe_C1 * dir.x);
}
