
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "shader.h"
#include "ShaderPreprocessor.h"

#define SX_SHADER_CACHE_MAGIC MAKEFOURCC('X', 'C', 'S', 'F') /*!< X Compiled Shader File*/
#define SX_SHADER_CACHE_VERSION 3

//##########################################################################

static void CreateShader(IGXContext *pContext, IGXPixelShader    **ppOut, const char *szCode, const char *szFile, GXMacro *pDefs)
{
	*ppOut = pContext->createPixelShaderFromString(szCode, pDefs);
}
static void CreateShader(IGXContext *pContext, IGXVertexShader   **ppOut, const char *szCode, const char *szFile, GXMacro *pDefs)
{
	*ppOut = pContext->createVertexShaderFromString(szCode, pDefs);
}
static void CreateShader(IGXContext *pContext, IGXGeometryShader **ppOut, const char *szCode, const char *szFile, GXMacro *pDefs)
{
	*ppOut = pContext->createGeometryShaderFromString(szCode, pDefs);
}
static void CreateShader(IGXContext *pContext, IGXComputeShader  **ppOut, const char *szCode, const char *szFile, GXMacro *pDefs)
{
	*ppOut = pContext->createComputeShaderFromString(szCode, pDefs);
}

static void CreateShaderFromData(IGXContext *pContext, IGXPixelShader    **ppOut, byte *pCode, UINT uSize)
{
	*ppOut = pContext->createPixelShaderFromBin(pCode, uSize);
}
static void CreateShaderFromData(IGXContext *pContext, IGXVertexShader   **ppOut, byte *pCode, UINT uSize)
{
	*ppOut = pContext->createVertexShaderFromBin(pCode, uSize);
}
static void CreateShaderFromData(IGXContext *pContext, IGXGeometryShader **ppOut, byte *pCode, UINT uSize)
{
	*ppOut = pContext->createGeometryShaderFromBin(pCode, uSize);
}
static void CreateShaderFromData(IGXContext *pContext, IGXComputeShader  **ppOut, byte *pCode, UINT uSize)
{
	*ppOut = pContext->createComputeShaderFromBin(pCode, uSize);
}

