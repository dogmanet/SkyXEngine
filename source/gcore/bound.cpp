
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "Bound.h"

void CreateCone(float fTopRadius, float fBottomRadius, float fHeight, IMesh ** ppMesh, IGXDevice * pDevice, UINT iSideCount)
{
	UINT iVC = iSideCount * 2;
	UINT iIC = (iSideCount - 2) * 6 + iSideCount * 6;
	float3_t * pVertices = new float3_t[iVC];
	USHORT * pIndices = new USHORT[iIC];

	//top
	UINT iCurV = 0;
	for(UINT i = 0; i < iSideCount; i++)
	{
		float A = (float)i / (float)iSideCount * SM_2PI;
		pVertices[iCurV] = float3_t(fTopRadius * cosf(A),0.0f, -fTopRadius * sinf(A));
		iCurV++;
	}

	//bottom
	for(UINT i = 0; i < iSideCount; i++)
	{
		float A = (float)i / (float)iSideCount * SM_2PI;
		pVertices[iCurV] = float3_t(fBottomRadius * cosf(A), -fHeight, fBottomRadius * sinf(A));
		iCurV++;
	}

	//top
	UINT iCurI = 0;
	for(UINT i = 0; i < iSideCount - 2; i++)
	{
		pIndices[iCurI] = 0;
		iCurI++;
		pIndices[iCurI] = 1 + i;
		iCurI++;
		pIndices[iCurI] = 2 + i;
		iCurI++;
	}

	//bottom
	for(UINT i = 0; i < iSideCount - 2; i++)
	{
		pIndices[iCurI] = iSideCount;
		iCurI++;
		pIndices[iCurI] = iSideCount + 1 + i;
		iCurI++;
		pIndices[iCurI] = iSideCount + 2 + i;
		iCurI++;
	}

	//side
	for(UINT i = 0; i < iSideCount; i++)
	{
		pIndices[iCurI] = (i + 1) % iSideCount;
		iCurI++;
		pIndices[iCurI] = i;
		iCurI++;
		pIndices[iCurI] = (iSideCount - i) % iSideCount + iSideCount;
		iCurI++;

		pIndices[iCurI] = (iSideCount - i) % iSideCount + iSideCount;
		iCurI++;
		pIndices[iCurI] = (2 * iSideCount - i - 1) % iSideCount + iSideCount;
		iCurI++;
		pIndices[iCurI] = (i + 1) % iSideCount;
		iCurI++;
	}

	IMesh *pMesh = SGCore_CrMesh(iVC, iIC, pVertices, pIndices);

	mem_delete_a(pIndices);
	mem_delete_a(pVertices);

	(*ppMesh) = pMesh;
}

void CreateSphere(float fRadius, UINT iSideCount, UINT iStackCount, IMesh ** ppMesh, IGXDevice * pDevice)
{
	UINT iVC = (iStackCount - 1) * iSideCount + 2;
	UINT iIC = (iStackCount - 1) * iSideCount * 6;
	float3_t * pVertices = new float3_t[iVC];
	USHORT * pIndices = new USHORT[iIC];
	
	UINT iCurV = 0;
	UINT iCurI = 0;
	pVertices[iCurV++] = float3_t(0.0f, fRadius, 0.0f);
	UINT uPrevBaseVtx;
	for(UINT i = 0; i < iStackCount; ++i)
	{
		float A = (float)(i + 1) / (float)iStackCount * SM_PI;
		float fCurrentRadius = sinf(A) * fRadius;
		float fCurrentY = cosf(A) * fRadius;
		UINT uBaseVtx = iCurV;
		for(UINT j = 0; j < iSideCount; ++j)
		{
			A = (float)j / (float)iSideCount * SM_2PI;
			float x = fCurrentRadius * cosf(A);
			float z = -fCurrentRadius * sinf(A);
			// north pole
			if(i == 0)
			{
				pVertices[iCurV++] = float3_t(x, fCurrentY, z);
				pIndices[iCurI++] = 0;
				pIndices[iCurI++] = uBaseVtx + j;
				pIndices[iCurI++] = uBaseVtx + (j + 1) % iSideCount;
			}
			// south pole
			else if(i == iStackCount - 1)
			{
				pIndices[iCurI++] = uPrevBaseVtx + (j + 1) % iSideCount;
				pIndices[iCurI++] = uPrevBaseVtx + j;
				pIndices[iCurI++] = uBaseVtx;
			}
			else
			{
				pVertices[iCurV++] = float3_t(x, fCurrentY, z);
				pIndices[iCurI++] = uPrevBaseVtx + j;
				pIndices[iCurI++] = uBaseVtx + (j + 1) % iSideCount;
				pIndices[iCurI++] = uPrevBaseVtx + (j + 1) % iSideCount;

				pIndices[iCurI++] = uPrevBaseVtx + j;
				pIndices[iCurI++] = uBaseVtx + j;
				pIndices[iCurI++] = uBaseVtx + (j + 1) % iSideCount;
			}
		}
		uPrevBaseVtx = uBaseVtx;
	}
	pVertices[iCurV++] = float3_t(0.0f, -fRadius, 0.0f);

	assert(iCurI == iIC);
	assert(iCurV == iVC);

	IMesh *pMesh = SGCore_CrMesh(iVC, iIC, pVertices, pIndices);

	mem_delete_a(pIndices);
	mem_delete_a(pVertices);

	(*ppMesh) = pMesh;
}

//##########################################################################

void CreateBoundingBoxMesh(const float3* min, const float3* max, IMesh** bbmesh, IGXDevice* device)
{
	float dist_x = abs(max->x - min->x);
	float dist_y = abs(max->y - min->y);
	float dist_z = abs(max->z - min->z);

	UINT iVC = 8;
	UINT iIC = 36;
	float3_t pVertices[] = {
		float3_t(min->x, min->y, min->z),
		float3_t(min->x, min->y, max->z),
		float3_t(min->x, max->y, min->z),
		float3_t(min->x, max->y, max->z),
		float3_t(max->x, min->y, min->z),
		float3_t(max->x, min->y, max->z),
		float3_t(max->x, max->y, min->z),
		float3_t(max->x, max->y, max->z),
	};
	USHORT pIndices[] = {
		0, 1, 3, 0, 3, 2,
		0, 6, 4, 0, 2, 6,
		0, 5, 1, 0, 4, 5,
		4, 7, 5, 4, 6, 7,
		6, 3, 7, 6, 2, 3,
		3, 5, 7, 3, 1, 5
	};

	IMesh *pMesh = SGCore_CrMesh(iVC, iIC, pVertices, pIndices);
	
	*bbmesh = pMesh;
}
