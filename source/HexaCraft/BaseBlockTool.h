#ifndef _CBaseBlockTool_H_
#define _CBaseBlockTool_H_

//#include "DSbaseTool.h"
#include "BaseBlock.h"

class CBaseBlockTool: /*public DSbaseTool, */public CBaseBlock
{
	DECLARE_CLASS(CBaseBlockTool, CBaseBlock);
public:
	void advance(float fTime);

	void createMesh(UINT iTexture, float fXpos, float fYpos, float fZpos, float R, float H, XResourceModelStaticVertex *pVertices, UINT *pIndices, UINT *pCurVertex, UINT *pCurIndex, bool * b, WORD * pBlocks)
	{
	}
	UINT checkVecCollision(const float3 &pos, const float3 &vec, const float3 &mypos)
	{
		return(0);
	}
	UINT getMaterialCount()
	{
		return(0);
	};
	/*DSMATERIAL GetMaterialById(UINT id)
	{
		return(NULL);
	}*/
	void deploy()
	{

	}
};

#endif