template <class T>
static int LoadShader(CShaderPreprocessor *pPreprocessor, IFileSystem *pFileSystem, const char *szPath, CShaderImpl<T> *pShader, GXMacro *aMacro)
{
	assert(Core_GetIXCore()->isOnMainThread());

	char szFullPath[SXGC_SHADER_MAX_SIZE_FULLPATH];
	char szFullPathCache[SXGC_SHADER_MAX_SIZE_FULLPATH];
	char szDir[SXGC_SHADER_MAX_SIZE_DIR];
	szDir[0] = 0;
	char szName[SXGC_SHADER_MAX_SIZE_NAME];
	StrParsePathName(szPath, szDir, szName);
	//! @todo: place actual value here
	const char *szGAPI = "dx11";
	if(szDir[0] != 0)
	{
		sprintf(szFullPath, "shaders/%s/%s", szDir, szPath);
		sprintf(szFullPathCache, "cache/shaders/%s/%s/%sc", szGAPI, szDir, szPath);
	}
	else
	{
		sprintf(szFullPath, "shaders/%s", szPath);
		sprintf(szFullPathCache, "cache/shaders/%s/%sc", szGAPI, szPath);
	}
	bool isSourceExists = pFileSystem->fileExists(szFullPath);
	bool isCacheExists = pFileSystem->fileExists(szFullPathCache);
	if(!isSourceExists && !isCacheExists)
	{
		return(LOAD_SHADER_FAIL);
	}

	if(isCacheExists)
	{
		IFile *pCacheFile = pFileSystem->openFile(szFullPathCache);
		if(pCacheFile)
		{
			uint32_t uMagic = 0;
			pCacheFile->readBin(&uMagic, sizeof(uMagic));
			if(uMagic != SX_SHADER_CACHE_MAGIC)
			{
				LibReport(REPORT_MSG_LEVEL_ERROR, "Invalid shader cache file (%s)\n", szFullPathCache);
			}
			else
			{
				uint32_t uVersion = 0;
				pCacheFile->readBin(&uVersion, sizeof(uVersion));
				if(uVersion != SX_SHADER_CACHE_VERSION)
				{
					LibReport(REPORT_MSG_LEVEL_WARNING, "Wrong shader cache file version (%s)\n", szFullPathCache);
				}
				else
				{
					uint32_t uMacroCount = 0;
					if(aMacro)
					{
						GXMacro *pMacro = aMacro;
						while(pMacro->szName)
						{
							++pMacro;
							++uMacroCount;
						}
					}

					do
					{
						size_t sizeChunkBegin = pCacheFile->getPos();
						uint32_t uChunkSize = 0;
						if(!pCacheFile->readBin(&uChunkSize, sizeof(uChunkSize)))
						{
							break;
						}
						size_t sizeNextBlockPos = pCacheFile->getPos() + uChunkSize;

						UINT uMacroCountFile = 0;
						pCacheFile->readBin(&uMacroCountFile, sizeof(uMacroCountFile));
						bool isMatched = uMacroCount == uMacroCountFile;
						if(isMatched)
						{
							for(UINT i = 0; i < uMacroCountFile; ++i)
							{
								uint8_t u8Len = 0;
								pCacheFile->readBin(&u8Len, sizeof(u8Len));
								char *szName = (char*)alloca(u8Len);
								pCacheFile->readBin(szName, u8Len);

								if(strcmp(aMacro[i].szName, szName))
								{
									isMatched = false;
									break;
								}

								u8Len = 0;
								pCacheFile->readBin(&u8Len, sizeof(u8Len));
								char *szDefinition = (char*)alloca(u8Len);
								pCacheFile->readBin(szDefinition, u8Len);

								if(strcmp(aMacro[i].szDefinition, szDefinition))
								{
									isMatched = false;
									break;
								}
							}
						}

						if(isMatched)
						{
							uint32_t uIncludeCount = 0;
							pCacheFile->readBin(&uIncludeCount, sizeof(uIncludeCount));
							bool isValid = true;
							for(UINT i = 0; i < uIncludeCount; ++i)
							{
								uint8_t u8Len = 0;
								pCacheFile->readBin(&u8Len, sizeof(u8Len));
								char *szName = (char*)alloca(u8Len);
								pCacheFile->readBin(szName, u8Len);

								time_t tMod = 0;
								pCacheFile->readBin(&tMod, sizeof(tMod));
								if(tMod != pFileSystem->getFileModifyTime(szName))
								{
									isValid = false;
									break;
								}
							}

							if(isValid || !isSourceExists)
							{
								UINT uCodeSize = 0;
								pCacheFile->readBin(&uCodeSize, sizeof(uCodeSize));
								byte *pCode;
								bool isFree = false;
								if(uCodeSize <= 32768)
								{
									pCode = (byte*)alloca(uCodeSize);
								}
								else
								{
									isFree = true;
									pCode = (byte*)malloc(uCodeSize);
								}
								pCacheFile->readBin(pCode, uCodeSize);

								T *pGXShader = NULL;
								CreateShaderFromData(g_pDevice, &pGXShader, pCode, uCodeSize);
								if(pGXShader)
								{
									mem_release(pCacheFile);
									pShader->m_pGXShader = pGXShader;
									return(LOAD_SHADER_CACHE);
								}

								if(isFree)
								{
									free(pCode);
								}
							}
							else
							{
								// clear from file
								// 0 - sizeChunkBegin
								// sizeNextBlockPos - end
								size_t sizeFile = pCacheFile->getSize();
								size_t sizeFileNew = sizeFile - (sizeNextBlockPos - sizeChunkBegin);
								byte *pBuffer;
								bool isFree = false;
								if(sizeFileNew <= 32768)
								{
									pBuffer = (byte*)alloca(sizeFileNew);
								}
								else
								{
									isFree = true;
									pBuffer = (byte*)malloc(sizeFileNew);
								}
								pCacheFile->setPos(0);
								pCacheFile->readBin(pBuffer, sizeChunkBegin);
								pCacheFile->setPos(sizeNextBlockPos);
								pCacheFile->readBin(pBuffer + sizeChunkBegin, sizeFile - sizeNextBlockPos);
								mem_release(pCacheFile);

								pCacheFile = pFileSystem->openFile(szFullPathCache, FILE_MODE_WRITE);
								if(pCacheFile)
								{
									pCacheFile->writeBin(pBuffer, sizeFileNew);
									mem_release(pCacheFile);
								}

								pCacheFile = pFileSystem->openFile(szFullPathCache);
								if(!pCacheFile)
								{
									break;
								}
								sizeNextBlockPos = sizeChunkBegin;

								if(isFree)
								{
									free(pBuffer);
								}
							}
						}

						pCacheFile->setPos(sizeNextBlockPos);
					}
					while(!pCacheFile->isEOF());
				}
			}

			mem_release(pCacheFile);
		}
	}

	if(!isSourceExists)
	{
		return(LOAD_SHADER_FAIL);
	}

	T *pGXShader = NULL;
	do
	{
		IFile *pFile = pFileSystem->openFile(szFullPath);
		if(pFile)
		{
			size_t sizeFile = pFile->getSize();
			char *szShader = (char*)alloca(sizeof(char) * (sizeFile + 1));
			pFile->readBin(szShader, sizeFile);
			szShader[sizeFile] = 0;
			uint32_t uMacroCount = 0;
			if(aMacro)
			{
				GXMacro *pMacro = aMacro;
				while(pMacro->szName)
				{
					char *szMacro = (char*)alloca(sizeof(char) * (strlen(pMacro->szName) + strlen(pMacro->szDefinition) + 2));
					sprintf(szMacro, "%s %s", pMacro->szName, pMacro->szDefinition);
					pPreprocessor->define(szMacro);
					++pMacro;
					++uMacroCount;
				}
			}
				
			String sResult = pPreprocessor->process(szShader, szFullPath);
			if(pPreprocessor->isError())
			{
				LibReport(REPORT_MSG_LEVEL_ERROR, "Preprocess shader error (%s):\n%s", szFullPath, pPreprocessor->getError());
			}
			else
			{
				CreateShader(g_pDevice, &pGXShader, sResult.c_str(), szFullPath, aMacro);

				if(pGXShader)
				{
					IFile *pCacheFile = pFileSystem->openFile(szFullPathCache, FILE_MODE_APPEND);
					if(!pCacheFile)
					{
						LibReport(REPORT_MSG_LEVEL_ERROR, "Unable to create shader cache file (%s):\n", szFullPathCache);
					}
					else
					{
						if(pCacheFile->getPos() == 0)
						{
							uint32_t uMagic = SX_SHADER_CACHE_MAGIC;
							pCacheFile->writeBin(&uMagic, sizeof(uMagic));
							uint32_t uVersion = SX_SHADER_CACHE_VERSION;
							pCacheFile->writeBin(&uVersion, sizeof(uVersion));
						}
						else
						{
							pCacheFile->setPos(0);
							uint32_t uMagic = 0;
							pCacheFile->readBin(&uMagic, sizeof(uMagic));
							uint32_t uVersion = 0;
							pCacheFile->readBin(&uVersion, sizeof(uVersion));
							pCacheFile->setPos(pCacheFile->getSize());

							if(uMagic != SX_SHADER_CACHE_MAGIC || uVersion != SX_SHADER_CACHE_VERSION)
							{
								mem_release(pCacheFile);
								pCacheFile = pFileSystem->openFile(szFullPathCache, FILE_MODE_WRITE);

								uint32_t uMagic = SX_SHADER_CACHE_MAGIC;
								pCacheFile->writeBin(&uMagic, sizeof(uMagic));
								uint32_t uVersion = SX_SHADER_CACHE_VERSION;
								pCacheFile->writeBin(&uVersion, sizeof(uVersion));
							}
						}

						if(!pCacheFile)
						{
							LibReport(REPORT_MSG_LEVEL_ERROR, "Unable to create shader cache file (%s):\n", szFullPathCache);
						}
						else
						{

							size_t sizeStartPos = pCacheFile->getPos();

							uint32_t uChunkSize = 0;
							pCacheFile->writeBin(&uChunkSize, sizeof(uChunkSize));

							pCacheFile->writeBin(&uMacroCount, sizeof(uMacroCount));

							for(UINT i = 0; i < uMacroCount; ++i)
							{
								uint8_t u8Len = (uint8_t)strlen(aMacro[i].szName) + 1;
								pCacheFile->writeBin(&u8Len, sizeof(u8Len));
								pCacheFile->writeBin(aMacro[i].szName, u8Len);

								u8Len = (uint8_t)strlen(aMacro[i].szDefinition) + 1;
								pCacheFile->writeBin(&u8Len, sizeof(u8Len));
								pCacheFile->writeBin(aMacro[i].szDefinition, u8Len);
							}

							uint32_t uIncludeCount = pPreprocessor->getIncludesCount();
							const char **pszIncludes = (const char**)alloca(sizeof(const char*) * uIncludeCount);
							pPreprocessor->getIncludes(pszIncludes);

							++uIncludeCount;
							pCacheFile->writeBin(&uIncludeCount, sizeof(uIncludeCount));

							uint8_t uLen = (uint8_t)strlen(szFullPath) + 1;
							pCacheFile->writeBin(&uLen, sizeof(uLen));
							pCacheFile->writeBin(szFullPath, uLen);

							time_t tMod = pFileSystem->getFileModifyTime(szFullPath);
							pCacheFile->writeBin(&tMod, sizeof(tMod));

							for(UINT i = 0; i < uIncludeCount - 1; ++i)
							{
								uLen = (uint8_t)strlen(pszIncludes[i]) + 1;
								pCacheFile->writeBin(&uLen, sizeof(uLen));
								pCacheFile->writeBin(pszIncludes[i], uLen);

								tMod = pFileSystem->getFileModifyTime(pszIncludes[i]);
								pCacheFile->writeBin(&tMod, sizeof(tMod));
							}
							UINT uCodeSize = 0;
							pGXShader->getData(NULL, &uCodeSize);
							byte *pCode;
							bool isFree = false;
							if(uCodeSize <= 32768)
							{
								pCode = (byte*)alloca(uCodeSize);
							}
							else
							{
								pCode = (byte*)malloc(uCodeSize);
								isFree = true;
							}
							pGXShader->getData(pCode, &uCodeSize);

							pCacheFile->writeBin(&uCodeSize, sizeof(uCodeSize));
							pCacheFile->writeBin(pCode, uCodeSize);

							uChunkSize = (uint32_t)(pCacheFile->getPos() - sizeStartPos - sizeof(uChunkSize));
							pCacheFile->setPos(sizeStartPos);
							pCacheFile->writeBin(&uChunkSize, sizeof(uChunkSize));

							mem_release(pCacheFile);

							if(isFree)
							{
								free(pCode);
							}
						}
					}
				}
			}
			pPreprocessor->reset();

			mem_release(pFile);
		}
	}
	while(!pGXShader && MessageBoxA(NULL, "Unable to compile shader. Want to retry?", "Shader error", MB_OKCANCEL | MB_ICONSTOP) == IDOK);

	if(!pGXShader)
	{
		return(LOAD_SHADER_FAIL);
	}

	pShader->m_pGXShader = pGXShader;


	return LOAD_SHADER_COMPLETE;
}

