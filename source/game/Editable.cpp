#include "Editable.h"

#if defined(_DEBUG)
#pragma comment(lib, "sxmtrl_d.lib")
#else
#pragma comment(lib, "sxmtrl.lib")
#endif

CEditable::CEditable(IXCore *pCore)
{
	m_pCore = pCore;
}

void CEditable::startup(IGXContext *pDevice)
{
	m_pDevice = pDevice;

	GXBLEND_DESC blendDesc;
	blendDesc.renderTarget[0].bBlendEnable = true;
	blendDesc.renderTarget[0].srcBlend = blendDesc.renderTarget[0].srcBlendAlpha = GXBLEND_BLEND_FACTOR;
	blendDesc.renderTarget[0].destBlend = blendDesc.renderTarget[0].destBlendAlpha = GXBLEND_INV_BLEND_FACTOR;
	m_pBlendColorFactor = m_pDevice->createBlendState(&blendDesc);

	GXRASTERIZER_DESC rsDesc;
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
	//GameData::m_pMgr->getCount();
	return(0);
}
IXEditorObject *CEditable::getObject(UINT id)
{
	//return(new CEditorObject(this, id));
	return(NULL);
}
IXEditorObject *CEditable::newObject(const char *szClassName)
{
	//return(new CEditorObject(this));
	return(NULL);
}
