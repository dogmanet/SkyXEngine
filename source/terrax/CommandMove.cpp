#include "CommandMove.h"

bool CCommandMove::exec()
{
	_move_obj *pObj;
	bool moved = false;
	for(UINT i = 0, l = m_aObjects.size(); i < l; ++i)
	{
		pObj = &m_aObjects[i];
		g_pLevelObjects[pObj->idObject]->setPos(pObj->vEndPos);
		moved = moved || memcmp(&pObj->vEndPos, &pObj->vStartPos, sizeof(pObj->vStartPos));
	}
	return(moved);
}
bool CCommandMove::undo()
{
	_move_obj *pObj;
	for(UINT i = 0, l = m_aObjects.size(); i < l; ++i)
	{
		pObj = &m_aObjects[i];
		g_pLevelObjects[pObj->idObject]->setPos(pObj->vStartPos);
	}
	return(true);
}

const char *CCommandMove::getText()
{
	return("move");
}

void CCommandMove::addObject(ID idObject)
{
	float3_t vPos = g_pLevelObjects[idObject]->getPos();
	m_aObjects.push_back({idObject, vPos, vPos});
}
void CCommandMove::setStartPos(const float3 &vPos)
{
	m_vStartPos = vPos;
}

void CCommandMove::setCurrentPos(const float3 &vPos)
{
	float3 vDelta = vPos - m_vStartPos;
	_move_obj *pObj;
	for(UINT i = 0, l = m_aObjects.size(); i < l; ++i)
	{
		pObj = &m_aObjects[i];
		pObj->vEndPos = (float3)(pObj->vStartPos + vDelta);
		g_pLevelObjects[pObj->idObject]->setPos(pObj->vEndPos);
	}
}