//##########################################################################

CShaderManager::CShaderManager()
{

	m_pPreprocessor = new CShaderPreprocessor(Core_GetIXCore()->getFileSystem());
}

CShaderManager::~CShaderManager()
{
	for (int i = 0; i < m_aVS.size(); ++i)
	{
		mem_delete(m_aVS[i]);
	}


	for(int i = 0; i < m_aPS.size(); ++i)
	{
		mem_delete(m_aPS[i]);
	}

	for(int i = 0; i < m_aGS.size(); ++i)
	{
		mem_delete(m_aGS[i]);
	}

	for(int i = 0; i < m_aCS.size(); ++i)
	{
		mem_delete(m_aCS[i]);
	}
			
	mem_delete(m_pPreprocessor);
}


const char* CShaderManager::getTextResultLoad(int iResult)
{
	switch(iResult)
	{
	case LOAD_SHADER_FAIL:
		return(COLOR_LRED "fail" COLOR_RESET);
	case LOAD_SHADER_COMPLETE:
		return(COLOR_LGREEN "complete" COLOR_RESET);
	case LOAD_SHADER_CACHE:
		return(COLOR_LMAGENTA "cache" COLOR_RESET);
	}

	return("unknown");
}

template<class T>
static void ReloadShaders(Array<T*> &aShaders)
{
	for(UINT i = 0, l = aShaders.size(); i < l; ++i)
	{
		mem_release(aShaders[i]->m_pGXShader);
	}
}

