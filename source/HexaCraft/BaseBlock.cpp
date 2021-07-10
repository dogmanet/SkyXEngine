#include "BaseBlock.h"

CBaseBlock::CBaseBlock()
{
}

WORD CBaseBlock::getType()
{
	return(HC_GET_TYPE(m_BlockType));
}

WORD CBaseBlock::getTypeID()
{
	return(HC_GET_ID(m_BlockType));
}

void CBaseBlock::setType(WORD type)
{
	m_BlockType = HC_MKTYPE(type, getTypeID());
}

WORD CBaseBlock::getTypeFull()
{
	return(m_BlockType);
}

void CBaseBlock::setTypeFull(WORD type)
{
	m_BlockType = type;
}

void CBaseBlock::CreateMesh(UINT blockID, UINT iTexture, float fXpos, float fYpos, float fZpos, float R, float H, XResourceModelStaticVertex *pVertices, UINT *pIndices, UINT *pCurVertex, UINT * pCurIndex, bool * b, WORD * pBlocks)
{
}

UINT CBaseBlock::CheckVecCollision(UINT blockID, const float3 &pos, const float3 &vec, const float3 &mypos)
{
	return(0);
}

//std::map<UINT, CBlockManager::vTable> CBlockManager::mBlocks;
CBlockManager::vTable CBlockManager::mBlocks[HC_MAX_BLOCKID];
Array<const char*> CBlockManager::ms_aszTextures;

CBaseBlock* CBlockManager::GetInstance(const UINT id/*, DSHCblockPool * pool*/)
{
	if(HC_GET_ID(id) < 0 || HC_GET_ID(id) >= HC_MAX_BLOCKID)
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "Unable to create unknown block id %d\n", HC_GET_ID(id));
		return(NULL);
	}
	return(mBlocks[HC_GET_ID(id)].create(id/*, pool*/));
}

#if 0
DSbaseTool * CBlockManager::CreateTool(const UINT id)
{
	if(HC_GET_ID(id) < 0 || HC_GET_ID(id) >= HC_MAX_BLOCKID)
	{
		DSconsole::write("[Warning]: Unable to create unknown tool id %d\n", HC_GET_ID(id));
		return(NULL);
	}
	return(mBlocks[HC_GET_ID(id)].createtool(id));
}

DSHCblock_MALLOC * CBlockManager::RequestMemPool(const UINT id)
{
	if(HC_GET_ID(id) < 0 || HC_GET_ID(id) >= HC_MAX_BLOCKID)
	{
		DSconsole::write("[Warning]: Unable to create pool for unknown block id %d\n", HC_GET_ID(id));
		return(NULL);
	}
	return(mBlocks[HC_GET_ID(id)].requestmp());
}
#endif

void CBlockManager::FreeInstance(CBaseBlock *block/*, DSHCblockPool * pool*/)
{
	const UINT id = block->getTypeFull();
	if(HC_GET_ID(id) < 0 || HC_GET_ID(id) >= HC_MAX_BLOCKID)
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "Unable to delete unknown block id %d\n", id);
		return;
	}
	mBlocks[HC_GET_ID(id)].remove(block/*, pool*/);
}

UINT CBlockManager::CountMaterials()
{
	/*UINT c = 0;
	for(UINT i = 0; i < HC_MAX_BLOCKID; i++)
	{
		c += mBlocks[i].getmaterialscount();
	}*/
	return(ms_aszTextures.size());
}

UINT CBlockManager::GetMaterialCount(UINT bid)
{
	if(HC_GET_ID(bid) < 0 || HC_GET_ID(bid) >= HC_MAX_BLOCKID)
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "Unknown block id %d\n", bid);
		return(NULL);
	}
	return(mBlocks[HC_GET_ID(bid)].getmaterialscount());
}

