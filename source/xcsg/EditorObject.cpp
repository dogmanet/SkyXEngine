#include "EditorObject.h"

//#include <render/sxrender.h>
#include <mtrl/sxmtrl.h>
#include <xcommon/resource/IXResourceManager.h>

#include "Editable.h"

CEditorObject::CEditorObject(CEditable *pEditable):
	m_pEditable(pEditable)
{
	pEditable->onObjectCreated(this);
	//m_szClassName = CEntityFactoryMap::GetInstance()->getClassNamePtr(szClassName);
	//assert(m_szClassName);
}

CEditorObject::~CEditorObject()
{
	m_pEditable->removeObject(this);
	
	for(UINT i = 0, l = m_aBrushes.size(); i < l; ++i)
	{
		mem_delete(m_aBrushes[i]);
	}
}


void XMETHODCALLTYPE CEditorObject::setPos(const float3_t &pos)
{
	//SAFE_CALL(m_pEntity, setPos, pos);
	//SAFE_CALL(m_pModel, setPosition, pos);
	float3 vOffset = pos - m_vPos;
	for(UINT i = 0, l = m_aBrushes.size(); i < l; ++i)
	{
		m_aBrushes[i]->move(vOffset);
	}
	m_vPos = pos;
}

void CEditorObject::fixPos()
{
	float3 vMin, vMax;
	getBound(&vMin, &vMax);
	m_vPos = (float3)((vMin + vMax) * 0.5f);
}

void XMETHODCALLTYPE CEditorObject::setSize(const float3_t &vSize)
{
	float3 vMin, vMax;
	getBound(&vMin, &vMax);

	float3 vRelativeSize = vSize / (vMax - vMin);

	for(UINT i = 0, l = m_aBrushes.size(); i < l; ++i)
	{
		//const SMAABB &aabb = m_aBrushes[i]->getAABB();

		m_aBrushes[i]->resize(m_vPos, /*(aabb.vMax - aabb.vMin) * */vRelativeSize);
	}
}

void XMETHODCALLTYPE CEditorObject::setOrient(const SMQuaternion &orient)
{
	//SAFE_CALL(m_pEntity, setOrient, orient);
	//SAFE_CALL(m_pModel, setOrientation, orient);

	SMQuaternion qOffset = m_qRot.Conjugate() * orient;
	for(UINT i = 0, l = m_aBrushes.size(); i < l; ++i)
	{
		m_aBrushes[i]->rotate(m_vPos, qOffset);
	}

	m_qRot = orient;
}

float3_t XMETHODCALLTYPE CEditorObject::getPos()
{
	//if(m_pEntity)
	//{
	//	m_vPos = m_pEntity->getPos();
	//	SAFE_CALL(m_pModel, setPosition, m_vPos);
	//}
	return(m_vPos);
}

SMQuaternion XMETHODCALLTYPE CEditorObject::getOrient()
{
	//if(m_pEntity)
	//{
	//	m_qRot = m_pEntity->getOrient();
	//	SAFE_CALL(m_pModel, setOrientation, m_qRot);
	//}
	return(m_qRot);
}

void XMETHODCALLTYPE CEditorObject::getBound(float3 *pvMin, float3 *pvMax)
{
	*pvMin = *pvMax = float3();

	if(!m_aBrushes.size())
	{
		return;
	}

	SMAABB aabb = m_aBrushes[0]->getAABB();
	for(UINT i = 0, l = m_aBrushes.size(); i < l; ++i)
	{
		aabb = SMAABBConvex(aabb, m_aBrushes[i]->getAABB());
	}

	*pvMin = aabb.vMin;
	*pvMax = aabb.vMax;
}