void CShaderManager::reloadAll()
{
	LibReport(REPORT_MSG_LEVEL_NOTICE, "Reloading shaders\n");

	for(UINT i = 0, l = m_aShaderKit.size(); i < l; ++i)
	{
		if(m_aShaderKit[i])
		{
			mem_release(m_aShaderKit[i]->m_pShaderKit);
		}
	}

	ReloadShaders(m_aVS);
	ReloadShaders(m_aPS);
	ReloadShaders(m_aGS);
	ReloadShaders(m_aCS);

	m_iLastAllLoadVS = 0;
	m_iLastAllLoadPS = 0;
	m_iLastAllLoadGS = 0;
	m_iLastAllLoadCS = 0;

	mem_delete(m_pPreprocessor);
	m_pPreprocessor = new CShaderPreprocessor(Core_GetIXCore()->getFileSystem());

	allLoad(true);
}

ID CShaderManager::existsPathMacro(SHADER_TYPE type, const char *szPath, GXMacro *aMacros)
{
	ID idShader = -1;
	GXMacro *pShaderMacro = NULL;
	if(type == SHADER_TYPE_VERTEX)
	{
		for(int i = 0; i < m_aVS.size(); ++i)
		{
			if(strcmp(m_aVS[i]->m_szPath, szPath) == 0)
			{
				idShader = i;
				pShaderMacro = m_aVS[i]->m_aMacros;
				break;
			}
		}
	}
	else if(type == SHADER_TYPE_PIXEL)
	{
		for(int i = 0; i < m_aPS.size(); ++i)
		{
			if(strcmp(m_aPS[i]->m_szPath, szPath) == 0)
			{
				idShader = i;
				pShaderMacro = m_aPS[i]->m_aMacros;
				break;
			}
		}
	}
	else if(type == SHADER_TYPE_GEOMETRY)
	{
		for(int i = 0; i < m_aGS.size(); ++i)
		{
			if(strcmp(m_aGS[i]->m_szPath, szPath) == 0)
			{
				idShader = i;
				pShaderMacro = m_aGS[i]->m_aMacros;
				break;
			}
		}
	}
	else if(type == SHADER_TYPE_COMPUTE)
	{
		for(int i = 0; i < m_aCS.size(); ++i)
		{
			if(strcmp(m_aCS[i]->m_szPath, szPath) == 0)
			{
				idShader = i;
				pShaderMacro = m_aCS[i]->m_aMacros;
				break;
			}
		}
	}

	if(ID_VALID(idShader))
	{
		GXMacro def = {NULL, NULL};
		if(!aMacros)
		{
			aMacros = &def;
		}
		UINT i = 0, j;
		bool bFound = false;
		while(aMacros[i].szName)
		{
			j = 0;
			bFound = false;
			while(pShaderMacro[j].szName)
			{
				if(!strcmp(aMacros[i].szName, pShaderMacro[j].szName))
				{
					if(strcmp(aMacros[i].szDefinition, pShaderMacro[j].szDefinition))
					{
						return(-1);
					}
					bFound = true;
					break;
				}
				++j;
			}
			if(!bFound)
			{
				return(-1);
			}
			++i;
		}
		j = 0;
		while(pShaderMacro[j].szName)
		{
			++j;
		}
		if(i != j)
		{
			return(-1);
		}
		return(idShader);
	}
	return(-1);
}

