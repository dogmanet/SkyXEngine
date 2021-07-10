
#include "SolidBlock.h"

#include "defines.h"

//#define cos30 0.86602540378443864676372317075294
//#define cos30x2 1.7320508075688772935274463415059
//#define cos30div2 0.43301270189221932338186158537647

enum HC_NBM
{
	HC_NBM_0 = 0x01,
	HC_NBM_1 = 0x02,
	HC_NBM_2 = 0x04,
	HC_NBM_3 = 0x08,
	HC_NBM_4 = 0x10,
	HC_NBM_5 = 0x20,
	HC_NBM_6 = 0x40,
};

float3_t g_fNormals[128] = {
	float3_t(0.000000f, 0.000000f, 0.000000f),
	float3_t(0.433008f, 0.499999f, 0.750003f),
	float3_t(0.433008f, 0.499999f, -0.750003f),
	float3_t(0.654651f, 0.755931f, 0.000000f),
	float3_t(-0.866024f, 0.500003f, 0.000000f),
	float3_t(-0.327324f, 0.755928f, 0.566950f),
	float3_t(-0.327324f, 0.755928f, -0.566950f),
	float3_t(0.000000f, 1.000000f, 0.000000f),
	float3_t(0.433008f, -0.499999f, 0.750003f),
	float3_t(0.499994f, 0.000000f, 0.866029f),
	float3_t(1.000000f, 0.000000f, 0.000000f),
	float3_t(0.821581f, 0.316229f, 0.474347f),
	float3_t(-0.499994f, 0.000000f, 0.866029f),
	float3_t(0.000000f, 0.316226f, 0.948684f),
	float3_t(0.000000f, 1.000000f, 0.000000f),
	float3_t(0.327324f, 0.755928f, 0.566950f),
	float3_t(0.433008f, -0.499999f, -0.750003f),
	float3_t(1.000000f, 0.000000f, 0.000000f),
	float3_t(0.499994f, 0.000000f, -0.866029f),
	float3_t(0.821581f, 0.316229f, -0.474347f),
	float3_t(-0.499994f, 0.000000f, -0.866029f),
	float3_t(0.000000f, 1.000000f, 0.000000f),
	float3_t(0.000000f, 0.316226f, -0.948684f),
	float3_t(0.327324f, 0.755928f, -0.566950f),
	float3_t(0.654651f, -0.755931f, 0.000000f),
	float3_t(0.821581f, -0.316229f, 0.474347f),
	float3_t(0.821581f, -0.316229f, -0.474347f),
	float3_t(1.000000f, 0.000000f, 0.000000f),
	float3_t(0.000000f, -1.000000f, 0.000000f),
	float3_t(0.499994f, 0.000000f, 0.866029f),
	float3_t(0.499994f, 0.000000f, -0.866029f),
	float3_t(0.866024f, 0.500003f, 0.000000f),
	float3_t(-0.866024f, -0.500003f, 0.000000f),
	float3_t(-0.499994f, 0.000000f, 0.866029f),
	float3_t(-0.499994f, 0.000000f, -0.866029f),
	float3_t(0.000000f, 1.000000f, 0.000000f),
	float3_t(-1.000000f, 0.000000f, 0.000000f),
	float3_t(-0.821581f, 0.316229f, 0.474347f),
	float3_t(-0.821581f, 0.316229f, -0.474347f),
	float3_t(-0.654651f, 0.755931f, 0.000000f),
	float3_t(-0.327324f, -0.755928f, 0.566950f),
	float3_t(0.000000f, -0.316226f, 0.948684f),
	float3_t(0.000000f, -1.000000f, 0.000000f),
	float3_t(0.499994f, 0.000000f, 0.866029f),
	float3_t(-0.821581f, -0.316229f, 0.474347f),
	float3_t(-0.499994f, 0.000000f, 0.866029f),
	float3_t(-1.000000f, 0.000000f, 0.000000f),
	float3_t(-0.433008f, 0.499999f, 0.750003f),
	float3_t(-0.327324f, -0.755928f, -0.566950f),
	float3_t(0.000000f, -1.000000f, 0.000000f),
	float3_t(0.000000f, -0.316226f, -0.948684f),
	float3_t(0.499994f, 0.000000f, -0.866029f),
	float3_t(-0.821581f, -0.316229f, -0.474347f),
	float3_t(-1.000000f, 0.000000f, 0.000000f),
	float3_t(-0.499994f, 0.000000f, -0.866029f),
	float3_t(-0.433008f, 0.499999f, -0.750003f),
	float3_t(0.000000f, -1.000000f, 0.000000f),
	float3_t(0.327324f, -0.755928f, 0.566950f),
	float3_t(0.327324f, -0.755928f, -0.566950f),
	float3_t(0.866024f, -0.500003f, 0.000000f),
	float3_t(-0.654651f, -0.755931f, 0.000000f),
	float3_t(-0.433008f, -0.499999f, 0.750003f),
	float3_t(-0.433008f, -0.499999f, -0.750003f),
	float3_t(0.000000f, 0.000000f, 0.000000f),
	float3_t(0.000000f, 0.000000f, 0.000000f),
	float3_t(-0.433008f, 0.499999f, 0.750003f),
	float3_t(-0.433008f, 0.499999f, -0.750003f),
	float3_t(-0.654651f, 0.755931f, 0.000000f),
	float3_t(0.866024f, 0.500003f, 0.000000f),
	float3_t(0.327324f, 0.755928f, 0.566950f),
	float3_t(0.327324f, 0.755928f, -0.566950f),
	float3_t(0.000000f, 1.000000f, 0.000000f),
	float3_t(-0.433008f, -0.499999f, 0.750003f),
	float3_t(-0.499994f, 0.000000f, 0.866029f),
	float3_t(-1.000000f, 0.000000f, 0.000000f),
	float3_t(-0.821581f, 0.316229f, 0.474347f),
	float3_t(0.499994f, 0.000000f, 0.866029f),
	float3_t(0.000000f, 0.316226f, 0.948684f),
	float3_t(0.000000f, 1.000000f, 0.000000f),
	float3_t(-0.327324f, 0.755928f, 0.566950f),
	float3_t(-0.433008f, -0.499999f, -0.750003f),
	float3_t(-1.000000f, 0.000000f, 0.000000f),
	float3_t(-0.499994f, 0.000000f, -0.866029f),
	float3_t(-0.821581f, 0.316229f, -0.474347f),
	float3_t(0.499994f, 0.000000f, -0.866029f),
	float3_t(0.000000f, 1.000000f, 0.000000f),
	float3_t(0.000000f, 0.316226f, -0.948684f),
	float3_t(-0.327324f, 0.755928f, -0.566950f),
	float3_t(-0.654651f, -0.755931f, 0.000000f),
	float3_t(-0.821581f, -0.316229f, 0.474347f),
	float3_t(-0.821581f, -0.316229f, -0.474347f),
	float3_t(-1.000000f, 0.000000f, 0.000000f),
	float3_t(0.000000f, -1.000000f, 0.000000f),
	float3_t(-0.499994f, 0.000000f, 0.866029f),
	float3_t(-0.499994f, 0.000000f, -0.866029f),
	float3_t(-0.866024f, 0.500003f, 0.000000f),
	float3_t(0.866024f, -0.500003f, 0.000000f),
	float3_t(0.499994f, 0.000000f, 0.866029f),
	float3_t(0.499994f, 0.000000f, -0.866029f),
	float3_t(0.000000f, 1.000000f, 0.000000f),
	float3_t(1.000000f, 0.000000f, 0.000000f),
	float3_t(0.821581f, 0.316229f, 0.474347f),
	float3_t(0.821581f, 0.316229f, -0.474347f),
	float3_t(0.654651f, 0.755931f, 0.000000f),
	float3_t(0.327324f, -0.755928f, 0.566950f),
	float3_t(0.000000f, -0.316226f, 0.948684f),
	float3_t(0.000000f, -1.000000f, 0.000000f),
	float3_t(-0.499994f, 0.000000f, 0.866029f),
	float3_t(0.821581f, -0.316229f, 0.474347f),
	float3_t(0.499994f, 0.000000f, 0.866029f),
	float3_t(1.000000f, 0.000000f, 0.000000f),
	float3_t(0.433008f, 0.499999f, 0.750003f),
	float3_t(0.327324f, -0.755928f, -0.566950f),
	float3_t(0.000000f, -1.000000f, 0.000000f),
	float3_t(0.000000f, -0.316226f, -0.948684f),
	float3_t(-0.499994f, 0.000000f, -0.866029f),
	float3_t(0.821581f, -0.316229f, -0.474347f),
	float3_t(1.000000f, 0.000000f, 0.000000f),
	float3_t(0.499994f, 0.000000f, -0.866029f),
	float3_t(0.433008f, 0.499999f, -0.750003f),
	float3_t(0.000000f, -1.000000f, 0.000000f),
	float3_t(-0.327324f, -0.755928f, 0.566950f),
	float3_t(-0.327324f, -0.755928f, -0.566950f),
	float3_t(-0.866024f, -0.500003f, 0.000000f),
	float3_t(0.654651f, -0.755931f, 0.000000f),
	float3_t(0.433008f, -0.499999f, 0.750003f),
	float3_t(0.433008f, -0.499999f, -0.750003f),
	float3_t(0.000000f, 0.000000f, 0.000000f)
};

