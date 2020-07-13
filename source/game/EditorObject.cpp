#include "EditorObject.h"

//#include <render/sxrender.h>
#include <mtrl/sxmtrl.h>

#include "Editable.h"

CEditorObject::CEditorObject(CEditable *pEditable, CBaseEntity *pEntity):
	m_pEditable(pEditable),
	m_pEntity(pEntity),
	m_idEnt(pEntity->getId())
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
	m_pEditable->removeObject(this);
}

void CEditorObject::_iniFieldList()
{
	proptable_t *pTable = CEntityFactoryMap::GetInstance()->getPropTable(m_szClassName);
	propdata_t *pField = NULL;
	X_PROP_FIELD xField;

	for(UINT i = 0; i < 16; ++i)
	{
		m_aszFlags[i] = NULL;
	}

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
					xField.editorType = XPET_FLAGS;
					xField.pEditorData = m_aszFlags;
					break;
				case PDE_TEXTFIELD:
					xField.editorType = XPET_TEXT; 
					break;
				}
				xField.szHelp = "";
				xField.szKey = pField->szKey;
				xField.szName = pField->szEdName;
				xField.isGeneric = !fstrcmp(pField->szKey, "origin") || !fstrcmp(pField->szKey, "rotation") || !fstrcmp(pField->szKey, "scale");

				m_aFields.push_back(xField);
			}

			if(pField->type == PDF_FLAG && pField->flags)
			{
				UINT uFlag = (UINT)pField->flags >> 16;
				uFlag = (UINT)lroundf(log2f((float)uFlag));
				assert(uFlag < 16);

				if(!m_aszFlags[uFlag])
				{
					m_aszFlags[uFlag] = pField->szEdName;
				}
			}
		}

		pTable = pTable->pBaseProptable;
	}
}

void XMETHODCALLTYPE CEditorObject::setPos(const float3_t &pos)
{
	if(m_pEntity)
	{
		m_pEntity->setPos(pos);
	}
	m_vPos = pos;
}

void XMETHODCALLTYPE CEditorObject::setScale(const float3_t &vScale)
{
	//@TODO: Implement me
	m_vScale = vScale;
}

void XMETHODCALLTYPE CEditorObject::setOrient(const SMQuaternion &orient)
{
	if(m_pEntity)
	{
		m_pEntity->setOrient(orient);
	}

	m_qRot = orient;
}

void XMETHODCALLTYPE CEditorObject::getBound(float3 *pvMin, float3 *pvMax)
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

void XMETHODCALLTYPE CEditorObject::renderSelection(bool is3D)
{
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
	}

	pCtx->setRasterizerState(m_pEditable->m_pRSWireframe);
	pCtx->setBlendFactor(GX_COLOR_ARGB(255, 255, 255, 0));
	m_pEntity->renderEditor(is3D);

	pCtx->setBlendState(pOldBlendState);
	pCtx->setRasterizerState(pOldRS);
	mem_release(pOldBlendState);
	mem_release(pOldRS);
}

bool XMETHODCALLTYPE CEditorObject::rayTest(const float3 &vStart, const float3 &vEnd, float3 *pvOut, ID *pidMtrl)
{
	// return(SGeom_TraceBeamId(m_idModel, &vStart, &vEnd, pvOut, pidMtrl));
	return(false);
}

void XMETHODCALLTYPE CEditorObject::remove()
{
	if(!m_pEntity)
	{
		return;
	}
	REMOVE_ENTITY(m_pEntity);
	m_pEntity = NULL;
	m_idEnt = -1;
}
void XMETHODCALLTYPE CEditorObject::preSetup()
{
}
void XMETHODCALLTYPE CEditorObject::postSetup()
{
	
}

void XMETHODCALLTYPE CEditorObject::create()
{
	assert(!m_pEntity);
	m_pEntity = CREATE_ENTITY(m_szClassName, GameData::m_pMgr);

	m_idEnt = m_pEntity->getId();

	m_pEntity->setFlags(m_pEntity->getFlags() | EF_LEVEL | EF_EXPORT);

	setPos(getPos());
	setOrient(getOrient());
	setScale(getScale());
}

void CEditorObject::resync()
{
	if(ID_VALID(m_idEnt))
	{
		m_pEntity = GameData::m_pMgr->getById(m_idEnt);
	}
}

void XMETHODCALLTYPE CEditorObject::setKV(const char *szKey, const char *szValue)
{
	if(m_pEntity)
	{
		m_pEntity->setKV(szKey, szValue);
	}
}
const char* XMETHODCALLTYPE CEditorObject::getKV(const char *szKey)
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
const X_PROP_FIELD* XMETHODCALLTYPE CEditorObject::getPropertyMeta(UINT uKey)
{
	assert(uKey < m_aFields.size());

	return(&m_aFields[uKey]);
}
UINT XMETHODCALLTYPE CEditorObject::getProperyCount()
{
	return(m_aFields.size());
}

const char* XMETHODCALLTYPE CEditorObject::getTypeName()
{
	return(m_pEditable->getName());
}
const char* XMETHODCALLTYPE CEditorObject::getClassName()
{
	return(m_szClassName);
}

void XMETHODCALLTYPE CEditorObject::setSelected(bool set)
{
	m_isSelected = set;

	m_pEditable->onSelectionChanged(this);
}
