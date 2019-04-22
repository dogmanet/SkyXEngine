#include "EditorObject.h"

//#include <render/sxrender.h>
#include <mtrl/sxmtrl.h>

#include "Editable.h"

Array<CEditorObject*> CEditorObject::ms_aObjects;

CEditorObject::CEditorObject(CEditable *pEditable, ID idModel):
	m_pEditable(pEditable)
{
	assert(ID_VALID(idModel));

	m_idModel = idModel;
	m_vPos = *SGeom_ModelGetPosition(idModel);
	m_sName = SGeom_ModelGetName(m_idModel);
	m_sModelName = SGeom_ModelGetPath4Model(m_idModel);
	m_bSegmentation = SGeom_ModelGetSegmentation(m_idModel);
	m_vScale = *SGeom_ModelGetScale(m_idModel);

	const float3 *rot = SGeom_ModelGetRotation(m_idModel);
	m_qRot = SMQuaternion(-rot->x, 'x') * SMQuaternion(-rot->y, 'y') * SMQuaternion(-rot->z, 'z');

	m_isCreated = true;

	ms_aObjects.push_back(this);
}

CEditorObject::CEditorObject(CEditable *pEditable):
	m_pEditable(pEditable)
{
	ms_aObjects.push_back(this);
}

CEditorObject::~CEditorObject()
{
	for(UINT i = 0, l = ms_aObjects.size(); i < l; ++i)
	{
		if(ms_aObjects[i] == this)
		{
			ms_aObjects.erase(i);
			break;
		}
	}
}

void CEditorObject::setPos(const float3_t &pos)
{
	if(ID_VALID(m_idModel))
	{
		SGeom_ModelSetPosition(m_idModel, &float3(pos));
	}
	BaseClass::setPos(pos);
}

void CEditorObject::setScale(const float3_t &pos)
{
	if(ID_VALID(m_idModel))
	{
		SGeom_ModelSetScale(m_idModel, &float3(pos));
	}
	BaseClass::setScale(pos);
}

void CEditorObject::setOrient(const SMQuaternion &orient)
{
	if(ID_VALID(m_idModel))
	{
		float3 vRotation = SMMatrixToEuler(orient.GetMatrix());
		vRotation.x *= -1.0f;
		vRotation.z *= -1.0f;
		SGeom_ModelSetRotation(m_idModel, &vRotation);
	}

	BaseClass::setOrient(orient);
}

void CEditorObject::getBound(float3 *pvMin, float3 *pvMax)
{
	if(!ID_VALID(m_idModel))
	{
		*pvMin = m_vPos - float3(0.1f, 0.1f, 0.1f);
		*pvMax = m_vPos + float3(0.1f, 0.1f, 0.1f);
		return;
	}
	SGeom_ModelGetMinMax(m_idModel, pvMin, pvMax);
}

void CEditorObject::renderSelection(bool is3D)
{
	if(!ID_VALID(m_idModel))
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
	mem_release(pOldRS);
}

bool CEditorObject::rayTest(const float3 &vStart, const float3 &vEnd, float3 *pvOut, ID *pidMtrl)
{
	return(SGeom_TraceBeamId(m_idModel, &vStart, &vEnd, pvOut, pidMtrl));
}

void CEditorObject::remove()
{
	if(!ID_VALID(m_idModel))
	{
		return;
	}
	m_isCreated = false;
	SGeom_ModelDelete(m_idModel);

	for(UINT i = 0, l = ms_aObjects.size(); i < l; ++i)
	{
		if(ms_aObjects[i]->m_idModel > m_idModel)
		{
			--ms_aObjects[i]->m_idModel;
		}
	}

	m_idModel = -1;
}
void CEditorObject::preSetup()
{
}
void CEditorObject::postSetup()
{
	
}

void CEditorObject::create()
{
	assert(!ID_VALID(m_idModel));
	m_isCreated = true;
	if(m_sModelName.length())
	{
		m_idModel = SGeom_ModelAdd(m_sModelName.c_str(), m_sName.c_str(), NULL, NULL, m_bSegmentation);
		setPos(getPos());
		setOrient(getOrient());
		setScale(getScale());
	}
}

void CEditorObject::setKV(const char *szKey, const char *szValue)
{
	if(!fstrcmp(szKey, "model"))
	{
		m_sModelName = szValue;
		if(m_isCreated)
		{
			if(ID_VALID(m_idModel))
			{
				SGeom_ModelDelete(m_idModel);
			}
			if(m_sModelName.length())
			{
				m_idModel = SGeom_ModelAdd(m_sModelName.c_str(), m_sName.c_str(), NULL, NULL, m_bSegmentation);
			}
			else
			{
				m_idModel = -1;
			}
		}
	}
	else if(!fstrcmp(szKey, "name"))
	{
		m_sName = szValue;
		if(ID_VALID(m_idModel))
		{
			SGeom_ModelSetName(m_idModel, szValue);
		}
	}
	else if(!fstrcmp(szKey, "segmentate"))
	{
		m_bSegmentation = !fstrcmp(szKey, "1") || !fstrcmp(szKey, "true") || !fstrcmp(szKey, "yes");
		if(ID_VALID(m_idModel))
		{
			SGeom_ModelDelete(m_idModel);
			m_idModel = SGeom_ModelAdd(m_sModelName.c_str(), m_sName.c_str(), NULL, NULL, m_bSegmentation);
		}
	}
}
const char *CEditorObject::getKV(const char *szKey)
{
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
	return("model");
}
