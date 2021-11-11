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
	return(m_aObjects.size());
}
IXEditorObject* XMETHODCALLTYPE CEditable::getObject(UINT id)
{
	return(m_aObjects[id]);
}
IXEditorObject* XMETHODCALLTYPE CEditable::newObject(const char *szClassName)
{
	if(!fstrcmp(szClassName, "Brush"))
	{
		return(new CEditorObject(this));
	}
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

void CEditable::save(const char *szLevelName)
{
	char szFile[1024];
	sprintf(szFile, "levels/%s/editor/xcsg.json", szLevelName);

	IFile *pFile = m_pCore->getFileSystem()->openFile(szFile, FILE_MODE_WRITE);
	if(!pFile)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "Unable to save data '%s'\n", szFile);
		return;
	}

	pFile->writeText("{\"brushes\":[\n");

	for(UINT i = 0, l = m_aObjects.size(); i < l; ++i)
	{
		CEditorObject *pObj = m_aObjects[i];
		pFile->writeText("\t%s{\n", i == 0 ? "" : ",");
		for(UINT j = 0, jl = pObj->getProperyCount(); j < jl; ++j)
		{
			const char *pKey = pObj->getPropertyMeta(j)->szKey;
			pFile->writeText("\t\t%s\"%s\": %s\n", j == 0 ? "" : ",", pKey, pObj->getKV(pKey));
		}
		pFile->writeText("\t}\n");
	}

	pFile->writeText("\n]}\n");

	mem_release(pFile);
}

void CEditable::load(const char *szLevelName, ID idPlugin)
{
	char szFile[1024];
	sprintf(szFile, "levels/%s/editor/xcsg.json", szLevelName);

	IFile *pFile = m_pCore->getFileSystem()->openFile(szFile, FILE_MODE_READ);
	if(!pFile)
	{
		return;
	}

	LibReport(REPORT_MSG_LEVEL_NOTICE, "loading level\n");

	IEventChannel<XEventLevelProgress> *pProgressChannel = m_pCore->getEventChannel<XEventLevelProgress>(EVENT_LEVEL_PROGRESS_GUID);
	XEventLevelProgress levelProgress;
	levelProgress.idPlugin = idPlugin;
	levelProgress.fProgress = 0.0f;
	levelProgress.szLoadingText = "Загрузка мира";
	levelProgress.type = XEventLevelProgress::TYPE_PROGRESS_BEGIN;
	pProgressChannel->broadcastEvent(&levelProgress);

	//loadLevel(szPathLevel);

	size_t sizeFile = pFile->getSize();
	char *szJSON = new char[sizeFile + 1];
	pFile->readBin(szJSON, sizeFile);
	szJSON[sizeFile] = 0;

	bool isLoaded = false;

	IXJSON *pJSON = (IXJSON*)m_pCore->getPluginManager()->getInterface(IXJSON_GUID);
	IXJSONItem *pRoot;
	if(pJSON->parse(szJSON, &pRoot))
	{
		IXJSONObject *pRootObj = pRoot->asObject();
		if(pRootObj)
		{
			for(UINT i = 0, l = pRootObj->size(); i < l; ++i)
			{
				const char *szKey = pRootObj->getKey(i);
				if(!strcmp(szKey, "brushes"))
				{
					IXJSONArray *pArr = pRootObj->at(i)->asArray();
					if(pArr)
					{
						for(UINT j = 0, jl = pArr->size(); j < jl; ++j)
						{
							IXJSONObject *pBrushObj = pArr->at(j)->asObject();
							if(pBrushObj)
							{
								CEditorObject *pObj = new CEditorObject(this);
								for(UINT k = 0, kl = pBrushObj->size(); k < kl; ++k)
								{
									pObj->setKV(pBrushObj->getKey(k), pBrushObj->at(k));
								}
							}
						}
						isLoaded = true;
					}
				}
			}

		}

		mem_release(pRoot);
	}

	if(!isLoaded)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "Unable to load '%s'\n", szFile);
	}

	mem_delete_a(szJSON);

	levelProgress.fProgress = 1.0f;
	levelProgress.type = XEventLevelProgress::TYPE_PROGRESS_END;
	pProgressChannel->broadcastEvent(&levelProgress);
	

	mem_release(pFile);
}

void CEditable::unload()
{
	for(UINT i = 0, l = m_aObjects.size(); i < l; ++i)
	{
		mem_release(m_aObjects[i]);
	}
	m_aObjects.clearFast();
}
