#ifndef _CSolidBlock_H_
#define _CSolidBlock_H_

#include "BaseBlock.h"

class CSolidBlock: public CBaseBlock
{
public:
	void advance(float fTime) override;

	void createMesh(UINT iTexture, float fXpos, float fYpos, float fZpos, float R, float H, XResourceModelStaticVertex *pVertices, UINT * pIndices, UINT * pCurVertex, UINT * pCurIndex, bool * b, WORD * pBlocks) override;

	UINT checkVecCollision(const float3 &pos, const float3 &vec, const float3 &mypos) override;

	static CBaseBlock* GetInstance(UINT id);

	static void CreateMesh(UINT blockID, UINT iTexture, float fXpos, float fYpos, float fZpos, float R, float H, XResourceModelStaticVertex *pVertices, UINT * pIndices, UINT * pCurVertex, UINT * pCurIndex, bool * b, WORD * pBlocks);

	static UINT CheckVecCollision(UINT blockID, const float3 &pos, const float3 &vec, const float3 &mypos);
};

#endif
