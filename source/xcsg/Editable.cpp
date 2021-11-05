#include "Editable.h"
#include "EditorObject.h"
#include <xcommon/IPluginManager.h>

CEditable::CEditable(IXCore *pCore):
	m_pCore(pCore)
{
	IXEditor *pEditor = (IXEditor*)pCore->getPluginManager()->getInterface(IXEDITOR_GUID);
	m_pEditorExtension = new CEditorExtension(this, pEditor);
}

CEditable::~CEditable()
{
	shutdown();
	mem_delete(m_pEditorExtension);
}

void XMETHODCALLTYPE CEditable::startup(IGXDevice *pDevice)
{
	m_pDevice = pDevice;

	//GXBlendDesc blendDesc;
	//blendDesc.renderTarget[0].useBlend = true;
	//blendDesc.renderTarget[0].blendSrcColor = blendDesc.renderTarget[0].blendSrcAlpha = GXBLEND_BLEND_FACTOR;
	//blendDesc.renderTarget[0].blendDestColor = GXBLEND_ONE;
	//blendDesc.renderTarget[0].blendDestAlpha = GXBLEND_INV_BLEND_FACTOR;
	//m_pBlendColorFactor = m_pDevice->createBlendState(&blendDesc);

	//GXRasterizerDesc rsDesc;
	//rsDesc.cullMode = GXCULL_NONE;
	//rsDesc.fillMode = GXFILL_WIREFRAME;
	//m_pRSWireframe = m_pDevice->createRasterizerState(&rsDesc);

	//IPluginManager *pPluginManager = m_pCore->getPluginManager();
	//IXMaterialSystem *pMaterialSystem = (IXMaterialSystem*)pPluginManager->getInterface(IXMATERIALSYSTEM_GUID);
	//m_pMaterialSystem = pMaterialSystem;

	//pMaterialSystem->getTexture("dev_white", &m_pWhiteTexture);
}
void CEditable::shutdown()
{
	//mem_release(m_pWhiteTexture);
	//mem_release(m_pBlendColorFactor);
	//mem_release(m_pRSWireframe);
}

UINT XMETHODCALLTYPE CEditable::getObjectCount()
{
	UINT uResult = 0;

	return(uResult);
}
IXEditorObject* XMETHODCALLTYPE CEditable::getObject(UINT id)
{
	return(NULL);
}
IXEditorObject* XMETHODCALLTYPE CEditable::newObject(const char *szClassName)
{
	return(NULL);
	//CEditorObject *pObj = new CEditorObject(this, szClassName);
	//m_aObjects.push_back(pObj);
	//return(pObj);
}

void CEditable::removeObject(CEditorObject *pObject)
{
	int idx = m_aObjects.indexOf(pObject);
	if(idx >= 0)
	{
		m_aObjects.erase(idx);
	}
}

//void CEditable::resync()
//{
//	for(UINT i = 0, l = m_aObjects.size(); i < l; ++i)
//	{
//		m_aObjects[i]->resync();
//	}
//}

//void CEditable::onSelectionChanged(CEditorObject *pObject)
//{
//	m_pEditorExtension->onSelectionChanged(pObject);
//}
