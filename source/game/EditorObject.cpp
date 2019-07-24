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

	_iniFieldList();

	m_vPos = pEntity->getPos();
	//@TODO: fix me
	m_vScale = float3(1.0f);
	m_qRot = pEntity->getOrient();
}

CEditorObject::CEditorObject(CEditable *pEditable, const char *szClassName):
	m_pEditable(pEditable)
{
	m_szClassName = CEntityFactoryMap::GetInstance()->getClassNamePtr(szClassName);
	assert(m_szClassName);

	_iniFieldList();
}

CEditorObject::~CEditorObject()
{
	
}

void CEditorObject::_iniFieldList()
{
	proptable_t *pTable = CEntityFactoryMap::GetInstance()->getPropTable(m_szClassName);
	propdata_t *pField = NULL;
	X_PROP_FIELD xField;
	while(pTable)
	{
		for(int i = 0; i < pTable->numFields; ++i)
		{
			pField = &pTable->pData[i];
			if(
				!(pField->flags & (PDFF_INPUT | PDFF_MESSAGE | PDFF_NOEDIT | PDFF_OUTPUT))
				&& pField->szKey 
				&& pField->szEdName
				&& pField->editor.type != PDE_NONE
				)
			{
				switch(pField->editor.type)
				{
				case PDE_COMBOBOX:
					xField.editorType = XPET_COMBO;
					xField.pEditorData = pField->editor.pData;
					break;
				case PDE_FILEFIELD:
					xField.editorType = XPET_FILE;
					xField.pEditorData = NULL;
					{
						editor_kv *pKV = (editor_kv*)pField->editor.pData;
						if(pKV && pKV[0].value)
						{
							if(!fstrcmp(pKV[0].value, "dse"))
							{
								xField.pEditorData = "model";
							}
							else if(!fstrcmp(pKV[0].value, "ogg"))
							{
								xField.pEditorData = "sound";
							}
						}
						//xField.pEditorData
					}
					break;
				case PDE_FLAGS:
					continue;
					break;
				case PDE_TEXTFIELD:
					xField.editorType = XPET_TEXT; 
					break;
				}
				xField.szHelp = "";
				xField.szKey = pField->szKey;
				xField.szName = pField->szEdName;

				m_aFields.push_back(xField);
			}
		}

		pTable = pTable->pBaseProptable;
	}
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
	*pvMin = *pvMax = float3();
	if(m_pEntity)
	{
		m_pEntity->getMinMax(pvMin, pvMax);
	}

	if(SMVector3Length2(*pvMax - *pvMin) < 0.0001f)
	{
		*pvMin = -(*pvMax = float3(0.1f, 0.1f, 0.1f));
	}

	*pvMin += m_vPos;
	*pvMax += m_vPos;
}

void CEditorObject::renderSelection(bool is3D)
{
	if(!m_pEntity)
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
		pDevice->setBlendFactor(GX_COLOR_ARGB(70, 255, 0, 0));
		m_pEntity->renderEditor(is3D);
	}

	pDevice->setRasterizerState(m_pEditable->m_pRSWireframe);
	pDevice->setBlendFactor(GX_COLOR_ARGB(255, 255, 255, 0));
	m_pEntity->renderEditor(is3D);

	pDevice->setBlendState(pOldBlendState);
	pDevice->setRasterizerState(pOldRS);
	mem_release(pOldBlendState);
	mem_release(pOldRS);
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

	m_pEntity->setFlags(m_pEntity->getFlags() | EF_LEVEL | EF_EXPORT);

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
	if(!m_pEntity)
	{
		return(NULL);
	}

	char tmp[4096];

	m_pEntity->getKV(szKey, tmp, sizeof(tmp));

	m_msPropCache[szKey] = tmp;
	return(m_msPropCache[szKey].c_str());
}
const X_PROP_FIELD *CEditorObject::getPropertyMeta(UINT uKey)
{
	assert(uKey < m_aFields.size());

	return(&m_aFields[uKey]);
}
UINT CEditorObject::getProperyCount()
{
	return(m_aFields.size());
}

const char *CEditorObject::getTypeName()
{
	return(m_pEditable->getName());
}
const char *CEditorObject::getClassName()
{
	return(m_szClassName);
}