UINT CBlockManager::GetMaterialById(const UINT bid, const UINT mid)
{
	if(HC_GET_ID(bid) < 0 || HC_GET_ID(bid) >= HC_MAX_BLOCKID)
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "Unknown block id %d\n", bid);
		return(NULL);
	}
	return(mBlocks[HC_GET_ID(bid)].getmaterialidbyid(mid));
}

const char* CBlockManager::GetMaterialById(const UINT mid)
{
	return(ms_aszTextures[mid]);
}

void CBlockManager::CreateMesh(UINT blockID, UINT iTexture, float fXpos, float fYpos, float fZpos, float R, float H, XResourceModelStaticVertex *pVertices, UINT *pIndices, UINT * pCurVertex, UINT * pCurIndex, bool * b, WORD * pBlocks)
{
	if(HC_GET_ID(blockID) < 0 || HC_GET_ID(blockID) >= HC_MAX_BLOCKID)
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "Unable to CreateMesh for unknown block id %d\n", blockID);
		return;
	}
	mBlocks[HC_GET_ID(blockID)].createmesh(blockID, iTexture, fXpos, fYpos, fZpos, R, H, pVertices, pIndices, pCurVertex, pCurIndex, b, pBlocks);
}

UINT CBlockManager::CheckVecCollision(UINT blockID, const float3 &pos, const float3 &vec, const float3 &mypos)
{
	if(HC_GET_ID(blockID) < 0 || HC_GET_ID(blockID) >= HC_MAX_BLOCKID)
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "Unable to checkveccollision unknown block id %d\n", blockID);
		return(NULL);
	}
	return(mBlocks[HC_GET_ID(blockID)].checkveccollision(blockID, pos, vec, mypos));
}
/*
void CBlockManager::DumpMemStats(UINT id)
{
	if(!mBlocks.count(HC_GET_ID(id)))
	{
		DSconsole::write("[Warning]: Unknown block id %d\n", id);
		return;
	}
	mBlocks[HC_GET_ID(id)].memdump();
}*/

#if 0
void CBlockManager::DumpMemStatsCMD(std::string cmd)
{
	//DumpMemStats(strToNum(cmd));
}
#endif

void CBlockManager::Init()
{
	//ms_aszTextures
	for(UINT i = 0; i < HC_MAX_BLOCKID; ++i)
	{
		UINT uMC = mBlocks[i].getmaterialscount();
		for(UINT j = 0; j < uMC; ++j)
		{
			const char *szMat = mBlocks[i].getmaterialbyid(j);
			int idx = ms_aszTextures.indexOf(szMat);
			if(idx < 0)
			{
				idx = (int)ms_aszTextures.size();
				ms_aszTextures[idx] = szMat;
			}

			mBlocks[i].setmaterialidbyid(j, (UINT)idx);
		}
	}
}

#if 0
DSHCblockPool::DSHCblockPool()
{
	//DSconsole::write("DSHCblockPool::DSHCblockPool()\n");
	memset(m_pMem, 0, sizeof(DSHCblock_MALLOC*) * HC_MAX_BLOCKID);
}

CBaseBlock * DSHCblockPool::Alloc(UINT id)
{
	UINT bid = HC_GET_ID(id);
	if(!m_pMem[bid])
	{
		m_pMem[bid] = CBlockManager::RequestMemPool(id);
	}
	CBaseBlock * nb = m_pMem[bid]->Alloc();
	nb->SetTypeFull(id);
	return(nb);
}

void DSHCblockPool::Free(CBaseBlock * ptr)
{
	UINT bid = ptr->GetTypeID();
	if(m_pMem[bid])
	{
		m_pMem[bid]->Free(ptr);
	}
}

DSHCblockPool::~DSHCblockPool()
{
	//DSconsole::write("DSHCblockPool::~DSHCblockPool()\n");
	Release();
}

void DSHCblockPool::Release()
{
	for(UINT i = 0; i < HC_MAX_BLOCKID; i++)
	{
		DS_SAFE_DELETE(m_pMem[i]);
	}
}
#endif
