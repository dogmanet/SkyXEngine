#include "CommandRotate.h"

bool XMETHODCALLTYPE CCommandRotate::exec()
{
	_rot_obj *pObj;
	bool moved = false;
	for(UINT i = 0, l = m_aObjects.size(); i < l; ++i)
	{
		pObj = &m_aObjects[i];
		g_pLevelObjects[pObj->idObject]->setPos(pObj->vEndPos);
		g_pLevelObjects[pObj->idObject]->setOrient(pObj->vEndOrient);
		moved = moved || memcmp(&pObj->vEndPos, &pObj->vStartPos, sizeof(pObj->vStartPos)) || memcmp(&pObj->vEndOrient, &pObj->vStartOrient, sizeof(pObj->vStartOrient));
	}
	return(moved);
}
bool XMETHODCALLTYPE CCommandRotate::undo()
{
	_rot_obj *pObj;
	for(UINT i = 0, l = m_aObjects.size(); i < l; ++i)
	{
		pObj = &m_aObjects[i];
		g_pLevelObjects[pObj->idObject]->setPos(pObj->vStartPos);
		g_pLevelObjects[pObj->idObject]->setOrient(pObj->vStartOrient);
	}
	return(true);
}

void CCommandRotate::addObject(ID idObject)
{
	float3_t vPos = g_pLevelObjects[idObject]->getPos();
	SMQuaternion qOrient = g_pLevelObjects[idObject]->getOrient();
	m_aObjects.push_back({idObject, vPos, vPos, qOrient, qOrient});
}
void CCommandRotate::setStartOrigin(const float3 &vOrigin)
{
	m_vOrigin = vOrigin;
}
void CCommandRotate::setStartPos(const float3 &vPos)
{
	m_vStartPos = vPos;
}

void CCommandRotate::setCurrentPos(const float3 &vPos, bool useSnap)
{
	float3 v1 = m_vStartPos - m_vOrigin;
	float3 v2 = vPos - m_vOrigin;
	SMQuaternion q(v1, v2);


	_rot_obj *pObj;
	for(UINT i = 0, l = m_aObjects.size(); i < l; ++i)
	{
		pObj = &m_aObjects[i];
		pObj->vEndPos = (float3)(m_vOrigin + q * (pObj->vStartPos - m_vOrigin));

		pObj->vEndOrient = pObj->vStartOrient * q;

		g_pLevelObjects[pObj->idObject]->setPos(pObj->vEndPos);
		g_pLevelObjects[pObj->idObject]->setOrient(pObj->vEndOrient);
	}
}