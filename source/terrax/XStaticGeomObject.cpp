#include "XStaticGeomObject.h"
#include <render/sxrender.h>
#include <mtrl/sxmtrl.h>
#include "terrax.h"

Array<CXStatixGeomObject*> CXStatixGeomObject::ms_aObjects;

CXStatixGeomObject::CXStatixGeomObject(ID idModel)
{
	assert(ID_VALID(idModel));

	m_idModel = idModel;
	m_vPos = *SGeom_ModelGetPosition(idModel);
	m_sName = SGeom_ModelGetName(m_idModel);
	m_sModelName = SGeom_ModelGetPath4Model(m_idModel);
	m_bSegmentation = SGeom_ModelGetSegmentation(m_idModel);

	ms_aObjects.push_back(this);
}

CXStatixGeomObject::CXStatixGeomObject()
{
	ms_aObjects.push_back(this);
}

CXStatixGeomObject::~CXStatixGeomObject()
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

void CXStatixGeomObject::setPos(const float3_t &pos)
{
	SGeom_ModelSetPosition(m_idModel, &float3(pos));
	BaseClass::setPos(pos);
}

float3_t CXStatixGeomObject::getScale()
{
	return(*SGeom_ModelGetScale(m_idModel));
}
void CXStatixGeomObject::setScale(const float3_t &pos)
{
	SGeom_ModelSetScale(m_idModel, &float3(pos));
}

SMQuaternion CXStatixGeomObject::getOrient()
{
	const float3 *rot = SGeom_ModelGetRotation(m_idModel);
	
	return(SMQuaternion(-rot->x, 'x') * SMQuaternion(-rot->y, 'y') * SMQuaternion(-rot->z, 'z'));
}
void CXStatixGeomObject::setOrient(const SMQuaternion &orient)
{
	float3 vRotation = SMMatrixToEuler(orient.GetMatrix());
	vRotation.x *= -1.0f;
	vRotation.z *= -1.0f;
	SGeom_ModelSetRotation(m_idModel, &vRotation);
}

void CXStatixGeomObject::getBound(float3 *pvMin, float3 *pvMax)
{
	SGeom_ModelGetMinMax(m_idModel, pvMin, pvMax);
}

void CXStatixGeomObject::renderSelection(bool is3D)
{
	IGXContext *pDevice = SGCore_GetDXDevice();
	IGXBlendState *pOldBlendState = pDevice->getBlendState();
	IGXRasterizerState *pOldRS = pDevice->getRasterizerState();

	pDevice->setTexture(SGCore_LoadTexGetTex(SRender_EditorGetWhiteTex()));
	pDevice->setBlendState(g_xRenderStates.pBlendColorFactor);

	if(is3D)
	{
		pDevice->setBlendFactor(GXCOLOR_ARGB(70, 255, 0, 0));
		SGeom_RenderSingly(0, m_idModel, SMtrl_MtlGetStdMtl(MTLTYPE_MODEL_STATIC));
	}

	pDevice->setRasterizerState(g_xRenderStates.pRSWireframe);
	pDevice->setBlendFactor(GXCOLOR_ARGB(255, 255, 255, 0));
	SGeom_RenderSingly(0, m_idModel, SMtrl_MtlGetStdMtl(MTLTYPE_MODEL_STATIC));

	pDevice->setBlendState(pOldBlendState);
	pDevice->setRasterizerState(pOldRS);
	mem_release(pOldBlendState);
	mem_release(pOldRS);
}

bool CXStatixGeomObject::rayTest(const float3 &vStart, const float3 &vEnd, float3 *pvOut, ID *pidMtrl)
{
	return(SGeom_TraceBeamId(m_idModel, &vStart, &vEnd, pvOut, pidMtrl));
}

void CXStatixGeomObject::remove()
{
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
void CXStatixGeomObject::preCreate()
{
}
void CXStatixGeomObject::postCreate()
{
	m_idModel = SGeom_ModelAdd(m_sModelName.c_str(), m_sName.c_str(), NULL, NULL, m_bSegmentation);
}

void CXStatixGeomObject::setKV(const char *szKey, const char *szValue)
{
	if(!strcmp(szKey, "model"))
	{
		m_sModelName = szValue;
		if(ID_VALID(m_idModel))
		{
			// recreate
		}
	}
	else if(!strcmp(szKey, "name"))
	{
		m_sName = szValue;
		if(ID_VALID(m_idModel))
		{
			SGeom_ModelSetName(m_idModel, szValue);
		}
	}
	else if(!strcmp(szKey, "segmentate"))
	{
		m_bSegmentation = !strcmp(szKey, "1") || !strcmp(szKey, "true") || !strcmp(szKey, "yes");
		if(ID_VALID(m_idModel))
		{
			// recreate
		}
	}
}
const char *CXStatixGeomObject::getKV(const char *szKey)
{
	if(!strcmp(szKey, "model"))
	{
		return(m_sModelName.c_str());
	}
	else if(!strcmp(szKey, "name"))
	{
		return(m_sName.c_str());
	}
	else if(!strcmp(szKey, "segmentate"))
	{
		m_szSegmentation[0] = m_bSegmentation ? '1' : '0';
		m_szSegmentation[1] = 0;
		return(m_szSegmentation);
	}
	return(NULL);
}
const char *CXStatixGeomObject::getPropertyKey(UINT uKey)
{
	static const char *s_szKeys[] = {
		"model",
		"name",
		"segmentate"
	};
	if(uKey > 2)
	{
		return(NULL);
	}
	return(s_szKeys[uKey]);
}
UINT CXStatixGeomObject::getProperyCount()
{
	return(3);
}
