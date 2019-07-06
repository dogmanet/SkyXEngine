#define LPV_DIM 32
#define LPV_DIMH 16
#define LPV_CELL_SIZE 1.0

int3 getGridPos(float3 worldPos)
{
	return((worldPos / LPV_CELL_SIZE) + int3(LPV_DIMH, LPV_DIMH, LPV_DIMH));
}

// https://github.com/mafian89/Light-Propagation-Volumes/blob/master/shaders/lightInject.frag and
// https://github.com/djbozkosz/Light-Propagation-Volumes/blob/master/data/shaders/lpvInjection.cs seem
// to use the same coefficients, which differ from the RSM paper. Due to completeness of their code, I will stick to their solutions.
/*Spherical harmonics coefficients - precomputed*/
#define SH_c0 0.282094792f // 1 / 2sqrt(pi)
#define SH_c1 0.488602512f // sqrt(3/pi) / 2

/*Cosine lobe coeff*/
#define SH_cosLobe_c0 0.886226925f // sqrt(pi)/2
#define SH_cosLobe_c1 1.02332671f // sqrt(pi/3)
#define Pi 3.1415926f

float4 dirToCosineLobe(float3 dir)
{
	//dir = normalize(dir);
	return(float4(SH_cosLobe_c0, -SH_cosLobe_c1 * dir.y, SH_cosLobe_c1 * dir.z, -SH_cosLobe_c1 * dir.x));
}

float4 dirToSH(float3 dir)
{
	return(float4(SH_c0, -SH_c1 * dir.y, SH_c1 * dir.z, -SH_c1 * dir.x));
}

// End of lpv.h

Texture3D<float4> lpvR :register(t0);
Texture3D<float4> lpvG :register(t1);
Texture3D<float4> lpvB :register(t2);

RWTexture3D<float4> lpvRW :register(u0);
RWTexture3D<float4> lpvGW :register(u1);
RWTexture3D<float4> lpvBW :register(u2);

static const float3 directions[] = {float3(0,0,1), float3(0,0,-1), float3(1,0,0), float3(-1,0,0) , float3(0,1,0), float3(0,-1,0)};

// With a lot of help from: http://blog.blackhc.net/2010/07/light-propagation-volumes/
// This is a fully functioning LPV implementation

// right up
float2 side[4] = {float2(1.0, 0.0), float2(0.0, 1.0), float2(-1.0, 0.0), float2(0.0, -1.0)};

// orientation = [ right | up | forward ] = [ x | y | z ]
float3 getEvalSideDirection(uint index, float3x3 orientation)
{
	const float smallComponent = 0.4472135; // 1 / sqrt(5)
	const float bigComponent = 0.894427; // 2 / sqrt(5)

	const float2 s = side[index];
	// *either* x = 0 or y = 0
	return(mul(orientation, float3(s.x * smallComponent, s.y * smallComponent, bigComponent)));
}

float3 getReprojSideDirection(uint index, float3x3 orientation)
{
	const float2 s = side[index];
	return(mul(orientation, float3(s.x, s.y, 0)));
}

// orientation = [ right | up | forward ] = [ x | y | z ]
float3x3 neighbourOrientations[6] = {
	// Z+
	float3x3(1, 0, 0,0, 1, 0,0, 0, 1),
	// Z-
	float3x3(-1, 0, 0,0, 1, 0,0, 0, -1),
	// X+
	float3x3(0, 0, 1,0, 1, 0,-1, 0, 0
	),
	// X-
	float3x3(0, 0, -1,0, 1, 0,1, 0, 0),
	// Y+
	float3x3(1, 0, 0,0, 0, 1,0, -1, 0),
	// Y-
	float3x3(1, 0, 0,0, 0, -1,0, 1, 0)
};

[numthreads(16, 2, 1)]
void main(uint3 dispatchThreadID: SV_DispatchThreadID, uint3 groupThreadID : SV_GroupThreadID)
{
	uint3 cellIndex = dispatchThreadID.xyz;

	// contribution
	float4 cR = (float4)0;
	float4 cG = (float4)0;
	float4 cB = (float4)0;

	for(uint neighbour = 0; neighbour < 6; ++neighbour)
	{
		float3x3 orientation = neighbourOrientations[neighbour];
		// TODO: transpose all orientation matrices and use row indexing instead? ie int3( orientation[2] )
		float3 mainDirection = mul(orientation, float3(0, 0, 1));

		uint3 neighbourIndex = cellIndex - directions[neighbour];
		float4 rCoeffsNeighbour = lpvR[neighbourIndex];
		float4 gCoeffsNeighbour = lpvG[neighbourIndex];
		float4 bCoeffsNeighbour = lpvB[neighbourIndex];

		const float directFaceSubtendedSolidAngle = 0.4006696846f / Pi / 2;
		const float sideFaceSubtendedSolidAngle = 0.4234413544f / Pi / 3;

		for (uint sideFace = 0; sideFace < 4; ++sideFace)
		{
			float3 evalDirection = getEvalSideDirection(sideFace, orientation);
			float3 reprojDirection = getReprojSideDirection(sideFace, orientation);

			float4 reprojDirectionCosineLobeSH = dirToCosineLobe(reprojDirection);
			float4 evalDirectionSH = dirToSH(evalDirection);

			cR += sideFaceSubtendedSolidAngle * dot(rCoeffsNeighbour, evalDirectionSH) * reprojDirectionCosineLobeSH;
			cG += sideFaceSubtendedSolidAngle * dot(gCoeffsNeighbour, evalDirectionSH) * reprojDirectionCosineLobeSH;
			cB += sideFaceSubtendedSolidAngle * dot(bCoeffsNeighbour, evalDirectionSH) * reprojDirectionCosineLobeSH;
		}

		float3 curDir = directions[neighbour];
		float4 curCosLobe = dirToCosineLobe(curDir);
		float4 curDirSH = dirToSH(curDir);

		int3 neighbourCellIndex = (int3)cellIndex + (int3)curDir;

		cR += directFaceSubtendedSolidAngle * max(0.0f, dot(rCoeffsNeighbour, curDirSH)) * curCosLobe;
		cG += directFaceSubtendedSolidAngle * max(0.0f, dot(gCoeffsNeighbour, curDirSH)) * curCosLobe;
		cB += directFaceSubtendedSolidAngle * max(0.0f, dot(bCoeffsNeighbour, curDirSH)) * curCosLobe;
	}

	lpvRW[dispatchThreadID.xyz] = lpvR[dispatchThreadID.xyz] + cR * 0.7;
	lpvGW[dispatchThreadID.xyz] = lpvG[dispatchThreadID.xyz] + cG * 0.7;
	lpvBW[dispatchThreadID.xyz] = lpvB[dispatchThreadID.xyz] + cB * 0.7;
}
