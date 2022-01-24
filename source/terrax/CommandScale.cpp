#include "CommandScale.h"

CCommandScale::~CCommandScale()
{
	for(UINT i = 0, l = m_aObjects.size(); i < l; ++i)
	{
		mem_release(m_aObjects[i].pObj);
	}
}

bool XMETHODCALLTYPE CCommandScale::exec()
{
	_scale_obj *pObj;
	bool moved = false;
	for(UINT i = 0, l = m_aObjects.size(); i < l; ++i)
	{
		pObj = &m_aObjects[i];
		pObj->pObj->setPos(pObj->vEndPos);
		pObj->pObj->setSize(pObj->vEndScale);
		moved = moved || memcmp(&pObj->vEndPos, &pObj->vStartPos, sizeof(pObj->vStartPos)) || memcmp(&pObj->vEndScale, &pObj->vStartScale, sizeof(pObj->vStartScale));
	}
	XUpdatePropWindow();
	return(moved);
}
bool XMETHODCALLTYPE CCommandScale::undo()
{
	_scale_obj *pObj;
	for(UINT i = 0, l = m_aObjects.size(); i < l; ++i)
	{
		pObj = &m_aObjects[i];
		pObj->pObj->setPos(pObj->vStartPos);
		pObj->pObj->setSize(pObj->vStartScale);
	}
	XUpdatePropWindow();
	return(true);
}

void CCommandScale::addObject(IXEditorObject *pObj)
{
	add_ref(pObj);
	float3_t vPos = pObj->getPos();
	float3 vMin, vMax;
	pObj->getBound(&vMin, &vMax);
	float3 vScale = vMax - vMin;
	m_aObjects.push_back({pObj, vPos, vPos, vScale, vScale});
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

static void CheckMinSize(float *pOut)
{
	float fGridStep = XGetGridStep();
	if(*pOut < 0.001f)
	{
		if(g_xConfig.m_bSnapGrid && fGridStep > 0.0f)
		{
			*pOut = fGridStep;
		}
		else
		{
			*pOut = 0.001f;
		}
	}
	
}

void CCommandScale::setCurrentPos(const float3 &_vPos)
{
	float3 vPos = XSnapToGrid(m_vDelta + _vPos);
	float3 vPosCoeff;
	float3 vBoxSize = m_vBoundMax - m_vBoundMin;
	float3 vNewBox = vBoxSize;
	float3 vPosOrigin = m_vBoundMin;
	float fGridStep = XGetGridStep();
	if(m_xDir & (XDIR_X_POS | XDIR_X_NEG))
	{
		if(m_xDir & XDIR_X_NEG)
		{
			vPosOrigin.x = m_vBoundMax.x;
			vNewBox.x = vPosOrigin.x - vPos.x;
		}
		else
		{
			vNewBox.x = vPos.x - vPosOrigin.x;
		}
		CheckMinSize(&vNewBox.x);
	}
	if(m_xDir & (XDIR_Y_POS | XDIR_Y_NEG))
	{
		if(m_xDir & XDIR_Y_NEG)
		{
			vPosOrigin.y = m_vBoundMax.y;
			vNewBox.y = vPosOrigin.y - vPos.y;
		}
		else
		{
			vNewBox.y = vPos.y - vPosOrigin.y;
		}
		CheckMinSize(&vNewBox.y);
		//vPosCoeff.y = vNewBox.y / (m_vBoundMax.y - m_vBoundMin.y);
	}
	if(m_xDir & (XDIR_Z_POS | XDIR_Z_NEG))
	{
		if(m_xDir & XDIR_Z_NEG)
		{
			vPosOrigin.z = m_vBoundMax.z;
			vNewBox.z = vPosOrigin.z - vPos.z;
		}
		else
		{
			vNewBox.z = vPos.z - vPosOrigin.z;
		}
		CheckMinSize(&vNewBox.z);
		//vPosCoeff.z = vNewBox.z / (m_vBoundMax.z - m_vBoundMin.z);
	}
	vPosCoeff = vNewBox / vBoxSize;

	//printf("%.3f, %.3f, %.3f\n", vPosCoeff.x, vPosCoeff.y, vPosCoeff.z);

	_scale_obj *pObj;
	for(UINT i = 0, l = m_aObjects.size(); i < l; ++i)
	{
		pObj = &m_aObjects[i];
		pObj->vEndPos = (float3)(vPosOrigin + (pObj->vStartPos - vPosOrigin) * vPosCoeff);

#if 0
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
#endif
		pObj->vEndScale = (float3)(pObj->vStartScale * vPosCoeff);
		pObj->pObj->setPos(pObj->vEndPos);
		pObj->pObj->setSize(pObj->vEndScale);
	}
}