bool CShaderManager::existsFile(const char *szPath)
{
	char tmppath[SXGC_SHADER_MAX_SIZE_FULLPATH];
	char tmpspath[SXGC_SHADER_MAX_SIZE_DIR];
	tmpspath[0] = 0;
	char tmpname[SXGC_SHADER_MAX_SIZE_NAME];
	StrParsePathName(szPath, tmpspath, tmpname);

	if (tmpspath[0] != 0)
		sprintf(tmppath, "shaders/%s/%s", tmpspath, szPath);
	else
		sprintf(tmppath, "shaders/%s", szPath);

	return(Core_GetIXCore()->getFileSystem()->fileExists(tmppath));
}

ID CShaderManager::preLoad(SHADER_TYPE type, const char *szPath, GXMacro *aMacros)
{
	ID id = existsPathMacro(type, szPath, aMacros);

	if(!ID_VALID(id))
	{
		ScopedLock lock(m_mxLock);

		CShader *pShader = 0;
		if(type == SHADER_TYPE_VERTEX)
		{
			CShaderVS *pVS = new CShaderVS();
			m_aVS.push_back(pVS);
			id = m_aVS.size() - 1;
			pShader = pVS;
		}
		else if(type == SHADER_TYPE_PIXEL)
		{
			CShaderPS *pPS = new CShaderPS();
			m_aPS.push_back(pPS);
			id = m_aPS.size() - 1;
			pShader = pPS;
		}
		else if(type == SHADER_TYPE_GEOMETRY)
		{
			CShaderGS *pGS = new CShaderGS();
			m_aGS.push_back(pGS);
			id = m_aGS.size() - 1;
			pShader = pGS;
		}
		else if(type == SHADER_TYPE_COMPUTE)
		{
			CShaderCS *pCS = new CShaderCS();
			m_aCS.push_back(pCS);
			id = m_aCS.size() - 1;
			pShader = pCS;
		}
		else
		{
			assert(!"Unknown shader type!");
			return(-1);
		}
		strcpy(pShader->m_szPath, szPath);
		
		if(aMacros)
		{
			int iCountMacros = 0;
			for(int i = 0; i < SXGC_SHADER_COUNT_MACRO; i++)
			{
				if(!aMacros[i].szName)
				{
					iCountMacros = i;
					break;
				}
			}

			for(int i = 0; i < iCountMacros; i++)
			{
				pShader->m_aMacros[i] = aMacros[i];
			}

			pShader->m_aMacros[iCountMacros].szName = 0;
			pShader->m_aMacros[iCountMacros].szDefinition = 0;
		}
	}

	return(id);
}