void XMETHODCALLTYPE CEditorObject::renderSelection(bool is3D, IXGizmoRenderer *pGizmoRenderer)
{
	for(UINT i = 0, l = m_aBrushes.size(); i < l; ++i)
	{
		m_aBrushes[i]->renderSelection(is3D, pGizmoRenderer);
	}

#if 0
	if(!m_pEntity)
	{
		return;
	}

	IGXDevice *pDevice = SGCore_GetDXDevice();
	IGXContext *pCtx = pDevice->getThreadContext();

	IGXBlendState *pOldBlendState = pCtx->getBlendState();
	IGXRasterizerState *pOldRS = pCtx->getRasterizerState();

	m_pEditable->m_pMaterialSystem->bindTexture(m_pEditable->m_pWhiteTexture);
	//pDevice->setTexture(m_pEditable->m_pWhiteTexture);
	pCtx->setBlendState(m_pEditable->m_pBlendColorFactor);

	if(is3D)
	{
		pCtx->setBlendFactor(GX_COLOR_ARGB(70, 255, 0, 0));
		m_pEntity->renderEditor(is3D);

		SAFE_CALL(m_pModel, render, 0, MF_OPAQUE | MF_TRANSPARENT);
	}

	pCtx->setRasterizerState(m_pEditable->m_pRSWireframe);
	pCtx->setBlendFactor(GX_COLOR_ARGB(255, 255, 255, 0));
	m_pEntity->renderEditor(is3D);
	SAFE_CALL(m_pModel, render, 0, MF_OPAQUE | MF_TRANSPARENT);

	pCtx->setBlendState(pOldBlendState);
	pCtx->setRasterizerState(pOldRS);
	mem_release(pOldBlendState);
	mem_release(pOldRS);
#endif
}

bool XMETHODCALLTYPE CEditorObject::rayTest(const float3 &vStart, const float3 &vEnd, float3 *pvOut, float3 *pvNormal, ID *pidMtrl, bool bReturnNearestPoint)
{
	float3 vMin, vMax;
	getBound(&vMin, &vMax);
	if(SMRayIntersectAABB(SMAABB(vMin, vMax), vStart, vEnd - vStart))
	{
		if(bReturnNearestPoint)
		{
			float3 vOut, vNormal, *pNormal = NULL;
			if(pNormal)
			{
				pNormal = &vNormal;
			}

			bool isFound = false;
			float fDist, fDist2 = FLT_MAX;
			for(UINT i = 0, l = m_aBrushes.size(); i < l; ++i)
			{
				if(m_aBrushes[i]->rayTest(vStart, vEnd, &vOut, pNormal, true, bReturnNearestPoint) &&
					(fDist = SMVector3Length2(vStart - vOut)) < fDist2
				)
				{
					fDist2 = fDist;
					isFound = true;
				}
			}

			if(isFound)
			{
				if(pvOut)
				{
					*pvOut = vOut;
				}
				if(pvNormal)
				{
					*pvNormal = vNormal;
				}
				return(true);
			}
		}
		else
		{
			for(UINT i = 0, l = m_aBrushes.size(); i < l; ++i)
			{
				if(m_aBrushes[i]->rayTest(vStart, vEnd, pvOut, pvNormal, true, bReturnNearestPoint))
				{
					return(true);
				}
			}
		}
	}
	return(false);
}

void XMETHODCALLTYPE CEditorObject::remove()
{
	for(UINT i = 0, l = m_aBrushes.size(); i < l; ++i)
	{
		m_aBrushes[i]->enable(false);
	}
}
void XMETHODCALLTYPE CEditorObject::preSetup()
{
}
void XMETHODCALLTYPE CEditorObject::postSetup()
{
	
}

void XMETHODCALLTYPE CEditorObject::create()
{
	for(UINT i = 0, l = m_aBrushes.size(); i < l; ++i)
	{
		m_aBrushes[i]->enable(true);
	}
}

