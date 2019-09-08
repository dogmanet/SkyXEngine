#include "Editable.h"
#include "EditorObject.h"

#if defined(_DEBUG)
#pragma comment(lib, "sxmtrl_d.lib")
#else
#pragma comment(lib, "sxmtrl.lib")
#endif

CEditable::CEditable(IXCore *pCore)
{
	m_pCore = pCore;
}

void CEditable::startup(IGXDevice *pDevice)
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

CEditable::~CEditable()
{
	shutdown();
}

UINT CEditable::getObjectCount()
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
IXEditorObject *CEditable::getObject(UINT id)
{
	UINT uResult = 0;
	CBaseEntity *pEnt;
	for(int i = 0, l = GameData::m_pMgr->getCount(); i < l; ++i)
	{
		if((pEnt = GameData::m_pMgr->getById(i)) && (pEnt->getFlags() & EF_LEVEL) && !(pEnt->getFlags() & EF_REMOVED))
		{
			if(uResult == id)
			{
				return(new CEditorObject(this, pEnt));
			}
			++uResult;
		}
	}
	
	return(NULL);
}
IXEditorObject *CEditable::newObject(const char *szClassName)
{
	return(new CEditorObject(this, szClassName));
}
