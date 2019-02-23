#include "CommandScale.h"

bool CCommandScale::exec()
{
	_scale_obj *pObj;
	bool moved = false;
	for(UINT i = 0, l = m_aObjects.size(); i < l; ++i)
	{
		pObj = &m_aObjects[i];
		g_pLevelObjects[pObj->idObject]->setPos(pObj->vEndPos);
		g_pLevelObjects[pObj->idObject]->setScale(pObj->vEndScale);
		moved = moved || memcmp(&pObj->vEndPos, &pObj->vStartPos, sizeof(pObj->vStartPos)) || memcmp(&pObj->vEndScale, &pObj->vStartScale, sizeof(pObj->vStartScale));
	}
	return(moved);
}
bool CCommandScale::undo()
{
	_scale_obj *pObj;
	for(UINT i = 0, l = m_aObjects.size(); i < l; ++i)
	{
		pObj = &m_aObjects[i];
		g_pLevelObjects[pObj->idObject]->setPos(pObj->vStartPos);
		g_pLevelObjects[pObj->idObject]->setScale(pObj->vStartScale);
	}
	return(true);
}

void CCommandScale::addObject(ID idObject)
{
	float3_t vPos = g_pLevelObjects[idObject]->getPos();
	float3_t vScale = g_pLevelObjects[idObject]->getScale();
	m_aObjects.push_back({idObject, vPos, vPos, vScale, vScale});
}
void CCommandScale::setStartAABB(const float3 &vAABBmin, const float3 &vAABBmax)
{
	m_vBoundMin = vAABBmin;
	m_vBoundMax = vAABBmax;
}
void CCommandScale::setTransformDir(X_DIR dir)
{
	m_xDir = dir;
}

void CCommandScale::setStartPos(const float3 &vPos)
{
	if(m_xDir & (XDIR_X_POS | XDIR_X_NEG))
	{
		if(m_xDir & XDIR_X_NEG)
		{
			m_vDelta.x = m_vBoundMin.x - vPos.x;
		}
		else
		{
			m_vDelta.x = m_vBoundMax.x - vPos.x;
		}
	}
	if(m_xDir & (XDIR_Y_POS | XDIR_Y_NEG))
	{
		if(m_xDir & XDIR_Y_NEG)
		{
			m_vDelta.y = m_vBoundMin.y - vPos.y;
		}
		else
		{
			m_vDelta.y = m_vBoundMax.y - vPos.y;
		}
	}
	if(m_xDir & (XDIR_Z_POS | XDIR_Z_NEG))
	{
		if(m_xDir & XDIR_Z_NEG)
		{
			m_vDelta.z = m_vBoundMin.z - vPos.z;
		}
		else
		{
			m_vDelta.z = m_vBoundMax.z - vPos.z;
		}
	}
}

void CCommandScale::setCurrentPos(const float3 &_vPos)
{
	float3 vPos = m_vDelta + _vPos;
	float3 vPosCoeff(1.0f, 1.0f, 1.0f);
	float3 vPosOrigin = m_vBoundMin;
	if(m_xDir & (XDIR_X_POS | XDIR_X_NEG))
	{
		if(m_xDir & XDIR_X_NEG)
		{
			vPosOrigin.x = m_vBoundMax.x;
			vPosCoeff.x = vPosOrigin.x - vPos.x;
		}
		else
		{
			vPosCoeff.x = vPos.x - vPosOrigin.x;
		}
		if(vPosCoeff.x < 0.01f)
		{
			vPosCoeff.x = 0.01f;
		}
		vPosCoeff.x = vPosCoeff.x / (m_vBoundMax.x - m_vBoundMin.x);
	}
	if(m_xDir & (XDIR_Y_POS | XDIR_Y_NEG))
	{
		if(m_xDir & XDIR_Y_NEG)
		{
			vPosOrigin.y = m_vBoundMax.y;
			vPosCoeff.y = vPosOrigin.y - vPos.y;
		}
		else
		{
			vPosCoeff.y = vPos.y - vPosOrigin.y;
		}
		if(vPosCoeff.y < 0.01f)
		{
			vPosCoeff.y = 0.01f;
		}
		vPosCoeff.y = vPosCoeff.y / (m_vBoundMax.y - m_vBoundMin.y);
	}
	if(m_xDir & (XDIR_Z_POS | XDIR_Z_NEG))
	{
		if(m_xDir & XDIR_Z_NEG)
		{
			vPosOrigin.z = m_vBoundMax.z;
			vPosCoeff.z = vPosOrigin.z - vPos.z;
		}
		else
		{
			vPosCoeff.z = vPos.z - vPosOrigin.z;
		}
		if(vPosCoeff.z < 0.01f)
		{
			vPosCoeff.z = 0.01f;
		}
		vPosCoeff.z = vPosCoeff.z / (m_vBoundMax.z - m_vBoundMin.z);
	}

	_scale_obj *pObj;
	for(UINT i = 0, l = m_aObjects.size(); i < l; ++i)
	{
		pObj = &m_aObjects[i];
		pObj->vEndPos = (float3)(vPosOrigin + (pObj->vStartPos - vPosOrigin) * vPosCoeff);
		SMQuaternion quat = g_pLevelObjects[pObj->idObject]->getOrient().Conjugate();
		float3 pc = (quat * vPosCoeff);
		if(pc.x < 0.0f)
		{
			pc.x = -1.0f * pc.x;
		}
		if(pc.y < 0.0f)
		{
			pc.y = -1.0f * pc.y;
		}
		if(pc.z < 0.0f)
		{
			pc.z = -1.0f * pc.z;
		}
		pObj->vEndScale = (float3)(pObj->vStartScale * pc);

		g_pLevelObjects[pObj->idObject]->setPos(pObj->vEndPos);
		g_pLevelObjects[pObj->idObject]->setScale(pObj->vEndScale);
	}
}
