#include "Editable.h"
#include "EditorObject.h"
#include "models.h"

extern CModels *g_pModels;

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
	blendDesc.renderTarget[0].useBlend = true;
	blendDesc.renderTarget[0].blendSrcColor = blendDesc.renderTarget[0].blendSrcAlpha = GXBLEND_BLEND_FACTOR;
	blendDesc.renderTarget[0].blendDestColor = blendDesc.renderTarget[0].blendDestAlpha = GXBLEND_INV_BLEND_FACTOR;
	m_pBlendColorFactor = m_pDevice->createBlendState(&blendDesc);

	GXRASTERIZER_DESC rsDesc;
	rsDesc.cullMode = GXCULL_NONE;
	rsDesc.fillMode = GXFILL_WIREFRAME;
	m_pRSWireframe = m_pDevice->createRasterizerState(&rsDesc);

	IPluginManager *pPluginManager = m_pCore->getPluginManager();
	IXMaterialSystem *pMaterialSystem = (IXMaterialSystem*)pPluginManager->getInterface(IXMATERIALSYSTEM_GUID);
	m_pMaterialSystem = pMaterialSystem;

	pMaterialSystem->getTexture("dev_white", &m_pWhiteTexture);
}
void CEditable::shutdown()
{
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
	return(g_pModels->getCountModel());
}
IXEditorObject *CEditable::getObject(UINT id)
{
	return(new CEditorObject(this, id));
}
IXEditorObject *CEditable::newObject(const char *szClassName)
{
	return(new CEditorObject(this));
}