void CSolidBlock::advance(float fTime)
{

}

void CSolidBlock::CreateMesh(UINT blockID, UINT iTexture, float fXpos, float fYpos, float fZpos, float R, float H, XResourceModelStaticVertex *pVertices, UINT * pIndices, UINT * pCurVertex, UINT * pCurIndex, bool * b, WORD * pBlocks)
{
	UINT type = HC_GET_TYPE(blockID);
	UINT bitmask = 0;
	if(type)
	{
		if(HC_BIT_SET(type, HC_BLOCK_TYPE_FULL))
		{
			if(iTexture == 0)
			{
				if(b[6]) // top
				{
					if(pIndices)
					{
						pIndices[*pCurIndex] = 0 + *pCurVertex;
						(*pCurIndex)++;
						pIndices[*pCurIndex] = 1 + *pCurVertex;
						(*pCurIndex)++;
						pIndices[*pCurIndex] = 2 + *pCurVertex;
						(*pCurIndex)++;

						pIndices[*pCurIndex] = 0 + *pCurVertex;
						(*pCurIndex)++;
						pIndices[*pCurIndex] = 3 + *pCurVertex;
						(*pCurIndex)++;
						pIndices[*pCurIndex] = 1 + *pCurVertex;
						(*pCurIndex)++;

						pIndices[*pCurIndex] = 1 + *pCurVertex;
						(*pCurIndex)++;
						pIndices[*pCurIndex] = 4 + *pCurVertex;
						(*pCurIndex)++;
						pIndices[*pCurIndex] = 2 + *pCurVertex;
						(*pCurIndex)++;

						pIndices[*pCurIndex] = 2 + *pCurVertex;
						(*pCurIndex)++;
						pIndices[*pCurIndex] = 5 + *pCurVertex;
						(*pCurIndex)++;
						pIndices[*pCurIndex] = 0 + *pCurVertex;
						(*pCurIndex)++;
					}
					else
					{
						(*pCurIndex) += 12;
					}

					//UINT icv = *pCurVertex;

					/*bitmask = HC_NBM_2;
					if(pBlocks[1 + 7] && HC_BIT_SET(pBlocks[8]->GetType(), HC_BLOCK_TYPE_FULL))
					{
						bitmask = HC_NBM_1;
					}
					if(pBlocks[1 + 7 + 7] && HC_BIT_SET(pBlocks[8]->GetType(), HC_BLOCK_TYPE_FULL))
					{
						bitmask = HC_NBM_4;
					}
					if(pBlocks[6 + 7] && HC_BIT_SET(pBlocks[8]->GetType(), HC_BLOCK_TYPE_FULL))
					{
						bitmask = HC_NBM_0;
					}
					if(pBlocks[6 + 7 + 7] && HC_BIT_SET(pBlocks[8]->GetType(), HC_BLOCK_TYPE_FULL))
					{
						bitmask = HC_NBM_3;
					}
					if(pBlocks[0 + 7] && HC_BIT_SET(pBlocks[8]->GetType(), HC_BLOCK_TYPE_FULL))
					{
						bitmask = HC_NBM_5;
					}*/

					if(pVertices)
					{
						pVertices[*pCurVertex].vPos = float3_t(fXpos - R, fYpos + H, fZpos);
						pVertices[*pCurVertex].vTex = float2_t(0.5f, 0.0f);
						pVertices[*pCurVertex].vNorm = float3_t(0.0f, 1.0f, 0.0f);
						pVertices[*pCurVertex].vTangent = float3_t(0.0f, 0.0f, 1.0f);
						pVertices[*pCurVertex].vBinorm = float3_t(1.0f, 0.0f, 0.0f);
						//pVertices[*pCurVertex].Norm = g_fNormals[bitmask];
						(*pCurVertex)++;
						pVertices[*pCurVertex].vPos = float3_t(fXpos + Rdiv2, fYpos + H, fZpos + Rcos30);
						pVertices[*pCurVertex].vTex = float2_t(0.5f + cos30div2, 0.75f);
						pVertices[*pCurVertex].vNorm = float3_t(0.0f, 1.0f, 0.0f);
						pVertices[*pCurVertex].vTangent = float3_t(0.0f, 0.0f, 1.0f);
						pVertices[*pCurVertex].vBinorm = float3_t(1.0f, 0.0f, 0.0f);
						(*pCurVertex)++;
						pVertices[*pCurVertex].vPos = float3_t(fXpos + Rdiv2, fYpos + H, fZpos - Rcos30);
						pVertices[*pCurVertex].vTex = float2_t(0.5f - cos30div2, 0.75f);
						pVertices[*pCurVertex].vNorm = float3_t(0.0f, 1.0f, 0.0f);
						pVertices[*pCurVertex].vTangent = float3_t(0.0f, 0.0f, 1.0f);
						pVertices[*pCurVertex].vBinorm = float3_t(1.0f, 0.0f, 0.0f);
						(*pCurVertex)++;
						pVertices[*pCurVertex].vPos = float3_t(fXpos - Rdiv2, fYpos + H, fZpos + Rcos30);
						pVertices[*pCurVertex].vTex = float2_t(0.5f + cos30div2, 0.25f);
						pVertices[*pCurVertex].vNorm = float3_t(0.0f, 1.0f, 0.0f);
						pVertices[*pCurVertex].vTangent = float3_t(0.0f, 0.0f, 1.0f);
						pVertices[*pCurVertex].vBinorm = float3_t(1.0f, 0.0f, 0.0f);
						(*pCurVertex)++;
						pVertices[*pCurVertex].vPos = float3_t(fXpos + R, fYpos + H, fZpos);
						pVertices[*pCurVertex].vTex = float2_t(0.5f, 1.0f);
						pVertices[*pCurVertex].vNorm = float3_t(0.0f, 1.0f, 0.0f);
						pVertices[*pCurVertex].vTangent = float3_t(0.0f, 0.0f, 1.0f);
						pVertices[*pCurVertex].vBinorm = float3_t(1.0f, 0.0f, 0.0f);
						(*pCurVertex)++;
						pVertices[*pCurVertex].vPos = float3_t(fXpos - Rdiv2, fYpos + H, fZpos - Rcos30);
						pVertices[*pCurVertex].vTex = float2_t(0.5f - cos30div2, 0.25f);
						pVertices[*pCurVertex].vNorm = float3_t(0.0f, 1.0f, 0.0f);
						pVertices[*pCurVertex].vTangent = float3_t(0.0f, 0.0f, 1.0f);
						pVertices[*pCurVertex].vBinorm = float3_t(1.0f, 0.0f, 0.0f);
						(*pCurVertex)++;
					}
					else
					{
						(*pCurVertex) += 6;
					}
				}
				if(b[7]) // bottom
				{
					if(pIndices)
					{
						pIndices[*pCurIndex] = 2 + *pCurVertex;
						(*pCurIndex)++;
						pIndices[*pCurIndex] = 1 + *pCurVertex;
						(*pCurIndex)++;
						pIndices[*pCurIndex] = 0 + *pCurVertex;
						(*pCurIndex)++;

						pIndices[*pCurIndex] = 1 + *pCurVertex;
						(*pCurIndex)++;
						pIndices[*pCurIndex] = 3 + *pCurVertex;
						(*pCurIndex)++;
						pIndices[*pCurIndex] = 0 + *pCurVertex;
						(*pCurIndex)++;

						pIndices[*pCurIndex] = 2 + *pCurVertex;
						(*pCurIndex)++;
						pIndices[*pCurIndex] = 4 + *pCurVertex;
						(*pCurIndex)++;
						pIndices[*pCurIndex] = 1 + *pCurVertex;
						(*pCurIndex)++;

						pIndices[*pCurIndex] = 0 + *pCurVertex;
						(*pCurIndex)++;
						pIndices[*pCurIndex] = 5 + *pCurVertex;
						(*pCurIndex)++;
						pIndices[*pCurIndex] = 2 + *pCurVertex;
						(*pCurIndex)++;
					}
					else
					{
						(*pCurIndex) += 12;
					}

					//UINT icv = *pCurVertex;


					if(pVertices)
					{
						pVertices[*pCurVertex].vPos = float3_t(fXpos - R, fYpos, fZpos);
						pVertices[*pCurVertex].vTex = float2_t(0.5f, 0.0f);
						pVertices[*pCurVertex].vNorm = float3_t(0.0f, -1.0f, 0.0f);
						pVertices[*pCurVertex].vTangent = float3_t(0.0f, 0.0f, 1.0f);
						pVertices[*pCurVertex].vBinorm = float3_t(1.0f, 0.0f, 0.0f);
						(*pCurVertex)++;
						pVertices[*pCurVertex].vPos = float3_t(fXpos + Rdiv2, fYpos, fZpos + Rcos30);
						pVertices[*pCurVertex].vTex = float2_t(0.5f + cos30div2, 0.75f);
						pVertices[*pCurVertex].vNorm = float3_t(0.0f, -1.0f, 0.0f);
						pVertices[*pCurVertex].vTangent = float3_t(0.0f, 0.0f, 1.0f);
						pVertices[*pCurVertex].vBinorm = float3_t(1.0f, 0.0f, 0.0f);
						(*pCurVertex)++;
						pVertices[*pCurVertex].vPos = float3_t(fXpos + Rdiv2, fYpos, fZpos - Rcos30);
						pVertices[*pCurVertex].vTex = float2_t(0.5f - cos30div2, 0.75f);
						pVertices[*pCurVertex].vNorm = float3_t(0.0f, -1.0f, 0.0f);
						pVertices[*pCurVertex].vTangent = float3_t(0.0f, 0.0f, 1.0f);
						pVertices[*pCurVertex].vBinorm = float3_t(1.0f, 0.0f, 0.0f);
						(*pCurVertex)++;
						pVertices[*pCurVertex].vPos = float3_t(fXpos - Rdiv2, fYpos, fZpos + Rcos30);
						pVertices[*pCurVertex].vTex = float2_t(0.5f + cos30div2, 0.25f);
						pVertices[*pCurVertex].vNorm = float3_t(0.0f, -1.0f, 0.0f);
						pVertices[*pCurVertex].vTangent = float3_t(0.0f, 0.0f, 1.0f);
						pVertices[*pCurVertex].vBinorm = float3_t(1.0f, 0.0f, 0.0f);
						(*pCurVertex)++;
						pVertices[*pCurVertex].vPos = float3_t(fXpos + R, fYpos, fZpos);
						pVertices[*pCurVertex].vTex = float2_t(0.5f, 1.0f);
						pVertices[*pCurVertex].vNorm = float3_t(0.0f, -1.0f, 0.0f);
						pVertices[*pCurVertex].vTangent = float3_t(0.0f, 0.0f, 1.0f);
						pVertices[*pCurVertex].vBinorm = float3_t(1.0f, 0.0f, 0.0f);
						(*pCurVertex)++;
						pVertices[*pCurVertex].vPos = float3_t(fXpos - Rdiv2, fYpos, fZpos - Rcos30);
						pVertices[*pCurVertex].vTex = float2_t(0.5f - cos30div2, 0.25f);
						pVertices[*pCurVertex].vNorm = float3_t(0.0f, -1.0f, 0.0f);
						pVertices[*pCurVertex].vTangent = float3_t(0.0f, 0.0f, 1.0f);
						pVertices[*pCurVertex].vBinorm = float3_t(1.0f, 0.0f, 0.0f);
						(*pCurVertex)++;
					}
					else
					{
						(*pCurVertex) += 6;
					}
				}
			}
			if(iTexture == 1)
			{
				if(b[2])
				{
					if(pIndices)
					{
						pIndices[*pCurIndex] = 0 + *pCurVertex;
						(*pCurIndex)++;
						pIndices[*pCurIndex] = 1 + *pCurVertex;
						(*pCurIndex)++;
						pIndices[*pCurIndex] = 2 + *pCurVertex;
						(*pCurIndex)++;

						pIndices[*pCurIndex] = 2 + *pCurVertex;
						(*pCurIndex)++;
						pIndices[*pCurIndex] = 1 + *pCurVertex;
						(*pCurIndex)++;
						pIndices[*pCurIndex] = 3 + *pCurVertex;
						(*pCurIndex)++;
					}
					else
					{
						(*pCurIndex) += 6;
					}


					if(pVertices)
					{
						pVertices[*pCurVertex].vPos = float3_t(fXpos - Rdiv2, fYpos + H, fZpos + Rcos30);
						pVertices[*pCurVertex].vTex = float2_t(0.0f, 0.0f);
						pVertices[*pCurVertex].vNorm = float3_t(-cos30, 0.0f, 0.5f);
						pVertices[*pCurVertex].vTangent = float3_t(-0.5f, 0.0f, -cos30);
						pVertices[*pCurVertex].vBinorm = float3_t(0.0f, -1.0f, 0.0f);
						(*pCurVertex)++;
						pVertices[*pCurVertex].vPos = float3_t(fXpos - R, fYpos + H, fZpos);
						pVertices[*pCurVertex].vNorm = float3_t(-cos30, 0.0f, 0.5f);
						pVertices[*pCurVertex].vTex = float2_t(1.0f, 0.0f);
						pVertices[*pCurVertex].vTangent = float3_t(-0.5f, 0.0f, -cos30);
						pVertices[*pCurVertex].vBinorm = float3_t(0.0f, -1.0f, 0.0f);
						(*pCurVertex)++;
						pVertices[*pCurVertex].vPos = float3_t(fXpos - Rdiv2, fYpos, fZpos + Rcos30);
						pVertices[*pCurVertex].vNorm = float3_t(-cos30, 0.0f, 0.5f);
						pVertices[*pCurVertex].vTex = float2_t(0.0f, 1.0f);
						pVertices[*pCurVertex].vTangent = float3_t(-0.5f, 0.0f, -cos30);
						pVertices[*pCurVertex].vBinorm = float3_t(0.0f, -1.0f, 0.0f);
						(*pCurVertex)++;
						pVertices[*pCurVertex].vPos = float3_t(fXpos - R, fYpos, fZpos);
						pVertices[*pCurVertex].vNorm = float3_t(-cos30, 0.0f, 0.5f);
						pVertices[*pCurVertex].vTex = float2_t(1.0f, 1.0f);
						pVertices[*pCurVertex].vTangent = float3_t(-0.5f, 0.0f, -cos30);
						pVertices[*pCurVertex].vBinorm = float3_t(0.0f, -1.0f, 0.0f);
						(*pCurVertex)++;
					}
					else
					{
						(*pCurVertex) += 4;
					}
				}

				if(b[3])
				{
					if(pIndices)
					{
						pIndices[*pCurIndex] = 0 + *pCurVertex;
						(*pCurIndex)++;
						pIndices[*pCurIndex] = 1 + *pCurVertex;
						(*pCurIndex)++;
						pIndices[*pCurIndex] = 2 + *pCurVertex;
						(*pCurIndex)++;

						pIndices[*pCurIndex] = 2 + *pCurVertex;
						(*pCurIndex)++;
						pIndices[*pCurIndex] = 1 + *pCurVertex;
						(*pCurIndex)++;
						pIndices[*pCurIndex] = 3 + *pCurVertex;
						(*pCurIndex)++;
					}
					else
					{
						(*pCurIndex) += 6;
					}

					if(pVertices)
					{
						pVertices[*pCurVertex].vPos = float3_t(fXpos + Rdiv2, fYpos + H, fZpos + Rcos30);
						pVertices[*pCurVertex].vNorm = float3_t(0.0f, 0.0f, 1.0f);
						pVertices[*pCurVertex].vTex = float2_t(0.0f, 0.0f);
						pVertices[*pCurVertex].vTangent = float3_t(-1.0f, 0.0f, 0.0f);
						pVertices[*pCurVertex].vBinorm = float3_t(0.0f, -1.0f, 0.0f);
						(*pCurVertex)++;
						pVertices[*pCurVertex].vPos = float3_t(fXpos - Rdiv2, fYpos + H, fZpos + Rcos30);
						pVertices[*pCurVertex].vNorm = float3_t(0.0f, 0.0f, 1.0f);
						pVertices[*pCurVertex].vTex = float2_t(1.0f, 0.0f);
						pVertices[*pCurVertex].vTangent = float3_t(-1.0f, 0.0f, 0.0f);
						pVertices[*pCurVertex].vBinorm = float3_t(0.0f, -1.0f, 0.0f);
						(*pCurVertex)++;
						pVertices[*pCurVertex].vPos = float3_t(fXpos + Rdiv2, fYpos, fZpos + Rcos30);
						pVertices[*pCurVertex].vNorm = float3_t(0.0f, 0.0f, 1.0f);
						pVertices[*pCurVertex].vTex = float2_t(0.0f, 1.0f);
						pVertices[*pCurVertex].vTangent = float3_t(-1.0f, 0.0f, 0.0f);
						pVertices[*pCurVertex].vBinorm = float3_t(0.0f, -1.0f, 0.0f);
						(*pCurVertex)++;
						pVertices[*pCurVertex].vPos = float3_t(fXpos - Rdiv2, fYpos, fZpos + Rcos30);
						pVertices[*pCurVertex].vNorm = float3_t(0.0f, 0.0f, 1.0f);
						pVertices[*pCurVertex].vTex = float2_t(1.0f, 1.0f);
						pVertices[*pCurVertex].vTangent = float3_t(-1.0f, 0.0f, 0.0f);
						pVertices[*pCurVertex].vBinorm = float3_t(0.0f, -1.0f, 0.0f);
						(*pCurVertex)++;
					}
					else
					{
						(*pCurVertex) += 4;
					}
				}

				if(b[4])
				{
					if(pIndices)
					{
						pIndices[*pCurIndex] = 0 + *pCurVertex;
						(*pCurIndex)++;
						pIndices[*pCurIndex] = 1 + *pCurVertex;
						(*pCurIndex)++;
						pIndices[*pCurIndex] = 2 + *pCurVertex;
						(*pCurIndex)++;

						pIndices[*pCurIndex] = 2 + *pCurVertex;
						(*pCurIndex)++;
						pIndices[*pCurIndex] = 1 + *pCurVertex;
						(*pCurIndex)++;
						pIndices[*pCurIndex] = 3 + *pCurVertex;
						(*pCurIndex)++;
					}
					else
					{
						(*pCurIndex) += 6;
					}

					if(pVertices)
					{
						pVertices[*pCurVertex].vPos = float3_t(fXpos + R, fYpos + H, fZpos);
						pVertices[*pCurVertex].vNorm = float3_t(cos30, 0.0f, 0.5f);
						pVertices[*pCurVertex].vTex = float2_t(0.0f, 0.0f);
						pVertices[*pCurVertex].vTangent = float3_t(-0.5f, 0.0f, cos30);
						pVertices[*pCurVertex].vBinorm = float3_t(0.0f, -1.0f, 0.0f);
						(*pCurVertex)++;
						pVertices[*pCurVertex].vPos = float3_t(fXpos + Rdiv2, fYpos + H, fZpos + Rcos30);
						pVertices[*pCurVertex].vNorm = float3_t(cos30, 0.0f, 0.5f);
						pVertices[*pCurVertex].vTex = float2_t(1.0f, 0.0f);
						pVertices[*pCurVertex].vTangent = float3_t(-0.5f, 0.0f, cos30);
						pVertices[*pCurVertex].vBinorm = float3_t(0.0f, -1.0f, 0.0f);
						(*pCurVertex)++;
						pVertices[*pCurVertex].vPos = float3_t(fXpos + R, fYpos, fZpos);
						pVertices[*pCurVertex].vNorm = float3_t(cos30, 0.0f, 0.5f);
						pVertices[*pCurVertex].vTex = float2_t(0.0f, 1.0f);
						pVertices[*pCurVertex].vTangent = float3_t(-0.5f, 0.0f, cos30);
						pVertices[*pCurVertex].vBinorm = float3_t(0.0f, -1.0f, 0.0f);
						(*pCurVertex)++;
						pVertices[*pCurVertex].vPos = float3_t(fXpos + Rdiv2, fYpos, fZpos + Rcos30);
						pVertices[*pCurVertex].vNorm = float3_t(cos30, 0.0f, 0.5f);
						pVertices[*pCurVertex].vTex = float2_t(1.0f, 1.0f);
						pVertices[*pCurVertex].vTangent = float3_t(-0.5f, 0.0f, cos30);
						pVertices[*pCurVertex].vBinorm = float3_t(0.0f, -1.0f, 0.0f);
						(*pCurVertex)++;
					}
					else
					{
						(*pCurVertex) += 4;
					}
				}


				if(b[5])
				{
					if(pIndices)
					{
						pIndices[*pCurIndex] = 0 + *pCurVertex;
						(*pCurIndex)++;
						pIndices[*pCurIndex] = 1 + *pCurVertex;
						(*pCurIndex)++;
						pIndices[*pCurIndex] = 2 + *pCurVertex;
						(*pCurIndex)++;

						pIndices[*pCurIndex] = 2 + *pCurVertex;
						(*pCurIndex)++;
						pIndices[*pCurIndex] = 1 + *pCurVertex;
						(*pCurIndex)++;
						pIndices[*pCurIndex] = 3 + *pCurVertex;
						(*pCurIndex)++;
					}
					else
					{
						(*pCurIndex) += 6;
					}


					if(pVertices)
					{
						pVertices[*pCurVertex].vPos = float3_t(fXpos + Rdiv2, fYpos + H, fZpos - Rcos30);
						pVertices[*pCurVertex].vNorm = float3_t(cos30, 0.0f, -0.5f);
						pVertices[*pCurVertex].vTex = float2_t(0.0f, 0.0f);
						pVertices[*pCurVertex].vTangent = float3_t(0.5f, 0.0f, cos30);
						pVertices[*pCurVertex].vBinorm = float3_t(0.0f, -1.0f, 0.0f);
						(*pCurVertex)++;
						pVertices[*pCurVertex].vPos = float3_t(fXpos + R, fYpos + H, fZpos);
						pVertices[*pCurVertex].vNorm = float3_t(cos30, 0.0f, -0.5f);
						pVertices[*pCurVertex].vTex = float2_t(1.0f, 0.0f);
						pVertices[*pCurVertex].vTangent = float3_t(0.5f, 0.0f, cos30);
						pVertices[*pCurVertex].vBinorm = float3_t(0.0f, -1.0f, 0.0f);
						(*pCurVertex)++;
						pVertices[*pCurVertex].vPos = float3_t(fXpos + Rdiv2, fYpos, fZpos - Rcos30);
						pVertices[*pCurVertex].vNorm = float3_t(cos30, 0.0f, -0.5f);
						pVertices[*pCurVertex].vTex = float2_t(0.0f, 1.0f);
						pVertices[*pCurVertex].vTangent = float3_t(0.5f, 0.0f, cos30);
						pVertices[*pCurVertex].vBinorm = float3_t(0.0f, -1.0f, 0.0f);
						(*pCurVertex)++;
						pVertices[*pCurVertex].vPos = float3_t(fXpos + R, fYpos, fZpos);
						pVertices[*pCurVertex].vNorm = float3_t(cos30, 0.0f, -0.5f);
						pVertices[*pCurVertex].vTex = float2_t(1.0f, 1.0f);
						pVertices[*pCurVertex].vTangent = float3_t(0.5f, 0.0f, cos30);
						pVertices[*pCurVertex].vBinorm = float3_t(0.0f, -1.0f, 0.0f);
						(*pCurVertex)++;
					}
					else
					{
						(*pCurVertex) += 4;
					}
				}


				if(b[0])
				{
					if(pIndices)
					{
						pIndices[*pCurIndex] = 0 + *pCurVertex;
						(*pCurIndex)++;
						pIndices[*pCurIndex] = 1 + *pCurVertex;
						(*pCurIndex)++;
						pIndices[*pCurIndex] = 2 + *pCurVertex;
						(*pCurIndex)++;

						pIndices[*pCurIndex] = 2 + *pCurVertex;
						(*pCurIndex)++;
						pIndices[*pCurIndex] = 1 + *pCurVertex;
						(*pCurIndex)++;
						pIndices[*pCurIndex] = 3 + *pCurVertex;
						(*pCurIndex)++;
					}
					else
					{
						(*pCurIndex) += 6;
					}

					if(pVertices)
					{
						pVertices[*pCurVertex].vPos = float3_t(fXpos - Rdiv2, fYpos + H, fZpos - Rcos30);
						pVertices[*pCurVertex].vNorm = float3_t(0.0f, 0.0f, -0.5f);
						pVertices[*pCurVertex].vTex = float2_t(0.0f, 0.0f);
						pVertices[*pCurVertex].vTangent = float3_t(1.0f, 0.0f, 0.0f);
						pVertices[*pCurVertex].vBinorm = float3_t(0.0f, -1.0f, 0.0f);
						(*pCurVertex)++;
						pVertices[*pCurVertex].vPos = float3_t(fXpos + Rdiv2, fYpos + H, fZpos - Rcos30);
						pVertices[*pCurVertex].vNorm = float3_t(0.0f, 0.0f, -0.5f);
						pVertices[*pCurVertex].vTex = float2_t(1.0f, 0.0f);
						pVertices[*pCurVertex].vTangent = float3_t(1.0f, 0.0f, 0.0f);
						pVertices[*pCurVertex].vBinorm = float3_t(0.0f, -1.0f, 0.0f);
						(*pCurVertex)++;
						pVertices[*pCurVertex].vPos = float3_t(fXpos - Rdiv2, fYpos, fZpos - Rcos30);
						pVertices[*pCurVertex].vNorm = float3_t(0.0f, 0.0f, -0.5f);
						pVertices[*pCurVertex].vTex = float2_t(0.0f, 1.0f);
						pVertices[*pCurVertex].vTangent = float3_t(1.0f, 0.0f, 0.0f);
						pVertices[*pCurVertex].vBinorm = float3_t(0.0f, -1.0f, 0.0f);
						(*pCurVertex)++;
						pVertices[*pCurVertex].vPos = float3_t(fXpos + Rdiv2, fYpos, fZpos - Rcos30);
						pVertices[*pCurVertex].vNorm = float3_t(0.0f, 0.0f, -0.5f);
						pVertices[*pCurVertex].vTex = float2_t(1.0f, 1.0f);
						pVertices[*pCurVertex].vTangent = float3_t(1.0f, 0.0f, 0.0f);
						pVertices[*pCurVertex].vBinorm = float3_t(0.0f, -1.0f, 0.0f);
						(*pCurVertex)++;
					}
					else
					{
						(*pCurVertex) += 4;
					}
				}

				if(b[1])
				{
					if(pIndices)
					{
						pIndices[*pCurIndex] = 0 + *pCurVertex;
						(*pCurIndex)++;
						pIndices[*pCurIndex] = 1 + *pCurVertex;
						(*pCurIndex)++;
						pIndices[*pCurIndex] = 2 + *pCurVertex;
						(*pCurIndex)++;

						pIndices[*pCurIndex] = 2 + *pCurVertex;
						(*pCurIndex)++;
						pIndices[*pCurIndex] = 1 + *pCurVertex;
						(*pCurIndex)++;
						pIndices[*pCurIndex] = 3 + *pCurVertex;
						(*pCurIndex)++;
					}
					else
					{
						(*pCurIndex) += 6;
					}

					if(pVertices)
					{
						pVertices[*pCurVertex].vPos = float3_t(fXpos - R, fYpos + H, fZpos);
						pVertices[*pCurVertex].vNorm = float3_t(-cos30, 0.0f, -0.5f);
						pVertices[*pCurVertex].vTex = float2_t(0.0f, 0.0f);
						pVertices[*pCurVertex].vTangent = float3_t(0.5f, 0.0f, -cos30);
						pVertices[*pCurVertex].vBinorm = float3_t(0.0f, -1.0f, 0.0f);
						(*pCurVertex)++;
						pVertices[*pCurVertex].vPos = float3_t(fXpos - Rdiv2, fYpos + H, fZpos - Rcos30);
						pVertices[*pCurVertex].vNorm = float3_t(-cos30, 0.0f, -0.5f);
						pVertices[*pCurVertex].vTex = float2_t(1.0f, 0.0f);
						pVertices[*pCurVertex].vTangent = float3_t(0.5f, 0.0f, -cos30);
						pVertices[*pCurVertex].vBinorm = float3_t(0.0f, -1.0f, 0.0f);
						(*pCurVertex)++;
						pVertices[*pCurVertex].vPos = float3_t(fXpos - R, fYpos, fZpos);
						pVertices[*pCurVertex].vNorm = float3_t(-cos30, 0.0f, -0.5f);
						pVertices[*pCurVertex].vTex = float2_t(0.0f, 1.0f);
						pVertices[*pCurVertex].vTangent = float3_t(0.5f, 0.0f, -cos30);
						pVertices[*pCurVertex].vBinorm = float3_t(0.0f, -1.0f, 0.0f);
						(*pCurVertex)++;
						pVertices[*pCurVertex].vPos = float3_t(fXpos - Rdiv2, fYpos, fZpos - Rcos30);
						pVertices[*pCurVertex].vNorm = float3_t(-cos30, 0.0f, -0.5f);
						pVertices[*pCurVertex].vTex = float2_t(1.0f, 1.0f);
						pVertices[*pCurVertex].vTangent = float3_t(0.5f, 0.0f, -cos30);
						pVertices[*pCurVertex].vBinorm = float3_t(0.0f, -1.0f, 0.0f);
						(*pCurVertex)++;
					}
					else
					{
						(*pCurVertex) += 4;
					}
				}
			}
		}
	}
}