void CShaderManager::allLoad(bool bReload)
{
	ScopedLock lock(m_mxLock);

	if(m_aVS.size() == m_iLastAllLoadVS && m_aPS.size() == m_iLastAllLoadPS && m_aGS.size() == m_iLastAllLoadGS && m_aCS.size() == m_iLastAllLoadCS)
		return;

	DWORD dwTime = GetTickCount();
	if(!bReload)
	{
		LibReport(REPORT_MSG_LEVEL_NOTICE, "Loading shaders\n");
	}

	const char *szResult;

	for(UINT i = 0, l = m_aVS.size(); i < l; ++i)
	{
		CShaderVS *pShader = m_aVS[i];
		if(pShader->m_pGXShader)
			continue;
		szResult = getTextResultLoad(LoadShader(m_pPreprocessor, Core_GetIXCore()->getFileSystem(), pShader->m_szPath, pShader, pShader->m_aMacros));
		LibReport(REPORT_MSG_LEVEL_NOTICE, "  VS id [" COLOR_LCYAN "%d" COLOR_RESET "], file [" COLOR_LCYAN "%s" COLOR_RESET "], result [%s] \n", i, pShader->m_szPath, szResult);
	}

	LibReport(REPORT_MSG_LEVEL_NOTICE, "  -------\n");

	for(UINT i = 0, l = m_aPS.size(); i < l; ++i)
	{
		CShaderPS *pShader = m_aPS[i];
		if(pShader->m_pGXShader)
			continue;
		szResult = getTextResultLoad(LoadShader(m_pPreprocessor, Core_GetIXCore()->getFileSystem(), pShader->m_szPath, pShader, pShader->m_aMacros));
		LibReport(REPORT_MSG_LEVEL_NOTICE, "  PS id [" COLOR_LCYAN "%d" COLOR_RESET "], file[" COLOR_LCYAN "%s" COLOR_RESET "], result [%s] \n", i, pShader->m_szPath, szResult);
	}

	LibReport(REPORT_MSG_LEVEL_NOTICE, "  -------\n");

	for(UINT i = 0, l = m_aGS.size(); i < l; ++i)
	{
		CShaderGS *pShader = m_aGS[i];
		if(pShader->m_pGXShader)
			continue;
		szResult = getTextResultLoad(LoadShader(m_pPreprocessor, Core_GetIXCore()->getFileSystem(), pShader->m_szPath, pShader, pShader->m_aMacros));
		LibReport(REPORT_MSG_LEVEL_NOTICE, "  GS id [" COLOR_LCYAN "%d" COLOR_RESET "], file[" COLOR_LCYAN "%s" COLOR_RESET "], result [%s] \n", i, pShader->m_szPath, szResult);
	}

	LibReport(REPORT_MSG_LEVEL_NOTICE, "  -------\n");

	for(UINT i = 0, l = m_aCS.size(); i < l; ++i)
	{
		CShaderCS *pShader = m_aCS[i];
		if(pShader->m_pGXShader)
			continue;
		szResult = getTextResultLoad(LoadShader(m_pPreprocessor, Core_GetIXCore()->getFileSystem(), pShader->m_szPath, pShader, pShader->m_aMacros));
		LibReport(REPORT_MSG_LEVEL_NOTICE, "  CS id [" COLOR_LCYAN "%d" COLOR_RESET "], file[" COLOR_LCYAN "%s" COLOR_RESET "], result [%s] \n", i, pShader->m_szPath, szResult);
	}

	m_iLastAllLoadVS = m_aVS.size();
	m_iLastAllLoadPS = m_aPS.size();
	m_iLastAllLoadGS = m_aGS.size();
	m_iLastAllLoadCS = m_aCS.size();

	for(UINT i = 0, l = m_aShaderKit.size(); i < l; ++i)
	{
		if(m_aShaderKit[i] && !(m_aShaderKit[i]->m_pShaderKit))
		{
			ID idVS = m_aShaderKit[i]->m_idVertexShader;
			ID idPS = m_aShaderKit[i]->m_idPixelShader;
			ID idGS = m_aShaderKit[i]->m_idGeometryShader;
			ID idCS = m_aShaderKit[i]->m_idComputeShader;

			m_aShaderKit[i]->m_pShaderKit = g_pDevice->createShader(
				ID_VALID(idVS) ? m_aVS[idVS]->m_pGXShader : NULL,
				ID_VALID(idPS) ? m_aPS[idPS]->m_pGXShader : NULL,
				ID_VALID(idGS) ? m_aGS[idGS]->m_pGXShader : NULL,
				ID_VALID(idCS) ? m_aCS[idCS]->m_pGXShader : NULL
				);
		}
	}
	LibReport(REPORT_MSG_LEVEL_NOTICE, "all shaders %sloaded, time " COLOR_LCYAN "%dms" COLOR_RESET "\n", bReload ? "re" : "", GetTickCount() - dwTime);
}

