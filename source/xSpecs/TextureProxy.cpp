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

bool XMETHODCALLTYPE CTextureProxy::resolveName(const char *szName, char *szOutput, UINT *puBufSize)
{
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
	return(false);
}
