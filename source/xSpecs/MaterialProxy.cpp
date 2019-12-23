#include "MaterialProxy.h"

CMaterialProxy::CMaterialProxy(IFileSystem *pFileSystem):
	m_pFileSystem(pFileSystem)
{
}

const char* XMETHODCALLTYPE CMaterialProxy::getDescription() const
{
	return("SkyXEngine material naming system");
}

bool XMETHODCALLTYPE CMaterialProxy::resolveName(const char *szName, char *szOutput, UINT *puBufSize, bool isForce)
{
	const char *szUPos = strstr(szName, "_");
	if(!szUPos)
	{
		return(false);
	}
	size_t sizeTexDir = szUPos - szName;

	const char *szDirName = "materials/";
	size_t sizeBuf = sizeTexDir + strlen(szDirName) + 1 + strlen(szName) + 4 + 1; // "/", ".mtl", "\0"

	char *szTexDir = (char*)alloca(sizeof(char) * (sizeTexDir + 1));
	memcpy(szTexDir, szName, sizeof(char) * sizeTexDir);
	szTexDir[sizeTexDir] = 0;

	char *szFullPath = (char*)alloca(sizeof(char) * sizeBuf);
	sprintf(szFullPath, "%s%s/%s.mtl", szDirName, szTexDir, szName);
	if(isForce || m_pFileSystem->fileExists(szFullPath))
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