ID CShaderManager::createKit(ID idVertexShader, ID idPixelShader, ID idGeometryShader, ID idComputeShader)
{
	if(
		!(ID_VALID(idVertexShader) && idVertexShader < m_aVS.size()) && 
		!(ID_VALID(idPixelShader) && idPixelShader < m_aPS.size()) && 
		!(ID_VALID(idGeometryShader) && idGeometryShader < m_aGS.size()) && 
		!(ID_VALID(idComputeShader) && idComputeShader < m_aCS.size())
		)
		return -1;

	ID idShaderKit = -1;

	for (int i = 0, il = m_aShaderKit.size(); i < il; ++i)
	{
		if(m_aShaderKit[i] && 
			m_aShaderKit[i]->m_idVertexShader == idVertexShader && 
			m_aShaderKit[i]->m_idPixelShader == idPixelShader && 
			m_aShaderKit[i]->m_idGeometryShader == idGeometryShader &&
			m_aShaderKit[i]->m_idComputeShader == idComputeShader
			)
		{
			idShaderKit = i;
			break;
		}
	}

	if (idShaderKit == -1)
	{
		CShaderKit *pShaderKit = new CShaderKit();
		pShaderKit->m_idVertexShader = idVertexShader;
		pShaderKit->m_idPixelShader = idPixelShader;
		pShaderKit->m_idGeometryShader = idGeometryShader;
		pShaderKit->m_idComputeShader = idComputeShader;

		if(
			(!ID_VALID(idVertexShader) || m_aVS[idVertexShader]->m_pGXShader) &&
			(!ID_VALID(idPixelShader) || m_aPS[idPixelShader]->m_pGXShader) &&
			(!ID_VALID(idGeometryShader) || m_aGS[idGeometryShader]->m_pGXShader) &&
			(!ID_VALID(idComputeShader) || m_aCS[idComputeShader]->m_pGXShader)
			)
			pShaderKit->m_pShaderKit = g_pDevice->createShader(
				ID_VALID(idVertexShader) ? m_aVS[idVertexShader]->m_pGXShader : NULL,
				ID_VALID(idPixelShader) ? m_aPS[idPixelShader]->m_pGXShader : NULL,
				ID_VALID(idGeometryShader) ? m_aGS[idGeometryShader]->m_pGXShader : NULL,
				ID_VALID(idComputeShader) ? m_aCS[idComputeShader]->m_pGXShader : NULL
				);

		m_aShaderKit.push_back(pShaderKit);
		idShaderKit = m_aShaderKit.size() - 1;
	}

	return idShaderKit;
}

