#include "EditorObject.h"

//#include <render/sxrender.h>
#include <mtrl/sxmtrl.h>

#include "Editable.h"

CEditorObject::CEditorObject(CEditable *pEditable, CBaseEntity *pEntity):
	m_pEditable(pEditable),
	m_pEntity(pEntity)
{
	assert(pEntity);

	m_szClassName = pEntity->getClassName();

	m_vPos = pEntity->getPos();
	//@TODO: fix me
	m_vScale = float3(1.0f);
	m_qRot = pEntity->getOrient();
}

CEditorObject::CEditorObject(CEditable *pEditable, const char *szClassName):
	m_pEditable(pEditable)
{
	m_pEntity = CREATE_ENTITY(szClassName, GameData::m_pMgr);
	m_szClassName = m_pEntity->getClassName();
}

CEditorObject::~CEditorObject()
{
	
}

void CEditorObject::setPos(const float3_t &pos)
{
	if(m_pEntity)
	{
		m_pEntity->setPos(pos);
	}
	BaseClass::setPos(pos);
}

void CEditorObject::setScale(const float3_t &pos)
{
	//@TODO: Implement me
	BaseClass::setScale(pos);
}

void CEditorObject::setOrient(const SMQuaternion &orient)
{
	if(m_pEntity)
	{
		m_pEntity->setOrient(orient);
	}

	BaseClass::setOrient(orient);
}

void CEditorObject::getBound(float3 *pvMin, float3 *pvMax)
{
	if(!m_pEntity)
	{
		*pvMin = m_vPos - float3(0.1f, 0.1f, 0.1f);
		*pvMax = m_vPos + float3(0.1f, 0.1f, 0.1f);
		return;
	}
	m_pEntity->getMinMax(pvMin, pvMax);
}

void CEditorObject::renderSelection(bool is3D)
{
	/*if(!ID_VALID(m_idModel))
	{
		return;
	}

	IGXContext *pDevice = SGCore_GetDXDevice();
	IGXBlendState *pOldBlendState = pDevice->getBlendState();
	IGXRasterizerState *pOldRS = pDevice->getRasterizerState();
	
	m_pEditable->m_pMaterialSystem->bindTexture(m_pEditable->m_pWhiteTexture);
	//pDevice->setTexture(m_pEditable->m_pWhiteTexture);
	pDevice->setBlendState(m_pEditable->m_pBlendColorFactor);

	if(is3D)
	{
		pDevice->setBlendFactor(GXCOLOR_ARGB(70, 255, 0, 0));
		SGeom_RenderSingly(0, m_idModel, SMtrl_MtlGetStdMtl(MTLTYPE_MODEL_STATIC));
	}

	pDevice->setRasterizerState(m_pEditable->m_pRSWireframe);
	pDevice->setBlendFactor(GXCOLOR_ARGB(255, 255, 255, 0));
	SGeom_RenderSingly(0, m_idModel, SMtrl_MtlGetStdMtl(MTLTYPE_MODEL_STATIC));

	pDevice->setBlendState(pOldBlendState);
	pDevice->setRasterizerState(pOldRS);
	mem_release(pOldBlendState);
	mem_release(pOldRS);*/
}

bool CEditorObject::rayTest(const float3 &vStart, const float3 &vEnd, float3 *pvOut, ID *pidMtrl)
{
	// return(SGeom_TraceBeamId(m_idModel, &vStart, &vEnd, pvOut, pidMtrl));
	return(false);
}

void CEditorObject::remove()
{
	if(!m_pEntity)
	{
		return;
	}
	REMOVE_ENTITY(m_pEntity);
	m_pEntity = NULL;
}
void CEditorObject::preSetup()
{
}
void CEditorObject::postSetup()
{
	
}

void CEditorObject::create()
{
	assert(!m_pEntity);
	m_pEntity = CREATE_ENTITY(m_szClassName, GameData::m_pMgr);

	setPos(getPos());
	setOrient(getOrient());
	setScale(getScale());
}

void CEditorObject::setKV(const char *szKey, const char *szValue)
{
	if(m_pEntity)
	{
		m_pEntity->setKV(szKey, szValue);
	}
}
const char *CEditorObject::getKV(const char *szKey)
{
#if 0
	if(!fstrcmp(szKey, "model"))
	{
		return(m_sModelName.c_str());
	}
	else if(!fstrcmp(szKey, "name"))
	{
		return(m_sName.c_str());
	}
	else if(!fstrcmp(szKey, "segmentate"))
	{
		m_szSegmentation[0] = m_bSegmentation ? '1' : '0';
		m_szSegmentation[1] = 0;
		return(m_szSegmentation);
	}
#endif
	return(NULL);
}
const X_PROP_FIELD *CEditorObject::getPropertyMeta(UINT uKey)
{
	static const X_PROP_FIELD s_szKeys[] = {
		{"model", "Model file", XPET_TEXT, NULL, ""},
		{"name", "Name", XPET_TEXT, NULL, ""},
		{"segmentate", "Segmentate", XPET_TEXT /* XPET_YESNO */, NULL, ""}
	};

	if(uKey > 2)
	{
		return(NULL);
	}
	return(&s_szKeys[uKey]);
}
UINT CEditorObject::getProperyCount()
{
	return(3);
}

const char *CEditorObject::getTypeName()
{
	return(m_pEditable->getName());
}
const char *CEditorObject::getClassName()
{
	return(m_szClassName);
}
