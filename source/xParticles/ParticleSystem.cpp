#include "TextureProxy.h"

CTextureProxy::CTextureProxy(IFileSystem *pFileSystem):
	m_pFileSystem(pFileSystem)
{
}

UINT XMETHODCALLTYPE CTextureProxy::getVersion()
{
	return(IXTEXTUREPROXY_VERSION);
}

const char* XMETHODCALLTYPE CTextureProxy::getDescription() const
{
	return("SkyXEngine texture naming system");
}

bool XMETHODCALLTYPE CTextureProxy::resolveName(const char *szName, char *szOutput, UINT *puBufSize, bool *pbWantLoad)
{
	if(pbWantLoad)
	{
		*pbWantLoad = false;
	}

	const char *szUPos = strstr(szName, "_");
	if(!szUPos)
	{
		return(false);
	}
	size_t sizeTexDir = szUPos - szName;

	const char *szDirName = "textures/";
	size_t sizeBuf = sizeTexDir + strlen(szDirName) + 1 + strlen(szName) + 4 + 1; // "/", ".dds", "\0"

	char *szTexDir = (char*)alloca(sizeof(char) * (sizeTexDir + 1));
	memcpy(szTexDir, szName, sizeof(char) * sizeTexDir);
	szTexDir[sizeTexDir] = 0;

	char *szFullPath = (char*)alloca(sizeof(char) * sizeBuf);
	sprintf(szFullPath, "%s%s/%s.dds", szDirName, szTexDir, szName);
	if(m_pFileSystem->fileExists(szFullPath))
	{
		if(szOutput)
		{
			strncpy(szOutput, szFullPath, *puBufSize);
			szOutput[*puBufSize - 1] = 0;
		}
		else
		{
			*puBufSize = (UINT)sizeBuf + 1;
		}
		return(true);
	}

	sprintf(szFullPath, "%s%s/%s", szDirName, szTexDir, szName);
	if(m_pFileSystem->fileExists(szFullPath))
	{
		if(szOutput)
		{
			strncpy(szOutput, szFullPath, *puBufSize);
			szOutput[*puBufSize - 1] = 0;
		}
		else
		{
			*puBufSize = (UINT)sizeBuf + 1;
		}
		return(true);
	}

	return(false);
}

bool XMETHODCALLTYPE CTextureProxy::loadTexture(const char *szName, IXResourceTexture **ppOut)
{
	return(false);
}

bool XMETHODCALLTYPE CTextureProxy::scanForTextures(IXTextureProxyScanList **ppOut)
{
	*ppOut = new CTextureProxyScanList(m_pFileSystem);
	return(true);
}

//#############################################################################

CTextureProxyScanList::CTextureProxyScanList(IFileSystem *pFileSystem):
	m_pFileSystem(pFileSystem)
{
	const char *szFullDir, *szDir;

	IFileIterator *pIterator = m_pFileSystem->getFolderList("textures");
	if(pIterator)
	{
		while((szFullDir = pIterator->next()))
		{
			// printf("%s\n", szFullDir);
			szDir = basename(szFullDir);

			size_t uLen1 = strlen(szDir);

			IFileIterator *pIter = m_pFileSystem->getFileList(szFullDir, "dds");
			const char *szFile, *szBasename;
			if(pIter)
			{
				while((szFile = pIter->next()))
				{
					// printf("=%s\n", szFile);
					szBasename = basename(szFile);

					size_t uLen2 = strlen(szBasename);

					if(uLen1 < uLen2 && memcmp(szDir, szBasename, uLen1) == 0 && szBasename[uLen1] == '_')
					{
						auto &item = m_aItems[m_aItems.size()];

						item.sFile = szFile;
						item.sName = szBasename;
						item.sName[uLen2 - 4] = 0;
					}
				}
			}
			mem_release(pIter);
		}
	}
	mem_release(pIterator);
}
CTextureProxyScanList::~CTextureProxyScanList()
{
}

UINT XMETHODCALLTYPE CTextureProxyScanList::getCount() const
{
	return(m_aItems.size());
}
const char* XMETHODCALLTYPE CTextureProxyScanList::getItem(UINT uIdx, const char **pszFileName) const
{
	assert(uIdx < m_aItems.size());

	const Item &item = m_aItems[uIdx];

	if(pszFileName)
	{
		*pszFileName = item.sFile.c_str();
	}

	return(item.sName.c_str());
}