#define VEC3_DOT(v1, v2) ((v1).x * (v2).x + (v1).y * (v2).y + (v1).z * (v2).z)
#define SQR(x) ((x) * (x))
#define VEC3_LEN(v1, v2) (sqrtf((SQR((v1).x - (v2).x) + SQR((v1).y - (v2).y) + SQR((v1).z - (v2).z))))


UINT CSolidBlock::CheckVecCollision(UINT blockID, const float3 &pos_, const float3 &vec, const float3 &mypos)
{
	//DSconsole::write_("%f\t%f\t%f\n", mypos.x, mypos.y, mypos.z);
	//DS::UTDrawLine(mypos, mypos);
	float3 v2 = mypos - pos_;
	if(VEC3_DOT(v2, vec) < 0.0f)
	{
		return(0);
	}
	float3 pos = pos_ - mypos;
	float2_t A(-HC_R, 0.0f);
	float2_t B(-Rdiv2, Rcos30);
	float2_t C(Rdiv2, Rcos30);
	float2_t D(HC_R, 0.0f);
	float2_t E(Rdiv2, -Rcos30);
	float2_t F(-Rdiv2, -Rcos30);

	float fCurLen = -1.0f;
	UINT res = 0;

	//bottom plane (7)
	float3 n(0.0f, -1.0f, 0.0f);
	if(VEC3_DOT(n, vec) < 0.0f)
	{
		float t = -pos.y / vec.y;
		float3 cp(pos.x + vec.x * t, 0.0f, pos.z + vec.z * t);
		//DS::UTDrawLine(cp + mypos, cp + mypos + n * 10.0f);

		float kz = 1.0f / cos30 * cp.z;
		if((cp.z < B.y) && (cp.z > F.y) && (cp.x < HC_R - kz) && (cp.x < HC_R + kz) && (cp.x > -HC_R + kz) && (cp.x > -HC_R - kz))
		{
			float fl = VEC3_LEN(cp, pos);
			if(fCurLen < 0.0f || fl < fCurLen)
			{
				fCurLen = fCurLen;
				res = 7;
			}
			//return(7);
		}
	}

	//top plane (6)
	n = float3(0.0f, 1.0f, 0.0f);
	if(VEC3_DOT(n, vec) < 0.0f)
	{
		float t = (HC_H - pos.y) / vec.y;
		float3 cp(pos.x + vec.x * t, 0.0f, pos.z + vec.z * t);
		float kz = 1.0f / cos30 * cp.z;
		//DS::UTDrawLine(cp + mypos, cp + mypos + n * 10.0f);
		if((cp.z < B.y) && (cp.z > F.y) && (cp.x < HC_R - kz) && (cp.x < HC_R + kz) && (cp.x > -HC_R + kz) && (cp.x > -HC_R - kz))
		{
			float fl = VEC3_LEN(cp, pos);
			if(fCurLen < 0.0f || fl < fCurLen)
			{
				fCurLen = fCurLen;
				res = 6;
			}
			//return(6);
		}
	}

	//DC-plane (4)
	n = float3(1.732f, 0.0f, 1.0f);
	if(VEC3_DOT(n, vec) < 0.0f)
	{
		float t = -(n.x * (pos.x - C.x) + n.y * pos.y + n.z * (pos.z - C.y)) / (vec.x * n.x + vec.y * n.y + vec.z * n.z);
		float3 cp(pos.x + vec.x * t, pos.y + vec.y * t, pos.z + vec.z * t);
		//DS::UTDrawLine(cp + mypos, cp + mypos + n * 10.0f);

		if(cp.x > C.x && cp.x < D.x && cp.z > D.y && cp.z < C.y && cp.y > 0 && cp.y < HC_H)
		{
			float fl = VEC3_LEN(cp, pos);
			if(fCurLen < 0.0f || fl < fCurLen)
			{
				fCurLen = fCurLen;
				res = 4;
			}
			//return(4);
		}
	}

	//DE-plane (5)
	n = float3(1.732f, 0.0f, -1.0f);
	if(VEC3_DOT(n, vec) < 0.0f)
	{
		float t = -(n.x * (pos.x - D.x) + n.y * pos.y + n.z * (pos.z - D.y)) / (vec.x * n.x + vec.y * n.y + vec.z * n.z);
		float3 cp(pos.x + vec.x * t, pos.y + vec.y * t, pos.z + vec.z * t);
		//DS::UTDrawLine(cp + mypos, cp + mypos + n * 10.0f);

		if(cp.x > E.x && cp.x < D.x && cp.z > E.y && cp.z < D.y && cp.y > 0 && cp.y < HC_H)
		{
			float fl = VEC3_LEN(cp, pos);
			if(fCurLen < 0.0f || fl < fCurLen)
			{
				fCurLen = fCurLen;
				res = 5;
			}
			//return(5);
		}
	}


	//AB-plane (2)
	n = float3(-1.732f, 0.0f, 1.0f);
	if(VEC3_DOT(n, vec) < 0.0f)
	{
		float t = -(n.x * (pos.x - A.x) + n.y * pos.y + n.z * (pos.z - A.y)) / (vec.x * n.x + vec.y * n.y + vec.z * n.z);
		float3 cp(pos.x + vec.x * t, pos.y + vec.y * t, pos.z + vec.z * t);
		//DS::UTDrawLine(cp + mypos, cp + mypos + n * 10.0f);

		if(cp.x > A.x && cp.x < B.x && cp.z > A.y && cp.z < B.y && cp.y > 0 && cp.y < HC_H)
		{
			float fl = VEC3_LEN(cp, pos);
			if(fCurLen < 0.0f || fl < fCurLen)
			{
				fCurLen = fCurLen;
				res = 2;
			}
			//return(2);
		}
	}

	//AF-plane (1)
	n = float3(-1.732f, 0.0f, -1.0f);
	if(VEC3_DOT(n, vec) < 0.0f)
	{
		float t = -(n.x * (pos.x - A.x) + n.y * pos.y + n.z * (pos.z - A.y)) / (vec.x * n.x + vec.y * n.y + vec.z * n.z);
		float3 cp(pos.x + vec.x * t, pos.y + vec.y * t, pos.z + vec.z * t);
		//DS::UTDrawLine(cp + mypos, cp + mypos + n * 10.0f);

		if(cp.x > A.x && cp.x < F.x && cp.z > F.y && cp.z < A.y && cp.y > 0 && cp.y < HC_H)
		{
			float fl = VEC3_LEN(cp, pos);
			if(fCurLen < 0.0f || fl < fCurLen)
			{
				fCurLen = fCurLen;
				res = 1;
			}
			//return(1);
		}
	}

	//BC-plane (3)
	n = float3(0.0f, 0.0f, 1.0f);
	if(VEC3_DOT(n, vec) < 0.0f)
	{
		float t = (Rcos30 - pos.z) / vec.z;
		float3 cp(pos.x + vec.x * t, pos.y + vec.y * t, pos.z + vec.z * t);
		//DS::UTDrawLine(cp + mypos, cp + mypos + n * 10.0f);

		if(cp.x > B.x && cp.x < C.x && cp.y > 0 && cp.y < HC_H)
		{
			float fl = VEC3_LEN(cp, pos);
			if(fCurLen < 0.0f || fl < fCurLen)
			{
				fCurLen = fCurLen;
				res = 3;
			}
			//return(3);
		}
	}

	//EF-plane (8)
	n = float3(0.0f, 0.0f, -1.0f);
	if(VEC3_DOT(n, vec) < 0.0f)
	{
		float t = (-Rcos30 - pos.z) / vec.z;
		float3 cp(pos.x + vec.x * t, pos.y + vec.y * t, pos.z + vec.z * t);
		//DS::UTDrawLine(cp + mypos, cp + mypos + n * 10.0f);
		if(cp.x > F.x && cp.x < E.x && cp.y > 0 && cp.y < HC_H)
		{
			float fl = VEC3_LEN(cp, pos);
			if(fCurLen < 0.0f || fl < fCurLen)
			{
				fCurLen = fCurLen;
				res = 8;
			}
			//return(8);
		}
	}

	return(res);
}


void CSolidBlock::createMesh(UINT iTexture, float fXpos, float fYpos, float fZpos, float R, float H, XResourceModelStaticVertex *pVertices, UINT * pIndices, UINT * pCurVertex, UINT * pCurIndex, bool * b, WORD * pBlocks)
{
	CreateMesh(m_BlockType, iTexture, fXpos, fYpos, fZpos, R, H, pVertices, pIndices, pCurVertex, pCurIndex, b, pBlocks);
}


UINT CSolidBlock::checkVecCollision(const float3 &pos, const float3 &vec, const float3 &mypos)
{
	return(CheckVecCollision(m_BlockType, pos, vec, mypos));
}