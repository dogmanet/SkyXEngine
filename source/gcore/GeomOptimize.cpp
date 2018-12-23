
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "GeomOptimize.h"

void OptimizeIndecesInSubsetUint16(uint16_t* ib, uint16_t numFaces, uint16_t numVerts)
{
	uint16_t* pdwRemap = new uint16_t[numFaces];
	D3DXOptimizeFaces(ib, numFaces, numVerts, FALSE, (DWORD*)pdwRemap);

	uint16_t* pCopyIB = new uint16_t[numFaces * 3];
	memcpy(pCopyIB, ib, numFaces * 3 * sizeof(uint16_t));

	for (int i = 0; i<numFaces; ++i)
	{
		int newFace = (int)pdwRemap[i];
		for (int j = 0; j<3; ++j)
		{
			ib[i * 3 + j] = pCopyIB[newFace * 3 + j];
		}
	}

	mem_delete_a(pCopyIB);
	mem_delete_a(pdwRemap);
}

void OptimizeIndecesInSubsetUint32(uint32_t* ib, uint32_t numFaces, uint32_t numVerts)
{
	uint32_t* pdwRemap = new uint32_t[numFaces];
	D3DXOptimizeFaces(ib, numFaces, numVerts, TRUE, (DWORD*)pdwRemap);

	uint32_t* pCopyIB = new uint32_t[numFaces * 3];
	memcpy(pCopyIB, ib, numFaces * 3 * sizeof(uint32_t));

	for (int i = 0; i<numFaces; ++i)
	{
		int newFace = (int)pdwRemap[i];
		for (int j = 0; j<3; ++j)
		{
			ib[i * 3 + j] = pCopyIB[newFace * 3 + j];
		}
	}

	mem_delete_a(pCopyIB);
	mem_delete_a(pdwRemap);
}