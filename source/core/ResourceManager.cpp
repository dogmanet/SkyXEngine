#include "ResourceManager.h"
#include <xcommon/IPluginManager.h>
#include "ResourceModelStatic.h"
#include "ResourceModelAnimated.h"

#if 1
#include "File.h"


#include <xcommon/resource/IXModelProvider.h>
#include <xcommon/resource/IXResourceManager.h>
#include <xcommon/resource/IXResourceModel.h>
#endif



CResourceManager::CResourceManager(IXCore *pCore):
	m_pCore(pCore)
{
	IPluginManager *pPluginManager = m_pCore->getPluginManager();

	IXModelLoader *pLoader;
	UINT ic = 0;
	while((pLoader = (IXModelLoader*)pPluginManager->getInterface(IXMODELLOADER_GUID, ic++)))
	{
		if(pLoader->getVersion() == IXMODELLOADER_VERSION)
		{
			for(UINT i = 0, l = pLoader->getExtCount(); i < l; ++i)
			{
				AAString sExt;
				strlwr(const_cast<char*>(sExt.getName()));
				sExt.setName(pLoader->getExt(i));
				m_mapModelLoaders[sExt].push_back(pLoader);
				m_aModelExts.push_back({pLoader->getDescription(), pLoader->getExt(i)});
			}
		}
	}

}

bool CResourceManager::getModel(const char *szName, IXResourceModel **ppOut, bool bForceReload)
{
	const AssotiativeArray<String, IXResourceModel*>::Node *pNode1;
	if(!bForceReload && m_mpModels.KeyExists(szName, &pNode1) && (*pNode1->Val))
	{
		(*pNode1->Val)->AddRef();
		*ppOut = *pNode1->Val;
		return(true);
	}

	size_t iLen = strlen(szName);
	const char *szExt = NULL;
	for(size_t i = iLen - 1; i > 0; --i)
	{
		if(szName[i] == '.')
		{
			szExt = &szName[i + 1];
			break;
		}
	}

	if(!szExt || !*szExt)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "Invalid model name '%s'. Missing extension\n", szName);
		return(false);
	}
	iLen = strlen(szExt);

	char *szLowcaseExt = (char*)alloca(sizeof(char) * (iLen + 1));
	memcpy(szLowcaseExt, szExt, sizeof(char) * (iLen + 1));
	strlwr(szLowcaseExt);

	CResourceModel *pResource = NULL;
	const AssotiativeArray<AAString, Array<IXModelLoader*>>::Node *pNode;
	if(m_mapModelLoaders.KeyExists(AAString(szLowcaseExt), &pNode))
	{
		auto &aLoaders = *pNode->Val;
#if 1
		CFile *pFile = new CFile();
		if(pFile->open(szName, CORE_FILE_BIN) < 0)
		{
			mem_delete(pFile);
		}
#else
		IFile *pFile = m_pCore->getFileSystem()->openFileBin(szName);
#endif
		if(!pFile)
		{
			LibReport(REPORT_MSG_LEVEL_ERROR, "File not found '%s'\n", szName);
			return(false);
		}
		for(UINT i = 0, l = aLoaders.size(); i < l; ++i)
		{
			IXModelLoader *pLoader = aLoaders[i];
			if(pLoader->open(pFile))
			{
				switch(pLoader->getType())
				{
				case XMT_STATIC:
					{
						CResourceModelStatic *pModel = new CResourceModelStatic(this);
						if(pLoader->loadAsStatic(pModel))
						{
							pResource = pModel;
						}
						else
						{
							mem_delete(pModel);
						}
					}
					break;
				case XMT_ANIMATED:
					{
						CResourceModelAnimated *pModel = new CResourceModelAnimated(this);
						if(pLoader->loadAsAnimated(pModel))
						{
							pResource = pModel;
						}
						else
						{
							mem_delete(pModel);
						}
					}
					break;
				}
				pLoader->close();

				if(pResource)
				{
					break;
				}
			}
			pFile->setPos(0);
		}

		mem_release(pFile);

		if(pResource)
		{
			*ppOut = pResource;
			if(bForceReload && m_mpModels.KeyExists(szName))
			{
				mem_release(m_mpModels[szName]);
			}
			m_mpModels[szName] = pResource;
			pResource->setFileName(m_mpModels.TmpNode->Key.c_str());

			for(UINT i = 0, l = pResource->getGibsCount(); i < l; ++i)
			{
				IXResourceModel *pGibResource;
				if(getModel(pResource->getGibName(i), &pGibResource, bForceReload))
				{
					pResource->setGib(i, pGibResource);
				}
				else
				{
					LibReport(REPORT_MSG_LEVEL_ERROR, "Unable to load gib #%u for model '%s'\n", i, szName);
					mem_release(pResource);
					return(false);
				}
			}

			if(pResource->getType() == XMT_ANIMATED)
			{
				CResourceModelAnimated *pModel = (CResourceModelAnimated*)pResource;
				for(UINT i = 0, l = pModel->getImportsCount(); i < l; ++i)
				{
					IXResourceModel *pImportResource;
					if(getModel(pModel->getImportName(i), &pImportResource, bForceReload))
					{
						pModel->setImport(i, pImportResource);
					}
					else
					{
						LibReport(REPORT_MSG_LEVEL_ERROR, "Unable to load import #%u for model '%s'\n", i, szName);
						mem_release(pResource);
						return(false);
					}
				}

				for(UINT i = 0, l = pModel->getPartsCount(); i < l; ++i)
				{
					IXResourceModel *pPartResource;
					if(getModel(pModel->getPartName(i), &pPartResource, bForceReload))
					{
						pModel->setPart(i, pPartResource);
					}
					else
					{
						LibReport(REPORT_MSG_LEVEL_ERROR, "Unable to load part #%u for model '%s'\n", i, szName);
						mem_release(pResource);
						return(false);
					}
				}
			}

			return(true);
		}
		else
		{
			LibReport(REPORT_MSG_LEVEL_ERROR, "No one plugin could load model '%s'\n", szName);
			return(false);
		}
	}
	else
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "No plugins for model extension found '%s'\n", szName);
		return(false);
	}

	assert(!"Should never get here!");
	return(false);
}
bool CResourceManager::getModelStatic(const char *szName, IXResourceModelStatic **ppOut, bool bForceReload)
{
	IXResourceModel *pModel = NULL;
	if(getModel(szName, &pModel, bForceReload))
	{
		if(pModel->getType() == XMT_STATIC)
		{
			*ppOut = pModel->asStatic();
			return(true);
		}
		mem_release(pModel);
	}
	*ppOut = NULL;
	return(false);
}
bool CResourceManager::getModelAnimated(const char *szName, IXResourceModelAnimated **ppOut, bool bForceReload)
{
	IXResourceModel *pModel = NULL;
	if(getModel(szName, &pModel, bForceReload))
	{
		if(pModel->getType() == XMT_ANIMATED)
		{
			*ppOut = pModel->asAnimated();
			return(true);
		}
		mem_release(pModel);
	}
	*ppOut = NULL;
	return(false);
}

void CResourceManager::onResourceModelRelease(CResourceModel *pResource)
{
	m_mpModels[pResource->getFileName()] = NULL;
}

UINT XMETHODCALLTYPE CResourceManager::getModelSupportedFormats()
{
	return(m_aModelExts.size());
}
const XFormatName* XMETHODCALLTYPE CResourceManager::getModelSupportedFormat(UINT uIndex)
{
	assert(uIndex < m_aModelExts.size());

	return(&m_aModelExts[uIndex]);
}