void XMETHODCALLTYPE CEditorObject::setKV(const char *szKey, const char *szValue)
{
	if(!fstrcmp(szKey, "brush"))
	{
		IXJSON *pJSON = (IXJSON*)m_pEditable->getCore()->getPluginManager()->getInterface(IXJSON_GUID);
		IXJSONItem *pRoot;
		if(pJSON->parse(szValue, &pRoot))
		{
			setKV(szKey, pRoot);
			
			mem_release(pRoot);
		}

		fixPos();
	}
	//if(m_pEntity)
	//{
	//	m_pEntity->setKV(szKey, szValue);
	//}
}

void CEditorObject::setKV(const char *szKey, IXJSONItem *pValue, bool bSkipFixPos)
{
	if(!fstrcmp(szKey, "brush"))
	{
		IXJSONArray *pArray = pValue->asArray();
		if(pArray)
		{
			for(UINT i = m_aBrushes.size(), l = pArray->size(); i < l; ++i)
			{
				m_aBrushes[i] = new CBrushMesh(m_pEditable->getCore());
			}

			for(UINT i = pArray->size(), l = m_aBrushes.size(); i < l; ++i)
			{
				mem_delete(m_aBrushes[i]);
			}

			m_aBrushes.resize(pArray->size());

			for(UINT i = 0, l = pArray->size(); i < l; ++i)
			{
				if(!m_aBrushes[i]->deserialize(pArray->at(i)->asObject()))
				{
					mem_delete(m_aBrushes[i]);
				}
			}

			for(UINT i = 0, l = m_aBrushes.size(); i < l; ++i)
			{
				if(!m_aBrushes[i])
				{
					m_aBrushes.erase(i);
					--i; --l;
				}
			}
		}

		float3_t vOldPos = getPos();
		fixPos();
		if(!bSkipFixPos)
		{
			setPos(vOldPos);
		}
	}
}

const char* XMETHODCALLTYPE CEditorObject::getKV(const char *szKey)
{
	if(!fstrcmp(szKey, "brush"))
	{
		m_aSerializedState.clearFast();
		m_aSerializedState.push_back('[');
		for(UINT i = 0, l = m_aBrushes.size(); i < l; ++i)
		{
			if(i != 0)
			{
				m_aSerializedState.push_back(',');
			}

			m_aBrushes[i]->serialize(&m_aSerializedState);
		}
		m_aSerializedState.push_back(']');
		m_aSerializedState.push_back(0);

		return(m_aSerializedState);
	}


	//if(!m_pEntity)
	{
		return(NULL);
	}
#if 0
	char tmp[4096];

	if(!fstrcmp(szKey, "guid"))
	{
		XGUIDToSting(*m_pEntity->getGUID(), tmp, sizeof(tmp));
	}
	else
	{
		m_pEntity->getKV(szKey, tmp, sizeof(tmp));
	}

	m_msPropCache[szKey] = tmp;
	return(m_msPropCache[szKey].c_str());
#endif
}
const X_PROP_FIELD* XMETHODCALLTYPE CEditorObject::getPropertyMeta(UINT uKey)
{
	static X_PROP_FIELD s_prop0 = {"brush", "brush", XPET_TEXT};
	switch(uKey)
	{
	case 0:
		return(&s_prop0);
	}
	return(NULL);
}
UINT XMETHODCALLTYPE CEditorObject::getProperyCount()
{
	return(1);
}

const char* XMETHODCALLTYPE CEditorObject::getTypeName()
{
	return(m_pEditable->getName());
}
const char* XMETHODCALLTYPE CEditorObject::getClassName()
{
	return("Brush");
}

void XMETHODCALLTYPE CEditorObject::setSelected(bool set)
{
	m_isSelected = set;

	//m_pEditable->onSelectionChanged(this);
}

void XMETHODCALLTYPE CEditorObject::setSimulationMode(bool set)
{
	//SAFE_CALL(m_pModel, enable, !set);
}

bool XMETHODCALLTYPE CEditorObject::hasVisualModel()
{
	return(true);
}

void CEditorObject::addBrush(CBrushMesh *pBrushMesh)
{
	m_aBrushes.push_back(pBrushMesh);
}