void CShaderManager::bind(ID idShaderKit)
{
	if (!(idShaderKit >= 0 && idShaderKit < m_aShaderKit.size() && m_aShaderKit[idShaderKit]))
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "shader kit not bind, id [%d]\n", idShaderKit);
		return;
	}

	g_pDevice->setShader(m_aShaderKit[idShaderKit]->m_pShaderKit);
}

void CShaderManager::unbind()
{
	g_pDevice->setShader(0);
}


bool CShaderManager::isLoaded(SHADER_TYPE type_shader, ID idShader)
{
	if (type_shader == SHADER_TYPE_VERTEX)
	{
		if (idShader < m_aVS.size())
		{
			if(m_aVS[idShader]->m_pGXShader)
				return true;
		}
	}
	else if(type_shader == SHADER_TYPE_PIXEL)
	{
		if(idShader < m_aPS.size())
		{
			if(m_aPS[idShader]->m_pGXShader)
				return true;
		}
	}
	else if(type_shader == SHADER_TYPE_GEOMETRY)
	{
		if(idShader < m_aGS.size())
		{
			if(m_aGS[idShader]->m_pGXShader)
				return true;
		}
	}
	else if(type_shader == SHADER_TYPE_COMPUTE)
	{
		if(idShader < m_aCS.size())
		{
			if(m_aCS[idShader]->m_pGXShader)
				return true;
		}
	}
	return false;
}

void CShaderManager::getPath(SHADER_TYPE type_shader, ID idShader, char *szPath)
{
	if (type_shader == SHADER_TYPE_VERTEX)
	{
		if (idShader < m_aVS.size())
		{
			//if (m_aVS[idShader]->m_pVertexShader)
				sprintf(szPath, "%s", m_aVS[idShader]->m_szPath);
		}
	}
	else if(type_shader == SHADER_TYPE_PIXEL)
	{
		if(idShader < m_aPS.size())
		{
			//if (m_aPS[idShader]->m_pPixelShader)
			sprintf(szPath, "%s", m_aPS[idShader]->m_szPath);
		}
	}
	else if(type_shader == SHADER_TYPE_GEOMETRY)
	{
		if(idShader < m_aGS.size())
		{
			//if (m_aGS[idShader]->m_pPixelShader)
			sprintf(szPath, "%s", m_aGS[idShader]->m_szPath);
		}
	}
	else if(type_shader == SHADER_TYPE_COMPUTE)
	{
		if(idShader < m_aCS.size())
		{
			//if (m_aGS[idShader]->m_pPixelShader)
			sprintf(szPath, "%s", m_aCS[idShader]->m_szPath);
		}
	}
}
