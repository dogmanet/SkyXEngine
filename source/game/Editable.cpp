#include "Editable.h"
#include "EditorObject.h"

CEditable::CEditable(IXCore *pCore):
	m_pCore(pCore),
	m_pEditorExtension(new CEditorExtension(this))
{
}

CEditable::~CEditable()
{
	shutdown();
	mem_delete(m_pEditorExtension);
}

void XMETHODCALLTYPE CEditable::startup(IGXDevice *pDevice)
{
	m_pDevice = pDevice;

	GXBlendDesc blendDesc;
	blendDesc.renderTarget[0].useBlend = true;
	blendDesc.renderTarget[0].blendSrcColor = blendDesc.renderTarget[0].blendSrcAlpha = GXBLEND_BLEND_FACTOR;
	blendDesc.renderTarget[0].blendDestColor = GXBLEND_ONE;
	blendDesc.renderTarget[0].blendDestAlpha = GXBLEND_INV_BLEND_FACTOR;
	m_pBlendColorFactor = m_pDevice->createBlendState(&blendDesc);

	GXRasterizerDesc rsDesc;
	rsDesc.cullMode = GXCULL_NONE;
	rsDesc.fillMode = GXFILL_WIREFRAME;
	m_pRSWireframe = m_pDevice->createRasterizerState(&rsDesc);

	IPluginManager *pPluginManager = m_pCore->getPluginManager();
	IXMaterialSystem *pMaterialSystem = (IXMaterialSystem*)pPluginManager->getInterface(IXMATERIALSYSTEM_GUID);
	m_pMaterialSystem = pMaterialSystem;

	pMaterialSystem->getTexture("dev_white", &m_pWhiteTexture);

	m_pszClassList = new const char*[getClassCount()];
	CEntityFactoryMap::GetInstance()->getListing(m_pszClassList, getClassCount());
}
void CEditable::shutdown()
{
	mem_delete_a(m_pszClassList);
	mem_release(m_pWhiteTexture);
	mem_release(m_pBlendColorFactor);
	mem_release(m_pRSWireframe);
}

UINT XMETHODCALLTYPE CEditable::getObjectCount()
{
	UINT uResult = 0;
	CBaseEntity *pEnt;
	for(int i = 0, l = GameData::m_pMgr->getCount(); i < l; ++i)
	{
		if((pEnt = GameData::m_pMgr->getById(i)) && (pEnt->getFlags() & EF_LEVEL) && !(pEnt->getFlags() & EF_REMOVED))
		{
			++uResult;
		}
	}
	return(uResult);
}
IXEditorObject* XMETHODCALLTYPE CEditable::getObject(UINT id)
{
	UINT uResult = 0;
	CBaseEntity *pEnt;
	for(int i = 0, l = GameData::m_pMgr->getCount(); i < l; ++i)
	{
		if((pEnt = GameData::m_pMgr->getById(i)) && (pEnt->getFlags() & EF_LEVEL) && !(pEnt->getFlags() & EF_REMOVED))
		{
			if(uResult == id)
			{
				CEditorObject *pObj = new CEditorObject(this, pEnt);
				m_aObjects.push_back(pObj);
				return(pObj);
			}
			++uResult;
		}
	}
	
	return(NULL);
}
IXEditorObject* XMETHODCALLTYPE CEditable::newObject(const char *szClassName)
{
	CEditorObject *pObj = new CEditorObject(this, szClassName);
	m_aObjects.push_back(pObj);
	return(pObj);
}

void CEditable::removeObject(CEditorObject *pObject)
{
	int idx = m_aObjects.indexOf(pObject);
	if(idx >= 0)
	{
		m_aObjects.erase(idx);
	}
}

void CEditable::resync()
{
	for(UINT i = 0, l = m_aObjects.size(); i < l; ++i)
	{
		m_aObjects[i]->resync();
	}
}

void CEditable::onSelectionChanged(CEditorObject *pObject)
{
	m_pEditorExtension->onSelectionChanged(pObject);
}
